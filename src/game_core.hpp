#ifndef GAME_CORE_HPP
#define GAME_CORE_HPP

#include <string>
#include <limits>

namespace wc
{
	// The place where all begins
	struct infos
	{
		unsigned long long population;
		unsigned long long money = 0;
		unsigned long long food = 0;
		unsigned long long water = 0;
	};
	

	class state
	{
		private:
			const unsigned id; // An unique numbe to identify it

		public:
			std::string name; // The name of the state
			infos land_infos;

			state(int id, std::string name);
			~state();
			bool loop();
	};
	
	
	
	
}

#endif
