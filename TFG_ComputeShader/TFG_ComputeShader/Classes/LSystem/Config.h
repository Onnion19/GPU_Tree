#pragma once

using namespace std;
static class Config {
public:
	static const int MAX_BRANCHES = 2048;
	static const int MAX_WIDTH = 16;
	static const int MAX_GENERATIONS = 10;
	static const int MAX_BUFFER_SIZE = MAX_BRANCHES * MAX_WIDTH * MAX_GENERATIONS;
};