//Node MCU conectado con sensor fotoeléctrico
//para detección en parqueadero. Con conexión a Gateway NodeRed
// - WMA - Sept. 20 de 2019


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>
#define tiempo_estabilizacion_vehiculo 100
#define SS_PIN 15
#define RST_PIN 2

const char* ssid = "MANTILLA";
const char* password = "Jrvc16128924";

MFRC522 rfid(SS_PIN, RST_PIN);
 
MFRC522::MIFARE_Key key; 
// Init array that will store new NUID 
byte nuidPICC[3];
String str_rfid="";

//const char* ssid = "RedProfinet";
//const char* password = "12345678";

// Add your MQTT Broker IP address RaspBerry Pi, example:
const char* mqtt_server = "192.168.0.10";

WiFiClient sensorRFID;
PubSubClient client(sensorRFID);

long lastMsg = 0;
char msg[50];
int value = 0;
int activar_led=0;

// LED Pin
int sensor_input = D1;
int bandera=0;
int cont_segundos=0;
char sensorString[10];
char strOn[] = "Bahia = 5 - Estado = Ocupada";
char strOff[] = "Bahia = 5 - Estado = Libre";
String estado_bahia = "";
char lector[8];
   

void setup() {
 
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);
   SPI.begin();          // Init SPI bus
  rfid.PCD_Init();      // Init MFRC522 
   for (byte i = 0; i < 6; i++) 
    {  key.keyByte[i] = 0xFF;
    }
   printHex(key.keyByte, MFRC522::MF_KEY_SIZE);

}

void loop() {
   
  if (!client.connected()) {
    reconnect();
  }
    leerTarjetaRFID();
    client.loop(); 
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("sensorRFID")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      //client.subscribe("nodeMCU/sensorfotoelectrico1");
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

//Esta funcion obtiene el valor de la tarjeta RFID
void leerTarjetaRFID() {
  // Buscamos tarjetas
  str_rfid = "";
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
 
  // SI la encuentra la leemos
  if ( ! rfid.PICC_ReadCardSerial())
    return;
 
  //Serial.print(F("Tipo de tarjeta PICC: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  //Serial.println(rfid.PICC_GetTypeName(piccType));
 
  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) 
      {
        Serial.println(F("La etiqueta no es Valida"));
        return;
      }
 
  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) 
    {
        //Serial.println(F("Una nueva tarjeta ha sido detectada."));
     
        Serial.println(F("El NUID de la etiqueta es:"));
        Serial.print(F("En hex: "));
        printHex(rfid.uid.uidByte, rfid.uid.size);
        str_rfid.toCharArray(lector,10);
        client.publish("nodeMCU/sensorRFID",lector);
        Serial.println();
      
       rfid.PICC_HaltA();
       rfid.PCD_StopCrypto1();
    }
    delay(600);
}

void printHex(byte *buffer, byte bufferSize) {

unsigned int longitud=0;
byte nibble_alto=0;  

  longitud = bufferSize;
  //rfid_valor.toCharArray(buffer, longitud);
  for (byte i = 0; i < bufferSize; i++) {
    //Serial.println("Este es el valor del ID.....");
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);

    String rfid_valor = String(buffer[i], HEX);
    
    nibble_alto = 0xF0 & buffer[i];
    
    if (nibble_alto == 0) {
      str_rfid = str_rfid + "0";
    }
    str_rfid = str_rfid + rfid_valor;  
    //Serial.println(rfid_valor);
  }
  /*Serial.println();
  Serial.print("Esta es la que sirve....");
  Serial.println(str_rfid);
  Serial.println();*/
  //Serial.println(rfid_valor);
}
