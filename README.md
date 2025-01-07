# Librería CMA para Arduino

La librería **CMA** permite la integración con un servidor **CMA Web SCADA** utilizando **WebSocket** y **Socket.IO**, facilitando la conexión, el envío de datos y la gestión de grupos de dispositivos en proyectos de monitoreo y control.

## Descripción

La librería está diseñada para ser utilizada con dispositivos basados en **ESP32** o **ESP8266** que se conectan a un servidor Web SCADA mediante la tecnología de WebSocket. Permite:

- Conectar a una red Wi-Fi.
- Conectar al servidor **CMA Web SCADA**.
- Enviar datos de sensores u otros dispositivos a grupos de manera periódica.
- Gestionar grupos y actualizar los datos en tiempo real.

## Características

- **Conexión Wi-Fi**: Conéctate fácilmente a una red Wi-Fi.
- **WebSocket**: Comunicación en tiempo real con un servidor CMA utilizando el protocolo WebSocket.
- **Gestión de grupos**: Envía mensajes y datos a grupos de dispositivos.
- **Envío periódico de datos**: Configura el intervalo de envío de datos para evitar sobrecargar la red.
- **Desarrollo con Arduino IDE**: Fácil integración en proyectos basados en Arduino y plataformas compatibles.

## Instalación

1. **Descarga la librería**:
   - Puedes descargar la librería desde el repositorio oficial de GitHub o copiar el código a tu entorno de desarrollo.
2. **Añadir la librería al Arduino IDE**:
   - Abre el **Arduino IDE**.
   - Ve a **Sketch > Incluir Librería > Añadir .ZIP Librería**.
   - Selecciona el archivo `.zip` o el directorio donde hayas descargado la librería.

## Ejemplo de uso

### Conexión a Wi-Fi y servidor

```cpp
#include <CMA.h>

// Configuración de conexiones con el servidor CMA
CMA CMA_SOCKET;

// Credenciales de la red Wi-Fi
char* ssid = "Nombre_de_tu_red";        // Nombre de la red WiFi
char* pass = "Contraseña_de_tu_red";    // Contraseña de la red WiFi

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
}

void loop() {
  delay(8);
  CMA_SOCKET.loop();

  // Enviar mensajes a los grupos
  CMA_SOCKET.sendMessageToGroup("1733865367813", 2);
  CMA_SOCKET.sendMessageToGroup("1733865397296", 3);
  CMA_SOCKET.sendMessageToGroup("1733865419974", 4);
  CMA_SOCKET.sendMessageToGroup("1733865475840", 5);

  // Evento de presión del botón
  CMA_SOCKET.onPushButtonOn("1736184508911",
    []() {
      Serial.println("Push button on");
    }
  );
}
```

### Descripción de funciones principales

- **connectToWiFiAndServer(char* ssid, char* pass)**: Conecta el dispositivo a la red Wi-Fi y al servidor Web SCADA de CMA. Requiere el nombre de la red Wi-Fi y la contraseña.
- **setMessageSendInterval(int time)**: Establece el intervalo de tiempo en milisegundos para el envío de datos. El valor por defecto es 1000 ms (1 segundo).

- **sendMessageToGroup(String groupName, float data)**: Envía un mensaje al grupo especificado con el dato proporcionado. Si el dato no es válido (NaN), se generará un valor aleatorio.

- **joinGroup(String groupName)**: Permite unirse a un grupo para recibir y enviar datos.

- **onPushButtonOn(String groupName)**: Permite registrar un callback para que se ejecute cuando se mantiene presionado el botón. _Usarse en el loop()._

- **onPushButtonOff(String groupName)**: Permite registrar un callback para que se ejecute cuando el boton no esta presionado. _Usarse en el loop()._

- **onPushButtonOnEvent(String groupName, const std::function<void()>& callback)**: Permite registrar un callback para cuando ocurre un evento de presión del botón. _Usarse en el setup()._

- **onPushButtonOffEvent(String groupName, const std::function<void()>& callback)**: Permite registrar un callback para cuando ocurre un evento de soltar el botón. _Usarse en el setup()._

## Variables globales

- **CMA_SOCKET**: Instancia de la clase **CMA** utilizada para gestionar la conexión y el envío de datos.
- **isConnectSocket**: Variable booleana que indica si la conexión con el servidor Socket.IO está activa.

## Requisitos

- **Arduino IDE** con soporte para **ESP32** o **ESP8266**.
- **Librerías**:
  - [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
  - [SocketIOclient](https://github.com/marvinroger/esp8266-websocket-client)
  - [WiFiMulti](https://github.com/esp8266/Arduino/tree/master/libraries/WiFiMulti)

## Contribuciones

Si deseas contribuir a la librería, por favor realiza un _fork_ del repositorio y envía tus cambios mediante _pull requests_. Asegúrate de seguir las mejores prácticas de codificación y de probar bien cualquier nueva funcionalidad.

## Licencia

Este proyecto está bajo la **Licencia MIT**. Puedes ver los detalles de la licencia en el archivo `LICENSE`.
