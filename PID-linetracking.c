#pragma config(Sensor, S1,     lichtsensor,    sensorLightActive)
#pragma config(Sensor, S2,     zonar,          sensorSONAR)
#pragma config(Sensor, S4,     kleursensor,    sensorColorNxtFULL)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define snelheid 40

int PowerA;
int PowerB;
int Tp = 0; //variabele voor basissnelheid
int basistoestand;

void calibrate ()
{
	int sensor;
	int min = 100;
	int max = 0;
	motor[motorA] = 10;
	motor[motorB] = -10;
	for (int i = 0; i < 600; i++)
	{
		sensor = SensorValue(lichtsensor);
		if (sensor <= min)
		{
			min = sensor;
		}
		else if (sensor >= max)
		{
			max = sensor;
		}
		if (i == 99)
		{
			motor[motorA] = -10;
			motor[motorB] = 10;
		}
		if (i == 299)
		{
			motor[motorA] = 10;
			motor[motorB] = -10;
		}
		if (i == 499)
		{
			motor[motorA] = -10;
			motor[motorB] = 10;
		}
		wait1Msec(10);
	}
	basistoestand = (max + min)/2;
}

task kruispuntdetectie()
{
	while(1)
	{

	if (SensorValue(kleursensor) == BLACKCOLOR)
	{
		Tp = 0;
	}
}
}

task accelerate() // hoogt geleidelijk de basissnelheid op
{
	for (int i = 0; i <= snelheid; i++)
	{
		wait10Msec(8);
		Tp = i;
	}
	stopTask(accelerate);
}

task geluid() // produceert geluid
{
	while(1)
	{
			playSoundFile("paardg.rso");
	}
}

task main()
{
	calibrate();
	startTask(geluid);
	startTask(accelerate);
	startTask(kruispuntdetectie);
	int Kp = 5; // proportionele constante
	int basistoestand = 46;// rand van de lijn
	while (true)
	{
		while (SensorValue(zonar) > 20)
		{
			int LightValue = SensorValue(lichtsensor);
			int error = LightValue - basistoestand; // afstand van basistoestand
			int Turn = Kp * error; //berekent hoe sterk de motorwaarde veranderd
			PowerA = Tp + Turn; //verandering van motorwaardeA ten opzichte van basissnelheid
			PowerB = Tp - Turn; //verandering van motorwaardeB ten opzichte van basissnelheid
			motor[motorA] = PowerA;
			motor[motorB] = PowerB;
			nVolume = 4;// de motor staat aan dus geluid moet aan
			wait1Msec(1);
		}

		motor[motorA] = 0;
		motor[motorB] = 0;
		nVolume = 0;// de motor staat uit dus geluid moet uit
		wait1Msec(1);
	}
}
