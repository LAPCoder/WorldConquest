// The main file where all primaries functions are called

#include <iostream>
#include <cstdlib>

#include "game_core.hpp"

int main(int argc, char **argv)
{
	std::atexit(wc::exitWorld);

	wc::initWorld(5, std::make_pair(10, 20));
	wc::loopAll();
	wc::exitWorld();

	return 0;
}
