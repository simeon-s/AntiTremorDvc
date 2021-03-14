 /*  HTTPS on ESP8266 with follow redirects, chunked encoding support
    Version 3.0
    Author: Sujay Phadke
    Github: @electronicsguy
    Copyright (C) 2018 Sujay Phadke <electronicsguy123@gmail.com>
    All rights reserved.

    Example Arduino program
*/

#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"
#include "DebugMacros.h"
#include <string.h>
#include "Wemos_d1_pin_def.h"
#include "Definitions.h"

HTTPSRedirect* client = nullptr;
// used to store the values of free stack and heap
// before the HTTPSRedirect object is instantiated
// so that they can be written to Google sheets
// upon instantiation

void readFromCell(); 
void googleSheetsMainRead(); 
int countCharacter(String s);
int indexOfChar(String inputString, char inputChar, int startingPos);
// Some strings methods of arduino aren't working as their cpp equivalents
String Substring(String inpStr, int startingIndex, int amountOfChars);
void stringNumbersToArray(String inputStr, int* arr);
String intToBinary(int inputNumber);
int NIndexOfChar(String inputString, char inputChar, int startingPos, int nTimes);
String outputString = "";
String inputStringFromGS;
String inputForMotors; 
String strMotorInfo;
String settingsString;
String inputForMotors2;
String temporaryString;


int8_t currentTempState;
int8_t linePos;
int8_t lengthInputFromGS;
int8_t buttonPin;
char arrayCharForInt[70];
int8_t inputConvertedArray[61]; // Int array for the main
int8_t actuatorNumArrayStep[40];
int8_t currentStep = 0;
int actuatorNumState;
void stringToInt(String inputStr); // String to int array 
void setArray();





void setup() {
  
  pinMode(MOTOR_1, OUTPUT);
  pinMode(MOTOR_2, OUTPUT);
  pinMode(MOTOR_3, OUTPUT);
  pinMode(MOTOR_4, OUTPUT);
  pinMode(MOTOR_5, OUTPUT);
  pinMode(MOTOR_6, OUTPUT);
  setPinsLow();
  Serial.begin(115200);
  Serial.flush();

  free_heap_before = ESP.getFreeHeap();
  free_stack_before = ESP.getFreeContStack();
  Serial.printf("Free heap: %u\n", free_heap_before);
  Serial.printf("Free stack: %u\n", free_stack_before);

  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  // flush() is needed to print the above (connecting...) message reliably,
  // in case the wireless connection doesn't go through
  Serial.flush();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Use HTTPSRedirect class to create a new TLS connection
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");

  Serial.print("Connecting to ");
  Serial.println(host);

  // Try to connect for a maximum of 5 times
  bool flag = false;
  for (int i = 0; i < 5; i++) {
    int retval = client->connect(host, httpsPort);
    if (retval == 1) {
      flag = true;
      break;
    }
    else
      Serial.println("Connection failed. Retrying...");
  }

  if (!flag) {
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    Serial.println("Exiting...");
    return;
  }

  /*
    if (client->setFingerprint(fingerprint)) {
      Serial.println("Certificate match.");
    } else {
      Serial.println("Certificate mis-match");
    }
  */

  // Send memory data to Google Sheets
//  payload = payload_base + "\"" + free_heap_before + "," + free_stack_before + "\"}";
//  client->POST(url2, host, payload, false);
//  payload = payload_base + "\"" + ESP.getFreeHeap() + "," + ESP.getFreeContStack() + "\"}";
//  client->POST(url2, host, payload, false);

  // Note: setup() must finish within approx. 1s, or the the watchdog timer
  // will reset the chip. Hence don't put too many requests in setup()
  // ref: https://github.com/esp8266/Arduino/issues/34

  Serial.println("\nGET: Write into cell 'A1'");
  Serial.println("=========================");



  //  payload = payload_base + "\"" + ESP.getFreeHeap() + "," + ESP.getFreeContStack() + "\"}";

  delete client;
  client = nullptr;
  googleSheetsMainRead();
  delay(2000);


}
void setPinsLow()
{
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, LOW);
  digitalWrite(MOTOR_3, LOW);
  digitalWrite(MOTOR_4, LOW);
  digitalWrite(MOTOR_5, LOW);
  digitalWrite(MOTOR_6, LOW);
}
void loop() {

  buttonPin = digitalRead(BUTTON1_PIN);
  if(buttonPin)
  {
      googleSheetsMainRead(); // Read new pattern from GS
  }
  else
  {
      setPinsLow();
      setMotors(); // Basic state of the program - the pattern is executing 
  }
 

}

