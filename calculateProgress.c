

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
