#include <ESP8266WiFi.h>                            // Thư viện ESP8266 WiFi
#include <BlynkSimpleEsp8266.h>                     // Thư viện Blynk cho ESP8266
#include <Adafruit_MQTT.h>                          // Thư viện Adafruit MQTT
#include <Adafruit_MQTT_Client.h>                   // Thư viện Adafruit MQTT Client
#include <Servo.h>                                  // Thư viện Servo

#define WIFI_SSID "your_wifi_ssid"                  // Tên mạng WiFi của bạn
#define WIFI_PASSWORD "your_wifi_password"          // Mật khẩu mạng WiFi của bạn
#define MQTT_SERVER "io.adafruit.com"               // Máy chủ MQTT của Adafruit
#define MQTT_PORT 1883                              // Cổng kết nối MQTT
#define MQTT_USERNAME "your_adafruit_username"      // Tên đăng nhập Adafruit IO
#define MQTT_PASSWORD "your_adafruit_key"           // Mật khẩu Adafruit IO

#define BLYNK_TEMPLATE_ID "TMPL6JtueX2yD"
#define BLYNK_TEMPLATE_NAME "Project"
#define BLYNK_AUTH_TOKEN "ZkQ-0WGhRTgyAptgtD-EXMv3sQ77yVmz"

#define SERVO_PIN D1                                // Chân điều khiển Servo
#define LED_PIN D2                                  // Chân điều khiển đèn LED

Servo servo;
int servoPosition = 0;
int ledState = LOW;

char mqttClientID[30];                              // Mảng này sẽ lưu trữ Client ID của kết nối MQTT
char mqttTopic[50];                                 // Mảng này sẽ lưu trữ chủ đề (topic) MQTT để gửi dữ liệu đến
char mqttSubscribeTopic[50];                        // Mảng này sẽ lưu trữ chủ đề (topic) MQTT để đăng ký nhận dữ liệu

WiFiClient client;                                  // khai báo để sử dụng và kết nối
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD);             // khai báo một đối tượng để kết nối và giao tiếp với máy chủ MQTT

BlynkTimer timer;                                   // khai báo một đối tượng để quản lý các hẹn giờ và lập lịch trong ứng dụng Blynk

BLYNK_WRITE(V1) {                                   // Callback khi giá trị trên Blynk Virtual Pin V1 thay đổi
  int ledValue = param.asInt();
  if (ledValue == 1) {
    digitalWrite(LED_PIN, HIGH);
    ledState = HIGH;
  } else {
    digitalWrite(LED_PIN, LOW);
    ledState = LOW;
  }
}

void connectToMQTT() {
  sprintf(mqttClientID, "%s-%s", MQTT_USERNAME, WiFi.macAddress().c_str());

  if (mqtt.connected()) {
    mqtt.disconnect();
  }

  Serial.print("Connecting to MQTT... ");

  while (!mqtt.connect(mqttClientID)) {       // Kết nối đến Adafruit IO MQTT
    Serial.print(".");
    delay(500);
  }

  Serial.println("Connected!");

  sprintf(mqttTopic, "%s/feeds/led-control", MQTT_USERNAME);
  sprintf(mqttSubscribeTopic, "%s/feeds/servo-control", MQTT_USERNAME);

  mqtt.subscribe(mqttSubscribeTopic);         // Đăng ký nhận thông điệp từ chủ đề MQTT
}

void reconnectBlynk() {
  if (!Blynk.connected()) {                   // Kiểm tra kết nối Blynk
    Serial.println("Reconnecting to Blynk...");
    Blynk.connect();                          // Kết nối lại Blynk
  }
}

void handleMqttMessage(char *topic, byte *payload, unsigned int length) {
  Serial.print("MQTT Message Arrived: [");
  Serial.print(topic);
  Serial.print("] ");
  
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.println(message);
  
  if (strcmp(topic, mqttSubscribeTopic) == 0) {
    int servoValue = message.toInt();
    moveServo(servoValue);                     // Di chuyển Servo dựa trên giá trị nhận được từ MQTT
  }
}

void moveServo(int value) {
  int newPosition = map(value, 0, 100, 0, 180);
  servo.write(newPosition);
  servoPosition = newPosition;
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) {         // Kết nối lại MQTT
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
  }

  Serial.println("MQTT Connected!");
  mqtt.subscribe(mqttSubscribeTopic);           // Đăng ký nhận thông điệp từ chủ đề MQTT
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  servo.attach(SERVO_PIN);

  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);          // Kết nối với Blynk

  connectToMQTT();                                                  // Kết nối đến Adafruit IO MQTT

  timer.setInterval(1000L, reconnectBlynk);                         // Đặt khoảng thời gian để kết nối lại Blynk
}

void loop() {
  Blynk.run();                               // Thực thi Blynk
  mqtt.loop();                               // Thực thi MQTT
  timer.run();                               // Thực thi Timer
}
