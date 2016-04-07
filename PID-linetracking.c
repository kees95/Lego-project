#pragma config(Sensor, S1,     lichtsensor,    sensorLightActive)
#pragma config(Sensor, S2,     zonar,          sensorSONAR)
#pragma config(Sensor, S4,     kleursensor,    sensorColorNxtFULL)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define snelheid 30

const int kMaxSizeOfMessage = 30;
const int INBOX = 5;
int bluetooth_1;
int PowerA;
int PowerB;
int Tp = 0; //variabele voor basissnelheid
int basistoestand = 0;
int stuurwaarde = 0;

task aan_uit ();
task accelerate();
task deaccelerate();
task linetracking();
task kruispuntdetectie();
task geluid();

task aan_uit ()
{
	while (1)
	{
		if (bluetooth_1 == 1)
		{
			stopTask(linetracking);
			startTask(linetracking);
			startTask(geluid);
		}
		if (bluetooth_1 == 2)
		{
			stopTask(linetracking);
			stopTask(geluid);
			clearSounds();
			motor[motorA] = 0;
			motor[motorB] = 0;
		}
	}
}

int bluetooth()
{
  TFileIOResult nBTCmdRdErrorStatus;
  int nSizeOfMessage;
  ubyte nRcvBuffer[kMaxSizeOfMessage];

	while (true)
  {
    // Check to see if a message is available
    nSizeOfMessage = cCmdMessageGetSize(INBOX);

    if (nSizeOfMessage > kMaxSizeOfMessage)
      nSizeOfMessage = kMaxSizeOfMessage;
    if (nSizeOfMessage > 0){
    	nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, INBOX);
    	nRcvBuffer[nSizeOfMessage] = '\0';
    	string s = "";
    	stringFromChars(s, (char *) nRcvBuffer);
    	displayCenteredBigTextLine(4, s);
    	if(s == "UP"){
    			return 1;
    	}
     if(s == "DOWN"){
    			return 2;
    	}
    	if(s == "LEFT"){
    			return 3;
    	}
    	if(s == "RIGHT"){
    			return 4;
    	}
    	if(s == "A"){
    			return 5;
    	}
    	if(s == "B"){
    			return 6;
    	}
    	if(s == "C"){
    			return 7;
    	}
    }
    wait1Msec(100);
  }
  //Als er niets gebeurt 0 retourneren.
  return 0;
}

void calibrate ()
{
	int sensor;
	int min = 100;
	int max = 0;
	motor[motorA] = 20;
	motor[motorB] = -20;
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
			motor[motorA] = -20;
			motor[motorB] = 20;
		}
		if (i == 299)
		{
			motor[motorA] = 20;
			motor[motorB] = -20;
		}
		if (i == 499)
		{
			motor[motorA] = -20;
			motor[motorB] = 20;
		}
		wait1Msec(10);
	}
	basistoestand = (max + min)/2;
}

task linetracking ()
{
	int Kp = 2;
	int Ki = 0.2;
	int Kd = 11;
	int integraal = 0;
	int vorige_error = 0;
	while (true)
	{
		while (SensorValue(zonar) > 10)
		{
				int LightValue = SensorValue(lichtsensor);
				stuurwaarde = basistoestand;
				int error = LightValue - stuurwaarde; // afstand van basistoestand
				int afgeleide = error - vorige_error; //afgeleide kijkt naar het verschil tussen twee opeenvolgende errors om zo te voorspellen wat de volgende error zal zijn
				integraal = integraal + error; //de integraal onthoud de som van variabele error, om de afwijking uit het verleden te corrigeren
				int Turn = (Kp * error) + (Ki*integraal) + (Kd * afgeleide); //berekent hoe sterk de motorwaarde veranderd
				PowerA = Tp - Turn; //verandering van motorwaardeA ten opzichte van basissnelheid
				PowerB = Tp + Turn; //verandering van motorwaardeB ten opzichte van basissnelheid
				motor[motorA] = PowerA;
				motor[motorB] = PowerB;
				vorige_error = error;
				if (motor[motorA] != 0)
					nVolume = 4;// de motor staat aan dus geluid moet aan
				else
					nVolume = 0;
		}
		motor[motorA] = 0;
		motor[motorB] = 0;
		clearSounds();
		stopTask(geluid);
	}
}

task rechtdoor ()
{
	while (SensorValue(kleursensor) == BLACKCOLOR)
	{
		motor[motorA] = snelheid;
		motor[motorB] = snelheid;
	}
	bluetooth_1 = 0;
	startTask(linetracking);
}

task linksaf ()
{
	stopTask(kruispuntdetectie);
	motor[motorA] = 15;
	motor[motorB] = 15;
	for (int i = 0; i <= 30; i++)
	{
		motor[motorA] = motor[motorA] - 1;
		motor[motorB] = motor[motorB] + 1;
		wait1Msec(60);
	}
	bluetooth_1 = 0;
	startTask(linetracking);
	startTask(kruispuntdetectie);
}

task rechtsaf ()
{
	stopTask(kruispuntdetectie);
	motor[motorA] = 15;
	motor[motorB] = 15;
	for (int i = 0; i <= 30; i++)
	{
		motor[motorB] = motor[motorB] - 1;
		motor[motorA] = motor[motorA] + 1;
		wait1Msec(60);
	}
	wait1Msec(500);
	bluetooth_1 = 0;
	startTask(linetracking);
	startTask(kruispuntdetectie);
}

task kruispuntdetectie()
{
	while(1)
	{

	if (SensorValue(kleursensor) == BLACKCOLOR)
	{
		stopTask(linetracking);
		if (bluetooth_1 == 3)
		{
			startTask(linksaf);
		}
		if (bluetooth_1 == 4)
		{
			startTask(rechtsaf);
		}
		else
		{
			startTask(rechtdoor);
		}
	}
}
}

task accelerate() // hoogt geleidelijk de basissnelheid op
{
	stopTask(deaccelerate);
	int j = Tp;
	for (int i = j; i <= snelheid; i++)
	{
		wait10Msec(8);
		Tp = i;
	}
	stopTask(accelerate);
}

task deaccelerate()
{
	stopTask(accelerate);
	int j = Tp;
	for (int i = j; i >= 0; i--)
	{
		wait1Msec(80);
		Tp = i;
		if (SensorValue(zonar) <= 10)
			break;
	}
	Tp = 0;
	stopTask(deaccelerate);
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
	startTask(linetracking);
	startTask(aan_uit);
	while (1)
		{
			bluetooth_1 = bluetooth();
			wait10Msec(1);
		}
}
