#pragma config(Motor,   motor1,        wristMotor,    tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,   motor2,        leftMotor,    tmotorVexIQ, PIDControl, reversed, driveRight, encoder)
#pragma config(Sensor,  port5,  			 gyro,          sensorVexIQ_Gyro)
#pragma config(Motor,   motor6,        rightMotor,     tmotorVexIQ, PIDControl, driveLeft, encoder)
#pragma config(Motor,   motor7,        armMotor,      tmotorVexIQ, PIDControl, encoder)
#pragma config(Motor,   motor12,       clawMotor,     tmotorVexIQ, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


//Prototypes & Global Constants & Variable declarations
task displayMyMotorPositions();
long calculateMotorCutOff( int degreesOfTravel, int powerLevel );
bool calculateProgress( int targetPosition, float motorEncoderValue );
void turnDegrees(float degreesToTurn);
void moveToNeutralPosition();
void moveRobot( int targetClawPosition, int targetWristPosition, int targetArmPosition, int armPowerLevel, int drivePowerLevel, float mmToDrive, int drift);



//int const 	MAX_BLOCK_LOADING_STEPS = 3, 			MAX_BLOCK_STACKING_STEPS = 3;
int const REVERSE_GRIP_ARM = 955, REVERSE_GRIP_WRIST = 460;
int const CARRY_BLOCKS_CLAW = -31, CARRY_BLOCKS_ARM = 100, CARRY_BLOCKS_WRIST = 423;
int const PICKUP_BLOCK_CLAW = -30, FULL_OPEN_CLAW = -145, WHEEL_WELL_OPEN_CLAW = -87;
int const STACK_BLOCKS_LOW_WRIST = 250;
int const	STACK_BLOCKS_LOW_ARM = 40;

int const AIM_BLOCKS_HIGH_WRIST = 490;
int const	AIM_BLOCKS_HIGH_ARM = 243;
int const	AIM_REVERSE_GRIP_ARM = 750;

int const STACK_BLOCKS_HIGH_WRIST = 152;
int const	STACK_BLOCKS_HIGH_ARM = 243;

int const DRIFT_LEFT = -1, DRIFT_RIGHT = 1, DRIFT_MODERATE_RIGHT = 2, DRIFT_FAR_RIGHT = 3, STRAIGHT = 0;


//bool	globalBlocksInClaw = false;

