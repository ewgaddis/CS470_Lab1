#include "potentialFields.h"

using namespace std;

void getMinDistV(const obstacle_t *obstacle,
				 const Vector &pos,
				 Vector *v, double *dist)
{
	Vector minV;
	double minDist = 999999.0;

	// Looks at each line of the obstacle defined by
	// its corner points
	for(int corner = 0; corner < obstacle->numCorners; ++corner)
	{
		// Gets the endpoints of the line
		int e0 = corner;
		int e1 = (corner + 1 == obstacle->numCorners ? 0 : corner + 1);

		Vector c0(obstacle->o_corner[e0][0],
					obstacle->o_corner[e0][1]);
		Vector c1(obstacle->o_corner[e1][0],
					obstacle->o_corner[e1][1]);

		// Gets the vector and distance from the closest
		// point on the line according to the given
		// position
		Vector vec;
		double distance;

		closestPointOnLine(c0, c1, pos, 0, &vec, &distance);

		// Only considers the shortest distance to the
		// obstacle
		if(distance < minDist)
		{
			minDist = distance;
			minV    = vec;
		}
	}

	*v    = minV;
	*dist = minDist;
}

vector<Vector> calcRepulsiveForcesFromObstacles(const Vector &pos,
												const vector<obstacle_t> &obstacles,
												double a, double b, double range)
{
	// A list of repulsive forces
	vector<Vector> forces;

	// Goes through each obstacle
	vector<obstacle_t>::const_iterator itObstacle = obstacles.begin();
	while(itObstacle != obstacles.end())
	{
		Vector v;
		double k;

		// Gets the vector and distance to the obstacle
		getMinDistV(&(*itObstacle), pos, &v, &k);

		if(k > 0.0)
		{
			// Gets the force direction, which is the negative
			// direction of the vector to the "closest" line
			Vector forceDir(-v.x / k, -v.y / k);

			// Calculates the magnitude of the force
			double forceMag = 0.0;

			if(k < range)
			{
				// Force = 1 / k
				forceMag = a / k + b;
			}

			Vector force(forceDir * forceMag);

			if(force.lengthSq() > 0.0)
			{
				// Adds the force to the list
				forces.push_back(force);
			}
		}

		++itObstacle;
	}

	return forces;
}

vector<Vector> calcTangentialForcesFromObstacles(const Vector &pos,
												 const vector<obstacle_t> &obstacles,
												 double a, double b, double range)
{
	// A list of repulsive forces
	vector<Vector> forces;

	// Goes through each obstacle
	vector<obstacle_t>::const_iterator itObstacle = obstacles.begin();
	while(itObstacle != obstacles.end())
	{
		Vector v;
		double k;

		// Gets the vector and distance to the obstacle
		getMinDistV(&(*itObstacle), pos, &v, &k);

		if(k > 0.0)
		{
			// Gets the force direction, which is the
			// direction perpendicular to the vector to the
			// "closest" line
			Vector forceDir;
			Vector(v.x / k, v.y / k).perpendicular(&forceDir);

			// Calculates the magnitude of the force
			double forceMag = 0.0;

			if(k < range)
			{
				// Force = 1 / k
				forceMag = a / k + b;
			}

			Vector force(forceDir * forceMag);

			if(force.lengthSq() > 0.0)
			{
				// Adds the force to the list
				forces.push_back(force);
			}
		}

		++itObstacle;
	}

	return forces;
}