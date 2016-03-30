#pragma config(Sensor, S1,     EyeRight,       sensorLightActive)
#pragma config(Sensor, S2,     EyeLeft,        sensorColorNxtFULL)
#pragma config(Sensor, S4,     sonarsensor,    sensorSONAR)
#pragma config(Motor,  motorA,          right,         tmotorNXT, PIDControl, driveRight, encoder)
#pragma config(Motor,  motorB,          left,          tmotorNXT, PIDControl, driveLeft, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define rightThreshold 45
#define speedlimit 80

int i = 0;

task accelerate();
task deaccelerate();
task geluid();

task geluid()
{
	while(1)
	{
		playSoundFile("paardg.rso");
		nVolume = 4;
	}
}

task deaccelerate ()
{
	stopTask(accelerate);
	int k = i;
	for (int j = k; j >= 0; j--)
	{
		i = j;
		wait1Msec(3);
	}
	startTask(accelerate);
	stopTask(deaccelerate);
}

task accelerate ()
{
	stopTask(deaccelerate);
	for (int j = 0; j <= speedlimit; j++)
	{
		wait10Msec(3);
		i = j;
	}
	stopTask(accelerate);
}

task main()
{
	startTask(geluid);
	while(1)
	{
		startTask(accelerate);
		while (SensorValue(sonarsensor) > 5)
		{
			startTask(geluid);
			while (SensorValue(sonarsensor) > 30)
			{
				if ((SensorValue(EyeLeft) == BLACKCOLOR) && (SensorValue(EyeRight) <= rightThreshold))
    		{
      	  	//
      	  	// Beide sensoren zien lijn, oftewel kruispunt.
      	  	//
    				clearSounds();
    				motor[left] = 0;
    				motor[right] = 0;
   	 		}

    		if ((SensorValue(EyeLeft) == BLACKCOLOR)&&(SensorValue(EyeRight)> rightThreshold))
    		{
        		//
        		// Only left sensor sees the line, we must turn left.
        		//
        		motor[left] = i;
        		motor[right] = i/(0.1 * speedlimit);
    		}

    		if ((SensorValue(EyeLeft) == WHITECOLOR) &&(SensorValue(EyeRight)< rightThreshold))
    		{
       	 		//
       	 		// Only right sensor sees the line, turn right.
        		//
        		motor[left] = i/(0.1 * speedlimit);
        		motor[right] = i;
    		}

				if ((SensorValue(EyeLeft) == WHITECOLOR)&&(SensorValue(EyeRight) > rightThreshold))
				{
					motor[left] = i/2;
      		motor[right] = i/2;
    		}
    		wait1Msec(5);
   		}
   		clearSounds();
   		startTask(deaccelerate);
		}
		clearSounds();
		i = 0;

	}
}