void TurnactuatorNum(int motorIndex, int motorState)
{


  Serial.println();
//  Serial.println("Setting motor ");
  switch(motorIndex)
  {
     case 0:
    motorPin = MOTOR_1;
   // Serial.print(" Motor 1 ");
    break;
      
     case 1:
    motorPin = MOTOR_2;
   // Serial.print(" Motor 2 ");
    break;
    
     case 2:
    motorPin = MOTOR_3;
   // Serial.print(" Motor 3 ");
    break;
    
     case 3:
    motorPin = MOTOR_4;
   // Serial.print(" Motor 4 ");

    break;
    
     case 4:
    motorPin = MOTOR_5;
    //Serial.print(" Motor 5 ");

    break;
    
     case 5:
     motorPin = MOTOR_6;
     //Serial.print(" Motor 6 ");
    break;
  }
  digitalWrite(motorPin, motorState);
  //Serial.print(" ST: ");
  //Serial.print(motorState);
    
}

void setMotors()
{
  setArray();
  stringToInt(inputForMotors2);
   temporaryString ="";
  for(int i =0; i<=59; i++)
  {
   temporaryString += inputConvertedArray[i];
  }
  currentStep = 0;
  currentTempState = 0;
  for(int k = 0; k<loopSteps; k++)
   {

        for(int i =0; i<actuatorNum; i++)
        {
          currentTempState = actuatorNumArrayStep[i] + currentStep;
          actuatorNumState = inputConvertedArray[currentTempState];

          if(actuatorNumState == 1)
          {
            TurnactuatorNum(i, 1);
          }
          else if(actuatorNumState == 0)
          {
            TurnactuatorNum(i, 0);
          }
          
         
        }
        delay(stepDurationActuator);
        //delay(200);
        currentStep++;
   }
}



void stringToInt(String inputStr) // String to int array
{
    
//    int inputStringSize = inputStr.length();
    int inputStringSize = 60;
    int temp;
      strcpy(arrayCharForInt, inputStr.c_str());

    for(int i =0; i<inputStringSize; i++)
    {
        temp =  arrayCharForInt[i]-48;
//        if(temp == -48)
//          inputConvertedArray[i] = 0;
//        else
          inputConvertedArray[i] = temp;

     // inputConvertedArray[i] =int(inputStr.charAt(i));
    }



 
}



void setArray()
{
    for(int i =0; i<actuatorNum;i++)
    {
     actuatorNumArrayStep[i] = loopSteps*i; // 1
    }
}


void googleSheetsMainRead()
{
  //long long timeStart = millis();
  static int error_count = 0;
  connect_count = 0;
  const unsigned int MAX_CONNECT = 20;
  static bool flag = false;
  //Serial.printf("Free heap: %u\n", ESP.getFreeHeap());
  //Serial.printf("Free stack: %u\n", ESP.getFreeContStack());

  if (!flag) {
    free_heap_before = ESP.getFreeHeap();
    free_stack_before = ESP.getFreeContStack();
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    flag = true;
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");
  }

  if (client != nullptr) {
    if (!client->connected()) {
      client->connect(host, httpsPort);
      payload = payload_base + "\"" + free_heap_before + "," + free_stack_before + "\"}";
//      client->POST(url2, host, payload, false);
    }
  }
  else {
    DPRINTLN("Error creating client object!");
    error_count = 5;
  }

  if (connect_count > MAX_CONNECT) {
    //error_count = 5;
    connect_count = 0;
    flag = false;
    delete client;
    return;
  }
  

 
  ReadFromCell();
  if (error_count > 3) {
    Serial.println("Halting processor...");
    delete client;
    client = nullptr;
    Serial.printf("Final free heap: %u\n", ESP.getFreeHeap());
    Serial.printf("Final stack: %u\n", ESP.getFreeContStack());
    Serial.flush();
    ESP.deepSleep(0);
  }
//    long long timeEnd = millis();
//    Serial.println();
//    Serial.print("-------------------------------- the time is ---------------------------"); // around 3 secs between every loop cycle
//    int diff = timeStart - timeEnd;
//    Serial.print(diff);

}


