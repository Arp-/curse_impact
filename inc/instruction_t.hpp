
#ifndef CURSE_IMPACT_INSTRUCTION_T_HPP
#define CURSE_IMPACT_INSTRUCTION_T_HPP

//enum class instruction_t {
//	NOP,
//	UP,
//	DOWN,
//	FORWARD,
//	BACKWARD,
//	SHOOT,
//	ULTIMATE
//};

struct instruction_t {
	enum class movement_t {
		NOP,
		UP,
		DOWN,
		BACKWARD,
		FORWARD
	};
	enum class attack_t {
		NOP,
		SHOOT,
		ULTIMATE
	};

	movement_t movement;
	attack_t attack;
	
};


#endif // CURSE_IMPACT_INSTRUCTION_T_HPP
