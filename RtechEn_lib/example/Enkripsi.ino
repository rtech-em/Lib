
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <RtechEnLIB.h>

#ifndef STASSID
#define STASSID "R-Tech Embedded"
#define STAPSK  "rtechjws123"
#endif

const char* host = "rtech";
const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

IPAddress local_ip(192, 168, 10, 10);
IPAddress gateway(192, 168, 10, 10);
IPAddress netmask(255, 255, 255, 0);

RTECHEN rtech;
char tmp[18];

//char *sp = "l_chdRhUsaie";

void setup() {
  Serial.begin(115200);
  Serial.println("");
  rtech.begin();

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, netmask);
  WiFi.softAP(ssid, password);

  MDNS.begin(host);

  httpUpdater.setup(&httpServer);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);  
  WiFi.macAddress().toCharArray(tmp, WiFi.macAddress().length() + 1);
  //Serial.print("MAC : "); Serial.println(tmp);
  rtech.encryption(tmp);
  //Serial.print("Check : "); Serial.println(rtech.checking(tmp, '123'));
  //rtech.decryption(sp);
}

void loop() {
  httpServer.handleClient();
  MDNS.update();

}
