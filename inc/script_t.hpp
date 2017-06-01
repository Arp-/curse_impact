
#ifndef CURSE_IMPACT_SCRIPT_HPP
#define CURSE_IMPACT_SCRIPT_HPP

#include <iomanip>
#include <map>
#include <vector>
#include "gamefield_t.hpp"
#include "event_t.hpp"
#include "texture_t.hpp"

class script_t {


	public:

		using event_list_t = std::vector<event_t>;
		using history_t = std::map<int, event_list_t>;
		using texture_list_t = std::vector<std::pair<std::string, texture_t>>;

		using texture_ship_assoc_t = std::map<int, texture_t>;

	public: //-- public functions --//

		script_t(gamefield_t& gf);

		~script_t() = default;

		void read_xml(const char* filepath); // should throw invalid file or file not found

		void tick();

		const texture_ship_assoc_t& texture_ship_assoc() const;


	private: //-- private functions --//

		event_t get_event_from_ship_node(const pugi::xml_node&);

		void read_enemy_ship_list(const pugi::xml_node&);

		void read_texture_list(const pugi::xml_node&);

		texture_t& texture_ref_by_name(const std::string& name);


	private: //-- private --//

		history_t history_;

		texture_list_t texture_list_;

		texture_ship_assoc_t texture_ship_association_;

		gamefield_t& gamefield_;

		time_t time_;

};

#endif // CURSE_IMPACT_SCRIPT_HPP
