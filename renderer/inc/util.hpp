
#ifndef CURSE_IMPACT_RENDERER_UTIL_HPP
#define CURSE_IMPACT_RENDERER_UTIL_HPP

#include <experimental/optional>

namespace renderer { namespace util {


	template <typename T>
	struct equality_compareable {
		// TODO verify if T has operator != defined
		inline bool operator==(const T& elem) const {
			return !(*static_cast<const T*>(this) != elem);
		}

		protected:
			equality_compareable() = default;
			equality_compareable(const equality_compareable&) = default;
			equality_compareable(equality_compareable&&) = default;
			equality_compareable& operator=(const equality_compareable&) = default;
			equality_compareable& operator=(equality_compareable&&) = default;

	};

	template <typename T>
	struct transitive_compareable {
		// TODO verify if equality_compareable and operator<() are defined in T

		inline bool operator<=(const T& elem) {
			return *static_cast<T*>(this) < elem || !(*static_cast<T*>(this) != elem);
		}

		inline bool operator>(const T& elem) {
			return !(*static_cast<T*>(this) < elem || !(*static_cast<T*>(this) != elem));
		}

		inline bool operator>=(const T& elem) {
			return !(*static_cast<T*>(this) < elem);
		}

		protected:
			transitive_compareable() = default;
			transitive_compareable(const transitive_compareable&) = default;
			transitive_compareable(transitive_compareable&&) = default;
			transitive_compareable& operator=(transitive_compareable&&) = default;
			transitive_compareable& operator=(const transitive_compareable&) = default;
	};




#if 0
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
				return *this;
			}

			naive_optional& operator=(nullopt_t opt) {
				this->init_ = false;
				return *this;
			}

			naive_optional& operator=(const T& value) {
				this->init_ = true;
				this->value_ = value;
				return *this;
			}


		private:

			bool init_;

			T value_;

	};

#endif
	template <typename T>
	using optional = std::experimental::optional<T>;

	inline int is_even(int a) {
		return a % 2 == 0;
	}
	// greated common divisor
	inline int binary_gcd(int a, int b) {
		if (a == 1 || b == 1) {
			return 1;
		}
		if (a == 0) {
			return b;
		}
		if (b == 0) {
			return a;
		}

		int d = 0;
		while (is_even(a) && is_even(b)) {
			a >>= 1;
			b >>= 1;
			d++;
		}
		while (a != b) {
			if (is_even(a)) {
				a >>= 1;
			} else if (is_even(b)) {
				b >>= 1;
			} else if (a > b) {
				a = (a-b)/2;
			} else {
				b = (b-a)/2;
			}
		}
		return a * (1 << d);
	}




}} // namespace renderere // namespace util



#endif // CURSE_IMPACT_RENDERER_UTIL_HPP
