/*ESP32 kart ile WebServer & 2 adet SPI iletişim protokolü kullanan modül çalıştırma
Nokia5110 SPI LCD + MAX6675 K Type Thermocouple + ESP32 WebServer STA Mode
Dr.TRonik YouTube/ Mayıs2024/ izmir/Türkiye*/

/********************************************************************
  GLOBALS___GLOBALS___GLOBALS___GLOBALS___GLOBALS___GLOBALS___
 ********************************************************************/
//Nokia5110 LCD
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//MAX6675 K type Thermocouple
#include "max6675.h"  // https://github.com/adafruit/MAX6675-library

//WEB SERVER
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>  // https://github.com/bbx10/WebServer_tng
#include "PageIndex.h"  // Aynı çalışma klasörü içinde olmalı...

WebServer server(80);

const char* ssid = "Your_SSID_name";
const char* password = "Your_pass";

//MAX6675 pin tanımları- Define the pins that are interfacing with the MAX6675 thermocouple amplifier.
int soPin = 27;   // SO=Serial Out
int csPin = 14;   // CS = chip select CS pin
int sckPin = 13;  // SCK = Serial Clock pin

//Create a MAX6675 object called thermocouple on the pins we’ve defined previously.
MAX6675 sensor(sckPin, csPin, soPin); 

/* Declare LCD object for SPI
 Adafruit_PCD8544(CLK,DIN,D/C,CE,RST); */
Adafruit_PCD8544 display = Adafruit_PCD8544(18, 23, 4, 15, 2);

int contrastValue = 60;  // Default Contrast Value

/********************************************************************
  SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___
 ********************************************************************/
void setup() {
  Serial.begin(9600);  // initialize serial monitor with 9600 baud
  delay(500);
  connectToWifi();
  beginServer();
  server.on("/", handleRoot);                        // Root web sayfası
  server.on("/readTemperature", handleTemperature);  // İstemicden gelen readTemperature isteği sonrasında handleTemperature fonksiyonu çalışır...

  /* Initialize the Display*/
  display.begin();

  /* Change the contrast using the following API*/
  display.setContrast(contrastValue);

  /* Clear the buffer */
  display.clearDisplay();
  display.display();
  delay(1000);

  /* TEXT */
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 1);
  display.setTextSize(2);
  display.println("|ESP32|");
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(19, 20);
  display.println("Dr.TRonik");
  display.setCursor(23, 32);
  display.println("YouTube");
  display.setCursor(0, 41);
  display.print(WiFi.localIP());
  display.display();
  delay(4000);
  display.clearDisplay();
}

/********************************************************************
  LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__
********************************************************************/
void loop() {

  server.handleClient();
  /* TEXT */
  Serial.print("C = ");
  Serial.println(sensor.readCelsius());
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("SICAKLIK:");
  display.setCursor(72, 12);
  display.print("'C");
  display.setCursor(20, 41);
  display.print("Dr.TRonik");
  display.setCursor(0, 13);
  display.setTextSize(3);
  
  //returns temperature in Celsius degrees & correction factor:
  display.print((sensor.readCelsius() - 2), 1);  //delay AT LEAST 250ms between reads!
  
  display.display();
  delay(2000);
  display.clearDisplay();
}

/********************************************************************
  VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs
********************************************************************/
void connectToWifi() {
  WiFi.enableSTA(true);
  delay(2000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void beginServer() {
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void handleRoot() {
  String s = MAIN_page;              // MAIN_page adı altında bir web sayfası içeriği...
  server.send(200, "text/html", s);  // Bu web sayfasının gösterilme işlemi...
}

void handleTemperature() {
  float t = (sensor.readCelsius()) - 2.00;  // iki derece kadar ısı farkı oluşabiliyor...
  String Temperature_Value = String(t);
  server.send(200, "text/plane", Temperature_Value);  // İstemci tarafından (function getTemperatureData()) ile talep edildiğinde "Temperature_value" değerini 200 kodu ile gönderme...
}
/* ___:
e-posta: bilgi@ronaer.com
https://www.instagram.com/dr.tronik2023/   
YouTube: Dr.TRonik: https://www.youtube.com/@DrTRonik
PCBWay: https://www.pcbway.com/project/member/shareproject/?bmbno=A0E12018-0BBC-4C
*/