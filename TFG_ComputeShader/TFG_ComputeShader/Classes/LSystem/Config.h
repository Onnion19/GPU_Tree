#pragma once

#include <time.h>

using namespace std;
static class Config {
public:
	static const int MAX_BRANCHES = 2046;
	static const int MAX_WIDTH = 16;
	static const int MAX_GENERATIONS = 10;
	static const int MAX_TREE = 150;
	static const int MAX_BUFFER_SIZE = MAX_BRANCHES * MAX_WIDTH * MAX_GENERATIONS * MAX_TREE;
	//Timers
	static clock_t temps_inicial ; 
	static clock_t temps_dades; 
	static clock_t temps_render; 
};