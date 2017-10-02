#ifndef CURSE_IMPACT_UNIQUE_HPP
#define CURSE_IMPACT_UNIQUE_HPP


template <typename T>
class unique {
	public:
		unique():id_(counter_++) { }
		~unique() = default;

		unsigned id() const {
			return this->id_;
		};

	private:

		static unsigned counter_;

		unsigned id_;

};

template <typename T> unsigned unique<T>::counter_ = 0;


#endif // CURSE_IMPACT_UNIQUE_HPP
