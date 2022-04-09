#include <Arduino.h>
#define USE_LORA
#define USE_DHT
#define USE_DISPLAY


#ifdef USE_DHT
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#endif

#ifdef USE_LORA
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#define SCK 5   // GPIO5  SCK
#define MISO 19 // GPIO19 MISO
#define MOSI 27 // GPIO27 MOSI
#define SS 18   // GPIO18 CS
#define RST 14  // GPIO14 RESET
#define DI00 26 // GPIO26 IRQ(Interrupt Request)
#define BAND 433E6 //Frequência do radio - exemplo : 433E6, 868E6, 915E6
#endif

#ifdef USE_DISPLAY
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
#include "images.h"
#endif

#ifdef USE_DHT
#define DHTPIN 35     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
DHT_Unified dht(DHTPIN, DHTTYPE);
  float temperatura;
  float umidade;
  String stemperatura = String("Temperatura :") + String(temperatura);
  String sumidade = String("Umidade :") + String(umidade);;
#endif

#ifdef USE_DISPLAY
#define SDA 21   // GPIO5  SCK
#define SCL 22 // GPIO19 MISO
#define DEMO_DURATION 3000
SSD1306Wire display(0x3c, SDA, SCL); 

void drawImageDemo() {
  display.drawXbm(34, 14, Tractian_Logo_width, Tractian_Logo_height, Tractian_Logo_bits);
}

void drawData() {
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(23, 22, stemperatura);
  display.drawString(23, 44, sumidade);
}

void drawInicio() {

  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(22, 22, "Ligando"); 
}

#endif
uint32_t delayMS;

void setup() {
  Serial.begin(9600);

#ifdef USE_DISPLAY
  display.init();
  display.flipScreenVertically();
  drawImageDemo();
#endif


   #ifdef USE_DHT
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;
  #endif
}
void loop() {
  

  #ifdef USE_DHT
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  temperatura = event.temperature;
  umidade = event.relative_humidity;
  if (isnan(temperatura)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(temperatura);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(umidade)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(umidade);
    Serial.println(F("%"));
  }
  #endif
}