task main() {

	/*
	*	using resetMotorEncoder is best. it allows you to repeated go back to a specific motor position for arm, wrist, stacker and claw movements.
	*	combining it with a timer to cancel motor movement if it gets stalled will stop the robot from freezing forever.
	*
	*	WRISTMOTOR - positive rotates the wrist up towards the top of the robot arm and negative rotates the wrist down towards the bottom of the arm
	*	0 degrees is the starting position underneath the arm
	*	490 is roughly with the wrist bent backwards flat against the top of the arm
	*
	*	ARM MOTOR(s) positive rotates the arm up and towards the back of the robot and negative rotates it down towards the stabilizing wheels
	*	0   degrees is the starting position with the arm between the "feet"
	*	955 degrees is with the arm fully backwards in reverse block pickup position
	*
	*	clawMotor negative opens the claw and positive closes the claw
	* 	0 degrees is with the tips of the claw gripper touching but not fully clenched
	*	-87 is about as far as it can open between the feet / wheels
	*	-145 degrees is wide as it can go
	*/

	//	int powerLevel = 100;
	//	int leftMotorSpeed, rightMotorSpeed, wristMotorSpeed, armMotorSpeed;
	long timeIn1mSec;
	float timeInSeconds, mmToDrive;
	int targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel;
	//	int blockGrabbingStep = 0, blockStackingStep = 0;

	resetMotorEncoder(armMotor);
	resetMotorEncoder(clawMotor);
	resetMotorEncoder(wristMotor);

	resetMotorEncoder(leftMotor);
	resetMotorEncoder(rightMotor);

	resetGyro(gyro);
	startGyroCalibration(gyro, gyroCalibrateSamples2048);
	eraseDisplay();
	getGyroCalibrationFlag(gyro);
	displayString(3, "calibrating gyro");
	wait1Msec(500);
	eraseDisplay();

	startTask(displayMyMotorPositions);

	clearTimer(T3);
	// Start our run


	//moveRobot( int targetClawPosition, int targetWristPosition, int targetArmPosition, int armPowerLevel, int drivePowerLevel, float mmToDrive);

	targetClawPosition = 0;
	targetWristPosition = 0;
	targetArmPosition = 250;
	armPowerLevel = 55;
	mmToDrive = - 515.0;
	drivePowerLevel = 100;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, DRIFT_MODERATE_RIGHT);
	//wait1Msec(50);

	//get into Reverse Grip position
	mmToDrive = 0.0;
	armPowerLevel = 50;
	drivePowerLevel = 0;
	targetClawPosition 	= FULL_OPEN_CLAW;
	targetWristPosition = REVERSE_GRIP_WRIST;
	targetArmPosition		=	REVERSE_GRIP_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, STRAIGHT);
	//wait1Msec(100);

	//grab the blocks
	mmToDrive = 0.0;
	armPowerLevel = 100;
	drivePowerLevel = 0;
	targetClawPosition 	= CARRY_BLOCKS_CLAW;
	targetWristPosition = REVERSE_GRIP_WRIST;
	targetArmPosition		=	REVERSE_GRIP_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, STRAIGHT);
	//wait1Msec(100);

	//pull forward to pick up the blocks
	mmToDrive = 210.0;
	armPowerLevel = 100;
	drivePowerLevel = 100;
	targetClawPosition 	= PICKUP_BLOCK_CLAW;
	targetWristPosition = REVERSE_GRIP_WRIST;
	targetArmPosition		=	REVERSE_GRIP_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, DRIFT_MODERATE_RIGHT);
	wait1Msec(100);

	//pick up blocks and put them in the carrying position
	mmToDrive = 0.0;
	armPowerLevel = 100;
	drivePowerLevel = 0;
	targetClawPosition 	= PICKUP_BLOCK_CLAW;
	targetWristPosition = CARRY_BLOCKS_WRIST;
	targetArmPosition		=	CARRY_BLOCKS_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, STRAIGHT);
	wait1Msec(100);

	//Left wheels pull off the ground on start so drift far right for a bit
	mmToDrive = 100.0;
	armPowerLevel = 0;
	drivePowerLevel = 75;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, DRIFT_FAR_RIGHT);

	//Drive to green stacking block hit wall first then backup
	mmToDrive = 1080.0;
	armPowerLevel = 0;
	drivePowerLevel = 75;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, DRIFT_RIGHT);
	wait1Msec(100);


	//Back up from wall
	mmToDrive = -210.0;
	drivePowerLevel = 75;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, DRIFT_MODERATE_RIGHT);
	wait1Msec(50);


	//Stack first 2 blocks
	mmToDrive = 0.0;
	armPowerLevel = 50;
	drivePowerLevel = 0;
	targetClawPosition 	= PICKUP_BLOCK_CLAW;
	targetWristPosition = STACK_BLOCKS_LOW_WRIST;
	targetArmPosition		=	STACK_BLOCKS_LOW_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, DRIFT_RIGHT);
	wait1Msec(200);

	//open claw to release block stack
	targetClawPosition 	= WHEEL_WELL_OPEN_CLAW;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, DRIFT_RIGHT);
	wait1Msec(100);

	//Drive back to Grab remainder of the first row
	mmToDrive = -1000.0;
	armPowerLevel = 45;
	drivePowerLevel = 100;
	targetClawPosition 	= FULL_OPEN_CLAW;
	targetWristPosition = STACK_BLOCKS_HIGH_WRIST;
	targetArmPosition		=	STACK_BLOCKS_HIGH_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, DRIFT_FAR_RIGHT);

	//Drive back to Grab remainder of the first row
	mmToDrive = -890.0;
	armPowerLevel = 45;
	drivePowerLevel = 100;
	targetClawPosition 	= FULL_OPEN_CLAW;
	targetWristPosition = STACK_BLOCKS_HIGH_WRIST;
	targetArmPosition		=	STACK_BLOCKS_HIGH_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, DRIFT_MODERATE_RIGHT);

	//get into Reverse Grip position
	mmToDrive = 0.0;
	armPowerLevel = 60;
	drivePowerLevel = 0;
	targetClawPosition 	= FULL_OPEN_CLAW;
	targetWristPosition = REVERSE_GRIP_WRIST;
	targetArmPosition		=	REVERSE_GRIP_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, STRAIGHT);

	//grab the blocks
	mmToDrive = 0.0;
	armPowerLevel = 100;
	drivePowerLevel = 0;
	targetClawPosition 	= CARRY_BLOCKS_CLAW;
	targetWristPosition = REVERSE_GRIP_WRIST;
	targetArmPosition		=	REVERSE_GRIP_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, STRAIGHT);
	//wait1Msec(100);

	//pull row 1 forward to scoring zone
	mmToDrive = 1610.0;
	armPowerLevel = 100;
	drivePowerLevel = 100;
	targetClawPosition 	= PICKUP_BLOCK_CLAW;
	targetWristPosition = REVERSE_GRIP_WRIST;
	targetArmPosition		=	REVERSE_GRIP_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, DRIFT_FAR_RIGHT);
	wait1Msec(100);


	//Open claw
	mmToDrive = 0;
	armPowerLevel = 100;
	drivePowerLevel = 100;
	targetClawPosition 	= FULL_OPEN_CLAW;
	targetWristPosition = REVERSE_GRIP_WRIST;
	targetArmPosition		=	REVERSE_GRIP_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, STRAIGHT);
	wait1Msec(100);

	//put arm in carrying position while backing up
	mmToDrive = -650.0;
	armPowerLevel = 100;
	drivePowerLevel = 100;
	targetClawPosition 	= FULL_OPEN_CLAW;
	targetWristPosition = CARRY_BLOCKS_WRIST;
	targetArmPosition		=	CARRY_BLOCKS_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, STRAIGHT);
	wait1Msec(100);


	//Turn left 80 degrees
	turnDegrees( 80.0);


	//Drive forward until hitting the wall
	mmToDrive = 950.0;
	armPowerLevel = 100;
	drivePowerLevel = 100;
	targetClawPosition 	= WHEEL_WELL_OPEN_CLAW;
	targetWristPosition = AIM_BLOCKS_HIGH_WRIST;
	targetArmPosition		=	AIM_BLOCKS_HIGH_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, STRAIGHT);
	wait1Msec(100);

	//back away from the wall
	mmToDrive = -65.0;
	armPowerLevel = 100;
	drivePowerLevel = 100;
	targetClawPosition 	= WHEEL_WELL_OPEN_CLAW;
	targetWristPosition = AIM_BLOCKS_HIGH_WRIST;
	targetArmPosition		=	AIM_BLOCKS_HIGH_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, STRAIGHT);
	wait1Msec(100);

	//Turn right
	turnDegrees( -95.0);

	//Drive backward while drifting right to hug the wall until we hit it
	mmToDrive = -1200.0;
	armPowerLevel = 100;
	drivePowerLevel = 100;
	targetClawPosition 	= WHEEL_WELL_OPEN_CLAW;
	targetWristPosition = AIM_BLOCKS_HIGH_WRIST;
	targetArmPosition		=	AIM_BLOCKS_HIGH_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, DRIFT_RIGHT);
	wait1Msec(100);

	//back up to drop the arm
	mmToDrive = 230.0;
	armPowerLevel = 50;
	drivePowerLevel = 100;
	targetClawPosition 	= FULL_OPEN_CLAW;
	targetWristPosition = REVERSE_GRIP_WRIST;
	targetArmPosition		=	AIM_REVERSE_GRIP_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, DRIFT_LEFT);
	//Grab row


		//get into Reverse Grip position
	mmToDrive = 0.0;
	armPowerLevel = 50;
	drivePowerLevel = 0;
	targetClawPosition 	= FULL_OPEN_CLAW;
	targetWristPosition = REVERSE_GRIP_WRIST;
	targetArmPosition		=	REVERSE_GRIP_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, STRAIGHT);
	//wait1Msec(100);

	//grab the blocks
	mmToDrive = 0.0;
	armPowerLevel = 100;
	drivePowerLevel = 0;
	targetClawPosition 	= CARRY_BLOCKS_CLAW;
	targetWristPosition = REVERSE_GRIP_WRIST;
	targetArmPosition		=	REVERSE_GRIP_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, STRAIGHT);
	//wait1Msec(100);

	//pull forward to drag blocks to scoring zone
	mmToDrive = 1600.0;
	armPowerLevel = 100;
	drivePowerLevel = 100;
	targetClawPosition 	= PICKUP_BLOCK_CLAW;
	targetWristPosition = REVERSE_GRIP_WRIST;
	targetArmPosition		=	REVERSE_GRIP_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, DRIFT_LEFT);
	wait1Msec(100);


	//Let go of the blocks
	mmToDrive = 0.0;
	armPowerLevel = 100;
	drivePowerLevel = 0;
	targetClawPosition 	= FULL_OPEN_CLAW;
	targetWristPosition = REVERSE_GRIP_WRIST;
	targetArmPosition		=	REVERSE_GRIP_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, STRAIGHT);
	//wait1Msec(100);

	//go over red blocks and grab them
	mmToDrive = -20.0;
	armPowerLevel = 100;
	drivePowerLevel = 100;
  targetClawPosition 	= PICKUP_BLOCK_CLAW;
	targetWristPosition = REVERSE_GRIP_WRIST;
	targetArmPosition		=	REVERSE_GRIP_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, DRIFT_LEFT);

	//move to stacking position red
	mmToDrive = -210;
	armPowerLevel = 100;
	drivePowerLevel = 100;
	targetClawPosition = FULL_OPEN_CLAW;
  targetWristPosition = REVERSE_GRIP_WRIST;
	targetArmPosition		=	REVERSE_GRIP_ARM;
	moveRobot( targetClawPosition, targetWristPosition, targetArmPosition, armPowerLevel, drivePowerLevel, mmToDrive, DRIFT_LEFT);

	moveToNeutralPosition();


	// Print out the total time it took to do our run

	timeIn1mSec = time1(T3);
	timeInSeconds = timeIn1mSec / 1000.0;

	stopTask(displayMyMotorPositions);

	eraseDisplay();
	displayString(3, "Elapsed Time: %f", timeInSeconds);


	repeat(10) {
		wait1Msec(1000);
	}

}


