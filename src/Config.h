#pragma once

typedef struct Config
{
	int width, height;
	double xUpper = 0.47;
	double xLower = -2.0;
	double yUpper = 1.12;
	double yLower = -1.12;
	bool boxZoom = true;

} Config;
