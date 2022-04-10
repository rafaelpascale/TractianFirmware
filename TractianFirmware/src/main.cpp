/*
Programa de Medição de Temperatura e Umidade com o Sensor DHT22
Dados de Temperatura e Umidade sendo enviados por Lora para uma distancia de ate 4,7km
A capacidade maxima de envio Lora é de 256Bytes, para arquivos maiores é preciso dividir o arquivo antes do envio
Os dados de temperatura e umidade estao sendo mostrados no display OLED
Autor Rafael Pascale 09/04/22
Empresa Tracitan 
*/

#include <Arduino.h>
#include <WiFi.h>


#define USE_LORA
#define USE_DHT
#define USE_DISPLAY
#define USE_BLE

#define ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())
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
#define BAND 915E6 //Frequência do radio - exemplo : 433E6, 868E6, 915E6
#endif

#ifdef USE_DISPLAY
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
#include "images.h"             // Imagem do Logo da Tractian
#endif

#ifdef USE_BLE
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
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


#ifdef USE_LORA
String mac = WiFi.macAddress();
String packet = String(mac) + ("|")+ String(temperatura) + ("|")+ String(umidade);   //variável usada para armazenar a string enviada, ao final ela é concatenada com a variável "values" abaixo
void sendPacket()
{
  LoRa.beginPacket();//beginPacket : abre um pacote para adicionarmos os dados para envio 
  LoRa.print(packet); //envia pacote    
  LoRa.endPacket(); //endPacket : fecha o pacote e envia //retorno= 1:sucesso | 0: falha
  Serial.println("Pacote Enviado desse modelo + temperatura + umidade"); 
}
#endif



#ifdef USE_DISPLAY
#define SDA 21   // GPIO5  SCK
#define SCL 22 // GPIO19 MISO
#define DEMO_DURATION 3000
SSD1306Wire display(0x3c, SDA, SCL); 

void drawImageDemo() {
  display.drawXbm(34, 14, Tractian_Logo_width, Tractian_Logo_height, Tractian_Logo_bits);
   display.display();
}

void drawData() {
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(23, 22, stemperatura);
  display.drawString(23, 44, sumidade);
   display.display();
}

void drawInicio() {

  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(22, 22, "Ligando"); 
   display.display();
}

#endif



#ifdef USE_BLE
BLECharacteristic *characteristicTX; //através desse objeto iremos enviar dados para o client
bool deviceConnected = false; //controle de dispositivo conectado

#define SERVICE_UUID           "ab0828b1-198e-4351-b779-901fa0e0371e" // UART service UUID
#define CHARACTERISTIC_UUID_RX "4ac8a682-9736-4e5d-932b-e9b31405049c"
#define CHARACTERISTIC_UUID_TX "0972EF8C-7613-4075-AD52-756F33D4DA91"


//callback para receber os eventos de conexão de dispositivos
class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

//callback  para envendos das características
class CharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *characteristic) {
      //retorna ponteiro para o registrador contendo o valor atual da caracteristica
      std::string rxValue = characteristic->getValue(); 
      //verifica se existe dados (tamanho maior que zero)
      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");

        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i]);
        }

        Serial.println();

        // Do stuff based on the command received
        if (rxValue.find("L1") != -1) { 
          Serial.print("Turning LED ON!");

        }
        else if (rxValue.find("L0") != -1) {
          Serial.print("Turning LED OFF!");

        }
        // Do stuff based on the command received from the app
        else if (rxValue.find("B1") != -1) { 
          Serial.print("Turning Buzzer ON!");

        }
        else if (rxValue.find("B0") != -1) {
          Serial.print("Turning Buzzer OFF!");

        }

        Serial.println();
        Serial.println("*********");
      }
    }
};
#endif





uint32_t delayMS;
unsigned long previousMillis = 0;

/*
************************************************************************************************************
********************************************SETUP***********************************************************
************************************************************************************************************
*/
void setup() {
  Serial.begin(9600);

#ifdef USE_DISPLAY
  display.init();
  display.flipScreenVertically();
  drawInicio() ;
  delay(3000);
#endif

  #ifdef USE_LORA
  SPI.begin(SCK,MISO,MOSI,SS); 
  LoRa.setPins(SS,RST,DI00); 
  LoRa.begin(BAND);
  if (!LoRa.begin(BAND)) {
    Serial.println(F("Erro ao Inciar o Lora "));
    while (true);}
  #endif

   #ifdef USE_DHT
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;
  #endif

  #ifdef USE_BLE
  BLEDevice::init("Tractian"); // nome do dispositivo bluetooth  
  BLEServer *server = BLEDevice::createServer(); //cria um BLE server 
  server->setCallbacks(new ServerCallbacks()); //seta o callback do server
  BLEService *service = server->createService(SERVICE_UUID);
  // Create a BLE Characteristic para envio de dados
  characteristicTX = service->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                      
  characteristicTX->addDescriptor(new BLE2902());

  // Create a BLE Characteristic para recebimento de dados
  BLECharacteristic *characteristic = service->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  characteristic->setCallbacks(new CharacteristicCallbacks());
  // Start the service
  service->start();
  // Start advertising (descoberta do ESP32)
  server->getAdvertising()->start();
  #endif
}


/*
***********************************************************************************************************
********************************************LOOP***********************************************************
***********************************************************************************************************
*/

void loop() {
  unsigned long currentMillis = millis();

  #ifdef USE_DISPLAY
    if (currentMillis - previousMillis >= 3000){
    previousMillis = currentMillis;
     drawImageDemo();
      }
   #endif

  #ifdef USE_DHT
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  temperatura = event.temperature;
  umidade = event.relative_humidity;
  if (isnan(temperatura)) {
    Serial.println(F("Erro ao medir Temperatura!"));
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

    #ifdef USE_DISPLAY
    if (currentMillis - previousMillis >= 3000){
    previousMillis = currentMillis;
    display.clear();
     drawData();
      }
   #endif

  #ifdef USE_LORA
   for (int i = 0; i <= 10; i++) {
   sendPacket();}
  #endif

  #ifdef USE_BLE
  if (deviceConnected) {
 // Convetendo um valor para uma array
  char tString[8]; 
  dtostrf(temperatura, 1, 2, tString); // float_val, min_width, digits_after_decimal, char_buffer
  char uString[8]; 
  dtostrf(umidade, 1, 2, uString); // float_val, min_width, digits_after_decimal, char_buffer

  characteristicTX->setValue(tString + "C"); //seta o valor que a caracteristica notificará (enviar)       
  characteristicTX->notify(); // Envia o valor para o smartphone  

  characteristicTX->setValue(uString); //seta o valor que a caracteristica notificará (enviar)       
  characteristicTX->notify(); // Envia o valor para o smartphone 

  }
  #endif

}
