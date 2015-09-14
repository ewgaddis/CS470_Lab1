#include "bot.h"

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
	static int counter = 0;

	team->speed(0, 1);
	Sleep(1000);
	team->shoot(0);
	Sleep(500);
	team->speed(0, -.5);
	Sleep(500);
	team->angvel(0, .5);
	Sleep(100);
	team->angvel(0, 0);
	team->shoot(0);
	Sleep(500);

	++counter;
	if(counter > 30)
	{
		return false;
	}

	return true;
}

void robot_post_update()
{
}