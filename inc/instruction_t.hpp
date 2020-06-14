
#ifndef CURSE_IMPACT_INSTRUCTION_T_HPP
#define CURSE_IMPACT_INSTRUCTION_T_HPP


struct instruction_t {
	enum class movement_t {
		NOP,
		UP,
		DOWN,
		BACKWARD,
		FORWARD,
		JUMP
	};
	enum class attack_t {
		NOP,
		SHOOT,
		SPECIAL
	};
	enum class other_t {
		NOP,
		QUIT
	};

	movement_t movement;
	attack_t attack;
	other_t other;
	
};


#endif // CURSE_IMPACT_INSTRUCTION_T_HPP
