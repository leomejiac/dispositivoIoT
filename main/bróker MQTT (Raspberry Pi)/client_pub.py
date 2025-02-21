import time
import paho.mqtt.client as mqtt

# Variable global para almacenar el mensaje recibido
received_message = None

# Callback cuando se recibe un mensaje en el tópico MQTT
def on_message(client, userdata, msg):
    global received_message
    received_message = msg.payload.decode()  # Decodificar el mensaje recibido
    print(f"Mensaje recibido en {msg.topic}: {received_message}")

# Callback para la publicación
def on_publish(client, userdata, mid):
    print("Message published")

# Configurar el cliente MQTT
client = mqtt.Client("rpi_client2")  # Nombre único para el cliente
client.on_publish = on_publish
client.on_message = on_message

client.connect('127.0.0.1', 1883)
client.subscribe("rpi/request")  # Suscribirse a un tópico para recibir mensajes

client.loop_start()  # Iniciar el bucle del cliente

while True:
    # Esperar hasta que se reciba un mensaje
    if received_message is not None:
        # Publicar un mensaje en respuesta al mensaje recibido
        msg_to_send = f"Respuesta a: {received_message}"
        pubMsg = client.publish(
            topic='rpi/broadcast',
            payload=msg_to_send.encode('utf-8'),
            qos=0,
        )
        pubMsg.wait_for_publish()
        print(f"Publicado mensaje: {msg_to_send} - Is published: {pubMsg.is_published()}")

        # Reiniciar la variable para esperar un nuevo mensaje
        received_message = None

    time.sleep(1)  # Esperar un segundo antes de comprobar nuevamente

client.loop_stop()  # Detener el bucle del cliente
client.disconnect()  # Desconectar del broker