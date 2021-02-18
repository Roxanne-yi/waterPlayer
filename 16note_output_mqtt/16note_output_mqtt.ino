#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Update these with values suitable for your network.
#define solenoidPower D0

const char* ssid = "7777777";
const char* password = "12345678";
const char* mqtt_server = "mqtt.eclipseprojects.io";

// solenoid connect to the Node MCU pins
const int solenoid1 = D1;
const int solenoid2 = D2;
const int solenoid3 = D4;
const int solenoid4 = D3;
const int solenoid5 = D5;
const int solenoid6 = D6;
const int solenoid7 = D7;
const int solenoid8 = D8;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<256> doc;
  deserializeJson(doc, payload, length);

  String section = doc["section"];
  int sectionIn = section.toInt();
  String statu = doc["status"];

//  for (int i = 0; i < length; i++) {
////    Serial.print((char)payload[i]);
//    section = (char)payload[i];  
//    statu = (char)payload[i];  
  if (statu == "start")
  {
   digitalWrite(solenoidPower,HIGH);
   // start chord
   digitalWrite(solenoid1, HIGH);
   delay(50);
   digitalWrite(solenoid1, LOW);
   delay(270);
   digitalWrite(solenoid3, HIGH);
   delay(50);
   digitalWrite(solenoid3, LOW);
   delay(270);
   digitalWrite(solenoid5, HIGH);
   delay(50);
   digitalWrite(solenoid5, LOW);
   delay(270);
   digitalWrite(solenoid1, HIGH);
   digitalWrite(solenoid3, HIGH);
   digitalWrite(solenoid5, HIGH);
   digitalWrite(solenoid8, HIGH);
   delay(50);
   digitalWrite(solenoid1, LOW);
   digitalWrite(solenoid3, LOW);
   digitalWrite(solenoid5, LOW);
   digitalWrite(solenoid8, LOW);
   delay(270);
   
  }
 
  else if(statu == "end")
  {
   // end chord
   digitalWrite(solenoid5, HIGH);
   delay(50);
   digitalWrite(solenoid5, LOW);
   delay(270);
   digitalWrite(solenoid3, HIGH);
   delay(50);
   digitalWrite(solenoid3, LOW);
   delay(270);
   digitalWrite(solenoid1, HIGH);
   delay(50);
   digitalWrite(solenoid1, LOW);
   delay(270);
   digitalWrite(solenoid1, HIGH);
   digitalWrite(solenoid3, HIGH);
   digitalWrite(solenoid5, HIGH);
   digitalWrite(solenoid8, HIGH);
   delay(50);
   digitalWrite(solenoid1, LOW);
   digitalWrite(solenoid3, LOW);
   digitalWrite(solenoid5, LOW);
   digitalWrite(solenoid8, LOW);
   delay(270);
   //stop output
   digitalWrite(solenoidPower,LOW); 
   }

   if(section != "null")
   {
    playSolenoid(sectionIn);
    }

//   switch(sectionIn){
//      case 1:
//        digitalWrite(solenoid4, HIGH);
//        delay(50);
//        digitalWrite(solenoid4, LOW);
//        break;
//      case 2:
//        
//        break;  
//      case 3:
//        
//        break;
//      case 4:
//        
//        break;  
//      case 5:
//        
//        break;
//      case 6:
//        
//        break;  
//      case 7:
//        
//        break;
//      case 8:
//        
//        break;
//      case 9:
//        
//        break;
//      case 10:
//        
//        break;  
//      case 11:
//        
//        break;
//      case 12:
//        
//        break;  
//      case 13:
//        
//        break;
//      case 14:
//        
//        break;  
//      case 15:
//        
//        break;
//      case 0:
//        
//        break;  
//       
//  }

