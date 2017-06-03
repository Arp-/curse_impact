
#include "pugixml/pugixml.hpp"
#include "script_t.hpp"
#include "util.hpp"
#include "error.hpp"
#include <iostream>
#include <algorithm>

enum class dimension {
	X,
	Y,
};

template <dimension dim_V>
static auto get_dimension(const gamefield_t& gf);
template <>
auto get_dimension<dimension::X>(const gamefield_t& gf) {
	return gf.rect().width_;
}
template <>
auto get_dimension<dimension::Y>(const gamefield_t& gf) {
	return gf.rect().height_;
}
//-----------------------------------------------------------------------------//
static const ship_t* ship_by_id(const gamefield_t::ship_list_t& ship_list, int id) {
	auto it = (std::find_if(ship_list.begin(), ship_list.end(), 
				[id](const ship_t& ship) { return ship.id() == id; }));

	if (it == ship_list.end()) {
		return nullptr;
	}
	return &(*it);
}
//-----------------------------------------------------------------------------//
template <dimension dim_V>
static int relatify_divided_value(
		const gamefield_t& gf, const std::string& val) {


	std::array<std::string, 2> str_repr;
	int index = 0;

	for (const auto& ch : val) {
		if (util::is_integral_value(ch)) {
			str_repr[index].push_back(ch);
		} else {
			index++;
		}
	}
	int first = std::stoi(str_repr[0]);
	int second = std::stoi(str_repr[1]);

	return (get_dimension<dim_V>(gf) * first / second);
}
//-----------------------------------------------------------------------------//
static void set_relative_position(
		const gamefield_t& gf, position_t& pos, const pugi::xml_node& node) {

	std::string x_repr = node.attribute("x").value();
	if (util::is_divisible_format(x_repr)) {
		pos.x_ = relatify_divided_value<dimension::X>(gf, x_repr);
	} else if (util::is_integral_format(x_repr)) {
		pos.x_ = std::stoi(x_repr);
	} else { 
		throw invalid_format_error { x_repr };
	}

	std::string y_repr = node.attribute("y").value();
	if (util::is_divisible_format(y_repr)) {
		pos.y_ = relatify_divided_value<dimension::Y>(gf, y_repr);
	} else if (util::is_integral_format(x_repr)) {
		pos.y_ = std::stoi(y_repr);
	}

	if (pos.x_ < 0) {
		pos.x_ = gf.rect().width_ + pos.x_;
	}
	if (pos.y_ < 0) {
		pos.y_ = gf.rect().height_ + pos.y_;
	}
}
//-----------------------------------------------------------------------------//
static void print_history(const script_t::history_t& hist) {
		for (const auto& ev_list : hist) {
			std::cout << "time: " << ev_list.first << " ";
			for (const auto& ev : ev_list.second) {
				std::cout << "id: " << ev.id_ << " ";
				std::cout << "position.x_: " << ev.position_.x_ << " ";
				std::cout << "position.y_: " << ev.position_.y_ << " ";
				if (ev.type_ == event_t::type::ATTACK) {
					std::cout << "ATTACK";
				} else if (ev.type_ == event_t::type::APPEAR) {
					std::cout << "APPEAR";
				} else if (ev.type_ == event_t::type::MOVEMENT) {
					std::cout << "MOVEMENT";
				} else {
					std::cout << "NOP";
				}
			std::cout << ", ";
			}
			std::cout << std::endl;
		}
}
//-----------------------------------------------------------------------------//
event_t 
script_t::get_event_from_ship_node(const pugi::xml_node& node) {
	static int ship_id_counter = 1;
	event_t event;
	event.id_ = ship_id_counter++;
	event.type_ = event_t::type::APPEAR;
	event.hp_ = atoi(node.attribute("hp").value());
	set_relative_position(this->gamefield_, event.position_, node);
	event.speed_ = atoi(node.attribute("speed").value());

	return event;
}
//-----------------------------------------------------------------------------//
event_t::type get_event_type(const pugi::xml_node& node) {
	// NOTE using std::string as a wrapper because pugixml 
	// returns const char* and that does memory address comparsion
	if (node.name() == std::string{"move"}) {
		return event_t::type::MOVEMENT;
	} else if (node.name() == std::string {"atk"}) {
		return event_t::type::ATTACK;
	}
	return event_t::type::NOP;
}
//-----------------------------------------------------------------------------//
event_t::direction get_event_direction(const pugi::xml_attribute& attr) {
	if (attr.value() == std::string {"up"}) {
		return event_t::direction::UP;
	} else if (attr.value() == std::string {"down"}) {
		return event_t::direction::DOWN;
	}
	return event_t::direction::NOP;
}
//-----------------------------------------------------------------------------//
static event_t get_event_from_ship_child_node(
		const event_t& parent_ev, const pugi::xml_node& child_node) {

	event_t event;
	event.id_ = parent_ev.id_;
	event.type_ = get_event_type(child_node);
	event.direction_ = get_event_direction(child_node.attribute("direction"));
	return event;
}
//-----------------------------------------------------------------------------//
script_t::script_t(gamefield_t& gf): gamefield_(gf), time_(0) {

}
//-----------------------------------------------------------------------------//
texture_t&
script_t::texture_ref_by_name(const std::string& name) {
	auto it = std::find_if(this->texture_list_.begin(),
			this->texture_list_.end(), [&name](const auto& pair) {
				return pair.first == name;
	});
	if (it == this->texture_list_.end()) {
		throw std::out_of_range("element not found");
	}
	return it->second;
}
//-----------------------------------------------------------------------------//
void
script_t::read_enemy_ship_list(const pugi::xml_node& root) {
	for (auto ship = root.child("enemy");
			ship; ship = ship.next_sibling("enemy")) {

		int time = atoi(ship.attribute("time").value());
		if (!this->history_.count(time)) {
			this->history_[time] = event_list_t {};
		}
		event_t&& ev = get_event_from_ship_node(ship);
		std::string texture_name = ship.attribute("texture_ref").value();
		std::cout << "refname: " <<
			 this->texture_ref_by_name(texture_name).matrix().size() << std::endl;
		this->texture_ship_association_.emplace(ev.id_,
				this->texture_ref_by_name(texture_name));
		this->history_[time].push_back(ev);
		for (const auto& child : ship) {
			int offset = atoi(child.attribute("offset").value());
			int offset_time = time + offset;
			if (!this->history_.count(offset_time)) {
				this->history_[offset_time] = event_list_t {};
			}
			event_t&& ev_child = get_event_from_ship_child_node(ev, child);
			this->history_[offset_time].push_back(ev_child);
		}
	}
}
//-----------------------------------------------------------------------------//
void
script_t::read_texture_list(const pugi::xml_node& root) {
	for (auto texture = root.child("texture");
			texture; texture = texture.next_sibling("texture")) {

		std::string id = texture.attribute("id").value();
		std::string res = texture.attribute("res").value();
		texture_t t = texture_t::read_from_file(res);
		this->texture_list_.push_back({id, t});
	}
}
//-----------------------------------------------------------------------------//
void
script_t::read_xml(const char* filepath) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filepath);
	// looks like i actually don't need to make this a static variable :)
	if (!result) {
		throw std::runtime_error("Failed to parse the xml file");
	}
	const auto& root = doc.child("level");
	read_texture_list(root);
	read_enemy_ship_list(root);

	// TODO REMOVE FROM PRODUCTION
	print_history(this->history_);
}
//-----------------------------------------------------------------------------//
void 
script_t::tick() {
	this->time_++;
	if (!this->history_.count(this->time_)) {
		return;
	}

	const auto& ev_list = this->history_[this->time_];
	for (const auto& ev : ev_list) {
		if (ev.type_ == event_t::type::APPEAR) {
			ship_t enemy { ev.position_, { 3, 3 }, ev.speed_, ev.hp_, ev.id_};
			this->gamefield_.add_enemy(enemy);
		} else if (ev.type_ == event_t::type::MOVEMENT) {
			this->gamefield_.move_enemy(ev.id_, ev.direction_);
		} else if (ev.type_ == event_t::type::ATTACK) {
			this->gamefield_.enemy_shoot(ev.id_);
		}
	}
}
//-----------------------------------------------------------------------------//
const script_t::texture_ship_assoc_t&
script_t::texture_ship_assoc() const {
	return this->texture_ship_association_;
}
