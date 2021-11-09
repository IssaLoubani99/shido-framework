#include <Wire.h>
#include <arduino-timer.h>
#include <Arduino_FreeRTOS.h>
#include <queue.h>

/******************************************************************
 Created with PROGRAMINO IDE for Arduino - 04.01.2021 04:08:09
 Project     : Terminal for arduino by Issa loubani and Khalil houmani (｡･∀･)ﾉﾞ
 Libraries   : Wire , Serial
 Author      : Issa loubani ^_^
 Description : Shell prompt able to interact with other devices 
 via I2C protocol. The Shell is implemented using UART protocol.
******************************************************************/

#define LED 14
#define FIRSTSLAVE 5
#define SECONDSLAVE 10

/* Global Variables do not change */
String command = "";
int chr = 0;
bool process = false;
String prompt = "AtMega2560#";

int queueSize=15;
int queuePointer = 0;
bool queueIsReady =false;

QueueHandle_t queue;

auto timer = timer_create_default();

void setup()
{

    // write your setup code here, to run once
    Wire.begin();        // join i2c bus (address optional for master)
    Serial.begin(9600);
    
    // show shell banner
    showBanner();
    
    // show atmega shell prompt 
    Serial.print(prompt);

    queue= xQueueCreate(queueSize, sizeof(String));
    
    if(queue != NULL){
      queueIsReady = true;
    }
    
    
}
void showBanner(){

  Serial.println("+------------+-------------------------------+");
  Serial.println("| Created By | Issa loubani, Khalil Houmani  |");
  Serial.println("+------------+-------------------------------+");
  Serial.println("| Coded By   | Issa loubani aka SHELLKING ;p |");
  Serial.println("| Version    | 1.0                           |");
  Serial.println("| Date       | 1/8/2021                      |");
  Serial.println("| Project    | SHIDO (Shell For arduino xD)  |");
  Serial.println("+------------+-------------------------------+");
  Serial.println("============= SHIDO Ver.1 =============");

}

void readCommand(){

  // 8 as backspace
  // 13 as ENTER
  // 0 as NULL
  
  chr = Serial.read(); // update char on every iteration

  if(chr > 0 && chr != 8 && chr!= 13){
    command += (char) chr; 
  }
  
  if(chr == 8){
    command = command.substring(0, command.length() - 1);
  }
  
  if(chr == 13){ // when enter is pressed process command
    storeCommandInQueue();
    process = true;
  }
  
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

void slaveSendCommand(int slaveAddress,String cmd){
  //char buff[cmd.length()];
  //cmd.toCharArray(buff, cmd.length() +1 );
  Wire.beginTransmission(slaveAddress);
  Wire.write(cmd.c_str());
  Wire.endTransmission();
  Serial.println("Command Sended !");
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

void help(){
      Serial.println("PIN");
      Serial.println("SS");
      Serial.println("CPIN");
      Serial.println("history");
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
        Serial.println("CPIN [PIN NUMBER] [ON || OFF] [TIME IN MILLISECONDS]");
      }
      
}

void ss(){
      String address = getValue(command,' ',1); // slave address

      int indexOfAddress = command.indexOf(address);
      String sCommand = command.substring(indexOfAddress + address.length() + 1, command.length());
      
      if(address != "" && sCommand != ""){
        slaveSendCommand(address.toInt(),sCommand);
      }else{
       Serial.println("SS [SLAVE ADDRESS] [COMMAND]");
      }
      
}

void history(){
  String temp ="";
  if(!queueIsReady || queuePointer == 0){
    Serial.println("History is not avaiable");
    return;
  }
 

  Serial.println(temp);

  while(queuePointer != 0){
    queuePointer--;
   

    xQueueReceive(queue,&temp,portMAX_DELAY);
    Serial.println(temp);
  }

}

void storeCommandInQueue(){
  xQueueSend(queue,&command,portMAX_DELAY);
  queuePointer++;
}

void loop()
{
  timer.tick(); // returns the number of ticks

  readCommand();
  
  if(process){

    
    if(command.startsWith("help")){
     help();
    }
    
    else if(command.startsWith("PIN")){
     pin();
    }
  
    else if(command.startsWith("CPIN")){
     cpin();
    }
    
    else if(command.startsWith("SS")){
      ss();
      
    }else if(command.startsWith("history")){
      history();
    }

    
 
  
  // reset values when finished
   Serial.print(prompt);
   command="";
   process = false;
  }
  


}
