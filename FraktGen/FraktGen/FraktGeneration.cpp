#include "FraktGeneration.h"

FraktGeneration::FraktGeneration(Config* wc)
{
	mapping[0] = std::tuple<float, float, float>(66.0f / 255.0f, 30.0f / 255.0f, 15.0f / 255.0f);
	mapping[1] = std::tuple<float, float, float>(25.0f / 255.0f, 7.0f / 255.0f, 26.0f / 255.0f);
	mapping[2] = std::tuple<float, float, float>(9.0f / 255.0f, 1.0f / 255.0f, 47.0f / 255.0f);
	mapping[3] = std::tuple<float, float, float>(4.0f / 255.0f, 4.0f / 255.0f, 73.0f / 255.0f);
	mapping[4] = std::tuple<float, float, float>(0.0f / 255.0f, 7.0f / 255.0f, 100.0f / 255.0f);
	mapping[5] = std::tuple<float, float, float>(12.0f / 255.0f, 44.0f / 255.0f, 138.0f / 255.0f);
	mapping[6] = std::tuple<float, float, float>(24.0f / 255.0f, 82.0f / 255.0f, 177.0f / 255.0f);
	mapping[7] = std::tuple<float, float, float>(57.0f / 255.0f, 125.0f / 255.0f, 209.0f / 255.0f);
	mapping[8] = std::tuple<float, float, float>(134.0f / 255.0f, 181.0f / 255.0f, 229.0f / 255.0f);
	mapping[9] = std::tuple<float, float, float>(211.0f / 255.0f, 236.0f / 255.0f, 248.0f / 255.0f);
	mapping[10] = std::tuple<float, float, float>(241.0f / 255.0f, 233.0f / 255.0f, 191.0f / 255.0f);
	mapping[11] = std::tuple<float, float, float>(248.0f / 255.0f, 201.0f / 255.0f, 95.0f / 255.0f);
	mapping[12] = std::tuple<float, float, float>(255.0f / 255.0f, 170.0f / 255.0f, 0.0f / 255.0f);
	mapping[13] = std::tuple<float, float, float>(204.0f / 255.0f, 128.0f / 255.0f, 0.0f / 255.0f);
	mapping[14] = std::tuple<float, float, float>(153.0f / 255.0f, 87.0f / 255.0f, 0.0f / 255.0f);
	mapping[15] = std::tuple<float, float, float>(106.0f / 255.0f, 52.0f / 255.0f, 3.0f / 255.0f);
	windowConf = wc;
}

void FraktGeneration::palette(std::vector<float>* pixels, int x, int y, int iter, int maxIter)
{
	if (iter < maxIter && iter > 0) {
		int i = iter % 16;
		(*pixels)[3 * (windowConf->width * y + x)] = std::get<0>(mapping[i]);
		(*pixels)[3 * (windowConf->width * y + x) + 1] = std::get<1>(mapping[i]);
		(*pixels)[3 * (windowConf->width * y + x) + 2] = std::get<2>(mapping[i]);
	}
	else
	{
		(*pixels)[3 * (windowConf->width * y + x)] = 0.0f;
		(*pixels)[3 * (windowConf->width * y + x) + 1] = 0.0f;
		(*pixels)[3 * (windowConf->width * y + x) + 2] = 0.0f;
	}
}

void FraktGeneration::calculateMandel(std::vector<float>* pixels, int lowerHeight, int upperHeight)
{
	for (int i = lowerHeight; i < upperHeight; ++i)
	{
		for (int j = 0; j < windowConf->width; ++j)
		{
			double x0 = ((double)j / windowConf->width) * (windowConf->xUpper - windowConf->xLower) + windowConf->xLower;
			double y0 = ((double)i / windowConf->height) * (windowConf->yUpper - windowConf->yLower) + windowConf->yLower;
			std::complex<double> z(0, 0);
			std::complex<double> c(x0, y0);
			int iter;
			int maxIter = 1000;
			for (iter = 0; std::abs(z) < 4 && iter < maxIter; ++iter)
			{
				z = z * z + c;
			}

			float color = 1.0f - (float)iter / maxIter;

			int index = 3 * (i + j);

			palette(pixels, j, i, iter, maxIter);
		}
	}

}

void FraktGeneration::writeMandel(std::vector<float>& pixels)
{
	std::vector<std::thread> threadVec;

	for (int i = 0; i < processor_count * 4; ++i)
	{
		int lowerBound = i * windowConf->height / (processor_count * 4);
		int upperBound = (i + 1) * windowConf->height / (processor_count * 4);
		threadVec.push_back(std::thread(&FraktGeneration::calculateMandel, this, &pixels, lowerBound, upperBound));
	}

	for (int i = 0; i < processor_count * 4; ++i)
	{
		threadVec[i].join();
	}
}