//  if (section == "1")
//   {
//  digitalWrite(solenoid4,HIGH);
//  delay(50);
//  digitalWrite(solenoid4,LOW);
//  delay(300);}
  Serial.println(section);
  Serial.println(statu);

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish("/team01/waterMarimba/pub", "hello world");
      // ... and resubscribe
      client.subscribe("/team01/waterMarimba/pub");
      client.subscribe("/team01/waterMarimba/sub");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void playSolenoid(int section)
{
        int solenoidPin[] = {solenoid1, solenoid2, solenoid3, solenoid4, solenoid5, solenoid6, solenoid7, solenoid8};

        int muxSection[16][16] =
        {
                {9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9},  //Section 0
                {-6, -6, -3, -6, -8 ,-6, -3, -5, -6, -6, -3, -6, -8, -6, -3, -5},  //Section 1
                {3, 3, 4, 5, 5, 4, 3, 2, 9, 9, 9, 9, 9, 9, 9, 9},  //Section 2
                {1, 2, 3, 1, 1, 2, 3, 1, 9, 9, 9, 9, 9, 9, 9, 9},  //Section 3
                {-3, -4, -5, -3, -4, -5, 0, 0, -3, -4, -5, -3, -4, -5, -6, -5},  //Section 4
                {-5, -6, -5, -6, -5, -5, 5, -5, -8, -7, -6, -5, -5, 3, 9, 9},  //Section 5
                {3, -3, -5, -6, -8, -8, -6, -5, 0, -5, -6, 5, 0, 0, 9, 9},  //Section 6
                {-1, -2, -3, -4, -5, -5, -5, -3, -4, -4, -4, -2, -1, -3, 5, 9},  //Section 7
                {-6, -5, -6, -5, -3, -6, 5, -5, -5, -3, -2, 1, 0, 0, 9, 9},  //Section 8
                {-3, -3, -3, -3, -3, -3, -3, -4, -5, -5, -5, -6, 5, -1, -2, 9},  //Section 9
                {1, 0, -2, 3, 0, -1, 3, 1, 3, 0, 0, 9, 9, 9, 9, 9},  //Section 10
                {3, 0, -4, 5, 0, -3, 5, 3, 5, 0, 0, 9, 9, 9, 9, 9},  //Section 11
                {-1, -1, -5, -5, -6, -6, 5, -4, -4, -3, -3, -2, -2, 1, 9, 9},  //Section 12
                {-3, -3, -3, -3, -3, -8, -8, -8, -7, -5, -5, -6, 5, 0, 0, 9},  //Section 13
                {-3, -5, 6, 6, -6, -6, -3, -5, 2, 2, -2, -2, 9, 9, 9, 9},  //Section 14
                {0, -5, -6, -7, -8, 0, 0, -7, -7, -6, -5, -4, 5, 3, 9, 9}  //Section 15
        };

        //loop through the 16 note
        for(int i = 0; i < 16; i ++)
        {
                int note = muxSection[section][i];
                
                if (note == 0)
                {
                  delay(160);
                  }
                if (note > 0 && note < 9)
                {
                int runSolenoid = note - 1;
                digitalWrite(solenoidPin[runSolenoid], HIGH);
                delay(50);
                digitalWrite(solenoidPin[runSolenoid], LOW);
                delay(270);
                }
                if (note < 0)
                {
                int runSolenoid = abs(note) - 1;
                digitalWrite(solenoidPin[runSolenoid], HIGH);
                delay(50);
                digitalWrite(solenoidPin[runSolenoid], LOW);
                delay(110);
                }
        }

}

void setup() {

  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  //Declaring tb66 control pins as Output
  pinMode(solenoid1, OUTPUT);
  pinMode(solenoid2, OUTPUT);
  pinMode(solenoid3, OUTPUT);
  pinMode(solenoid4, OUTPUT);
  pinMode(solenoid5, OUTPUT);
  pinMode(solenoid6, OUTPUT);
  pinMode(solenoid7, OUTPUT);
  pinMode(solenoid8, OUTPUT);
  pinMode(solenoidPower, OUTPUT);
  digitalWrite(solenoidPower, LOW);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
}
