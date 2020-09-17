#include "ArduinoJson.h"
#include "PubSubClient.h"
#include <WiFi.h>
#include <ModbusMaster.h>


////// Inisialisasi Pin //////
#define MAX485_DE      4
#define MAX485_RE_NEG  0
#define RX2 16
#define TX2 17

////// MQTT //////
const char* _mqttServer = "mqtt.antares.id";
const int _mqttPort = 1883;
const int _secureMqttPort = 8883;
bool _debug = true;
bool cekPub;
char* _wifiSSID;
char* _wifiPass;
String _accessKey = "XXXXXXX:XXXXXXX";
String _jsonDataString = "{}";
String _jsonSubDataString;
String _subscriptionTopic;
String _receivedTopic;
String namaProject = "Heyy";
String namaDevice = "HELOO";

const char* ssid = "......";
const char* pass = "haha1112";

/////// Data ///////
uint8_t result;
float temp, hum;

ModbusMaster node;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println(".......................");
  }
  Serial.println("Connected!");
  Serial.print("IP ADDRES : ");
  Serial.print(WiFi.localIP());
  Serial.println();
  Serial.println(WiFi.localIP());
  node.begin(1, Serial2);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  set_MqttServer();
}

void loop() {
  check_MqttConnection();
  readSensor();
  addData("SUHU", temp);
  addData("HUM", hum); 
  publishData(namaProject, namaDevice);
}
