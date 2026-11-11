// Feito por Arthur Miguel Schlichting

#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"

// -------------------------------
// CONFIGURACOES WI-FI
// -------------------------------

const char* ssid = "Wokwi-GUEST";
const char* password = "";

// -------------------------------
// CONFIGURACOES MQTT
// -------------------------------

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

const char* mqtt_client_id = "ESP32_DHT22_WOKWI";

// Topicos MQTT
const char* topico_temp = "temp_wokwi_dht_pv";
const char* topico_hum = "hum_wokwi_dht_pv";

// -------------------------------
// CONFIGURACAO DO SENSOR
// -------------------------------

const int dht_pin = 4;

DHTesp dhtsensor;

// -------------------------------
// OBJETOS DE COMUNICACAO
// -------------------------------

WiFiClient espClient;
PubSubClient client(espClient);

// -------------------------------
// CONEXAO WI-FI
// -------------------------------

void connect_wifi() {

  Serial.println();
  Serial.print("Conectando ao Wi-Fi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi conectado!");

  Serial.print("Endereco IP: ");
  Serial.println(WiFi.localIP());
}

// -------------------------------
// CONEXAO MQTT
// -------------------------------

void reconnect_MQTT() {

  while (!client.connected()) {

    Serial.println("Conectando ao broker MQTT...");

    if (client.connect(mqtt_client_id)) {

      Serial.println("Cliente MQTT conectado!");

    } else {

      Serial.print("Falha na conexao. Estado MQTT: ");
      Serial.println(client.state());

      delay(5000);
    }
  }
}

// -------------------------------
// LEITURA E PUBLICACAO DO DHT22
// -------------------------------

void captura_dht() {

  TempAndHumidity data = dhtsensor.getTempAndHumidity();

  if (isnan(data.temperature) || isnan(data.humidity)) {

    Serial.println("Erro na leitura do DHT22.");
    delay(2000);
    return;
  }

  Serial.println("Temperatura: " + String(data.temperature, 2) + " °C");

  Serial.println("Umidade: " + String(data.humidity, 1) + " %");

  client.publish(
    topico_temp,
    String(data.temperature, 2).c_str()
  );

  client.publish(
    topico_hum,
    String(data.humidity, 1).c_str()
  );

  Serial.println("Dados publicados no MQTT.");
  Serial.println("-------------------------");

  delay(2000);
}

// -------------------------------
// INICIALIZACAO
// -------------------------------

void setup() {

  Serial.begin(115200);

  // Inicializa o sensor DHT22
  dhtsensor.setup(dht_pin, DHTesp::DHT22);

  // Conecta ao Wi-Fi
  connect_wifi();

  // Configura o servidor MQTT
  client.setServer(mqtt_server, mqtt_port);
}

// -------------------------------
// EXECUCAO PRINCIPAL
// -------------------------------

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    connect_wifi();
  }

  if (!client.connected()) {
    reconnect_MQTT();
  }

  client.loop();

  captura_dht();
}