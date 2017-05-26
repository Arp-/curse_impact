

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
		for (auto ch : str) {
			if (!is_integral_value(ch)) {
				return false;
			}
		}
		return true;
	}

	template <typename string_T>
	bool is_divisible_format(const string_T& str) {
		auto it = str.begin();
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


} // namespace util


#endif // CURSE_IMPACT_UTIL_HPP
