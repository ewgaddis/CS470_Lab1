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
DumbAgent *dumb3;
PDAgent *pd1;
vector <PDAgent*> agents;

void world_init(BZRC *my_team)
{
	team = my_team;
	vector<tank_t> tanks;
	team->get_mytanks(&tanks);
	//dumb1 = new DumbAgent(team, 0);
	//dumb2 = new DumbAgent(team, 1);
	//dumb3 = new DumbAgent(team, 2);
	//pd1 = new PDAgent(team, 0);
	agents = vector<PDAgent*>();
	for (int i = 0; i < 3; i++)
	{
		agents.push_back(new PDAgent(team, i));
		//agents.at(i) = new PDAgent(team, i);
	}

}

void robot_pre_update()
{
}

bool robot_update()
{
	//dumb1->Update();
	//dumb2->Update();
	//dumb3->Update();
	//pd1->Update();
	for (int i = 0; i < agents.size(); i++)
	{
		string color;
		if (i % 3 == 0){
			color = "red";
		}
		else if (i % 3 == 1)
			color = "blue";
		if (i % 3 == 2)
			color = "purple";

		agents.at(i)->Update(color);
	}
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