task displayMyMotorPositions(){

	float clawEncoderValue, wristEncoderValue, armEncoderValue, calibratedGyroDegrees, calibratedGyroHeading;
	float leftMotorValue, rightMotorValue;



	while(true) {

		calibratedGyroDegrees = getGyroDegreesFloat(gyro);
		calibratedGyroHeading = getGyroHeadingFloat(gyro);

		clawEncoderValue 		= getMotorEncoder(clawMotor);
		wristEncoderValue 	= getMotorEncoder(wristMotor);
		armEncoderValue 		= getMotorEncoder(armMotor);
		leftMotorValue			= getMotorEncoder(leftMotor);
		rightMotorValue			= getMotorEncoder(rightMotor);

		eraseDisplay();
		displayString(0, "claw:  %f3", clawEncoderValue);
		displayString(1, "wrist: %f3", wristEncoderValue);
		displayString(2, "arm:   %f3", armEncoderValue);
		displayString(3, "L: %f3  R: %f3", leftMotorValue, rightMotorValue);
		displayString(4, "d: %f3  h: %f3", calibratedGyroDegrees, calibratedGyroHeading);

		wait1Msec(500);
	}
}


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

bool calculateProgress( int targetPosition, float motorEncoderValue ){

	float progress, progressDifference;
	bool finished;

	if (targetPosition != 0 )		progress = motorEncoderValue / (float) targetPosition;
	else 	{
		progressDifference = abs( motorEncoderValue - (float) targetPosition);
		if (progressDifference < 5.0) progress = 1.0;
		else progress = 0.5;
	}

	finished = false;
	if ( (progress > .98) && (progress < 1.02) ) finished = true;

	return finished;
}




