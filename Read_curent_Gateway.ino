#include <SoftwareSerial.h>
String data = "";
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define M0 12
#define M1 14
#define Tx1 4
#define Rx1 5

//#define WIFI_AP "TOTOLINK_N200RE"
//#define WIFI_PASSWORD ""

//#define WIFI_AP "TNT Holdings"
//#define WIFI_PASSWORD "19001234"

#define WIFI_AP "TNT.HR.2.4Ghz"
#define WIFI_PASSWORD "999999999"

#define TOKEN "gCSJYGpPx0y03z7KReug"

char thingsboardServer[] = "192.168.1.43";

WiFiClient wifiClient;



PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;

SoftwareSerial LoRa(Tx1, Rx1); // RX, TX

String voltageStr = "";
String currentStr = "";
String powerStr = "";
String frequencyStr = "";
String energyStr = "";
String pfStr = "";

String voltageStr1 = "";
String currentStr1 = "";
String powerStr1 = "";
String frequencyStr1 = "";
String energyStr1 = "";
String pfStr1 = "";

String voltageStr2 = "";
String currentStr2 = "";
String powerStr2 = "";
String frequencyStr2 = "";
String energyStr2 = "";
String pfStr2 = "";


void sendToThingsboard() {

}

bool readData() {
  if (LoRa.available()) {
    data = LoRa.readString();
    data.replace("\n", "");
    data.replace("\r", "");
    data.replace("}{", ",");
    Serial.print("data receive is: ");
    Serial.println(data);

    DynamicJsonDocument doc(600);
    deserializeJson(doc, data);
    JsonObject obj = doc.as<JsonObject>();


    String tempVoltageStr = obj[String("voltage")];
    String tempCurrentStr = obj[String("current")];
    String tempPowerStr = obj[String("power")];
    String tempFrequencyStr = obj[String("frequency")];
    String tempEnergyStr = obj[String("energy")];
    String tempPfStr = obj[String("pf")];

    String tempVoltageStr1 = obj[String("voltage1")];
    String tempCurrentStr1 = obj[String("current1")];
    String tempPowerStr1 = obj[String("power1")];
    String tempFrequencyStr1 = obj[String("frequency1")];
    String tempEnergyStr1 = obj[String("energy1")];
    String tempPfStr1 = obj[String("pf1")];

    String tempVoltageStr2 = obj[String("voltage2")];
    String tempCurrentStr2 = obj[String("current2")];
    String tempPowerStr2 = obj[String("power2")];
    String tempFrequencyStr2 = obj[String("frequency2")];
    String tempEnergyStr2 = obj[String("energy2")];
    String tempPfStr2 = obj[String("pf2")];

    voltageStr = tempVoltageStr;
    currentStr = tempCurrentStr;
    powerStr = tempPowerStr;
    frequencyStr = tempFrequencyStr;
    energyStr = tempEnergyStr;
    pfStr = tempPfStr;

    voltageStr1 = tempVoltageStr1;
    currentStr1 = tempCurrentStr1;
    powerStr1 = tempPowerStr1;
    frequencyStr1 = tempFrequencyStr1;
    energyStr1 = tempEnergyStr1;
    pfStr1 = tempPfStr1;

    voltageStr2 = tempVoltageStr2;
    currentStr2 = tempCurrentStr2;
    powerStr2 = tempPowerStr2;
    frequencyStr2 = tempFrequencyStr2;
    energyStr2 = tempEnergyStr2;
    pfStr2 = tempPfStr2;

    //        Serial.print("voltage1=");
    //        Serial.println(voltageStr1);
    //        Serial.print("current1=");
    //        Serial.println(currentStr1);
    //        Serial.print("power1=");
    //        Serial.println(powerStr1);
    //        Serial.print("frequency1=");
    //        Serial.println(frequencyStr1);
    return true;
  }
  return false;
}


void setup() {
  Serial.begin(9600);
  LoRa.begin(9600);
  LoRa.setTimeout(2000);
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

  payload += "\"vol\":"; payload += voltageStr; payload += ",";
  payload += "\"cur\":"; payload += currentStr; payload += ",";
  payload += "\"pow\":"; payload += powerStr; payload += ",";
  payload += "\"freq\":"; payload += frequencyStr; payload += ",";
  payload += "\"ene\":"; payload += energyStr; //payload += ",";
  //payload += "\"pf\":"; payload += pfStr;

  payload += ",";
  payload += "\"vol1\":"; payload += voltageStr1; payload += ",";
  payload += "\"cur1\":"; payload += currentStr1; payload += ",";
  payload += "\"pow1\":"; payload += powerStr1; payload += ",";
  payload += "\"freq1\":"; payload += frequencyStr1; payload += ",";
  payload += "\"ene1\":"; payload += energyStr1; //payload += ",";
  //  payload += "\"pf1\":"; payload += pfStr1;

  payload += ",";
  payload += "\"vol2\":"; payload += voltageStr2; payload += ",";
  payload += "\"cur2\":"; payload += currentStr2; payload += ",";
  payload += "\"pow2\":"; payload += powerStr2; payload += ",";
  payload += "\"freq2\":"; payload += frequencyStr2; payload += ",";
  payload += "\"ene2\":"; payload += energyStr2; //payload += ",";
  //  payload += "\"pf2\":"; payload += pfStr2;


  payload += "}";

  char telemetry[payload.length() + 1];
  payload.toCharArray( telemetry, payload.length() + 1 );
  client.publish( "v1/devices/me/telemetry", telemetry );
  Serial.println(telemetry);
  Serial.print("Length is: ");
  Serial.println(payload.length());
  Serial.println("------------------------");
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
