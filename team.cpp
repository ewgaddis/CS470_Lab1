#include "team.h"

#include "gnuplotter.h"
#include "potentialFields.h"
#include "dumbAgent.h"
#include "pdAgent.h"

#include <vector>

using namespace std;

BZRC *team;
DumbAgent *dumb1;
DumbAgent *dumb2;
PDAgent *pd1;

void world_init(BZRC *my_team)
{
	team = my_team;
	dumb1 = new DumbAgent(team, 2);
	dumb2 = new DumbAgent(team, 1);
	pd1 = new PDAgent(team, 0);
}

void robot_pre_update()
{
}

bool robot_update()
{
	dumb1->Update();
	dumb2->Update();
	pd1->Update();
	/*GNUPlotter plotter;

	cout << "Creating .gpi file..." << endl;

	plotter.createFile("./test.gpi", "My Test");

	cout << "Plotting obstacles..." << endl;

	vector<obstacle_t> obstacles;
	team->get_obstacles(&obstacles);
	plotter.drawObstacles(obstacles);

	cout << "Plotting potential field..." << endl;

	int samples = 50;
	int d = 800 / samples;

	for(int x = -400; x <= 400; x += d)
	{
		for(int y = -400; y <= 400; y += d)
		{
			Vector pos(x, y);

			//vector<Vector> forces = calcRepulsiveForcesFromObstacles(pos, obstacles, 100.0, 0.0, 75.0);
			vector<Vector> forces = calcTangentialForcesFromObstacles(pos, obstacles, 100.0, 5.0, 75.0);

			Vector netForce;

			vector<Vector>::iterator itForce = forces.begin();
			while(itForce != forces.end())
			{
				Vector force = (*itForce);

				netForce += force;
				++itForce;
			}

			if(netForce.lengthSq() < 1.0)
			{
				netForce.normalize();
			}

			plotter.drawArrow(pos.x, pos.y, netForce, 1);
		}
	}

	plotter.finishFile();
	*/

	return true;
}

void robot_post_update()
{
}