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

  // Evento de presión del botón
  CMA_SOCKET.onPushButtonOnEvent("1736184508911", 
    []() {
      Serial.println("Push button event on");
    }
  );

  // Evento de soltar el botón
  CMA_SOCKET.onPushButtonOffEvent("1736184508911", 
    []() {
      Serial.println("Push button event off");
    }
  );
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

  // Evento que se ejecuta cuando se mantiene presionado el botón
  CMA_SOCKET.onPushButtonOn("1736184508911", 
    []() {
      Serial.println("Push button on");
    }
  );

  // Evento que se ejecuta cuando se suelta el botón
  CMA_SOCKET.onPushButtonOff("1736184508911", 
    []() {
      Serial.println("Push button off");
    }
  );
}

