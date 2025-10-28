#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//informações da rede WIFI
const char* ssid = "CSI-Lab";         //SSID da rede WIFI
const char* password = "In@teLCS&I";  //senha da rede wifi

//informações do broker MQTT
const char* mqttServer = "192.168.40.7";  //servidor
const char* mqttUser = "csilab";           //usuário
const char* ID = "ecoteste12";
const char* mqttPassword = "WhoAmI#2024";          //senha
const int mqttPort = 1883;                         //porta
const char* mqttTopicPub = "Teste";  // Tópico onde o ESP vai publicar mensagens

WiFiClientSecure espClient;  
PubSubClient client(espClient);

void connect() { 
  while (!client.connected()) { 
    Serial.println("Conectando ao Broker MQTT...");

    if (client.connect(ID, mqttUser, mqttPassword)) {
      Serial.println("Conectado ao MQTT!");
    } else {
      Serial.printf("Falha na conexão MQTT. Código de erro: %d\n", client.state());
      Serial.println("Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado!");

  // Configuração do cliente seguro
  espClient.setInsecure();  // REMOVE CHECAGEM DO CERTIFICADO
  espClient.setTimeout(30);  

  client.setServer(mqttServer, mqttPort);

  connect();
}

void loop() {

  if (!client.connected()) {
    connect();
  }
  client.loop();
}
