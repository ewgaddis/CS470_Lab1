#include "team.h"

#include "gnuplotter.h"

#include <vector>

using namespace std;

BZRC *team;

void world_init(BZRC *my_team)
{
	team = my_team;
}

void robot_pre_update()
{
}

bool robot_update()
{
	GNUPlotter plotter;

	plotter.createFile("./test.gpi", "My Test");

	vector<obstacle_t> obstacles;
	team->get_obstacles(&obstacles);
	plotter.drawObstacles(obstacles);

	plotter.finishFile();

	return false;
}

void robot_post_update()
{
}