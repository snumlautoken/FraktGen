#pragma once
#include <tuple>
#include <vector>
#include <complex>
#include <thread>
#include "Config.h"

class FraktGeneration
{
private:
	std::tuple<float, float, float> mapping[16];
	Config* windowConf;
	int processor_count = std::thread::hardware_concurrency();
	void palette(std::vector<float>* pixels, int x, int y, int iter, int maxIter);
	void calculateMandel(std::vector<float>* pixels, int lowerHeight, int upperHeight);
public:
	FraktGeneration(Config* wc);
	void writeMandel(std::vector<float>& pixels);
};
