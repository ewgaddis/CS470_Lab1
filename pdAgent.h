#include "team.h"
#include "potentialFields.h"

class PDAgent {
	BZRC* myTeam;
	int botIndex;
	Vector* curVector;
public: PDAgent(BZRC* team, int index);

		void Update();
};