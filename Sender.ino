#define btn 19

#include <esp_now.h>
#include <WiFi.h>

// SUBSTITUA PELO ENDEREÇO MAC DO SEU RECEPTOR
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Estrutura para envio dos dados (deve ser idêntica no receptor)
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

// Callback de envio atualizado para ESP32 Core 3.x
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  pinMode(btn, INPUT_PULLUP);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Registrar callback de envio
  esp_now_register_send_cb(OnDataSent);
  
  // Registrar peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
    
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  int btn_state = digitalRead(btn);

  Serial.print("o botão está em: ");
  Serial.println(btn_state);

  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = btn_state;
  myData.c = 1.2;
  myData.d = false;
  
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(50);
}