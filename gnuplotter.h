#ifndef GNUPLOTTER_H
#define GNUPLOTTER_H

#include "team.h"
#include "geometry.h"

#include <vector>

class GNUPlotter
{
private:
	FILE *file;

public:
	GNUPlotter();

	void createFile(const char *fileName, const char *title);
	void finishFile();

	void drawArrow(double x, double y, const Vector &v);
	void drawLine(double x1, double y1, double x2, double y2, int color);
	void drawObstacles(const std::vector<obstacle_t> &obstacles);
};

#endif