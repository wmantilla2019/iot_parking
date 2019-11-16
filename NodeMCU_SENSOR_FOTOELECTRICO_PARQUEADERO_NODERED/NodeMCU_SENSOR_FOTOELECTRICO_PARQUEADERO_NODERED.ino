//Node MCU conectado con sensor fotoeléctrico
//para detección en parqueadero. Con conexión a Gateway NodeRed
// - WMA - Sept. 20 de 2019


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
#define tiempo_estabilizacion_vehiculo 100

const char* ssid = "MANTILLA";
const char* password = "Jrvc16128924";

//const char* ssid = "RedProfinet";
//const char* password = "12345678";

// Add your MQTT Broker IP address RaspBerry Pi, example:
const char* mqtt_server = "192.168.0.10";

WiFiClient sensorFotoelectrico7;
PubSubClient client(sensorFotoelectrico7);
Ticker parpadearled; 

long lastMsg = 0;
char msg[50];
int value = 0;
int activar_led=0;
const String quote = "\"";

// LED Pin
int sensor_input = D1;
int bandera=0;
int estuve_parpadeando=0;
int cont_segundos=0;
char sensorString[8];
char strOn[] = "{\"bahia\": 7, \"estado\": \"ocupada\"}";
char strOff[] = "{\"bahia\": 7, \"estado\": \"libre\"}";
String estado_bahia = "";

void parpadeo() {
static int estadoled=0;

  //Este bloque if es para hacer parpadear el LED cada 0.1s
    if (estadoled==0) {
      digitalWrite(LED_BUILTIN, 1);  
      estadoled = 1;
    }else if (estadoled==1) {
      digitalWrite(LED_BUILTIN, 0);
      estadoled = 0;
    }
    cont_segundos++;
    //El valor de tiempo_estabilizacion_vehiculo es el tiempo deseado multiplicado por 10. Cambiar si es necesario.
    if (cont_segundos>=tiempo_estabilizacion_vehiculo) {
      //Transcurrieron n segundos
      cont_segundos = 0;
      Serial.println("Vehículo estabilizado y ocupando bahia.");
      Serial.println("Enviando información al Broker");
      client.publish("nodeMCU/sensorFotoelectrico7", strOn);
      parpadearled.detach();
      digitalWrite(LED_BUILTIN, 0);  //Deja encendido el LED. Nota: el LED tiene lógica inversa
      estuve_parpadeando = 0;
    }

    //Sí, mientras está parpadeando, el vehículo se sale de la bahía, debe culminar el parpadeo,
    //el LED debe quedar apagado, todo queda en condiciones iniciales y no debe mandar nada al Broker
    if (digitalRead(sensor_input)==1){
      Serial.println("Presencia finalizada");
      parpadearled.detach(); //Desactiva el parpadeo
      digitalWrite(LED_BUILTIN, 1); //Apaga el LED
      cont_segundos = 0;
      bandera = 0;
    }
}

void setup() {
  pinMode(sensor_input, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
  delay(300);
  Serial.begin(9600);
  //Serial.println("Detectando presencia...");
  delay(500);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //El sensor fotoeléctrico tiene lógica inversa
  if ((digitalRead(sensor_input)==0) && (bandera==0)) {
    Serial.println("Presencia detectada");
    Serial.println("Esperando estabilización del vehículo...");
    //Se repite cada 0.1s
    parpadearled.attach(0.1, parpadeo);
    //sprintf(sensorString, "%d", 1);
    bandera = 1;
    estuve_parpadeando = 1;
  }
  else if ((digitalRead(sensor_input)==1) && (bandera==1)){
    Serial.println("Presencia finalizada");
    // Publishes values
    sprintf(sensorString, "%d", 0);
    if (estuve_parpadeando!=1) {
      client.publish("nodeMCU/sensorFotoelectrico7", strOff);  
    }
    if (estuve_parpadeando==1) {
      estuve_parpadeando = 0;  
    }
    digitalWrite(LED_BUILTIN, 1);
    bandera = 0;
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("sensorFotoelectrico7")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      //client.subscribe("nodeMCU/sensorFotoelectrico7");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}
