#include "game_core.hpp"

unsigned wc::state::idCounter = 0;
std::vector<std::string> wc::state::allStates = std::vector({std::string("")});

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
	// TODO: change the throws
	#define safeAdd(a, b, min, max) do { \
		     if ((a) > (max))       throw std::overflow_error("The number is too big"); \
		else if ((a) < (min))       throw std::underflow_error("The number is too low"); \
		else if ((b) > (max))       throw std::overflow_error("The number is too big"); \
		else if ((b) < (min))       throw std::underflow_error("The number is too low"); \
		else if ((a) > (max) - (b)) throw std::overflow_error("The number will be too big"); \
		else if ((a) < (min) + (b)) throw std::underflow_error("The number will be too low"); \
		else (a) += (b); \
	} while (0)
	#define safeMultiply(a, b, min, max) do { \
		     if ( (a) > (max)) throw std::overflow_error("The number is too big"); \
		else if ( (a) < (min)) throw std::underflow_error("The number is too low"); \
		else if ( (b) > (max)) throw std::overflow_error("The number is too big"); \
		else if (-(b) > (max)) throw std::overflow_error("The number is too low"); \
		else if ( \
			abs(b) > 1 && /* Multiplier will increase abs(a) */ \
			(long double)(a) > (long double)(max) / (long double)(b) \
		) throw std::overflow_error("The number will be too big"); \
		else if ( \
			abs(b) < 1 && /* Multiplier will decrease abs(a) */ \
			(long double)(a) * (long double)(b) < (long double)(min) \
		) throw std::underflow_error("The number will be too low"); \
		else (a) *= (b); \
	} while (0)

	// Update all variables
	safeMultiply(
		this->land_infos.happiness,
		happiness_multiplier,
		0,
		std::numeric_limits<double>::max()
	);
	safeMultiply(
		this->land_infos.happiness,
		calcImpactTaxesOnHappiness(this->land_infos.money.taxes),
		0,
		std::numeric_limits<double>::max()
	);
	safeMultiply(
		this->land_infos.population,
		this->land_infos.happiness,
		0,
		std::numeric_limits<unsigned>::max()
	);

	if (this->land_infos.population < 1)
		throw std::underflow_error("The number will be too low");


	std::stringstream message;

	message << "#=====================#"
	      << "\n| ID: " << this->id
	      << "\n| Name: " << this->name
	      << "\n| Pop: " << this->land_infos.population
	      << "\n| Strength: " << this->land_infos.strength
	      << "\n| Happiness: " << this->land_infos.happiness
	      << "\n#=====================#"
	      << "\n";

	std::cout << message.str();
	return 0;
}

/**
 * @brief Initialise all the states in one function
 * 
 * @param num_of_states The number of states you want
 * @param pop_range X,Y: The population will be choosen randomly between these numbers.
 * @return true if error
 * @return false if ok
 */
bool wc::initWorld(unsigned num_of_states, std::pair<ull, ull> pop_range)
{
	world = new state[num_of_states];
	nStates = num_of_states;

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::ranlux24 generator(seed);
	std::uniform_int_distribution<int> distribution(pop_range.first,pop_range.second);

	for (unsigned i = 0; i < num_of_states; i++)
	{
		world[i].land_infos.population = distribution(generator);
	}

	return 0;
}

void wc::exitWorld()
{
	if (world != nullptr)
	{
		delete[] world;
		world = nullptr;
	}
	return;
}

std::string wc::generateName()
{
	std::string name, tmp;

	std::ifstream syllabes("common-syllables.txt");

	// The firs line is the most used, then the second...
	#define syllabes_length (323-1) // -1 bc the first line is a comment

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::ranlux24 generator(seed);
	std::uniform_int_distribution<unsigned> uniform_int_distribution(0, syllabes_length);
	std::poisson_distribution<unsigned> poisson_distribution(5.1);

	for (unsigned i = 0; i < poisson_distribution(generator); i++)
	{
		syllabes.seekg(std::ios::beg);
		syllabes.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
		for (unsigned j = 0; j < uniform_int_distribution(generator) - 1; ++j){
			syllabes.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
		}
		syllabes >> tmp;
		name = name + tmp;
	}

	return name;
}

bool wc::loopAll()
{
	while (true)
	{
		auto time_start = std::chrono::system_clock::now().time_since_epoch();

		for (unsigned i = 0; i < wc::nStates; i++)
		{
			try
			{
				world[i].loop();
			}
			catch (std::overflow_error &e)
			{
				std::cerr << e.what() << "\nError: exiting\n";
				wc::exitWorld();
				return 1;
			}
			catch (std::underflow_error &e)
			{
				std::cerr << e.what() << "\nError: exiting\n";
				wc::exitWorld(); // Todo: replace this by deleting the country
				return 1;
			}
		}

		while (std::chrono::system_clock::now().time_since_epoch() <= time_start + std::chrono::milliseconds(1000 / FREQUENCY))
		{}
	}
	return 0;
}
