
#ifndef CURSE_IMPACT_SCRIPT_HPP
#define CURSE_IMPACT_SCRIPT_HPP

#include <iomanip>
#include <map>
#include <vector>
#include "gamefield_t.hpp"
#include "event_t.hpp"

class script_t {


	public:

		using event_list_t = std::vector<event_t>;
		using history_t = std::map<int, event_list_t>;

	public: //-- public functions --//

		script_t(gamefield_t& gf);

		~script_t() = default;

		void read_xml(const char* filepath); // should throw invalid file or file not found

		void tick();


	private: //-- private functions --//

		event_t get_event_from_ship_node(const pugi::xml_node&);


	private: //-- private --//

		history_t history_;

		gamefield_t& gamefield_;

		time_t time_;

};

#endif // CURSE_IMPACT_SCRIPT_HPP
