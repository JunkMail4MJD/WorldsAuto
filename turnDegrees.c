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
