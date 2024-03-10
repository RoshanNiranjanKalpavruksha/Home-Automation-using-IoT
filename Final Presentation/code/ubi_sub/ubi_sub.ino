/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"

/****************************************
 * Define Constants
 ****************************************/
#define TOKEN "BBFF-F8yVMuPkZ0prU48KOF5GJr5BG3X1Oh" // Your Ubidots TOKEN
#define WIFINAME "Rohan" //Your SSID
#define WIFIPASS "1234556789" // Your Wifi Pass
#define triacPulse 4 //D2
#define ZVC 12 //D6

Ubidots client(TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/
int val;

void ICACHE_RAM_ATTR acon()
{
  // Serial.println("REad");
   
  delayMicroseconds(val); // read AD0
  digitalWrite(triacPulse, HIGH);
  
  // Serial.println(digitalRead(triacPulse));
  
  delayMicroseconds(50);  //delay 50 uSec on output pulse to turn on triac
  digitalWrite(triacPulse, LOW);
  
  // Serial.println(digitalRead(triacPulse));
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  int j= length-1;
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    int mymsg = atoi ((const char*)payload);
    val=mymsg;
  }
  //Serial.print(val);
  val=map(val,0,100,7200,200);
  Serial.println();
}

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  client.ubidotsSubscribe("nodemcu","bulb"); //Insert the dataSource and Variable's Labels

   pinMode(ZVC, INPUT_PULLUP);
  //digitalWrite(2, INPUT_PULLUP); // pull up
  pinMode(triacPulse, OUTPUT);
  attachInterrupt(ZVC, acon, FALLING); // attach Interrupt at PIN2
  
  }

void loop() {
  // put your main code here, to run repeatedly:
  if(!client.connected()){
      client.reconnect();
      client.ubidotsSubscribe("nodemcu","bulb"); //Insert the dataSource and Variable's Labels
      }
  client.loop();
  }
