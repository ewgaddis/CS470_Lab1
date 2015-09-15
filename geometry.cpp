#include "geometry.h"

Vector operator * (double s, const Vector &v)
{
	return Vector(s * v.x, s * v.y);
}

std::ostream & operator << (std::ostream &out, const Vector &v)
{
	return out << "[" << v.x << ", " << v.y << "]";
}

void closestPointOnLine(const Vector &endpoint0,
						const Vector &endpoint1,
						const Vector &p,
						Vector *q, Vector *v,
						double *dist)
{
	Vector line(endpoint1);
	line -= endpoint0;

	Vector u(p);
	u -= endpoint0;

	double d = line.dot(u);

	Vector closestPoint;

	if(d <= 0.0)
	{
		closestPoint = endpoint0;
	}
	else
	{
		double lineLenSq = line.lengthSq();

		if(d >= lineLenSq)
		{
			closestPoint = endpoint1;
		}
		else
		{
			double lineLen = sqrt(lineLenSq);
			d /= lineLen;

			Vector dir(line.x / lineLen, line.y / lineLen);

			closestPoint = sVector(dir.x * d, dir.y * d) + endpoint0;
		}
	}

	if(q)
	{
		*q = closestPoint;
	}

	if(v || dist)
	{
		Vector w(closestPoint.x - p.x, closestPoint.y - p.y);

		if(v)
		{
			*v = w;
		}

		if(dist)
		{
			*dist = w.length();
		}
	}
}