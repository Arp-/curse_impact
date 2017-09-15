
#ifndef CURSE_IMPACT_SCRIPT_HPP
#define CURSE_IMPACT_SCRIPT_HPP

#include <iomanip>
#include <map>
#include <vector>
#include "gamefield_t.hpp"
#include "event_t.hpp"
#include "texture_t.hpp"
#include "rect_t.hpp"


class script_t {

	public:

		using event_list_t = std::vector<appear_event_t>;
		using history_t = std::map<int, event_list_t>;
		using texture_list_t = std::vector<std::pair<std::string, texture_t>>;
		using rectangle_list_t = std::vector<std::pair<std::string, rect_t>>;

		using texture_ship_assoc_t = std::map<int, texture_t>;

		using ship_event_container_t = std::map<int, ship_event_list_t>;

	public: //-- public functions --//

		script_t(gamefield_t& gf);

		~script_t() = default;

		void read_xml(const char* filepath); // should throw invalid file or file not found

		void tick();

		void tick_active_ship_list();

		const texture_ship_assoc_t& texture_ship_assoc() const;


	private: //-- private functions --//

		appear_event_t get_event_from_ship_node(const pugi::xml_node&);

		void read_enemy_ship_list(const pugi::xml_node&);

		void read_texture_list(const pugi::xml_node&);

		void read_rectangle_list(const pugi::xml_node&);

		texture_t& texture_ref_by_name(const std::string& name);

		int rect_index_by_name(const std::string& name) const;


	private: //-- private --//

		history_t history_;

		ship_event_container_t ship_event_cont_;

		texture_list_t texture_list_;

		rectangle_list_t rectangle_list_;

		texture_ship_assoc_t texture_ship_association_;

		gamefield_t& gamefield_;

		time_t time_;

};

#endif // CURSE_IMPACT_SCRIPT_HPP
