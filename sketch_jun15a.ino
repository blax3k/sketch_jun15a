#include <SoftReset.h>
#include <avr/wdt.h>
#include <Time.h>
#include <TimeLib.h>

#define TIME_MSG_LEN 21 // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER "TIME" // Header tag for serial time sync message
#define TIME_REQUEST 7 // ASCII bell character requests a time sync message 
#define VAR_COUNT 10 //the number of global variables in the var array

const byte numChars = 52;
char receivedChars[numChars];
boolean newData = false;

String variables[VAR_COUNT];
const int led = 13;
int state = 0; String stateName = "state";
String serialNumber = "12345", serialNumberName = "serialNumber";
String firmNumber = "9876544321", firmNumberName = "firmNumber";
//Testing variables
boolean boolVar = true; String boolVarName = "boolVar";
char charVar = 'C'; String charVarName = "charName";
long longVar = 30000002; String longVarName = "longName";
unsigned long ulongVar = 3234; String ulongVarName = "ulongName";
word wordVar = 33755; String wordVarName = "wordName";
float floatVar = 33.75567; String floatVarName = "floatName";
double doubleVar = 656.2353; String doubleVarName = "doubleName";


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  MCUSR = 0;
  state = digitalRead(led);
  fillVarArray(); //fill the variable string array
  for (int i = 0; i < VAR_COUNT; i++) //send variables to the GUI
  {
    printVariable(i, variables[i]);//output the current variables
    digitalClockDisplay();        //output the current time on the board
  }
}

// the loop function runs over and over again forever
void loop() {
  if (Serial.available() > 0) { //make sure that the serial port is available
    receiveMsg();
    if (newData) {
      String data(receivedChars);            //read serial buffer as a string
      if (data.indexOf("ON") != -1) {
        digitalWrite(led, HIGH);
        state = 1;
        String message = String("STATE=") + state;
        sendMessage(message);
        sendMessage("LED turned ON");
      }
      else if (data.indexOf("OFF") != -1)                   //checks if it is "OFF"
      {
        digitalWrite(led, LOW);
        state = 0;
        String message = String("STATE=") + state;
        sendMessage(message);
        sendMessage("LED turned OFF");
      }
      else if ( (data.indexOf("STATE") != -1))              //checks if it is "STATE"
      {
        Serial.write("STATE=");
        Serial.print(state);
        Serial.println();
      }
      else if ((data.indexOf("INFO") != -1)) {              //request for firmware and serial
        String info = "info=" + firmNumber + "~" + serialNumber + "\n";
        Serial.print(info);
      }
      else if ((data.indexOf("SYNCTIME") != -1)) {          //Sync the time with the computer
        processSyncMessage(data); //receive the current time
        digitalClockDisplay(); //output the current time
      }
      else if ((data.indexOf("GETTIME") != -1)) {           //Send the time to the computer
        digitalClockDisplay(); //output the current time
      }
      else if ((data.indexOf("GETVARS") != -1)) {           //request for variable list
        for (int i = 0; i < VAR_COUNT; i++)
        {
          printVariable(i, variables[i]);//output the current variables
        }
        Serial.write("DONE\n"); //message that we're done sending variables
      }
      else if ((data.indexOf("GETVAR") != -1)) {            //get a specific variable
        data.replace("GETVAR", "");
        const char * c = data.c_str();
        int index = atoi(c);
        getVar(index);
      }
      else if ((data.indexOf("SETVAR") != -1)) {            //set a specific variable
        String dataCopy = data;
        dataCopy.replace("SETVAR", "");                     //remove the header
        String indexPart = getValue(dataCopy, '~', 0);      //get the index
        String newValue = getValue(dataCopy, '~', 1);       //get the new value
        int index = indexPart.toInt();                      //get the index
        setVar(index, newValue);
        printVariable(index, variables[index]);
      }
      else if ((data.indexOf("RES") != -1)) {               //Sync the time with the computer
        Serial.write("Rebooted\n");
        software_Reboot();
      }
      newData = false;
    }
  }

}

void receiveMsg() {
  static boolean recvInProgress = false;
  static byte ndx = 0;    //the index of chars received
  char startMarker = '<';
  char endMarker = '>';
  char rc;              //the received character

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}
/*
   uses the Watchdog to perform a hard reset of the system
*/
void software_Reboot() {

  wdt_enable(WDTO_15MS); //enable the watchdog
  while (1) { }         //stall the system
}

void sendMessage(String message)
{
  Serial.print(message);
  Serial.write('\n');
}

// https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}







