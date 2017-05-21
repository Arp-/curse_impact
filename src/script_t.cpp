
#include "pugixml/pugixml.hpp"
#include "script_t.hpp"
#include <iostream>

static void print_history(const script_t::history_t& hist) {
		for (const auto& ev_list : hist) {
			std::cout << "time: " << ev_list.first << " ";
			for (const auto& ev : ev_list.second) {
				std::cout << "id: " << ev.id_ << " ";
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
static event_t get_event_from_ship_node(const pugi::xml_node& node) {
	static int ship_id_counter = 1;
	event_t event;
	event.id_ = ship_id_counter++;
	event.type_ = event_t::type::APPEAR;
	event.hp_ = atoi(node.attribute("hp").value());
	event.position_.x_ = atoi(node.attribute("x").value());
	event.position_.y_ = atoi(node.attribute("y").value());
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
void
script_t::read_xml(const char* filepath) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filepath);
	// looks like i actually don't need to make this a static variable :)
	if (!result) {
		throw std::runtime_error("Failed to parse the xml file");
	}
	const auto& root = doc.child("level");
	for (const auto& ship : root) {
		int time = atoi(ship.attribute("time").value());
		if (!this->history_.count(time)) {
			this->history_[time] = event_list_t {};
		}
		event_t&& ev = get_event_from_ship_node(ship);
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
		
	}

}
