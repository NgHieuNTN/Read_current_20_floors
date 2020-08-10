#include <SoftwareSerial.h>
String data = "";
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define M0 12
#define M1 14
#define Tx1 4
#define Rx1 5

#define WIFI_AP "TOTOLINK_N200RE"
#define WIFI_PASSWORD ""

//#define WIFI_AP "TNT Holdings"
//#define WIFI_PASSWORD "19001234"

#define TOKEN "16gysyAuoOMDC2BLwCVV"

char thingsboardServer[] = "192.168.1.43";

WiFiClient wifiClient;



PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;

SoftwareSerial LoRa(Tx1, Rx1); // RX, TX

String voltageStr = "";
String currentStr = "";
String powerStr = "";
String frequencyStr = "";

String voltageStr1 = "";
String currentStr1 = "";
String powerStr1 = "";
String frequencyStr1 = "";

String voltageStr2 = "";
String currentStr2 = "";
String powerStr2 = "";
String frequencyStr2 = "";



void sendToThingsboard() {

}

bool readData() {
  if (LoRa.available()) {
    data = LoRa.readString();
    Serial.print("data receive is: ");
    Serial.println(data);

    DynamicJsonDocument doc(500);
    deserializeJson(doc, data);
    JsonObject obj = doc.as<JsonObject>();


    String tempVoltageStr = obj[String("voltage")];
    String tempCurrentStr = obj[String("current")];
    String tempPowerStr = obj[String("power")];
    String tempFrequencyStr = obj[String("frequency")];

    String tempVoltageStr1 = obj[String("voltage1")];
    String tempCurrentStr1 = obj[String("current1")];
    String tempPowerStr1 = obj[String("power1")];
    String tempFrequencyStr1 = obj[String("frequency1")];

    String tempVoltageStr2 = obj[String("voltage2")];
    String tempCurrentStr2 = obj[String("current2")];
    String tempPowerStr2 = obj[String("power2")];
    String tempFrequencyStr2 = obj[String("frequency2")];

    voltageStr = tempVoltageStr;
    currentStr = tempCurrentStr;
    powerStr = tempPowerStr;
    frequencyStr = tempFrequencyStr;

    voltageStr1 = tempVoltageStr1;
    currentStr1 = tempCurrentStr1;
    powerStr1 = tempPowerStr1;
    frequencyStr1 = tempFrequencyStr1;

    voltageStr2 = tempVoltageStr2;
    currentStr2 = tempCurrentStr2;
    powerStr2 = tempPowerStr2;
    frequencyStr2 = tempFrequencyStr2;


    //    Serial.print("voltage=");
    //    Serial.println(voltageStr);
    //    Serial.print("current=");
    //    Serial.println(currentStr);
    //    Serial.print("power=");
    //    Serial.println(powerStr);
    //    Serial.print("frequency=");
    //    Serial.println(frequencyStr);
    return true;
  }
  return false;
}


void setup() {
  Serial.begin(9600);
  LoRa.begin(9600);
  LoRa.setTimeout(1000);
  delay(10);
  InitWiFi();
  client.setServer(thingsboardServer, 1883);
  Serial.println("Start");

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);

  digitalWrite(M0, 0);
  digitalWrite(M1, 0);

}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  if (readData() == true) {
    getAndSendTemperatureAndHumidityData();
  }
  client.loop();
  data = "";
}

void getAndSendTemperatureAndHumidityData() {
  Serial.println("Collecting  data.");



  String payload = "{";
  payload += "\"voltage\":"; payload += voltageStr; payload += ",";
  payload += "\"current\":"; payload += currentStr; payload += ",";
  payload += "\"power\":"; payload += powerStr; payload += ",";
  payload += "\"frequency\":"; payload += frequencyStr;

  payload += ",\"voltage1\":"; payload += voltageStr1; payload += ",";
  payload += "\"current1\":"; payload += currentStr1; payload += ",";
  payload += "\"power1\":"; payload += powerStr1; payload += ",";
  payload += "\"frequency1\":"; payload += frequencyStr1;

  payload += ",\"voltage2\":"; payload += voltageStr2; payload += ",";
  payload += "\"current2\":"; payload += currentStr2; payload += ",";
  payload += "\"power2\":"; payload += powerStr2; payload += ",";
  payload += "\"frequency2\":"; payload += frequencyStr2;
  payload += "}";

  char telemetry[300];
  payload.toCharArray( telemetry, 300 );
  client.publish( "v1/devices/me/telemetry", telemetry );
  Serial.println( telemetry);
  Serial.print("Data send is: ");
  Serial.println( payload);
  payload = "";

}
void InitWiFi() {
  Serial.println("Connecting to AP ...");
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}
void reconnect() {
  while (!client.connected()) {
    status = WiFi.status();
    if (status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    if ( client.connect("ESP8266 Device", TOKEN, NULL)) {
      Serial.println( "[DONE]" );
    }
    else {
      Serial.print("[FAILED] [ rc = " );
      Serial.print(client.state() );
      Serial.println( " : retrying in 5 seconds]");
      delay( 5000 );
    }
  }
}
