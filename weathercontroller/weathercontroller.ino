/*
  weathercontroller      
*/
#include "DHT.h"

#define DHTTYPE DHT11   // DHT 11

#define _DHT11_PIN 7
#define RELAY_PUMP 6
#define LIGHT_MOSFET 5
#define SWITCH_HEATER 4
#define HALOGEN_HEATER_MOSFET 3
#define BTN_PUMP 2
#define WATERMETER A3
#define POT_PIN A5

#define WATERMETER_LOW 0
#define WATERMETER_HIGH 1023
// #define WATERMETER_LOW 200
// #define WATERMETER_HIGH 600

#define SWITCH_HEATER_ON 1
#define BTN_PUMP_PRESSED 0

#define TEMP_CHECK_PERIOD 2000
#define TEMP_MIN 25
#define TEMP_MAX 65
#define HYSTERESIS 10

#define DEBOUNCE_TIME_MAX 150
#define DEBOUNCE_TIME_MIN 200

DHT dht(_DHT11_PIN, DHTTYPE);

unsigned int temp=50, temp_prev = temp, real_temp;
int _pwm, pot_value;
int waterlevel;
long last_temp_check = 0;

void setup(){
  Serial.begin(115200);
  dht.begin();

  pinMode(LIGHT_MOSFET, OUTPUT);
  pinMode(HALOGEN_HEATER_MOSFET, OUTPUT);
  pinMode(RELAY_PUMP, OUTPUT);

  pinMode(SWITCH_HEATER, INPUT_PULLUP);
  pinMode(BTN_PUMP, INPUT_PULLUP);

  real_temp = 50;

  digitalWrite(HALOGEN_HEATER_MOSFET, 1);
  last_temp_check = millis();
}

void loop() {

//  waterlevel = analogRead(WATERMETER);
//  if ((waterlevel >= WATERMETER_LOW) && (waterlevel <= WATERMETER_HIGH)) {
    if (digitalRead(BTN_PUMP) == BTN_PUMP_PRESSED) {
      digitalWrite(RELAY_PUMP, 1);
    } else {
      digitalWrite(RELAY_PUMP, 0);
    }
//  } else {
//    digitalWrite(RELAY_PUMP, 0);
//  }

  if ((millis() - last_temp_check) > TEMP_CHECK_PERIOD) {

    temp = (int)dht.readTemperature();

    if (temp != temp_prev) {
      temp_prev = temp;
    }
    
    if (temp > real_temp) {
      digitalWrite(HALOGEN_HEATER_MOSFET, 0);
    } else if (temp < (real_temp - HYSTERESIS)) {
      digitalWrite(HALOGEN_HEATER_MOSFET, 1);
    }

    last_temp_check = millis();
  }

  if (digitalRead(SWITCH_HEATER) == SWITCH_HEATER_ON) {
    digitalWrite(HALOGEN_HEATER_MOSFET, 0);
  }

  pot_value = analogRead(POT_PIN);
  _pwm = map(pot_value, 0, 1023, 0, 255);
  analogWrite(LIGHT_MOSFET, _pwm);

  Serial.print(real_temp);
  Serial.print("\t");
  Serial.print(temp);
  Serial.print("\t");
  Serial.print(real_temp - HYSTERESIS);
  // Serial.print(temp);
  Serial.print("\t");
  // Serial.print("\t");
  Serial.print(waterlevel);
  Serial.println();

}