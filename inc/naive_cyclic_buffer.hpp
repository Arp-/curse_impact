
#ifndef CURSE_IMPACT_NAIVE_CYCLIC_BUFFER
#define CURSE_IMPACT_NAIVE_CYCLIC_BUFFER

#include <memory>
#include <cstring>

// TODO move this file int some utility directory maybe?
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
				puts(__PRETTY_FUNCTION__);
				allocator_type alloc;
				constexpr size_type initial_value = 8;
				this->begin_ = 
					std::allocator_traits<decltype(alloc)>::allocate(alloc, initial_value);
				printf("this->begin_: %p\n", this->begin_);
				this->end_ = this->begin_;
				this->current_ = this->begin_;
				memset(this->begin_, 0, sizeof(value_type) * initial_value);
				this->buffer_end_ = this->begin_ + initial_value;
			}

			~dynamic_cyclic_buffer_t() {
				puts(__PRETTY_FUNCTION__);
				allocator_type alloc;
				for (auto it = this->begin_; it != this->end_; ++it) {
					std::allocator_traits<decltype(alloc)>::destroy(alloc, it);
				}
				std::allocator_traits<decltype(alloc)>::deallocate(
						alloc, this->begin_, this->buffer_end_ - this->begin_);
			}

			void push_back(const elem_T& elem) {
				puts(__PRETTY_FUNCTION__);
				allocator_type alloc;
				if (this->end_ == this->buffer_end_) { 
					this->reallocate_n_copy(); 
				}
				std::allocator_traits<decltype(alloc)>::construct(alloc, this->end_, elem);
				++this->end_;
			}

			template <typename ...Args> 
			void emplace_back(Args&& ...args) { 
				puts(__PRETTY_FUNCTION__);
				printf("this->begin_: %p\n", this->begin_);
				allocator_type alloc;
				if (this->end_ == this->buffer_end_) { 
					this->reallocate_n_copy(); 
				}
				std::allocator_traits<decltype(alloc)>::construct(alloc, this->end_, args...);
				++this->end_;
			}

			value_type& get() {
				puts(__PRETTY_FUNCTION__);
				return *this->current_;
			}

			const value_type& get() const {
				puts(__PRETTY_FUNCTION__);
				return this->get();
			}

			value_type& ptr_forward() {
				puts(__PRETTY_FUNCTION__);
				++this->current_;
				if (this->current_ == this->end_) {
					this->current_ = this->begin_;
				}
				return *this->current_;
			}

			value_type& ptr_backward() {
				puts(__PRETTY_FUNCTION__);
				--this->current_;
				if (this->current_ < this->begin_) {
					this->current_ = this->end_ -1;
				}
				return *this->current_;
			}

			bool empty() const {
				puts(__PRETTY_FUNCTION__);
				return this->begin_ == this->end_;
			}

		private: //-- private functions --//

			void reallocate_n_copy() {
				puts(__PRETTY_FUNCTION__);
				allocator_type alloc;
				size_type size = this->buffer_end_ - this->begin_;
				size_type current_delta = this->current_ - this->begin_;
				size_type new_size = size << 1;
				printf("new_size %u\n", new_size);
				auto new_begin = 
					std::allocator_traits<decltype(alloc)>::allocate(alloc, new_size);
				printf("new_begin %p\n", new_begin);
				memcpy(new_begin, this->begin_, size * sizeof(value_type));
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