void 	moveToNeutralPosition() {

	//	Position for being able to Drive and clear block height
	int powerLevel = 50, armPosition = 0, wristPosition = 0, clawPosition = 0;


	moveRobot( clawPosition, wristPosition, armPosition, powerLevel, 0, 0.0, STRAIGHT);

}


long calculateMotorCutOff( int degreesOfTravel, int powerLevel ) {
	long mSeconds;
	float degreesOfTravel_F, powerLevel_F, mSeconds_F;

	degreesOfTravel_F = (float) degreesOfTravel;
	powerLevel_F = (float) powerLevel;


	/**           margin of error for cutoff timer 50%
	*
	*        1000 mSeconds     60 Seconds        100 Power level              1 Revolution      degreesOfTravel (input)
	*        _____________  *  ____________ *  ___________________________ *  ____________  *  ____________
	*          1  Second        1 Minute         110 Revolutions / minute     360 Degrees       powerLevel (input)
	*
	*
	*/

	mSeconds = 0;

	if (powerLevel != 0) {
		mSeconds_F = ( 1.5 * (1000.0*60.0*(100.0/110.0)*(1.0/360.0)*( degreesOfTravel_F / powerLevel_F )));
		mSeconds = (long) mSeconds_F;
	}
	return mSeconds;

}

void turnDegrees(float degreesToTurn){
	/*
	for the gyro it registers
	positive degrees for turns left
	negative degrees for turns right
	*/

	int 	powerLevel = 25;
	bool  keepTurning;
	bool  turnRobotLeft;
	float degreesTurned;


	resetGyro(gyro);

	if (degreesToTurn >= 0) turnRobotLeft = true;
	else turnRobotLeft = false;

	keepTurning = true;

	if (turnRobotLeft) {
		setMotorSpeed(rightMotor, powerLevel);
		setMotorSpeed(leftMotor, -powerLevel);
	}
	else {
		setMotorSpeed(rightMotor,-powerLevel);
		setMotorSpeed(leftMotor, powerLevel);
	}

	while ( keepTurning ){

		degreesTurned = getGyroDegreesFloat(gyro);
		if ( turnRobotLeft && (     abs(degreesTurned) > abs( degreesToTurn - 1.5)) ) {
			keepTurning = false;
			setMotorSpeed(rightMotor, 0);
			setMotorSpeed(leftMotor,  0);
		}
		if ( !turnRobotLeft && ( abs(degreesTurned) > abs( degreesToTurn + 1.5)) ) {
			keepTurning = false;
			setMotorSpeed(rightMotor, 0);
			setMotorSpeed(leftMotor,  0);
		}
	}
	setMotorSpeed(rightMotor, 0);
	setMotorSpeed(leftMotor,  0);

	return;

}
