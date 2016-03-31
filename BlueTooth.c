#pragma platform(NXT)

const int kMaxSizeOfMessage = 30;
const int INBOX = 5;

/* BlueTooth functie
	 Pollt de brick elke 100ms voor nieuwe
	 instructies en retourneert deze vervolgens.

	 How-to:
	 bluetooth();

	 mogelijke return waarden (int)
	 UP = 1
	 DOWN = 2
	 LEFT = 3
	 RIGHT = 4
	 A = 5
	 B = 6
	 C = 7
*/
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
/* Voorbeeld stukje code voor debugging en testing.
task main(){
	while(1){
	int a = 0;
	a = bluetooth();
 	displayCenteredBigTextLine(4, "%d", a);
}
}
*/
