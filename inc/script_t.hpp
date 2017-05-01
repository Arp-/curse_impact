
#ifndef CURSE_IMPACT_SCRIPT_HPP
#define CURSE_IMPACT_SCRIPT_HPP

#include <iomanip>
#include "gamefield_t.hpp"

class script_t {

	public: //-- public functions --//

		script_t(gamefield_t& gf);

		~script_t() = default;

		void read_xml(const std::string& filepath); // should throw invalid file or file not found

	private: //-- private --//

		gamefield_t& gamefield_;

		time_t time_;

};

#endif // CURSE_IMPACT_SCRIPT_HPP
