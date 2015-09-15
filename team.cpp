#include "team.h"

#include "gnuplotter.h"
#include "potentialFields.h"

#include <vector>
#include <conio.h>

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

	cout << "Creating .gpi file..." << endl;

	plotter.createFile("./test.gpi", "My Test");

	cout << "Plotting obstacles..." << endl;

	vector<obstacle_t> obstacles;
	team->get_obstacles(&obstacles);
	plotter.drawObstacles(obstacles);

	vector<flag_t> flags;
	team->get_flags(&flags);

	cout << "Plotting potential field..." << endl;

	Vector goal(flags.at(0).pos[0], flags.at(0).pos[1]);
	drawPotentialField(plotter, (void*)&goal, 25, 0,
					   0.25, 25.0, 1.0);

	plotter.finishFile();

	team->angvel(0, 1.0);

	return false;
}

void robot_post_update()
{
}