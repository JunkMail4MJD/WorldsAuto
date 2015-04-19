
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
