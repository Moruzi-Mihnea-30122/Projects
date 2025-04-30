#include <ESP8266WiFi.h>
#include <espnow.h>

int ena = 14;
int in1a = 12;
int in2a = 13;
int enb = 4;
int in1b = 5;
int in2b = 16;
int myData[5];

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println("Data recieved");
  Serial.println(myData[1]);
  Serial.println(myData[2]);
  Serial.println(myData[3]);
  Serial.println(myData[4]);

  analogWrite(ena, myData[1]);
  analogWrite(enb, myData[2]);
  if(myData[3] == 0) {
    digitalWrite(in1a, LOW);
    digitalWrite(in2a, HIGH);
  }else{
    digitalWrite(in1a, HIGH);
    digitalWrite(in2a, LOW);
  }

  if(myData[4] == 0) {
    digitalWrite(in1b, LOW);
    digitalWrite(in2b, HIGH);
  }else{
    digitalWrite(in1b, HIGH);
    digitalWrite(in2b, LOW);
  }
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  pinMode(ena, OUTPUT);
  pinMode(in1a, OUTPUT);
  pinMode(in2a, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(in1b, OUTPUT);
  pinMode(in2b, OUTPUT);
}

void loop() {
  
}