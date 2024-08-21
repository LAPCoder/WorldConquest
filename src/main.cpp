// The main file where all primaries functions are called

#include <iostream>
#include <cstdlib>

#include "game_core.hpp"

int main(int argc, char **argv)
{
	std::atexit(wc::exitWorld);

	return 0;
}
