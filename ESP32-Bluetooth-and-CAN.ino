#include <SPI.h>
#include <mcp2515.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
char receivedChar;// received value will be stored as CHAR in this variable
int LEDpin = 2;

struct can_frame canMsg;
MCP2515 mcp2515(5);
unsigned long dez_rpm =0, dez_speed=0;
int dez_gear=0;
String hilfsstring, string_1, string_2, gear;
unsigned long currentTime=0, startTime=0;

void setup() {
  Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS);
  mcp2515.setNormalMode();
  
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");

  SerialBT.begin("ESP32"); //Bluetooth device name
  pinMode(LEDpin, OUTPUT);

  
}

void loop() {
  /*if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    SerialBT.print(canMsg.can_id, HEX); // print ID
    SerialBT.print(" "); 
    SerialBT.print(canMsg.can_dlc, HEX); // print DLC
    SerialBT.print(" ");
    
    for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
      SerialBT.print(canMsg.data[i],HEX);
      SerialBT.print(" ");
    }

    SerialBT.println();      
  }*/

 if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    
    if (canMsg.can_id==0x120) { //RPM

      string_1=String(canMsg.data[0], HEX); 
      string_2=String(canMsg.data[1], HEX); 
      if(string_1.length()==1) string_1="0"+string_1; 
      if(string_2.length()==1) string_2="0"+string_2; 

      hilfsstring=string_1+string_2;


  	  //4-stellige Hex-Werte als char-Array speichern:
      char buf[4];
      hilfsstring.toCharArray(buf, 5);
 
      //char-Array in Dezimal-Werte umwandeln
      dez_rpm = strtol(buf, NULL, 16); 



    }

    if (canMsg.can_id==0x12D) { //Speed

      string_1=String(canMsg.data[0], HEX); 
      string_2=String(canMsg.data[1], HEX); 
      if(string_1.length()==1) string_1="0"+string_1; 
      if(string_2.length()==1) string_2="0"+string_2; 

      hilfsstring=string_1+string_2;

  	  //4-stellige Hex-Werte als char-Array speichern:
      char buf[4];
      hilfsstring.toCharArray(buf, 5);
 
      //char-Array in Dezimal-Werte umwandeln
      dez_speed = strtol(buf, NULL, 16); 
    }

    if (canMsg.can_id==0x129) { //Gear

      gear=String(canMsg.data[0], HEX); 
      //if(hilfsstring.length()==1) hilfsstring=hilfsstring+"0"; 
      
      gear = gear.charAt(0);
    }

    currentTime = millis();

    if (currentTime>=startTime+333) {
      SerialBT.print("R");
      SerialBT.print(dez_rpm);
      //SerialBT.print(" U/min");

      SerialBT.print(" S");
      SerialBT.print(dez_speed/160);
      //SerialBT.print(" km/h");

      SerialBT.print(" G");
      SerialBT.print(gear);
      SerialBT.println(" ");

      startTime=millis();

    }

    

  }

}

 