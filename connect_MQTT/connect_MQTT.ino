#include <ESP8266WiFi.h>
#include <MQTT.h>

const char ssid[] = "7777777";
const char pass[] = "12345678";

//define 74HC4067
#define sensorPower D5
int s0 = D0;
int s1 = D1;
int s2 = D2;
int s3 = D3;
int Threshold = 100;

//Mux in "SIG" pin
int SIG_pin = A0;
int section = 0;

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  
  WiFi.begin(ssid, pass);
  client.begin("mqtt.eclipseprojects.io", net);
  
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("watermarimba")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/team01/waterMarimba/pub");
}

void setup() {
  Serial.begin(115200);
        
          // water level sensor
        pinMode(s0, OUTPUT);
        pinMode(s1, OUTPUT);
        pinMode(s2, OUTPUT);
        pinMode(s3, OUTPUT);
        pinMode(sensorPower, OUTPUT);

        digitalWrite(s0, LOW);
        digitalWrite(s1, LOW);
        digitalWrite(s2, LOW);
        digitalWrite(s3, LOW);
        digitalWrite(sensorPower, LOW);

  connect();
}

void loop() {
  
  //water level sensor
  section = retSection(4,Threshold);
  delay(3000);  
     
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("/team01/waterMarimba/pub","{\"section\":" + String(section) + "}");
  }
}
// water level sensor - section
int retSection(int n, int threshold)
{ 
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);       // wait 10 milliseconds
  int water_value[] = {0, 0, 0, 0};
  int section = 0;
  for(int i = 0; i < n; i ++)
        {
                Serial.print("Value at water level sensor ");
                Serial.print(i);
                Serial.print(" is : ");
                int v = readMux(i);
                Serial.println(v);
                if (v > threshold){
                  water_value[i] = 1; }
                  else if(v <= threshold)
                  {
                  water_value[i] = 0; }
                Serial.println(water_value[i]);
        }
       // pow(double x, double y）幂次方
        section = water_value[0]*pow(2,3) + water_value[1]*pow(2,2) + water_value[2]*pow(2,1) + water_value[3]*pow(2,0);
        Serial.print("Section no.: ");
        Serial.println(section);
        Serial.println(" ");
        digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
        return section;
}

//water level sensor - connect to demux
int readMux(int channel)
{
        int controlPin[] = {s0, s1, s2, s3};

        int muxChannel[16][4] =
        {
                {0, 0, 0, 0}, //channel 0
                {1, 0, 0, 0}, //channel 1
                {0, 1, 0, 0}, //channel 2
                {1, 1, 0, 0}, //channel 3
                {0, 0, 1, 0}, //channel 4
                {1, 0, 1, 0}, //channel 5
                {0, 1, 1, 0}, //channel 6
                {1, 1, 1, 0}, //channel 7
                {0, 0, 0, 1}, //channel 8
                {1, 0, 0, 1}, //channel 9
                {0, 1, 0, 1}, //channel 10
                {1, 1, 0, 1}, //channel 11
                {0, 0, 1, 1}, //channel 12
                {1, 0, 1, 1}, //channel 13
                {0, 1, 1, 1}, //channel 14
                {1, 1, 1, 1} //channel 15
        };
        
        //loop through the 4 sig
        for(int i = 0; i < 4; i ++)
        {

                digitalWrite(controlPin[i], muxChannel[channel][i]);
        }

        //read the value at the SIG pin
        int val = analogRead(SIG_pin);

        //return the value
        return val;
}
