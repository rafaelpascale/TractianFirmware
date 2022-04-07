#include <Arduino.h>
//#define USE_LORA
//#define USE_DHT


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

#ifdef USE_DHT
#define DHTPIN 35     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
DHT_Unified dht(DHTPIN, DHTTYPE);
#endif


#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
#include "images.h"            // Imagem da Tractian 

uint32_t delayMS;

void setup() {
  Serial.begin(9600);
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
  float temperatura = event.temperature;
  float umidade = event.relative_humidity;
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
