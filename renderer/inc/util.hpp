
#ifndef CURSE_IMPACT_RENDERER_UTIL_HPP
#define CURSE_IMPACT_RENDERER_UTIL_HPP

#include <experimental/optional>

namespace renderer { namespace util {

	template <typename T>
	struct equality_compareable {
		// TODO verify if T has operator != defined
		inline bool operator==(const T& elem) {
			return !(*static_cast<T*>(this) != elem);
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
	// IMPORTANT!!! DO NOT USE THIS IN PRODUCTION, I ONLY DID THIS BECAUSE OF
	// CURIOSITY, at first, but then because of lazyness copied here,
	// AND SHOULD USE std::experimental::optional or std::optional if
	// available.
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



}} // namespace renderere // namespace util



#endif // CURSE_IMPACT_RENDERER_UTIL_HPP
