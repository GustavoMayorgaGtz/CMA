#ifndef CMA_H
#define CMA_H

#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <WebSockets.h>
#include <SocketIOclient.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>

// Definir una macro para debug
#define CMA_DEBUG 1 // Actívalo (1) o desactívalo (0)

#if CMA_DEBUG
    #define DEBUG_PRINT(x)   Serial.print(x)
    #define DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)
    #define DEBUG_PRINTLN(x) Serial.println(x)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
    #define DEBUG_PRINTF(...)
#endif

// Configuración de WiFi
extern WiFiMulti wifiManager;  // Cambiado de Wifi a wifiManager

extern bool socketConnected;  // Cambiado de isConnectSocket a socketConnected
extern void socketIOEventHandler(socketIOmessageType_t type, uint8_t *payload, size_t length); // Cambiado el nombre de la función
extern SocketIOclient socketClient;  // Cambiado de client_io a socketClient
extern int messageSendInterval;  // Cambiado de waiting_send_time a messageSendInterval
extern unsigned long currentTime;  // Cambiado de time_now a currentTime
extern unsigned long previousTime;  // Cambiado de time_aux a previousTime

// Estructura para manejar los tiempos de los grupos
struct GroupTime
{
    String groupName;  // Cambiado de groupname a groupName
    long timestamp;    // Cambiado de time a timestamp
    GroupTime* nextNode;  // Cambiado de time_next a nextNode
};

struct PushButton
{
    String groupName;  // Cambiado de groupname a groupName
    String status;        // Cambiado de status a status
    bool joined; //Muestra el estado del boton
    std::function<void()> onPushButtonOnCallback;
    std::function<void()> onPushButtonOffCallback;
    PushButton* nextNode;  // Cambiado de servo_next a nextNode
};

// Funciones externas
extern void appendGroupTime(String groupName, long timestamp);  // Cambiado de append a appendGroupTime
extern long getGroupTimestamp(String groupNameSearch);  // Cambiado de timeByGroupname a getGroupTimestamp
extern void updateGroupTimestamp(String groupNameSearch, long newTimestamp);  // Cambiado de settimeByGroupname a updateGroupTimestamp
void joinGroup(String groupName); // Unirse a un grupo de websocket

class CMA
{
public:
    CMA();
    // ---------------------------------------- UTILS ---------------------------------------- //
    static StaticJsonDocument<200> parseJsonData(String data);  // Cambiado de getData a parseJsonData
    void connectToWiFiAndServer(char* ssid, char* pass);  // Cambiado de connect a connectToWiFiAndServer
    
    void setMessageSendInterval(int interval);  // Cambiado de set_refresh_time a setMessageSendInterval
    // ---------------------------------------- INDICADORES ---------------------------------------- //
    void sendMessageToGroup(String groupName, float messageData = NAN);  // Cambiado de sendToGroup a sendMessageToGroup
    // ---------------------------------------- PULSADORES ---------------------------------------- //
    void onPushButtonOn(String groupName, const std::function<void()>& callback); // Callback para cuando se mantiene presiona el botón
    void onPushButtonOff(String groupName, const std::function<void()>& callback); // Callback para cuando se suelta el botón
    void onPushButtonOnEvent(String groupName, const std::function<void()>& callback); // Callback para evento de presión
    void onPushButtonOffEvent(String groupName, const std::function<void()>& callback); // Callback para evento de soltar
    // ---------------------------------------- LOOP ---------------------------------------- //
    void loop();

private:
    void initializeDebugger();
};

#endif

