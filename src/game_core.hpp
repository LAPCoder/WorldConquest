#pragma once

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
#include <chrono>

typedef unsigned long long ull;
typedef unsigned char      byte;

#define FREQUENCY 60 // In Hz

namespace wc
{
	// The place where all begins
	struct infos
	{
		ull population;
		ull food = 0;
		ull water = 0;

		// In a fight: strength*soldier is your global strength
		// The person with the most strength wins.
		double strength = 1;

		// Used to modify the pop. 1 is neutral, higher is better
		double happiness = 1;

		#define happiness_multiplier .999

		struct money
		{
			ull money = 0;
			double attack_cost = 5; // Per soldier
			double spends = 0; // Everyday
			double income = 0; // Everyday
			float taxes = .0; // In %

			// If taxes > max_taxes, happiness *= (taxes-max_taxes) * taxes_impact
			#define taxes_impact .4
			#define max_taxes .15 // If taxes > max_taxes, happiness drops
			#define calcImpactTaxesOnHappiness(t) ((t) > max_taxes ? ((t)-max_taxes) * taxes_impact:1)
		} money;
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

			explicit state();
			explicit state(const std::string &);
			~state();
			bool loop();
	};
	
	inline state *world;
	inline unsigned nStates;

	bool initWorld(unsigned, std::pair<ull, ull>);
	void exitWorld();
	std::string generateName();
	bool loopAll();
}

#endif
