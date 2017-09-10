

#ifndef CURSE_IMPACT_UTIL_HPP
#define CURSE_IMPACT_UTIL_HPP

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
		if (cont.count(key)) {
			cont[key] = elem_T {};
		}
	}


} // namespace util


#endif // CURSE_IMPACT_UTIL_HPP
