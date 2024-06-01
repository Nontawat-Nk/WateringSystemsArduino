#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// กำหนดพินสำหรับ LED และเซ็นเซอร์
const int ledPin = 2;
const int ledPin3 = 3;
const int analogPin = A0;  // พินอนาล็อกสำหรับ LDR

// เริ่มต้นเซ็นเซอร์ DHT
DHT dht;
LiquidCrystal_I2C lcd(0x27, 16, 2); // เปลี่ยน 0x27 เป็นที่อยู่ I2C ของ LCD ถ้าแตกต่าง

// กำหนดพินสำหรับการเชื่อมต่อกับไดรเวอร์ TP6600
// ซ้าย-ขวา
#define Y_STEP_PIN 5
#define Y_DIR_PIN 7
#define Y_EN_PIN 6
// ด้านหน้า
#define X_STEP_PIN 9
#define X_DIR_PIN 10
#define X_EN_PIN 8

// กำหนดค่าคงที่
const float STEPS_PER_REVOLUTION = 1600; // ตัวอย่าง: 200 สเต็ปสำหรับการหมุนครบหนึ่งรอบ (สำหรับมอเตอร์ 1.8°)
const float ANGLE_PER_STEP = 360.0 / STEPS_PER_REVOLUTION;
int startState = 1; // กำหนดสถานะเริ่มต้น

void setup() {
  // ตั้งพิน LED เป็นเอาต์พุต
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  // ตั้งพินมอเตอร์เป็นเอาต์พุต
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_EN_PIN, OUTPUT);
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_EN_PIN, OUTPUT);
  pinMode(13, OUTPUT);

  // กำหนดทิศทางและเปิดใช้งาน
  digitalWrite(Y_DIR_PIN, HIGH); // กำหนดทิศทาง
  digitalWrite(Y_EN_PIN, LOW);   // เปิดใช้งานไดรเวอร์
  digitalWrite(X_DIR_PIN, HIGH); // กำหนดทิศทาง
  digitalWrite(X_EN_PIN, LOW);   // เปิดใช้งานไดรเวอร์

  // เริ่มต้นการสื่อสารแบบอนุกรม
  Serial.begin(9600);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)");

  // เริ่มต้นเซ็นเซอร์ DHT
  dht.setup(3); // กำหนดเซ็นเซอร์ DHT ต่อกับขา 3

  // เริ่มต้น LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // อ่านค่าจากเซ็นเซอร์อนาล็อก (LDR)
  int val = analogRead(analogPin);  // อ่านค่าจาก LDR
  Serial.print("LDR Value = "); // พิมพ์ข้อความ
  Serial.println(val); // พิมพ์ค่าของ LDR

  // ควบคุม LED ตามค่าของ LDR
  if (val < 500) {
    digitalWrite(ledPin, LOW); // ปิด LED ที่ขา 2
    digitalWrite(ledPin3, HIGH); // เปิด LED ที่ขา 3
  } else {
    digitalWrite(ledPin, HIGH); // เปิด LED ที่ขา 2
    digitalWrite(ledPin3, LOW); // ปิด LED ที่ขา 3
  }

  // อ่านค่าจากเซ็นเซอร์ DHT11
  float humidity = dht.getHumidity(); // อ่านค่าความชื้น
  float temperatureC = dht.getTemperature(); // อ่านค่าอุณหภูมิในหน่วยเซลเซียส
  float temperatureF = dht.toFahrenheit(temperatureC); // แปลงค่าอุณหภูมิเป็นหน่วยฟาเรนไฮต์

  // ตรวจสอบว่าการอ่านค่าล้มเหลวหรือไม่ ถ้าใช่ให้จบการทำงานตรงนี้
  if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.setCursor(0, 0);
    lcd.print("DHT Read Error");
    return;
  }

  // พิมพ์ผลลัพธ์จากเซ็นเซอร์ DHT11 ไปยังหน้าจออนุกรม
  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperatureC, 1);
  Serial.print("\t\t");
  Serial.println(temperatureF, 1); // พิมพ์อุณหภูมิในหน่วยฟาเรนไฮต์

  // แสดงผลลัพธ์บน LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hum: ");
  lcd.print(humidity, 1);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperatureC, 1);
  lcd.print("C ");
  lcd.print(temperatureF, 1);
  lcd.print("F");

  // ควบคุมมอเตอร์ตามสถานะเริ่มต้น
  if (startState == 1) {
    for (int j = 0; j < 1; j++) {
      Serial.print(j);
      Serial.println(" Xaxis 1000");
      digitalWrite(13, HIGH);
      delay(3000); // รอ 3 วินาที
      digitalWrite(13, LOW);
      for (int i = 0; i < 3; i++) {
        RotageYaxis(1700);
        digitalWrite(13, HIGH);
        delay(3000); // รอ 3 วินาที
        digitalWrite(13, LOW);
      }
      RotageXaxis(1700);
      digitalWrite(13, HIGH);
      delay(3000); // รอ 3 วินาที
      digitalWrite(13, LOW);
      for (int i = 0; i < 3; i++) {
        RotageYaxis(-1700);
        digitalWrite(13, HIGH);
        delay(3000); // รอ 3 วินาที
        digitalWrite(13, LOW);
      }
      RotageXaxis(1700);
      digitalWrite(13, HIGH);
      delay(3000); // รอ 3 วินาที
      digitalWrite(13, LOW);
      for (int i = 0; i < 3; i++) {
        RotageYaxis(1700);
        digitalWrite(13, HIGH);
        delay(3000); // รอ 3 วินาที
        digitalWrite(13, LOW);
      }
      RotageXaxis(-1700 * 2);
      delay(100);
      RotageYaxis(-1700 * 3);
      delay(100);
      startState = 0;
    }
  }

  // รอหนึ่งวินาทีก่อนทำซ้ำ
  delay(1000);
}

