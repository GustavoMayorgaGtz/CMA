#include <Arduino.h>
#include <CMA.h>

// Configuración de conexiones con el servidor CMA
CMA CMA_SOCKET;

// Credenciales de la red WiFi
char* ssid = "CyberSphere2.4";        // Nombre de la red WiFi
char* pass = "KD6JVB8kmCRe";    // Contraseña de la red WiFi

/**
 * @brief Función de configuración inicial de Arduino.
 */
void setup() {
  Serial.begin(9600);
  
  // Conectar al servidor de CMA
  CMA_SOCKET.connectToWiFiAndServer(ssid, pass);
  CMA_SOCKET.setMessageSendInterval(10000); // Establecer el envío de datos cada 10 segundos
}

void pushButtonOn() {
  Serial.println("Push button on");
}

/**
 * @brief Bucle principal que se ejecuta continuamente.
 */
void loop() {
  delay(8);
  CMA_SOCKET.loop();
  
  // Enviar mensajes a los grupos
  CMA_SOCKET.sendMessageToGroup("1733865367813", 2);
  CMA_SOCKET.sendMessageToGroup("1733865397296", 3);
  CMA_SOCKET.sendMessageToGroup("1733865419974", 4);
  CMA_SOCKET.sendMessageToGroup("1733865475840", 5);

  CMA_SOCKET.pushButtonOn("1736184508911", pushButtonOn);

  CMA_SOCKET.pushButtonOff("1736184508911", 
    []() {
      Serial.println("Push button off");
    }
  );
}

