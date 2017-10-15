

#ifndef CURSE_IMPACT_UTIL_HPP
#define CURSE_IMPACT_UTIL_HPP

#include <algorithm>
#include <functional>

namespace util {


	template <typename T, typename G>
	struct is_same  {
		static constexpr bool value = false;
	};

	template <typename T>
	struct is_same<T, T> {
		static constexpr bool value = true;
	};

	template <typename T, typename Arg, typename ...Args>
	struct is_in_type_list {
		static constexpr bool value = is_same<T,Arg>::value || is_in_type_list<T, Args...>::value;
	};

	template <typename T, typename Arg>
	struct is_in_type_list<T, Arg> {
		static constexpr bool value = is_same<T, Arg>::value;
	};

	template <typename char_T>
	bool is_integral_value(char_T ch) {
		static_assert(is_in_type_list<char_T,char, short, int>::value, "is not in type list");
		return static_cast<char_T>('0') <= ch && ch <= static_cast<char_T>('9');
	}

	template <typename string_T>
	bool is_integral_format(const string_T& str) {
		auto it = str.begin();
		if (*it == '-' || *it == '+') { ++it; } // ship sign
		for (;it != str.end(); ++it) {
			if (!is_integral_value(*it)) {
				return false;
			}
		}
		return true;
	}

	template <typename string_T>
	bool is_divisible_format(const string_T& str) {
		auto it = str.begin();
		if (*it == '-' || *it == '+') { ++it; } // ship sign
		for (; it != str.end() && is_integral_value(*it); ++it);
		if (it == str.end() || *it != '/' ) {
			return false;
		}
		++it; // go through the char '/'
		for (; it != str.end() && is_integral_value(*it); ++it); // look for integral numbers
		if (it != str.end()) {
			return false;
		}
		return true;
	}

	template <typename elem_T, typename container_T, typename key_T>
	void set_if_undef(container_T& cont, const key_T& key /* default */) {
		if (!cont.count(key)) {
			cont[key] = elem_T {};
		}
	}

	template <typename iterator_T, typename binary_predicate_T>
	iterator_T max_if(iterator_T begin, iterator_T end, binary_predicate_T comparator) {
		iterator_T max_it = begin;
		for (iterator_T it = begin; it != end; it++) {
			if (comparator(*max_it, *it)) {
				max_it = it;
			}
		}
		return max_it;
	}

	template <typename T, typename C>
	void remove_duplicates(T& container, C comparator) {
		auto end = container.end();
		for (auto it = container.begin(); it != end; ++it) {
			end = std::remove_if(it+1, end, [it, comparator](auto elem) {
				return comparator(*it, elem);
			});
		}
		container.erase(end, container.end());
	}

	class scoped_initializer {

		public:
			scoped_initializer(std::function<void(void)>&& constructor, 
					std::function<void(void)>&& destructor);

			~scoped_initializer();

		private: 
			std::function<void(void)> destructor_;

	};


	// IMPORTANT!!! DO NOT USE THIS IN PRODUCTION, I ONLY DID THIS BECAUSE OF
	// CURIOSITY AND SHOULD USE std::experimental::optional or std::optional if
	// available
	template <typename T>
	class naive_optional {

		private:
			struct nullopt_t { };

		public:


			naive_optional():init_(false) {}

			naive_optional(nullopt_t n):init_(false) {}

			naive_optional(T value):init_(true), value_(value) {}

			~naive_optional() = default;

			explicit operator bool() const {
				return this->init_;
			}

			T& operator*() {
				return this->value_;
			}

			const T& operator*() const {
				return this->value_;
			}

			T* operator->() {
				return &this->value_;
			}

			naive_optional& operator=(const naive_optional& opt) {
				this->init_ = opt.init_;
				this->value_ = opt.value_;
			}

			naive_optional& operator=(nullopt_t opt) {
				this->init_ = false;
			}

			naive_optional& operator=(const T& value) {
				this->init_ = true;
				this->value_ = value;
			}


		private:

			bool init_;

			T value_;

	};




} // namespace util


#endif // CURSE_IMPACT_UTIL_HPP
