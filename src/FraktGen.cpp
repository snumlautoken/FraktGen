#include "FraktGraphics.h"
#include <thread>
#include <iostream>
#include <chrono>
#include <complex>
#include <vector>
#include <tuple>

int main(int argc, char *argv[])
{
	int maxIter = 1000;
	if(argc > 1)
		maxIter = atoi(argv[1]);
	FraktGraphics fGr(900, 900);
	fGr.init();
	fGr.run(maxIter);
}
