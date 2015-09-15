#include "gnuplotter.h"

#include <iostream>

using namespace std;

GNUPlotter::GNUPlotter() : file(0)
{
}

void GNUPlotter::createFile(const char *fileName, const char *title)
{
	file = fopen(fileName, "w");

	if(!file)
	{
		cout << "Failed to create file \"" << fileName << "\" for gnuplot" << endl;
		return;
	}

	fprintf(file, "set title \"%s\"\n", title);
	fprintf(file, "set xrange [-400.0: 400.0]\n");
	fprintf(file, "set yrange [-400.0: 400.0]\n");
	fprintf(file, "unset key\n");
	fprintf(file, "set size square\n");
}

void GNUPlotter::drawArrow(double x, double y, const Vector &v, int color)
{
	if(!file)
	{
		return;
	}

	fprintf(file, "set arrow from %f, %f to %f, %f lt %i\n",
			x - v.x / 2.0, y - v.y / 2.0,
			x + v.x / 2.0, y + v.y / 2.0,
			color);
}

void GNUPlotter::drawLine(double x1, double y1, double x2, double y2, int color)
{
	if(!file)
	{
		return;
	}

	fprintf(file, "set arrow from %f, %f to %f, %f nohead lt %i\n",
			x1, y1, x2, y2, color);
}

void GNUPlotter::drawObstacles(const vector<obstacle_t> &obstacles)
{
	if(!file)
	{
		return;
	}

	vector<obstacle_t>::const_iterator itObstacle = obstacles.begin();
	while(itObstacle != obstacles.end())
	{
		const obstacle_t *o = &(*itObstacle);

		for(int c = 0; c < o->numCorners; ++c)
		{
			int e0 = c;
			int e1 = (c + 1 == o->numCorners ? 0 : c + 1);

			drawLine(o->o_corner[e0][0], o->o_corner[e0][1],
					 o->o_corner[e1][0], o->o_corner[e1][1],
					 3);
		}

		++itObstacle;
	}
}

void GNUPlotter::finishFile()
{
	if(file)
	{
		fprintf(file, "plot '-' with lines\n");
		fprintf(file, "0 0 0 0\ne\n");

		fclose(file);
		file = 0;
	}
}