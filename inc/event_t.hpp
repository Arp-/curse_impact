

#ifndef CURSE_IMPACT_EVENT_T_HPP
#define CURSE_IMPACT_EVENT_T_HPP

#include <map>

#include "instruction_t.hpp"
#include "position_t.hpp"




struct ship_event_t {

	ship_event_t();
	~ship_event_t() = default;

	enum class type {
		NOP,
		MOVEMENT,
		ATTACK,
		APPEAR, // TODO remove once everything is set
	};

	enum class direction {
		NOP,
		UP,
		DOWN
	};

	type type_;
	direction direction_;

};

// TODO remove once the others are finished
struct event_t {

	event_t();
	~event_t() = default;

	int id_;
	int rect_id_;
	ship_event_t::type type_; 
	ship_event_t::direction direction_;
	position_t position_;
	int hp_;
	int speed_;
};

template <typename T>
struct is_event_t {
	// TODO implement
	static constexpr bool value = true;
};

template <typename event_T>
class event_list_t : std::map<int, event_T> {
	static_assert(is_event_t<event_T>::value, "is not a valid event_T");

	public: //-- public functions --//
		event_list_t() = default;
		~event_list_t() = default;

		event_t tick() {
			auto index_count = this->count(this->tick_ % this->size());
			if (index_count) {
				return this->at(this->tick_);
			}
			return event_T {};
		}

	private: //-- private members --//
		int tick_;
};

using ship_event_list_t = event_list_t<ship_event_t>;



#endif // CURSE_IMPACT_EVENT_T_HPP
