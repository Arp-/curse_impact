

#ifndef CURSE_IMPACT_EVENT_T_HPP
#define CURSE_IMPACT_EVENT_T_HPP

#include "instruction_t.hpp"
#include "position_t.hpp"



struct event_t {

	enum class direction {
		NOP,
		UP,
		DOWN,
	};

	enum class type {
		NOP,
		APPEAR,
		MOVEMENT,
		ATTACK,
	};


	int id_;
	int rect_id_;
	type type_; 
	direction direction_;
	position_t position_;
	int hp_;
	int speed_;




};

#endif // CURSE_IMPACT_EVENT_T_HPP
