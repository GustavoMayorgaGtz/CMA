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
    PushButton* nextNode;  // Cambiado de servo_next a nextNode
};

// Funciones externas
extern void appendGroupTime(String groupName, long timestamp);  // Cambiado de append a appendGroupTime
extern long getGroupTimestamp(String groupNameSearch);  // Cambiado de timeByGroupname a getGroupTimestamp
extern void updateGroupTimestamp(String groupNameSearch, long newTimestamp);  // Cambiado de settimeByGroupname a updateGroupTimestamp

class CMA
{
public:
    CMA();
    static StaticJsonDocument<200> parseJsonData(String data);  // Cambiado de getData a parseJsonData
    void connectToWiFiAndServer(char* ssid, char* pass);  // Cambiado de connect a connectToWiFiAndServer
    void joinGroup(String groupName);
    void loop();
    void setMessageSendInterval(int interval);  // Cambiado de set_refresh_time a setMessageSendInterval
    void sendMessageToGroup(String groupName, float messageData = NAN);  // Cambiado de sendToGroup a sendMessageToGroup
    void pushButtonOn(String groupName, const std::function<void()>& callback);
    void pushButtonOff(String groupName, const std::function<void()>& callback);

private:
    void initializeDebugger();
};

#endif

