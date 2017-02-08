#include <Wire.h>
#include <BH1750.h> 
#include <SoftwareSerial.h>
#include <DHT.h>
#include <Servo.h>

#define ESP_TX 7
#define ESP_RX 6
#define LAMP_PIN 5
#define RELAY_PIN 4
#define SERVO_PIN 3
#define DHT_PIN 2
#define WATERMETER A3

#define WATERLEVEL_HIGH 800
#define LAMP_INVERT_INTERVAL 500
#define SENSOR_POLL_INTERVAL 100
#define DHTTYPE DHT11

#define LIGHT_LOW 300
#define LIGHT_HIGH 2000

DHT dht(DHT_PIN, DHTTYPE);
BH1750 lightMeter; //на A4 && A5
SoftwareSerial esp(ESP_RX, ESP_TX);
Servo fortochka;

long last_temp_check = 0;
long last_lamp_change = 0;
long last_sensors_poll = 0;
int temperature, lux, waterlevel;

void setup() {
  Serial.begin(115200);
  esp.begin(19200);
  dht.begin();
   // fortochka.attach(SERVO_PIN);

  lightMeter.begin();

  Serial.println(F("templica_test"));

  pinMode(LAMP_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  last_temp_check = millis();
  last_lamp_change = millis();
  last_sensors_poll = millis();
  digitalWrite(RELAY_PIN,HIGH);
  digitalWrite(LAMP_PIN,HIGH);
  
// delay(1000);
digitalWrite(RELAY_PIN,LOW);
digitalWrite(LAMP_PIN,LOW);
// delay(1000);
// fortochka.write(170);
// delay(2000);
// fortochka.write(10);


}
int lastpos = 0, targetpos;

void loop() {

  // if ((millis() - last_lamp_change) > LAMP_INVERT_INTERVAL) {
  //   digitalWrite(LAMP_PIN, !digitalRead(LAMP_PIN));
  //   last_lamp_change = millis();
  // }

  if ((millis() - last_sensors_poll) > SENSOR_POLL_INTERVAL) {
    waterlevel = analogRead(WATERMETER);
    temperature = dht.readTemperature();
    lux = lightMeter.readLightLevel();

    // fortochka.attach(SERVO_PIN);
    // targetpos = map(lux, 0, 1250, 120, 50);
    // if (lastpos < targetpos) {
    //   for(int i=lastpos; i < targetpos; i++){
    //     fortochka.write(i);
    //     delay(15);
    //   }
    // } else {
    //   for(int i=targetpos; i > lastpos; i--){
    //     fortochka.write(i);
    //     delay(15);
    //   }
    // }
    // lastpos = targetpos;
    // fortochka.detach();//

    if (lux <= LIGHT_LOW) {
      digitalWrite(LAMP_PIN, 1);
    }
    if (lux >= LIGHT_HIGH) {
      digitalWrite(LAMP_PIN, 0);
    }

    // if (waterlevel < WATERLEVEL_LOW) {
    //   digitalWrite(RELAY_PIN, HIGH);
    // }
    if (waterlevel >= 800) {
      digitalWrite(RELAY_PIN, 0);
    } else {
      digitalWrite(RELAY_PIN, 1);
    }

    Serial.print(F("waterlevel"));
    Serial.print(F("\t"));
    Serial.print(F("temperature"));
    Serial.print(F("\t"));
    Serial.print(F("lux"));
    Serial.print(F("\t"));
    Serial.print(F("lastpos"));
    Serial.print(F("\t"));
    Serial.println();

    Serial.print(waterlevel);
    Serial.print("\t\t");
    if (!isnan(temperature)) {
      Serial.print(temperature);
    } else {
      Serial.print("--");
    }
    Serial.print("\t\t");
    Serial.print(lux);
    Serial.print("\t\t");    
    Serial.print(lastpos);
    Serial.print("\t\t");
    Serial.println();

  //  last_sensors_poll = millis();
  }
}
