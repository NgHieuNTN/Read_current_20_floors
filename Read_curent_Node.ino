#include <SoftwareSerial.h>
#include <PZEM004Tv30.h>


#define M0 12
#define M1 13
#define Tx1 11
#define Rx1 10

PZEM004Tv30 pzem(9, 8);   //tx,rx
PZEM004Tv30 pzem1(7, 6);
PZEM004Tv30 pzem2(5, 4);
SoftwareSerial LoRa(Tx1, Rx1);

float voltage[3]  = {0.0};
float current[3]  = {0.0};
float power[3]  = {0.0};
float energy[3]  = {0.0};
float freq[3]  = {0.0};
float pf[3]  = {0.0};


boolean send = 0;
//boolean demo = true;
boolean demo = false;

unsigned long timeOld = 0;
unsigned long timeInterval = 1000; // mili second

void setup() {
  Serial.begin(9600);
  LoRa.begin(9600);
  timeOld = millis();

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);

  digitalWrite(M0, 0);
  digitalWrite(M1, 0);

}

void readData(PZEM004Tv30 pzemTemp, int device) {
  if (demo == false) {

    voltage[device] = pzemTemp.voltage();
    if (voltage[device] != NAN) {
      //Serial.print("Voltage: "); Serial.print(voltage[device]); Serial.println("V");
    } else {
      Serial.println("Error reading voltage");
    }
    current[device] = pzemTemp.current();
    if (current[device] != NAN) {
      //Serial.print("Current: "); Serial.print(current[device]); Serial.println("A");
    } else {
      Serial.println("Error reading current");
    }
    power[device] = pzemTemp.power();
    if (current[device] != NAN) {
      //Serial.print("Power: "); Serial.print(power[device]); Serial.println("W");
    } else {
      Serial.println("Error reading power");
    }
    energy[device] = pzemTemp.energy();
    if (current[device] != NAN) {
      //Serial.print("Energy: "); Serial.print(energy[device], 3); Serial.println("kWh");
    } else {
      Serial.println("Error reading energy");
    }
    freq[device] = pzemTemp.frequency();
    if (current[device] != NAN) {
      //Serial.print("Frequency: "); Serial.print(freq[device], 1); Serial.println("Hz");
    } else {
      Serial.println("Error reading frequency");
    }
    pf[device] = pzemTemp.pf();
    if (current[device] != NAN) {
      //Serial.print("PF: "); Serial.println(pf[device]);
    } else {
      Serial.println("Error reading power factor");
    }
    Serial.println();
  } else {
    //    Serial.println("Demo Mode, all = random");
    //    voltage  = random(190, 300);
    //    current  = random(0, 100);
    //    power  = random(0, 10000);
    //    energy  = random(0, 1000);
    //    freq  = random(50, 55);
    //    pf  = random(0, 300);
  }
}

void loop() {

  unsigned long timeNow = millis();

  if (timeNow > timeOld) {
    if (timeNow - timeOld > timeInterval) {
      send  = true;
    }
  } else {
    if (timeOld - timeNow > timeInterval) {
      send  = true;
    }
  }


  if (send == true) {

    readData(pzem, 0);
    readData(pzem1, 1);
    readData(pzem2, 2);

    String dataSend = "{\"voltage\":\"";
    dataSend += String(voltage[0]);
    dataSend += "\",\"current\":\"";
    dataSend += String(current[0]);
    dataSend += "\",\"power\":\"";
    dataSend += String(power[0]);
    dataSend += "\",\"frequency\":\"";
    dataSend += String(freq[0]);
    dataSend += "\"";

    dataSend += ",\"voltage1\":\"";
    dataSend += String(voltage[1]);
    dataSend += "\",\"current1\":\"";
    dataSend += String(current[1]);
    dataSend += "\",\"power1\":\"";
    dataSend += String(power[1]);
    dataSend += "\",\"frequency1\":\"";
    dataSend += String(freq[1]);
    dataSend += "\"";

    dataSend += ",\"voltage2\":\"";
    dataSend += String(voltage[2]);
    dataSend += "\",\"current2\":\"";
    dataSend += String(current[2]);
    dataSend += "\",\"power2\":\"";
    dataSend += String(power[2]);
    dataSend += "\",\"frequency2\":\"";
    dataSend += String(freq[2]);
    dataSend += "\"}";
    LoRa.println(dataSend);
    Serial.println(dataSend);
    dataSend = "";
    timeOld = millis();
  }
  send  = false;
}
