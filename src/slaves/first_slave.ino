#include <Wire.h>
#include <arduino-timer.h>

/******************************************************************
 Created with PROGRAMINO IDE for Arduino - 04.01.2021 04:08:09
 Project     : Terminal for arduino by Issa loubani and Khalil houmani (｡･∀･)ﾉﾞ
 Libraries   : Wire , Serial
 Author      : Issa loubani ^_^
 Description : Shell prompt able to interact with other devices 
 via I2C protocol. The Shell is implemented using UART protocol.
******************************************************************/

#define SLAVEADDRESS 5
volatile bool processCMD = false;
String command = "";

auto timer = timer_create_default();

void setup()
{

    // write your setup code here, to run once
    Serial.begin(9600);
    Wire.begin(SLAVEADDRESS);
    Wire.onReceive(i2cReceive);
    
    delay(1000);
    
    Serial.print("Device ");
    Serial.print(SLAVEADDRESS);
    Serial.println(" is ready.");
}

String reconstructCommand(int nbOfBytes){
 char buff[nbOfBytes];
 for(int i = 0;i < nbOfBytes;i++) {
    buff[i]= (char) Wire.read();
  }

 return String(buff);
}

void changePINState(int led , bool state){
  int power = LOW;
  
  if(state){
      power= HIGH;
  }
  
  digitalWrite(led,power);
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void i2cReceive(int nbOfBytes){
  Serial.println("Received !");
  command = reconstructCommand(nbOfBytes);
  Serial.println(command);
  // parse command and do task
  processCMD = true;
    
}
bool changePinStateAfter(void *argument /* optional argument given to in/at/every */) {
  void** arg= argument;
  bool state = false;

 if(arg[1] == 1){
   state = true;
 }

  Serial.print(state); 
  
  changePINState(arg[0],state);
  return true; // to repeat the action - false to stop
}

void pin(){
      String ledPin = getValue(command,' ',1);
      String state = getValue(command,' ',2);
      bool pinState = false;
      
      if(state.equalsIgnoreCase("on")){
        pinState = true;
      }
      
      if(ledPin != "" && state != ""){
        changePINState(ledPin.toInt(),pinState);
      }else{
        Serial.println("PIN [PIN NUMBER] [ON || OFF]");
      }
}

void cpin(){
      String ledPin = getValue(command,' ',1);
      String state = getValue(command,' ',2);
      String time = getValue(command,' ',3);
      
     int pinState = 0;
      
      if(state.equalsIgnoreCase("on")){
        pinState = 1;
      }
      
      
      if(ledPin != "" && state != "" && time != ""){
        
        changePINState(ledPin.toInt(),pinState);
        void *args[2];
        args[0] = ledPin.toInt();
        
        if(pinState){
        args[1] = 0;
        }else{
        args[1] = 1;
        }
       
        timer.in(time.toInt(), changePinStateAfter, args); // with argument


      }else{
        Serial.println("CP [PIN NUMBER] [ON || OFF] [TIME IN MILLISECONDS]");
      }
      
}

void loop()
{
    timer.tick(); // returns the number of ticks

    if(processCMD){
    
    if(command.startsWith("PIN")){
     pin();
    }
  
    else if(command.startsWith("CPIN")){
     cpin();
    }
   
  
    processCMD = false; // reset when finished
    }

}
