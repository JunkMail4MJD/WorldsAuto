//#include "ClawBotGlobalConstants.h"

void moveRobot( int targetClawPosition, int targetWristPosition, int targetArmPosition, int armPowerLevel, int drivePowerLevel, float mmToDrive, int drift) {

	int 		degreesOfTravel, targetWheelPosition, currentPowerLevel;
	long 		armCutOffTimer, clawCutOffTimer, wristCutOffTimer, driveCutOffTimer;
	long 		timeIn1mSec;
	bool 		motorsMoving = true, clawFinished = false, armFinished = false, wristFinished = false, driveFinished = false;
	bool		finished;
	float 	leftMotorValue, rightMotorValue, drivePowerLevel_F, currentPowerLevel_F;


	clearTimer(T1);

	degreesOfTravel   = abs( targetArmPosition - (int) getMotorEncoder(armMotor) );
	armCutOffTimer 		= calculateMotorCutOff( degreesOfTravel, armPowerLevel );

	degreesOfTravel   = abs( targetClawPosition - (int) getMotorEncoder(clawMotor) );
	clawCutOffTimer		= calculateMotorCutOff( degreesOfTravel, armPowerLevel );

	degreesOfTravel   = abs( targetWristPosition - (int) getMotorEncoder(wristMotor) );
	wristCutOffTimer	= calculateMotorCutOff( degreesOfTravel, armPowerLevel );

	resetMotorEncoder(leftMotor);
	resetMotorEncoder(rightMotor);
	targetWheelPosition   = (int)(mmToDrive / 210.0 * 360.0);
	driveCutOffTimer	= calculateMotorCutOff( abs(targetWheelPosition), drivePowerLevel );

	drivePowerLevel_F =  (float) drivePowerLevel;

	setMotorTarget(armMotor,  	 targetArmPosition, 		armPowerLevel);
	setMotorTarget(clawMotor,    targetClawPosition, 		100);
	setMotorTarget(wristMotor,   targetWristPosition , 	armPowerLevel);


	while(motorsMoving) {
		timeIn1mSec = time1(T1);

		if ( !clawFinished ) {
			finished 		= calculateProgress( targetClawPosition, 	getMotorEncoder(clawMotor) 	);
			if (finished || (timeIn1mSec > clawCutOffTimer) ) {
				clawFinished = true;
				setMotorSpeed(clawMotor, 0);
			}
			else if (targetClawPosition == PICKUP_BLOCK_CLAW) {
				clawFinished = true;
				setMotorSpeed(clawMotor, 60);
			}
		}

		if ( !wristFinished ) {
			finished 		= calculateProgress( targetWristPosition, getMotorEncoder(wristMotor) );
			if (finished || (timeIn1mSec > wristCutOffTimer) ) {
				wristFinished = true;
				setMotorSpeed(wristMotor, 0);
			}
		}

		if ( !armFinished ) {
			finished 		= calculateProgress( targetArmPosition, 	getMotorEncoder(armMotor) 	);
			if (finished || (timeIn1mSec > armCutOffTimer) ) {
				armFinished = true;
				setMotorSpeed(armMotor, 0);
			}
		}

		if ( !driveFinished) {
			leftMotorValue 		= getMotorEncoder(leftMotor);
			rightMotorValue 	= getMotorEncoder(rightMotor);

			if 			( timeIn1mSec <  50) currentPowerLevel_F = drivePowerLevel_F / 4.0;
			else if ( timeIn1mSec < 100) currentPowerLevel_F = drivePowerLevel_F / 2.0;
			else if ( timeIn1mSec < 150) currentPowerLevel_F = drivePowerLevel_F * (3 / 4);
			else currentPowerLevel_F = drivePowerLevel_F;

			currentPowerLevel = (int) currentPowerLevel_F;

			if ( drift == DRIFT_RIGHT ) {
				setMotorTarget(leftMotor,		 targetWheelPosition,   currentPowerLevel );
				currentPowerLevel = (int) ( currentPowerLevel_F * .98);
				setMotorTarget(rightMotor,	 targetWheelPosition,   currentPowerLevel);
			}
			else if ( drift == DRIFT_MODERATE_RIGHT) {
				setMotorTarget(leftMotor,		 targetWheelPosition,   currentPowerLevel);
				currentPowerLevel = (int) ( currentPowerLevel_F * .94);
				setMotorTarget(rightMotor,	 targetWheelPosition,   currentPowerLevel);
			}
			else if ( drift == DRIFT_FAR_RIGHT) {
				setMotorTarget(leftMotor,		 targetWheelPosition,   currentPowerLevel);
				currentPowerLevel = (int) ( currentPowerLevel_F * .90);
				setMotorTarget(rightMotor,	 targetWheelPosition,   currentPowerLevel);
			}
			else if ( drift == DRIFT_LEFT ) {
				setMotorTarget(rightMotor,	 targetWheelPosition,   currentPowerLevel);
				currentPowerLevel = (int) ( currentPowerLevel_F * .98);
				setMotorTarget(leftMotor,		 targetWheelPosition,   currentPowerLevel);

			}
			else {
				if ( (abs(leftMotorValue) - 3) > abs(rightMotorValue) ) {
					setMotorTarget(rightMotor,	 targetWheelPosition,   currentPowerLevel);
					currentPowerLevel = (int) ( currentPowerLevel_F * .94);
					setMotorTarget(leftMotor,		 targetWheelPosition,   currentPowerLevel);

				}
				else if ( (abs(rightMotorValue) - 3) > abs(rightMotorValue) ) {
					setMotorTarget(leftMotor,		 targetWheelPosition,   currentPowerLevel);
					currentPowerLevel = (int) ( currentPowerLevel_F * .94);
					setMotorTarget(rightMotor,	 targetWheelPosition,   currentPowerLevel);
				}
				else {
					setMotorTarget(leftMotor,		 targetWheelPosition,   currentPowerLevel);
					setMotorTarget(rightMotor,	 targetWheelPosition,   currentPowerLevel);
				}
			}


			finished 		= calculateProgress( targetWheelPosition, 	leftMotorValue 	);
			if (finished || (timeIn1mSec > driveCutOffTimer) ) {
				driveFinished = true;
				setMotorSpeed(leftMotor,  0);
				setMotorSpeed(rightMotor, 0);
			}

		}

		if ( clawFinished && armFinished && wristFinished && driveFinished)
			motorsMoving = false;
	}

}