// ฟังก์ชันหมุนแกน X
void RotageXaxis(float targetAngle) {
  int steps = targetAngle / ANGLE_PER_STEP;
  if (targetAngle < 0) {
    digitalWrite(X_DIR_PIN, LOW); // หมุนตามเข็มนาฬิกา
  } else {
    digitalWrite(X_DIR_PIN, HIGH); // หมุนทวนเข็มนาฬิกา
  }
  Serial.println(targetAngle);
  Serial.println(" Deg");
  // หมุนมอเตอร์ไปยังตำแหน่งเป้าหมาย
  for (int i = 0; i < abs(steps); i++) {
    digitalWrite(X_STEP_PIN, HIGH);
    delayMicroseconds(80); // ปรับค่าหน่วงเวลาเพื่อความเร็วที่ต้องการ
    digitalWrite(X_STEP_PIN, LOW);
    delayMicroseconds(80); // ปรับค่าหน่วงเวลาเพื่อความเร็วที่ต้องการ
  }
}

// ฟังก์ชันหมุนแกน Y
void RotageYaxis(float targetAngle) {
  int steps = targetAngle / ANGLE_PER_STEP;
  if (targetAngle < 0) {
    digitalWrite(Y_DIR_PIN, LOW); // หมุนตามเข็มนาฬิกา
  } else {
    digitalWrite(Y_DIR_PIN, HIGH); // หมุนทวนเข็มนาฬิกา
  }
  Serial.println(targetAngle);
  Serial.println(" Deg");
  // หมุนมอเตอร์ไปยังตำแหน่งเป้าหมาย
  for (int i = 0; i < abs(steps); i++) {
    digitalWrite(Y_STEP_PIN, HIGH);
    delayMicroseconds(50); // ปรับค่าหน่วงเวลาเพื่อความเร็วที่ต้องการ
    digitalWrite(Y_STEP_PIN, LOW);
    delayMicroseconds(50); // ปรับค่าหน่วงเวลาเพื่อความเร็วที่ต้องการ
  }
}
