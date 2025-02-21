#include <PubSubClient.h>   

void connect_mqttServer()
{
    while (!client.connected())
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            setup_wifi();
        }
        Serial.print("Attempting MQTT connection...");

        if (client.connect("ESP32_client1"))
        { 
            Serial.println("connected");
            client.subscribe("rpi/broadcast");
        }
        else
        {
            Serial.print("failed, rc=");x
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
