#ifndef CURSE_IMPLACT_BORDER_DRAWER_T_HPP
#define CURSE_IMPLACT_BORDER_DRAWER_T_HPP

#include <ncurses.h>
#include "position_t.hpp"
#include "rect_t.hpp"
#include "util.hpp"

namespace display {

// wanted to try this builder pattern
class border_drawer_t {

	private: 

		struct style_t {
			style_t() = default;
			constexpr style_t(char h, char v, char c):
				horizontal_(h), vertical_(v), corner_(c) { }
			char horizontal_;
			char vertical_;
			char corner_;
		};

		class builder_t {

			public: //-- public members --//

				builder_t() = default;

				~builder_t() = default;

				builder_t& window(WINDOW* win);

				builder_t& position(const position_t& position);

				builder_t& rect(const rect_t& rect);

				builder_t& style(char vertical, char horizontal, char corner);

				util::naive_optional<border_drawer_t> build();


			private: //-- private member variables --//

				WINDOW* win_;

				rect_t rect_;

				position_t pos_;

				style_t style_;

		};


	public: //-- public functions --//

		border_drawer_t() = default;

		~border_drawer_t() = default;

		void draw();


	public: //-- public static functions --//

		static builder_t builder();


	private: //-- private members funcions --//


		border_drawer_t(WINDOW* win, position_t pos, rect_t rect, style_t style);


	private:

		WINDOW* window_;

		position_t position_;

		rect_t rect_;

		style_t style_;

		friend class border_drawer_t::builder_t;

};

}

#endif // CURSE_IMPLACT_BORDER_DRAWER_T_HPP
