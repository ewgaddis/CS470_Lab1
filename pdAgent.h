#include "team.h"
#include "potentialFields.h"

class PDAgent {
	BZRC* myTeam;
	int botIndex;
	Vector* curVector;
	team_t* colorBase;
	string color;
	Vector* baseCenter;
public: PDAgent(BZRC* team, int index);

		void Update();
private:
	boolean isInBase(team_t* team, flag_t* flag);
	void setBaseCenter();
};