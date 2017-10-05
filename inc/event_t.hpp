

#ifndef CURSE_IMPACT_EVENT_T_HPP
#define CURSE_IMPACT_EVENT_T_HPP

#include <map>
#include <vector>
#include <utility>
#include <iostream>

#include "instruction_t.hpp"
#include "position_t.hpp"
#include "util.hpp"

#ifdef DEBUG
# include <ncurses.h>
#endif 




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
struct appear_event_t {

	appear_event_t();
	~appear_event_t() = default;

	int id_;
	int rect_id_;
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
class event_list_t {

	static_assert(is_event_t<event_T>::value, "is not a valid event_T");

	using key_T = int;

	using event_vec_t = std::vector<event_T>;
	public: //-- public types --//

		enum class repeat_type {
			SINGULAR,
			REPEATABLE,
		};


	public: //-- public functions --//
		event_list_t(): event_list_t(repeat_type::SINGULAR) { }

		event_list_t(repeat_type rt): 
			container_(),
			tick_(0),
			repeat_type_(rt) { }

		event_list_t(bool repeatable):
			event_list_t(
					repeatable?(repeat_type::REPEATABLE):(repeat_type::SINGULAR)) { }

		~event_list_t() = default;

		const event_vec_t* tick() {

			auto contains = this->container_.count(this->tick_);
			event_vec_t* ptr = nullptr;
			if (contains) {
				ptr = &(this->container_.at(this->tick_));
			}
			if (this->repeat_type_ == repeat_type::REPEATABLE) {
				this->tick_ %= this->last_tick_;
			}
			this->tick_++;
			return ptr;
		}

		event_vec_t& at(const key_T& index) {
			return this->container_.at(index);
		}

		const event_vec_t& at(const key_T& index) const {
			return this->at(index);
		}

		void insert(const key_T& key, const event_T& ev) {
			util::set_if_undef<event_vec_t>(this->container_, key);
			this->container_.at(key).push_back(ev);
			finalize();
		}

		size_t size() const {
			return this->container_.size();
		}

	private: //-- private functions --//

		void finalize() {
			auto max_it = util::max_if(this->container_.begin(), this->container_.end(),
					[](const auto& max, const auto& elem) {
						return max.first < elem.first;
			});
			this->last_tick_ = max_it->first;
		}


	private: //-- private members --//

		std::map<key_T, event_vec_t> container_;

		key_T tick_;

		repeat_type repeat_type_;

		key_T last_tick_;

};

using ship_event_list_t = event_list_t<ship_event_t>;



#endif // CURSE_IMPACT_EVENT_T_HPP
