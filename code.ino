#include<SoftwareSerial.h> 
#include<Servo.h>
#define ena 3  // chân ena nối với chân số 3 của arduino
#define in1 2  // chân in1 nối với chân số 2 của arduino
#define in2 4  // chân in2 nối với chân số 4 của arduino
#define enb 5  // chân enb nối với chân số 5 của arduino
#define in3 6  // chân in3 nối với chân số 6 của arduino
#define in4 7  // chân in4 nối với chân số 7 của arduino
#define trig 8 //chân trig của cảm biến nối chân số 8 của arduino
#define echo 9 //chân echo của cảm biến nối chân số 9 của arduino
#define servoPin 10 //Chân servo nối chân số 10 của arduino
#define sensor 0
SoftwareSerial hc05(12, 13); //TX,RX của hc05
Servo myServo; //Đối tượng Servo
char value = "";  //Biến lưu giá trị trả về của control pass
unsigned long timePeriod; //Biến lưu giờ gian của sóng cảm biến
int limitedDistance = 25;

void autoMode();
int measureDistance();
void forward();
void backward();
void left();
void right();
void stopp();
void resetServo();
void rotateRight();
void rotateLeft();
int checkSensor();
void setup() {
  Serial.begin(19200);
  hc05.begin(9600);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  myServo.attach(servoPin);
  pinMode(sensor, INPUT);
  myServo.write(100); 
  delay(500);
}
void loop() {
  if (hc05.available()){ 
    value = hc05.read();
    Serial.println(value);    
    switch (value){
      case 'F':
        forward(250);
        Serial.println("ĐI THẲNG");
        break;
      case 'B':
        backward(250);
        Serial.println("LÙI");
        break;
      case 'L':
        left();
        Serial.println("RẼ TRÁI");
        break;
      case 'R':
        right();
        Serial.println("RẼ PHẢI");
        break; 
      case 'A':
        autoMode();
        Serial.println("Tự động");
        break;             
      case 'S':
        stopp(); 
        Serial.println("DỪNG");       
    }   
  }
  delay(100);
}
void autoMode(){
  while(true){
    if(hc05.available()){
      value = hc05.read();
      Serial.println(value); 
      if(value == 'C'){
        stopp(); 
        Serial.println("DỪNG");
        return;
      }
    }
    int distance = measureDistance();
    int sensorValue = checkSensor();
    Serial.print("Khoang cach: ");
    Serial.println(distance);
    Serial.print("Cam bien: ");
    Serial.println(sensorValue);
    if(distance > limitedDistance && sensorValue == 1){
      forward(150);  
      Serial.println("Di thang");
    }else{
      stopp(); delay(100);
      rotateLeft();
      int leftDistance = measureDistance(); delay(500);
      Serial.print("Khoang cach trai: ");
      Serial.println(leftDistance);
      resetServo();
      rotateRight();
      int rightDistance = measureDistance(); delay(500);
      Serial.print("Khoang cach phai: ");
      Serial.println(rightDistance);
      resetServo();
      if(rightDistance < limitedDistance && leftDistance < limitedDistance){
        backward(150); delay(250); stopp(); delay(300);
        left(); delay(800);
        Serial.println("Quay dau");
      }else{
        if(rightDistance >= leftDistance){
          backward(150); delay(300); stopp(); delay(300);
          Serial.println("Re phai");
          right(); delay(500); stopp(); delay(300);
        }else{
          backward(150); delay(300); stopp(); delay(300);
          Serial.println("Re trai");
          left(); delay(500); stopp(); delay(300);
        }
      }
    }
    delay(150);  
  }
}
/******************************Tien******************************/
void forward(int pwm){
  analogWrite(ena,pwm);
  analogWrite(enb,pwm);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
/******************************Lui******************************/
void backward(int pwm){
  analogWrite(ena,pwm);
  analogWrite(enb,pwm);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
/******************************Re trai******************************/
void left(){
  analogWrite(ena,250);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
/******************************Re phai******************************/
void right(){
  analogWrite(enb,250);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
/******************************Dung******************************/
void stopp(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
}
int checkSensor(){
  int sensorValue = digitalRead(sensor);
  return sensorValue;
}
int measureDistance(){
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  //Đo độ rộng xung HIGH ở chân echo
  timePeriod = pulseIn(echo,HIGH);
  return int(timePeriod / 2/ 29.412); 
}
void rotateLeft(){
  myServo.write(165);
  delay(1000);
}
void rotateRight(){
  myServo.write(40);
  delay(1000);
}
void resetServo(){
  myServo.write(100);
}
