#include "pdAgent.h"
#include "geometry.h"
#include <math.h>

PDAgent::PDAgent(BZRC* team, int index){
	myTeam = team;
	botIndex = index;
	curVector = new Vector();
}

void PDAgent::Update(){
	Vector newDirection = Vector();
	vector <tank_t> myTanks;
	vector <obstacle_t> myObst;
	myTeam->get_mytanks(&myTanks);
	myTeam->get_obstacles(&myObst);
	vector <Vector> rForces = calcRepulsiveForcesFromObstacles(
		Vector(myTanks[botIndex].pos[0], myTanks[botIndex].pos[1]), myObst, 1, 1, 1);
	vector <Vector> tForces = calcTangentialForcesFromObstacles(
		Vector(myTanks[botIndex].pos[0], myTanks[botIndex].pos[1]), myObst, 1, 1, 1);
	for each (Vector v in rForces)
	{
		newDirection += v;
	}
	for each(Vector v in tForces)
	{
		newDirection += v;
	}

	//finding the angle
	double angle = (acos((curVector->dot(newDirection)/
		(curVector->length()*newDirection.length())))*180.0)/M_1_PI;

	double a = (angle / 60)+1650;
	//stop and turn to new angle
	myTeam->speed(botIndex, 0);
	Sleep(500);
	myTeam->angvel(botIndex, 1);
	Sleep(a); //turns 60 degrees now
	myTeam->angvel(botIndex, 0);

	myTeam->speed(botIndex, 1);
	Sleep(2000);
	myTeam->shoot(botIndex);
	myTeam->speed(botIndex, 1);
}