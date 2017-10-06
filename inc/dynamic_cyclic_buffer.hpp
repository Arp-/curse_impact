
#ifndef CURSE_IMPACT_NAIVE_CYCLIC_BUFFER
#define CURSE_IMPACT_NAIVE_CYCLIC_BUFFER

#include <memory>
#include <cstring>

namespace util {

	template <typename elem_T, typename allocator_T = std::allocator<elem_T>>
	class dynamic_cyclic_buffer_t {

		public: 
			using iterator = elem_T*;
			using value_type = elem_T;
			using size_type = size_t;
			using allocator_type = allocator_T;

		public: 
			dynamic_cyclic_buffer_t() {
				allocator_type alloc;
				constexpr size_type initial_value = 8;
				this->begin_ = 
					std::allocator_traits<decltype(alloc)>::allocate(alloc, initial_value);
				this->end_ = this->begin_;
				this->current_ = this->begin_;
				memset(this->begin_, 0, sizeof(value_type) * initial_value);
				this->buffer_end_ = this->begin_ + initial_value;
			}

			~dynamic_cyclic_buffer_t() {
				allocator_type alloc;
				for (auto it = this->begin_; it != this->end_; ++it) {
					std::allocator_traits<decltype(alloc)>::destroy(alloc, it);
				}
				std::allocator_traits<decltype(alloc)>::deallocate(
						alloc, this->begin_, this->buffer_end_ - this->begin_);
			}

			void push_back(const elem_T& elem) {
				allocator_type alloc;
				if (this->end_ == this->buffer_end_) { 
					this->reallocate_n_copy(); 
				}
				std::allocator_traits<decltype(alloc)>::construct(alloc, this->end_, elem);
				++this->end_;
			}

			template <typename ...Args> 
			void emplace_back(Args&& ...args) { 
				allocator_type alloc;
				if (this->end_ == this->buffer_end_) { 
					this->reallocate_n_copy(); 
				}
				std::allocator_traits<decltype(alloc)>::construct(alloc, this->end_, args...);
				++this->end_;
			}

			value_type& get() {
				return *this->current_;
			}

			const value_type& get() const {
				return this->get();
			}

			value_type& ptr_forward() {
				++this->current_;
				if (this->current_ == this->end_) {
					this->current_ = this->begin_;
				}
				return *this->current_;
			}

			value_type& ptr_backward() {
				--this->current_;
				if (this->current_ < this->begin_) {
					this->current_ = this->end_ -1;
				}
				return *this->current_;
			}

			bool empty() const {
				return this->begin_ == this->end_;
			}

		private: //-- private functions --//

			void reallocate_n_copy() {
				allocator_type alloc;
				size_type size = this->buffer_end_ - this->begin_;
				size_type current_delta = this->current_ - this->begin_;
				size_type new_size = size << 1;
				auto new_begin = 
					std::allocator_traits<decltype(alloc)>::allocate(alloc, new_size);
				for (auto i = this->begin_; i != this->end_; ++i) {
					auto delta = i - this->begin_;
					std::allocator_traits<decltype(alloc)>::construct(alloc, new_begin + delta, *i);
					std::allocator_traits<decltype(alloc)>::destroy(alloc, i);
				}
				//memcpy(new_begin, this->begin_, size * sizeof(value_type));
				std::allocator_traits<decltype(alloc)>::deallocate(alloc, this->begin_, size);
				this->begin_ = new_begin;
				this->current_ = this->begin_ + current_delta;
				this->end_ = this->begin_ + size;
				this->buffer_end_ = this->begin_ + new_size;
			}

		private:

			typename std::allocator_traits<allocator_type>::pointer current_;

			typename std::allocator_traits<allocator_type>::pointer begin_;

			typename std::allocator_traits<allocator_type>::pointer end_;

			typename std::allocator_traits<allocator_type>::pointer buffer_end_;

	};

}


#endif // CURSE_IMPACT_NAIVE_CYCLIC_BUFFER
