#ifndef POTENTIAL_FIELDS_H
#define POTENTIAL_FIELDS_H

#include "team.h"
#include "geometry.h"

#include <vector>

std::vector<Vector> calcRepulsiveForcesFromObstacles(const Vector &pos,
													 const std::vector<obstacle_t> &obstacles,
													 double a, double range);

std::vector<Vector> calcTangentialForcesFromObstacles(const Vector &pos,
													  const std::vector<obstacle_t> &obstacles,
													  double a, double range);

#endif