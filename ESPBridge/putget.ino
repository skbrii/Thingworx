#include <SoftwareSerial.h>
#define ESPRX 6
#define ESPTX 7

SoftwareSerial esp(ESPRX, ESPTX);
char server[] = "xxx.xxx.xxx.xxx";
char appKey[] = "xxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxxx";

String readBuffer;

void setup() {
  clearBuf();
  esp.begin(19200);
  Serial.begin(115200);
  delay(100);
  setServer();
  delay(100);
  float data=22;
  sendData(String(data),"temperature");
  Serial.println(getData("ventState"));
}

void loop() {
  while (esp.available() > 0)
  {
    Serial.write(esp.read());
  }
  while (Serial.available() > 0)
  {
    esp.write(Serial.read());
  }
  // sendData(25.0);
}

void setServer()
{
  esp.print(F("\r\n"));
  esp.print(F("hxx.xx.xx.xx\r\n"));
  esp.print(F("p80\r\n"));
}

void sendData(String data,String param)
{
  clearBuf();
  String json=String("{\""+param+"\": "+data+"}");
  esp.print(F("aPUT /Thingworx/Things/TestT/Properties/"));
  esp.print(param);
  esp.println(F(" HTTP/1.1"));
  esp.print(F("aHost: "));
  esp.println(server);
  esp.println(F("aContent-Type: application/json"));
  esp.print(F("aContent-Length: "));
  esp.println(json.length());
  esp.print(F("aappKey: "));
  esp.println(appKey);
  esp.println(F("a"));
  esp.println("a"+json);
  esp.println(F("a"));
  esp.println(F("C"));
}

void clearBuf()
{
  esp.print(F("i\r\n"));
}

String getData(String param)
{
  String Data;
    clearBuf();
  esp.println(F("Mjson:"));
  esp.print(F("aGET /Thingworx/Things/TestT/Properties/"));
  esp.print(param);
  esp.println(F(" HTTP/1.1"));
  esp.print(F("aHost: "));
  esp.println(server);
  esp.println(F("aAccept: application/json"));
  esp.print(F("aappKey: "));
  esp.println(appKey);
  esp.println(F("a"));
  esp.println(F("C"));
  return Data;
}
