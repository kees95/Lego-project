#pragma config(Sensor, S2,     EyeLeft,        sensorColorNxtFULL)
#pragma config(Sensor, S3,     EyeRight,       sensorLightActive)
#pragma config(Sensor, S4,     ZONAR,          sensorSONAR)


task biep(){
	while(True){
		wait10Msec(75);
		PlaySoundFile("sheep.rso");
	}
}



task main ()
{
int rightThreshold = 45;  //should determine the threshold by calibration
StartTask(biep);
while (true)
{
		while (SensorValue(ZONAR) < 5)
			{
				StopTask(biep)
				motor[motorA] = 0;
				motor[motorB] = 0;
			}
		while (SensorValue(ZONAR) < 20)
		{
			StopTask(biep)
			int j = 35;
			for (int i = 30; i >= 0; i--)
			{
				if (SensorValue(ZONAR) < 5)
			{
				break;
				}
				j -= 2;
				motor[motorA] = i;
				motor[motorB] = i;
				wait1Msec(55);
			}
			motor[motorA] = 0;
			motor[motorB] = 0;
			StopTask(biep)
			wait10Msec(500);
			StartTask(biep);

		}
    if ((SensorValue(EyeLeft) == BLACKCOLOR) && (SensorValue(EyeRight) <= rightThreshold))
    {
        //
        // Both sensors see the line, so go straight.
        //
    		motor[motorA] = 0;
    		motor[motorB] = 0;
    }
    if ((SensorValue(EyeLeft) == BLACKCOLOR)&&(SensorValue(EyeRight)> rightThreshold))
    {
        //
        // Only left sensor sees the line, we must turn left.
        //
        motor[motorA] = 15;
        motor[motorB] = 50;
    }
    if ((SensorValue(EyeLeft) == WHITECOLOR) &&(SensorValue(EyeRight)< rightThreshold))
    {
        //
        // Only right sensor sees the line, turn right.
        //
        motor[motorA] = 50;
        motor[motorB] = 15;
    }
		if  ((SensorValue(EyeLeft) == WHITECOLOR)&&(SensorValue(EyeRight)> rightThreshold))
		{
			motor[motorA] = 35;
      motor[motorB] = 35;
    }

}
}
