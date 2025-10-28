#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define LED D2

const char* ssid = "CSI-Lab";
const char* password = "In@teLCS&I";  // senha da rede wifi

// Informações do broker MQTT
const char* mqttServer = "fe20ec57058e4343bd79fa6dca488cac.s1.eu.hivemq.cloud";   // servidor
const char* mqttUser = "INATEL";           // usuário
const char* mqttPassword = "Telecom2025";  // senha
const int mqttPort = 8883;                 // porta
const char* mqttTopicSub = "Teste/#";       // tópico que será assinado
const char* ID = "Lampada";            // Nome do dispositivo, deve ser único

WiFiClient espClient;
PubSubClient client(espClient);

// Função de recebimento e processamento de mensagens vindas do broker
void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';          // Finaliza a string recebida
  int MSG = atoi((char*)payload);  // Converte a mensagem para inteiro
  String topico = topic;

  Serial.print("Mensagem no tópico: ");
  Serial.println(topico);
  Serial.print("Mensagem: ");
  Serial.println(MSG);
  Serial.println("-----------------------");

  if (MSG == 1) {
    Serial.println("Led aceso!");
    digitalWrite(LED, HIGH);
  } else if (MSG == 0) {
    Serial.println("Led apagado!");
    digitalWrite(LED, LOW);
  }

  Serial.println("**********************************************************************");
}

void connect() { // Função de reconexão com o broker
  while (!client.connected()) { // Enquanto a conexão com o broker não for realizada
    Serial.println("Conectando ao Broker MQTT...");
    if (client.connect(ID, mqttUser, mqttPassword)) {
      Serial.println("Conectado");
      client.subscribe(mqttTopicSub); // Subscreve no tópico
    } else {
      Serial.print("Falha, estado: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); // Conecta na rede WI-FI
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso na rede ");
  Serial.println(ssid);

  // Configurando o servidor MQTT e a função de callback
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  Serial.println("MQTT conectando...");
  connect();
}

void loop() {
  if (!client.connected()) {
    connect();
  }
  client.loop();
}
