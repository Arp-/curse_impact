
#include "pugixml/pugixml.hpp"
#include "script_t.hpp"
#include "util.hpp"
#include "error.hpp"
#include <iostream>
#include <algorithm>

#ifdef DEBUG
# include <ncurses.h>
#endif

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
				std::cout << ", ";
			}
			std::cout << std::endl;
		}
}
//-----------------------------------------------------------------------------//
int
script_t::rect_index_by_name(const std::string& name) const {
	for (size_t i = 0; i < this->rectangle_list_.size(); i++) {
		if (this->rectangle_list_[i].first == name) {
			return static_cast<int>(i);
		}
	}
	return -1;
}
//-----------------------------------------------------------------------------//
appear_event_t 
script_t::get_event_from_ship_node(const pugi::xml_node& node) {
	static int ship_id_counter = 1;
	appear_event_t event;
	event.id_ = ship_id_counter++;
	event.rect_id_ = rect_index_by_name(node.attribute("rect").value());
	event.hp_ = atoi(node.attribute("hp").value());
	set_relative_position(this->gamefield_, event.position_, node);
	event.speed_ = atoi(node.attribute("speed").value());

	return event;
}
//-----------------------------------------------------------------------------//
ship_event_t::type get_event_type(const pugi::xml_node& node) {
	// NOTE using std::string as a wrapper because pugixml 
	// returns const char* and that does memory address comparsion
	// EDIT i could actually user strcmp from cstdlib for it
	// TODO use strcmp from cstdlib, or cstring
	if (node.name() == std::string{"move"}) {
		return ship_event_t::type::MOVEMENT;
	} else if (node.name() == std::string {"atk"}) {
		return ship_event_t::type::ATTACK;
	}
	return ship_event_t::type::NOP;
}
//-----------------------------------------------------------------------------//
ship_event_t::direction get_event_direction(const pugi::xml_attribute& attr) {
	if (attr.value() == std::string {"up"}) {
		return ship_event_t::direction::UP;
	} else if (attr.value() == std::string {"down"}) {
		return ship_event_t::direction::DOWN;
	}
	return ship_event_t::direction::NOP;
}
//-----------------------------------------------------------------------------//
static ship_event_t
make_ship_event(const pugi::xml_node& action) {
	ship_event_t ev;
	ev.type_ = get_event_type(action);
	ev.direction_ = get_event_direction(action.attribute("direction"));

	return ev;
}
//-----------------------------------------------------------------------------//
script_t::script_t(gamefield_t& gf, std::string&& res_path):
	gamefield_(gf), time_(0), resource_path_(res_path) {

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
static ship_event_list_t::repeat_type 
is_repeat_attr_set(const pugi::xml_node& ship) {
	const pugi::xml_attribute attr = ship.attribute("repeat");
	if (attr.empty()) {
		return ship_event_list_t::repeat_type::SINGULAR;
	}
	if (attr.value() == std::string { "true" }) {
		return ship_event_list_t::repeat_type::REPEATABLE;
	}
	return ship_event_list_t::repeat_type::SINGULAR;
}
//-----------------------------------------------------------------------------//
ship_event_list_t
static make_ship_event_list(const pugi::xml_node& ship) {

	ship_event_list_t ship_event_list { is_repeat_attr_set(ship) };

	for (const auto& child : ship) {
		int offset = atoi(child.attribute("offset").value());
		ship_event_list.insert(offset, make_ship_event(child));
	}
	std::cout << "event_list_size: " << ship_event_list.size() << std::endl;
	return ship_event_list;
}
//-----------------------------------------------------------------------------//
void
script_t::read_enemy_ship_list(const pugi::xml_node& root) {
	for (auto ship = root.child("enemy");
			ship; ship = ship.next_sibling("enemy")) {

		int time = atoi(ship.attribute("time").value());
		util::set_if_undef<event_list_t>(this->history_, time);

		appear_event_t&& ev = get_event_from_ship_node(ship);
		std::string texture_name = ship.attribute("texture_ref").value();
		this->texture_ship_association_.emplace(ev.id_,
				this->texture_ref_by_name(texture_name));
		this->history_[time].push_back(ev);
		this->ship_event_cont_[ev.id_] = make_ship_event_list(ship);
	}
}
//-----------------------------------------------------------------------------//
void
script_t::read_texture_list(const pugi::xml_node& root) {
	for (auto texture = root.child("texture");
			texture; texture = texture.next_sibling("texture")) {

		std::string id = texture.attribute("id").value();
		std::string res = this->resource_path_ + texture.attribute("res").value();
		texture_t t = texture_t::read_from_file(res);
		this->texture_list_.push_back({id, t});
	}
}
//-----------------------------------------------------------------------------//
void 
script_t::read_rectangle_list(const pugi::xml_node& root) {
	for (auto rect = root.child("rectangle");
			rect; rect = rect.next_sibling("rectangle")) {

		std::string width = rect.attribute("width").value();
		std::string height = rect.attribute("height").value();
		std::string id = rect.attribute("id").value();
		rect_t r;
		r.width_ = std::stoi(width);
		r.height_ = std::stoi(height);

		this->rectangle_list_.push_back({id, r});

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
	read_rectangle_list(root);
	read_enemy_ship_list(root);

	// TODO REMOVE FROM PRODUCTION
	print_history(this->history_);
}
//-----------------------------------------------------------------------------//
void
script_t::tick_active_ship_list() {
	const auto& active_enemy_list = this->gamefield_.enemy_list();
	for (const auto& enemy : active_enemy_list) {
		const auto& ship_ev_list = this->ship_event_cont_[enemy.id()].tick();
		if (!ship_ev_list) {
			continue;
		}
		for (const auto& ship_ev : *ship_ev_list) {
			if (ship_ev.type_ == ship_event_t::type::MOVEMENT) {
				this->gamefield_.move_enemy(enemy.id(), ship_ev.direction_);
			} else if (ship_ev.type_ == ship_event_t::type::ATTACK) {
				this->gamefield_.enemy_shoot(enemy.id());
			}
		}
	}
}
//-----------------------------------------------------------------------------//
void 
script_t::tick() {
	this->time_++;
	if (this->history_.count(this->time_)) {
		const auto& ev_list = this->history_[this->time_];

		for (const auto& ev : ev_list) {
			rect_t r = this->rectangle_list_[ev.rect_id_].second;
			if (ev.rect_id_ < 0) {
				r.width_ = 3;
				r.height_ = 3;
			}
			ship_t enemy { ev.position_, r, ev.speed_, ev.hp_, ev.id_};
			this->gamefield_.add_enemy(enemy);
		}
	}
	tick_active_ship_list();
}
//-----------------------------------------------------------------------------//
const script_t::texture_ship_assoc_t&
script_t::texture_ship_assoc() const {
	return this->texture_ship_association_;
}
//-----------------------------------------------------------------------------//
bool
script_t::end() const {
	auto max = std::max(this->history_.begin(), this->history_.end(), 
			[](const auto& pair1, const auto& pair2) {
		return pair1->first < pair2->first;
	});
	return static_cast<long>(max->first) < this->time_;
}
