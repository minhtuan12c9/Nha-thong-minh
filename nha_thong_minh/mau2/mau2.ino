#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <Servo.h>    // Thư viện Servo
Servo servo;          // Khai báo đối tượng servo

#define BLYNK_TEMPLATE_ID "TMPL6JtueX2yD"
#define BLYNK_TEMPLATE_NAME "Project"
#define BLYNK_AUTH_TOKEN "nalD87Ul5wRw8WMQCY1UI6CurMdB9-TW"

// Thông tin kết nối WiFi
char auth[] = "nalD87Ul5wRw8WMQCY1UI6CurMdB9-TW";  // Thay YOUR_AUTH_TOKEN bằng token Blynk của bạn
char ssid[] = "STUDENT";   // Thay YOUR_WIFI_SSID bằng tên WiFi của bạn
char pass[] = "";   // Thay YOUR_WIFI_PASS bằng mật khẩu WiFi của bạn

// Chân kết nối
#define swPin D1      // Chân kết nối switch
#define buttonPin D5
#define ledPin1 D8     // Chân kết nối đèn LED
#define ledPin2 D7     // Chân kết nối đèn LED
#define servopin 2

// Biến trạng thái
bool ledState = LOW;
bool ledState1 = false;
bool servoState = LOW;

void setup() {
  Serial.begin(9600);
  // Thiết lập chân đầu vào và đầu ra
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(swPin, INPUT_PULLUP);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  servo.attach(servopin);
  // Kết nối WiFi
  Blynk.begin(auth, ssid, pass);
  // Gán giá trị ban đầu cho đèn LED
  digitalWrite(ledPin1, ledState);
  digitalWrite(ledPin2, ledState);
}
BLYNK_WRITE(V2){
  int relayVal = param.asInt();
  if(relayVal == 1){
    digitalWrite(ledPin1, HIGH);                 // bật relay
  }else {
    digitalWrite(ledPin1, LOW);
  }
  delay(1000);
}
BLYNK_WRITE(V3){
  int relayVal = param.asInt();
  if(relayVal == 1){
    servo.write(180);
  }else {
    servo.write(0);
  }
 // delay(100);
}
BLYNK_WRITE(V4){
  int relayVal = param.asInt();
  if(relayVal == 1){
    digitalWrite(ledPin2, HIGH);                 // bật relay
  }else {
    digitalWrite(ledPin2, LOW);
  }
  delay(1000);
}

void loop() {
  Blynk.run();
  // Đọc trạng thái của nút bấm
  bool swState = digitalRead(swPin);
  bool buttonState = digitalRead(buttonPin);

  
  // Nếu nút bấm được nhấn (trạng thái LOW)
  if (buttonState == LOW) {
    // Đảo trạng thái đèn LED
    ledState = !ledState;
    // Gửi giá trị mới của đèn LED lên Blynk App
    Blynk.virtualWrite(V4, ledState);
    // Cập nhật trạng thái đèn LED
    digitalWrite(ledPin2, ledState); 
  }


//  if (swState == HIGH) {
//      // Đảo trạng thái của đèn LED
//      ledState1 = !ledState1;
//      // Gửi giá trị mới của đèn LED lên Blynk App
//      Blynk.virtualWrite(V2, ledState1);
//      // Bật hoặc tắt đèn LED dựa trên trạng thái mới
//      digitalWrite(ledPin1, ledState1 ? HIGH : LOW);
//  
//      // Delay để tránh việc đọc lại switch liên tục trong một khoảng thời gian ngắn
//      delay(1000);
//    }

    if (swState == LOW){
      digitalWrite(ledPin1, HIGH);
      Blynk.virtualWrite(V2, HIGH);
    }else{
      digitalWrite(ledPin1, LOW);
      Blynk.virtualWrite(V2, LOW);
    }
    delay(1000);  // Đợi 200ms để tránh nhận nhiều lần nhấn từ cùng một lần nhấn
}
