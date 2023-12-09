#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_TEMPLATE_ID "TMPL6JtueX2yD"
#define BLYNK_TEMPLATE_NAME "Project"
#define BLYNK_AUTH_TOKEN "nalD87Ul5wRw8WMQCY1UI6CurMdB9-TW"

// Thông tin kết nối WiFi
char auth[] = "nalD87Ul5wRw8WMQCY1UI6CurMdB9-TW";  // Thay YOUR_AUTH_TOKEN bằng token Blynk của bạn
char ssid[] = "long";   // Thay YOUR_WIFI_SSID bằng tên WiFi của bạn
char pass[] = "11111111";   // Thay YOUR_WIFI_PASS bằng mật khẩu WiFi của bạn

// Chân kết nối
#define buttonPin D1  // Chân kết nối nút bấm
#define ledPin D2     // Chân kết nối đèn LED

// Biến trạng thái
bool ledState = LOW;

void setup() {
  Serial.begin(9600);
  // Thiết lập chân đầu vào và đầu ra
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  // Kết nối WiFi
  Blynk.begin(auth, ssid, pass);
  // Gán giá trị ban đầu cho đèn LED
  digitalWrite(ledPin, ledState);
}
BLYNK_WRITE(V2){
  int relayVal = param.asInt();
  if(relayVal == 1){
    digitalWrite(ledPin, HIGH);                 // bật relay
  }else {
    digitalWrite(ledPin, LOW);    
  }
  delay(1000);
}

void loop() {
  Blynk.run();

  // Đọc trạng thái của nút bấm
  bool buttonState = digitalRead(buttonPin);

  // Nếu nút bấm được nhấn (trạng thái LOW)
  if (buttonState == LOW) {
    // Đảo trạng thái đèn LED
    ledState = !ledState;

    // Gửi giá trị mới của đèn LED lên Blynk App
    Blynk.virtualWrite(V2, ledState);

    // Cập nhật trạng thái đèn LED
    digitalWrite(ledPin, ledState);
    delay(500);  // Đợi 200ms để tránh nhận nhiều lần nhấn từ cùng một lần nhấn
  }
}
