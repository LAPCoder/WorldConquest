#include "game_core.hpp"

// This is the game core

wc::state::state() : id(idCounter++)
{
	do
	{
		this->name = generateName();
	} while (std::find(allStates.begin(), allStates.end(), this->name) != allStates.end());
	allStates.emplace_back(this->name);
	return;
}

wc::state::state(const std::string &given_name) : id(idCounter++)
{
	this->name = given_name;
	return;
}

wc::state::~state()
{
	return;
}

bool wc::state::loop()
{
	std::stringstream message;

	message << "#=====================#"
	        << "| ID: " << this->id
			<< "| Name: " << this->name
			<< "#=====================#";

	std::cout << message.str();
	return 0;
}

bool wc::initWorld(unsigned &num_of_states, std::pair<ull, ull> &pop_range)
{
	world = new state[num_of_states]();

	for (unsigned i = 0; i < num_of_states; i++)
	{
		world = {};
	}

	return 0;
}

void wc::exitWorld()
{
	delete world;
	return;
}

std::string wc::generateName()
{
	std::string name;

	std::ifstream syllabes("common-syllables.txt");

	// The firs line is the most used, then the second...
	#define syllabes_length (323-1) // -1 bc the firs line is a comment

	std::default_random_engine generator;
	std::geometric_distribution<unsigned> geometric_distribution(0.2);
	std::poisson_distribution<unsigned> poisson_distribution(5.1);

	for (unsigned i = 0; i < poisson_distribution(generator); i++)
	{
		syllabes.seekg(std::ios::beg);
		for(int i=0; i < geometric_distribution(generator) - 1 + 1; ++i){
			syllabes.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
		}
		syllabes >> name;
	}

	return name;
}