void ReadFromCell()
{

 
  if (client->GET(url3, host)) {
    ++connect_count;
//    Serial.println(tempStr);
  }
  else {
    ++error_count;
    DPRINT("Error-count while connecting: ");
    DPRINTLN(error_count);
  }
  Serial.println("GET Data from cell 'A1 ':");
  inputStringFromGS = client->getLineStr();

  lengthInputFromGS = inputStringFromGS.length();
  linePos = NIndexOfChar(inputStringFromGS, '|',0,SETTINGS_PARAMETERS_NUM);
  inputForMotors = Substring(inputStringFromGS, linePos,lengthInputFromGS-linePos);
  inputForMotors2 = Substring(inputStringFromGS, linePos+1,60);



  
  settingsString = inputStringFromGS.substring(0,14);
  Serial.println("-------------------------------");
  stringNumbersToArray(settingsString, settingsArray);
  actuatorNum = settingsArray[0];
  stepDurationActuator = settingsArray[1];
  loopCycleTime = settingsArray[2];
  loopSteps = settingsArray[3];
//  Serial.println();
//  Serial.println("Settings: ");
//  Serial.print(actuatorNum); Serial.print(" ");
//  Serial.print(stepDurationActuator); Serial.print(" ");
//  Serial.print(loopCycleTime); Serial.print(" "); 
//  Serial.print(loopSteps); Serial.print(" ");
//  Serial.println();
//  Serial.println("-------------------------------");

//  delay(300);
}




int countCharacter(String s) {
    int count = 0;

    for (int i = 0; i < s.length(); i++)
        if (s[i] == '|') count++;

    return count;
}


int indexOfChar(String inputString, char inputChar, int startingPos)
{
    int stringLength = inputString.length();
    int elementIndex = -1;
    for (int i = startingPos; i < stringLength; i++)
    {
        if (inputString.charAt(i) == inputChar)
        {
            elementIndex = i;
            break;
        }
    }

    return elementIndex;
}



String Substring(String inpStr, int startingIndex, int amountOfChars)
{
    outputString = "";
    for (int i = startingIndex; i < amountOfChars+ startingIndex; i++)
    {
        outputString += inpStr.charAt(i);
    }

    return outputString;
}

void stringNumbersToArray(String inputStr, int* arr)
{
    String numberFromStringTemp = "";
    int verticalLinePrevPosition = 0;
    int verticalLineCurrentPosition = 0;
    int lines = countCharacter(inputStr);
    int stringLength = inputStr.length();

    int pos = 0;
    for (int i = 0; i <= lines; i++)
    {
        verticalLineCurrentPosition = indexOfChar(inputStr, '|', verticalLinePrevPosition);
 
        pos = verticalLineCurrentPosition - verticalLinePrevPosition;
        numberFromStringTemp = Substring(inputStr, verticalLinePrevPosition, pos); //inputStr.SubstringNewing(verticalLinePrevPosition, pos);


        if (verticalLineCurrentPosition == -1)
        {
            numberFromStringTemp = Substring(inputStr, verticalLinePrevPosition, stringLength - verticalLinePrevPosition);
            //inputStr.SubstringNewing(verticalLinePrevPosition, stringLength - verticalLinePrevPosition);
        }
    
        verticalLinePrevPosition = verticalLineCurrentPosition + 1;
          arr[i] =  numberFromStringTemp.toInt();
//          Serial.println(arr[i]);
    }
}
int NIndexOfChar(String inputString, char inputChar, int startingPos, int nTimes)
{
    int stringLength = inputString.length();
    int elementIndex = -1;
    int counterNTimes = 0;
    for (int i = startingPos; i < stringLength; i++)
    {
        if (inputString.charAt(i) == inputChar)
        {
            counterNTimes++;
        }
        if(counterNTimes == nTimes)
        {
            elementIndex = i;
            break;
        }
    }

    return elementIndex;
}

String intToBinary(int inputNumber)
{
    String binary("");
    int mask = 1;
    for (int i = 0; i < BYTES_MASK; i++)
    {
        if ((mask & inputNumber) >= 1)
            binary = "1" + binary;
        else
            binary = "0" + binary;
        mask <<= 1;
    }
    
   String newStr = Substring(binary, BYTES_TO_USE-BYTES_TO_USE, BYTES_TO_USE+1);
   return newStr;
}
