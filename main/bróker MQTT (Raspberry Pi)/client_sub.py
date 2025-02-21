import paho.mqtt.client as mqtt
import firebase_admin
from firebase_admin import credentials, firestore
import json

# Configurar Firebase
cred = credentials.Certificate("hidroweb-data-firebase-adminsdk-6rh5u-b5d838b5a7.json")  # Asegúrate de usar la ruta correcta
firebase_admin.initialize_app(cred)
db = firestore.client()

# Callback cuando se recibe un mensaje en el tópico MQTT
def on_message(client, userdata, msg):
    print(f"Mensaje recibido en {msg.topic}: {msg.payload.decode()}")
    try:
        # Decodificar el mensaje recibido (esperamos que sea JSON)
        data = json.loads(msg.payload.decode())
        
        # Acceder a los valores del JSON
        temperature = data["temperature"]
        pH = data["pH"]
        waterLevel = data["waterLevel"]
        dateTime = data["dateTime"]
        cultivoId = data["cultivoId"]
        sensor = data["sensorId"]
        usuario = data["userId"]

        # Subir cada campo a Firestore como documento independiente
        db.collection('Sensores').add({
            'cultivoId': cultivoId,
            'dateTime': dateTime,
            'pH': pH,
            'sensorId': sensor,
            'temperature': temperature,
            'userId': usuario,
            'waterLevel': waterLevel,
            
        })
        print(f"Datos subidos a Firestore: Temperatura={temperature}, pH={pH}, Nivel de Agua={waterLevel}, Fecha y Hora={dateTime}")
    
    except Exception as e:
        print(f"Error al procesar los datos: {e}")

# Configurar el cliente MQTT
mqtt_broker = "localhost"  # La Raspberry Pi es el broker MQTT
mqtt_port = 1883
topic = "esp32/sensor1"

client = mqtt.Client()
client.on_message = on_message

client.connect(mqtt_broker, mqtt_port, 60)
client.subscribe(topic)

# Mantener el cliente ejecutándose y escuchando mensajes
client.loop_forever()