#include <SoftwareSerial.h>
#include <PZEM004Tv30.h>
#include <avr/pgmspace.h>

#define M0 12
#define M1 13
#define Tx1 2
#define Rx1 3

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

PZEM004Tv30 pzem(9, 8);   //rx,tx
PZEM004Tv30 pzem1(7, 6);
PZEM004Tv30 pzem2(5, 4);
SoftwareSerial LoRa(Rx1, Tx1);

float voltage[3]  = {0.0};
float current[3]  = {0.0};
float power[3]  = {0.0};
float energy[3]  = {0.0};
float freq[3]  = {0.0};
float pf[3]  = {0.0};


boolean send = true;
//boolean demo = true;
boolean demo = false;

unsigned long timeOld = 0;
unsigned long timeInterval = 5000; // mili second

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

    Serial.print("Ram free is: ");
    Serial.println(freeMemory());
    voltage[device] = pzemTemp.voltage();
    Serial.println("Read 2");
    delay(10);
    current[device] = pzemTemp.current();
    Serial.println("Read 3");
    delay(10);
    power[device] = pzemTemp.power();
    Serial.println("Read 4");
    delay(10);
    energy[device] = pzemTemp.energy();
    Serial.println("Read 5");
    delay(10);
    freq[device] = pzemTemp.frequency();
    Serial.println("Read 6");
    delay(10);
    pf[device] = pzemTemp.pf();
    Serial.println("Read 7");
    //delay(10);
  } else {
    //    //Serial.println("Demo Mode, all = random");
    //    voltage  = random(190, 300);
    //    current  = random(0, 100);
    //    power  = random(0, 10000);
    //    energy  = random(0, 1000);
    //    freq  = random(50, 55);
    //    pf  = random(0, 300);
  }
}

void loop() {

  Serial.println("step 1");

  if (millis() >= timeOld) {
    if (millis() - timeOld > timeInterval) {
      send  = true;
    }
    Serial.println("step 2");
  } else {
    Serial.println("step 3");
    if (timeOld - millis() > timeInterval) {
      send  = true;;
    }
  }


  if (send == true) {
    Serial.println("Send");

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
    dataSend += "\",\"energy\":\"";
    dataSend += String(energy[0]);
    dataSend += "\",\"pf\":\"";
    dataSend += String(pf[0]);
    dataSend += "\"}";
    LoRa.println(dataSend);
    Serial.println("OK1");

    dataSend = "";
    dataSend = "{\"voltage1\":\"";
    dataSend += String(voltage[1]);
    dataSend += "\",\"current1\":\"";
    dataSend += String(current[1]);
    dataSend += "\",\"power1\":\"";
    dataSend += String(power[1]);
    dataSend += "\",\"frequency1\":\"";
    dataSend += String(freq[1]);
    dataSend += "\",\"energy1\":\"";
    dataSend += String(energy[1]);
    dataSend += "\",\"pf1\":\"";
    dataSend += String(pf[1]);
    dataSend += "\"}";
    LoRa.println(dataSend);
    Serial.println("OK2");

    dataSend = "";
    dataSend = "{\"voltage2\":\"";
    dataSend += String(voltage[2]);
    dataSend += "\",\"current2\":\"";
    dataSend += String(current[2]);
    dataSend += "\",\"power2\":\"";
    dataSend += String(power[2]);
    dataSend += "\",\"frequency2\":\"";
    dataSend += String(freq[2]);
    dataSend += "\",\"energy2\":\"";
    dataSend += String(energy[2]);
    dataSend += "\",\"pf2\":\"";
    dataSend += String(pf[2]);
    dataSend += "\"}";
    LoRa.println(dataSend);
    dataSend = "";
    timeOld = millis();
    Serial.println("Send done!");
  }
  send  = false;
}

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
