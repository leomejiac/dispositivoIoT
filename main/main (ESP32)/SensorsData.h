#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <OneWire.h>
#include <DallasTemperature.h>

// Selecciona el pin al que se conecta el sensor de temperatura
const int oneWireBus = 4;
const int pinNivel = 34;
const int potPin = A0;
float ph;
float Value = 0;

// Comunicar que vamos a utilizar la interfaz OneWire
OneWire oneWire(oneWireBus);
// Indica que el sensor utilizará la interfaz OneWire
DallasTemperature sensors(&oneWire);

// Función para inicializar los sensores
void initSensors() {
    sensors.begin();
    pinMode(pinNivel, INPUT);
    pinMode(potPin, INPUT);
}

// Función para leer la temperatura
float leerTemperature() {
    sensors.requestTemperatures();
    return sensors.getTempCByIndex(0);
}

float leerNivel() {
    int valNivel = analogRead(pinNivel);  // Leer el valor analógico del sensor
    int umbral = 500;                     
    if (valNivel > umbral) {
        valNivel = 1; // Hay agua
    } else {
        valNivel = 0; // No hay agua
    }
    return valNivel; // Retorna el valor modificado de valNivel
}

float leerPh(){
    Value = analogRead(potPin);
    float voltage = (Value * (5.0 / 4095.0));
    ph = (-5.6114 * voltage + 31.248) - 1;
    return ph;
}
#endif // SENSOR_DATA_H