#include "FraktGraphics.h"
#include <thread>
#include <iostream>
#include <chrono>
#include <complex>
#include <vector>
#include <tuple>

int main()
{
	FraktGraphics fGr(900, 900);
	fGr.init();
	fGr.run();
}
