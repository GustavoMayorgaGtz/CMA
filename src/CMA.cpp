#include "CMA.h"
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <WebSockets.h>
#include <SocketIOclient.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>

// Variables globales
bool socketConnected = false;           // Indica si el socket está conectado
SocketIOclient socketClient;            // Cliente SocketIO
WiFiMulti wifiManager;                  // Maneja múltiples conexiones Wi-Fi
int messageSendInterval = 1000;         // Intervalo de tiempo entre envíos de mensajes (en milisegundos)
unsigned long currentTime;              // Almacena el tiempo actual
unsigned long previousTime = 0;         // Variable auxiliar para gestionar tiempos

// Nodo principal de la lista de tiempos
GroupTime* timeListHead = NULL;

// Constructor de la clase CMA
CMA::CMA() {}

// Función para crear un nodo de tipo GroupTime
GroupTime* createGroupTimeNode(String groupName, long timestamp)
{
    GroupTime* newNode = new GroupTime; // Usamos `new` en lugar de `malloc` para una mejor gestión de memoria
    if (newNode == NULL)
    {
        Serial.println("Error al asignar memoria");
        return NULL; // Si no se pudo asignar memoria, devolvemos NULL
    }
    newNode->groupName = groupName; // Asignamos el nombre del grupo
    newNode->timestamp = timestamp; // Asignamos el tiempo (timestamp)
    newNode->nextNode = NULL;       // El siguiente nodo es NULL, es el final de la lista
    return newNode;
}

// Función para agregar un nodo al final de la lista de tiempos
void appendGroupTime(String groupName, long timestamp)
{
    GroupTime* newNode = createGroupTimeNode(groupName, timestamp); // Creamos el nuevo nodo
    if (timeListHead == NULL)
    {
        // Si la lista está vacía, el nuevo nodo se convierte en el primer nodo
        timeListHead = newNode;
    }
    else
    {
        // Si la lista no está vacía, recorremos hasta el final y agregamos el nuevo nodo
        GroupTime* currentNode = timeListHead;
        while (currentNode->nextNode != NULL)
        {
            currentNode = currentNode->nextNode;
        }
        currentNode->nextNode = newNode; // Enlazamos el nuevo nodo
    }
}

// Función para buscar el timestamp asociado a un grupo
long getGroupTimestamp(String groupName)
{
    if (timeListHead == NULL)
    {
        Serial.println("La lista de tiempos está vacía.");
        return -1; // Si la lista está vacía, devolvemos -1
    }

    GroupTime* currentNode = timeListHead;
    while (currentNode != NULL)
    {
        if (currentNode->groupName == groupName)
        {
            return currentNode->timestamp;  // Si encontramos el grupo, devolvemos su timestamp
        }
        currentNode = currentNode->nextNode;
    }
    return -1; // Si no se encuentra el grupo, devolvemos -1
}

// Función para actualizar el timestamp de un grupo específico
void updateGroupTimestamp(String groupName, long newTimestamp)
{
    if (timeListHead == NULL)
    {
        Serial.println("La lista de tiempos está vacía.");
        return;
    }

    GroupTime* currentNode = timeListHead;
    while (currentNode != NULL)
    {
        if (currentNode->groupName == groupName)
        {
            currentNode->timestamp = newTimestamp; // Actualizamos el timestamp del grupo
        }
        currentNode = currentNode->nextNode;
    }
}

// Función que procesa el JSON recibido
StaticJsonDocument<200> CMA::parseJsonData(String jsonData)
{
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, jsonData); // Deserializamos el JSON
    if (error)
    {
        DEBUG_PRINTLN("Error al deserializar JSON: ");
        DEBUG_PRINTLN(error.f_str());
    }
    return doc; // Retornamos el JSON procesado
}

// Inicialización del depurador (debugger)
void CMA::initializeDebugger()
{
#if CMA_DEBUG
    DEBUG_PRINTLN("Debugger inicializado");
#endif
}

