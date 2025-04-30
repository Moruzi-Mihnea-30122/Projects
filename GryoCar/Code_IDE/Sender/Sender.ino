#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = { 0xC4, 0xD8, 0xD5, 0x03, 0x56, 0xA5 };
Adafruit_MPU6050 mpu;

// Callback function called when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == 0 ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  delay(5000);
  // Init Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initilize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else {
    Serial.println("Initialized ESP-NOW");
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // Add peer
  if (esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 0, NULL, 0) != 0) {
    Serial.println("Failed to add peer");
    return;
  } else {
    Serial.println("Added peer");
  }

  if (esp_now_is_peer_exist(broadcastAddress)) {
    Serial.println("Peer exists");
  } else
    Serial.println("No exists");
  
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

void loop() {
  //y<0 front
  //y>0 back
  //x<0 right
  //x>0 left
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  int myData[5];
  double accx, accy;
  accx = a.acceleration.x;
  accy = a.acceleration.y;
  int in1b, in1a;
  int pwmx, pwmy;

  if(accy<0) {
    in1a = 1;
    in1b = 1;
  }
  else {
    in1a = 0;
    in1b = 0;
  }
  pwmx = 255/10 * accy;
  pwmy = 255/10 * accy;

  if(accx<-1){
    in1a = 1-in1a;
    pwmx = 255/10 * accx;
  }else if(accx>1) {
    in1b = 1-in1b;
    pwmy = 255/10* accx;
  }
  if(pwmx < 0) pwmx = -pwmx;
  if(pwmy < 0) pwmy = -pwmy;

  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  
  myData[1] = pwmx;
  myData[2] = pwmy;
  myData[3] = in1a;
  myData[4] = in1b;

  Serial.println("");
  Serial.print("Pwm x: ");
  Serial.print(pwmx);
  Serial.print("Pwm y: ");
  Serial.print(pwmy);

  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  delay(150);
}
