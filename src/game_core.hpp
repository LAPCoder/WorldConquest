#ifndef GAME_CORE_HPP
#define GAME_CORE_HPP

#include <string>
#include <limits>
#include <thread>
#include <vector>
#include <random>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <functional> 
#include <fstream>
#include <sstream>
#include <iostream>

typedef unsigned long long ull;
typedef unsigned char      byte;

namespace wc
{
	// The place where all begins
	struct infos
	{
		ull population;
		ull money = 0;
		ull food = 0;
		ull water = 0;
	};

	class state
	{
		private:
    		static unsigned idCounter;
			static std::vector<std::string> allStates;
			const  unsigned id; // An unique number to identify it

		public:
			std::string name; // The name of the state
			infos land_infos;

			state();
			state(const std::string &);
			~state();
			bool loop();
	};

	unsigned state::idCounter = 0;
	
	state *world;

	bool initWorld(unsigned, std::pair<ull, ull>);
	void exitWorld();
	std::string generateName();
}

#endif