// Evento de SocketIO que maneja las diferentes acciones
void socketIOEventHandler(socketIOmessageType_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case sIOtype_DISCONNECT:
        DEBUG_PRINTLN("[SocketIO] Desconectado del servidor.");
        if (payload != nullptr && length > 0)
        {
            char buffer[length + 1]; 
            memcpy(buffer, payload, length); // Copiamos el payload a un buffer
            buffer[length] = '\0';           // Añadimos el terminador nulo
            DEBUG_PRINTF("Payload: %s\n", buffer); // Mostramos el payload
        }
        else
        {
            DEBUG_PRINTLN("Payload vacío o nulo");
        }
        socketConnected = false;
        socketClient.begin("www.cmasystems.com.mx", 80, "/socket.io/?EIO=4");
        socketClient.onEvent(socketIOEventHandler);
        break;

    case sIOtype_CONNECT:
        DEBUG_PRINTLN("[SocketIO] Conectado al servidor.");
        socketConnected = true;
        socketClient.send(sIOtype_CONNECT, "/");
        break;

    case sIOtype_EVENT:
    {
        String eventData = (char *)payload;
        eventData = eventData.substring(1, eventData.length() - 1); // Procesamos el payload
        StaticJsonDocument<200> doc = CMA::parseJsonData(eventData);
        DEBUG_PRINTLN(String((const char *)payload)); // Mostramos el payload completo
        break;
    }

    case sIOtype_ACK:
    case sIOtype_ERROR:
    case sIOtype_BINARY_EVENT:
    case sIOtype_BINARY_ACK:
        break;
    }
}

// Función para definir el intervalo de envío de mensajes
void CMA::setMessageSendInterval(int interval)
{
    if (interval < 1000)
    {
        interval = 1000; // No permitir intervalos menores a 1000 ms
    }
    messageSendInterval = interval;
}

// Conectar a la red Wi-Fi y al servidor SocketIO
void CMA::connectToWiFiAndServer(char *ssid, char *password)
{
    wifiManager.addAP(ssid, password); // Añadimos el SSID y la contraseña
    DEBUG_PRINTLN("[CMA] Conectando a Wi-Fi...");
    while (wifiManager.run() != WL_CONNECTED)
    {
        delay(500); // Esperamos un poco antes de reintentar
        DEBUG_PRINTLN("[CMA] Wi-Fi no conectado. Reintentando...");
    }
    DEBUG_PRINTLN("[CMA] Wi-Fi conectado.");

    DEBUG_PRINTLN("[CMA] Intentando conectar con el servidor...");
    socketClient.begin("www.cmasystems.com.mx", 80, "/socket.io/?EIO=4");
    socketClient.onEvent(socketIOEventHandler);
}

// Bucle principal para la conexión con el servidor
void CMA::loop()
{
    currentTime = millis(); // Capturamos el tiempo actual
    socketClient.loop();    // Llamamos al loop del cliente SocketIO
}

// Función para unirse a un grupo
void CMA::joinGroup(String groupName)
{
    DynamicJsonDocument doc(1024);
    JsonArray jsonArray = doc.to<JsonArray>();
    jsonArray.add("joinGroup"); // Evento para unirse al grupo
    JsonObject groupObject = jsonArray.createNestedObject();
    groupObject["group"] = groupName; // Añadimos el nombre del grupo
    String output;
    serializeJson(doc, output); // Serializamos el JSON
    socketClient.sendEVENT(output); // Enviamos el evento al servidor
}

// Función para enviar datos a un grupo
void CMA::sendMessageToGroup(String groupName, float messageData)
{
    if (getGroupTimestamp(groupName) == -1)
    {
        appendGroupTime(groupName, currentTime); // Si el grupo no existe, lo agregamos
    }
    else
    {
        // Si ha pasado suficiente tiempo, actualizamos el timestamp y enviamos los datos
        if (currentTime - getGroupTimestamp(groupName) >= messageSendInterval)
        {
            updateGroupTimestamp(groupName, currentTime); // Actualizamos el timestamp
            if (socketConnected)
            {
                if (isnan(messageData)) 
                {
                    messageData = random(1, 101); // Si el dato es NaN, generamos un número aleatorio
                }

                DynamicJsonDocument doc(1024);
                JsonArray jsonArray = doc.to<JsonArray>();
                jsonArray.add("sendMessageToGroup"); // Evento para enviar mensaje al grupo
                JsonObject messageObject = jsonArray.createNestedObject();
                messageObject["groupName"] = groupName;
                messageObject["message"] = String(messageData); // Añadimos el mensaje al JSON
                String output;
                serializeJson(doc, output); // Serializamos el JSON
                socketClient.sendEVENT(output); // Enviamos el evento al servidor
                DEBUG_PRINTLN("[CMA] ¡Mensaje enviado al grupo!");
            }
        }
    }
}
