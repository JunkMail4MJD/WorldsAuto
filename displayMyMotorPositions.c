

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

		wait1Msec(1000);
	}
}
