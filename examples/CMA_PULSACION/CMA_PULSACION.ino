#include <Arduino.h>
#include <CMA.h>

// Configuración de conexiones con el servidor CMA
CMA CMA_SOCKET;

// Credenciales de la red WiFi
char* ssid = "<your ssid>";        // Nombre de la red WiFi
char* pass = "<your password>";    // Contraseña de la red WiFi

/**
 * @brief Función de configuración inicial de Arduino.
 */
void setup() {
  Serial.begin(9600);
  
  // Conectar al servidor de CMA
  CMA_SOCKET.connectToWiFiAndServer(ssid, pass);
  

  // Evento de presión del botón
  CMA_SOCKET.onPushButtonOnEvent("1736352988319", 
    []() {
      Serial.println("Push button event on");
    }
  );

  // Evento de soltar el botón
  CMA_SOCKET.onPushButtonOffEvent("1736352988319", 
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
  
  // Evento que se ejecuta cuando se mantiene presionado el botón
  CMA_SOCKET.onPushButtonOn("1736352988319", 
    []() {
      Serial.println("Push button on");
    }
  );

  // Evento que se ejecuta cuando se suelta el botón
  CMA_SOCKET.onPushButtonOff("1736352988319", 
    []() {
      Serial.println("Push button off");
    }
  );
}

