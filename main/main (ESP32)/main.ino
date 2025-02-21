#include "WifiConnection.h"
#include <PubSubClient.h>
#include <SensorsData.h>
#include <ArduinoJson.h>
#include <time.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
//#include "MqttServer.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -18000, 60000); // Zona horaria UTC-5 (Ecuador)

WiFiClient espClient;
PubSubClient client(espClient);
const char *mqtt_server = "192.168.100.30";
//const char *mqtt_server = "172.20.10.3";
//const char *mqtt_server = "192.168.200.41";
const char* mqtt_topic = "cultivos/Sensores";

long lastMsg = 0;


void setup()
{
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    initSensors();
    timeClient.begin();
}

void loop()
{
    if(!client.connected()){
      connect_mqttServer();
    }
    client.loop();
    long now = millis();
    if(now - lastMsg > 4000){
      lastMsg = now;
    }
    timeClient.update();  // Actualiza la hora desde el servidor NTP

 // Obtener la fecha y hora
  unsigned long epochTime = timeClient.getEpochTime();
  String dateTimeString = formatDateTime(epochTime);

    float nivel = leerNivel();
    float temp = leerTemperature();
    float ph = leerPh();
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["cultivoId"] = "FGemeh2IBX7OuBpYRqKf";
    jsonDoc["dateTime"] = dateTimeString;
    jsonDoc["pH"] = ph;
    jsonDoc["sensorId"] = "sensor123";
    jsonDoc["temperature"] = temp;
    jsonDoc["userId"] = "vRvjve8tbfQlrkYkNqow9D1cHfV2";
    jsonDoc["waterLevel"] = nivel;
    // Convertir JSON a cadena
    char jsonBuffer[512];
    serializeJson(jsonDoc, jsonBuffer);
    // Publicar mensaje en MQTT
    client.publish("esp32/sensor1", jsonBuffer);
    // Imprimir datos en el monitor serial
    Serial.println("Datos enviados:");
    Serial.println(jsonBuffer);

    // Esperar 30 minutos
    //delay(30 * 60 * 1000);
    // Esperar 10 minutos
    //delay(10*60*1000);
    // Esperar 10 segundos
    delay(60000);
}

// Connection MQTTSERVER   

void connect_mqttServer()
{
    while (!client.connected())
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            setup_wifi();
        }

        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("ESP32_client1"))
        { 
            Serial.println("connected");
            client.subscribe("rpi/broadcast");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" trying again in 2 seconds");
            delay(2000);
        }
    }
}

void callback(char *topic, byte *message, unsigned int length)
{
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();
}

// Función para formatear la fecha y hora
String formatDateTime(unsigned long epochTime) {
  struct tm *timeinfo;
  time_t rawtime = epochTime;
  timeinfo = localtime(&rawtime);
  
  char buffer[20];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  return String(buffer);
}