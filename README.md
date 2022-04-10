TRACTIAN CHALLENGE

A Temperature and Humidity Measurement Program was developed with the DHT22 Sensor that simulates sending data every 60 seconds to another location with 100 meters.
The Temperature and Humidity Data are being sent by Lora for a distance of up to 4.7km, the maximum sending capacity of Lora is 256Bytes, for larger files it is necessary to split the file before sending
The temperature and humidity data are being shown on the OLED display and sent by BLE, if it is necessary to increase the battery charge life, just do not set the USE_DISPLAY
BLE can send small messages up to 50 meters, if Lora can't send the data, just set the USE_BLE
Deep Sleep is being used to save battery, every 60 seconds esp 32 wakes up
Author Rafael Pascale 04/09/22




DESAFIO TRACTIAN 

Foi desenvolvido um Programa de Medição de Temperatura e Umidade com o Sensor DHT22 que simula um envio de dados a cada 60 segundos para um outro local com 100 metros.
Os Dados de Temperatura e Umidade estão sendo enviados por Lora para uma distancia de ate 4,7km, a capacidade maxima de envio Lora é de 256Bytes, para arquivos maiores é preciso dividir o arquivo antes do envio
Os dados de temperatura e umidade estao sendo mostrados no display OLED e enviados por BLE, caso seja necessario aumentar a vida da carga da bateria basta não definir o USE_DISPLAY
O BLE consegue mandar pequenas mensagens até 50 metros, caso o Lora não consiga mandar os dados, basta definir o USE_BLE
O deep Sleep está sendo usado para economizar bateria, a cada 60 segundos o esp 32 é acordado 
Autor Rafael Pascale 09/04/22
