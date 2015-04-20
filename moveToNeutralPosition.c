//#include "ClawBotGlobalConstants.h"

void 	moveToNeutralPosition() {

	//	Position for being able to Drive and clear block height
	int powerLevel = 50, armPosition = 0, wristPosition = 0, clawPosition = 0;


	moveRobot( clawPosition, wristPosition, armPosition, powerLevel, 0, 0.0, STRAIGHT);

}
