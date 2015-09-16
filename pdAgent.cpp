#include "pdAgent.h"
#include "geometry.h"
#include <math.h>

PDAgent::PDAgent(BZRC* team, int index){
	myTeam = team;
	botIndex = index;
	curVector = new Vector();
	vector<team_t> teams;
	//team->get_teams(&teams);
	//team_t* teams; = [];// = new array();
	color = "green";
	/*printf("\nget base/color info");
	if (myTeam->get_teams(&teams))
		printf("\nwe got them!");
	else
		printf("\nAngst");
	printf("\nSurvived");
	colorBase = &teams[0];
	int i = 0;
	while (!colorBase->color.compare(color))
	{
		colorBase = &teams[++i];
	}
	colorBase = new team_t(teams[i]);
	setBaseCenter();*/

}

void PDAgent::Update(){
	Vector newDirection = Vector();
	vector <tank_t> myTanks;// = new vector <tank_t>();
	vector <obstacle_t> myObst;
	vector <flag_t> allFlags;
	myTeam->get_mytanks(&myTanks);
	myTeam->get_obstacles(&myObst);
	myTeam->get_flags(&allFlags);
	flag_t goal = allFlags[0];
	Vector aForce;
	//if (myTanks[botIndex].flag.compare("-")){
		int i = 0;
		while (goal.color == color && goal.poss_color != color)// && !isInBase(colorBase, &goal))//&& goal.pos is outside our base
		{
			goal = allFlags[++i];
		}
		printf("\nbotIndex %d", myTanks[botIndex].index);
		printf("\nbotPos %f %f", myTanks[botIndex].pos[0], myTanks[botIndex].pos[1]);
		printf("\ngoalPos %f %f", goal.pos[0], goal.pos[1]);
		printf(goal.color.c_str());
		printf(goal.poss_color.c_str());
		aForce = calcAttractiveForceToGoal(Vector(myTanks[botIndex].pos[0], myTanks[botIndex].pos[1]), Vector(goal.pos[0], goal.pos[1]),
			1, 2000000000, 1);
	//}
	//else {
		//aForce = calcAttractiveForceToGoal(Vector(myTanks[botIndex].pos[0], myTanks[botIndex].pos[1]), *baseCenter,
		//	1, 800, 1000);
		//aForce = Vector();
	//}
	printf("\naForce: %f %f", aForce.x, aForce.y);

	//end frobbing - f = a/dist + b
	//last param = range that obstacles affect bot.
	vector <Vector> rForces = calcRepulsiveForcesFromObstacles(
		Vector(myTanks[botIndex].pos[0], myTanks[botIndex].pos[1]), myObst, 1, 0, 100);
	vector <Vector> tForces = calcTangentialForcesFromObstacles(
		Vector(myTanks[botIndex].pos[0], myTanks[botIndex].pos[1]), myObst, 1, 0, 100);

	newDirection += aForce;
	for each (Vector v in rForces)
	{
		newDirection += v;
	}
	for each(Vector v in tForces)
	{
		newDirection += v;
	}

	//finding the angle
	printf("\nbotAngle: %f", myTanks[botIndex].angle);
	double angle = (acos((curVector->dot(newDirection)/
		(curVector->length()*newDirection.length())))*180.0)/M_1_PI;

	double a = (angle / 60.0)*1650.0;
	printf("\nA: %f", angle);
	//stop and turn to new angle
	myTeam->speed(botIndex, 0);
	Sleep(500);
	myTeam->angvel(botIndex, 1);
	Sleep(a); //turns 60 degrees now
	myTeam->angvel(botIndex, 0);

	myTeam->speed(botIndex, newDirection.length()/1000);
	Sleep(2000);
	myTeam->shoot(botIndex);
	//myTeam->speed(botIndex, newDirection.length());
}
boolean PDAgent::isInBase(team_t* team, flag_t* flag){
	double minx = 1000000;
	double miny = 1000000;
	double maxx = 0;
	double maxy = 0;
	for (int i = 0; i < 4; i++)
	{
		if (minx > team->base_corner[i][0])
			minx = team->base_corner[i][0];
		if (maxx < team->base_corner[i][0])
			maxx = team->base_corner[i][0];
		if (miny > team->base_corner[i][1])
			miny = team->base_corner[i][1];
		if (maxy > team->base_corner[i][1])
			maxy = team->base_corner[i][1];
	}
	if (flag->pos[0] > minx && flag->pos[0]<maxx && flag->pos[1]>miny && flag->pos[1] < maxy)
		return true;
	return false;
}

void PDAgent::setBaseCenter(){
	double minx = 1000000;
	double miny = 1000000;
	double maxx = 0;
	double maxy = 0;
	for (int i = 0; i < 4; i++)
	{
		if (minx > colorBase->base_corner[i][0])
			minx = colorBase->base_corner[i][0];
		if (maxx < colorBase->base_corner[i][0])
			maxx = colorBase->base_corner[i][0];
		if (miny > colorBase->base_corner[i][1])
			miny = colorBase->base_corner[i][1];
		if (maxy > colorBase->base_corner[i][1])
			maxy = colorBase->base_corner[i][1];
	}
	baseCenter = new Vector(maxx - minx, maxy - miny);
}