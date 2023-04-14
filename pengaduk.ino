#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <SharpDistSensor.h>

RTC_DS3231 rtc;

const int buttonPin1 = 2; // Pin untuk push button 1
const int buttonPin2 = 3; // Pin untuk push button 2
const int flowSensorPin = 4; // Pin untuk flow sensor
const int valvePin = 5; // Pin untuk valve
const int relayPin = 6; // Pin untuk relay
const int motorPin = 7; // Pin untuk motor
const int proximitySensorPin = A0; // Pin untuk proximity sensor

volatile int waterVolume = 0; // Jumlah air yang mengalir
int waterCapacity = 1000; // Kapasitas air yang diinginkan
bool valveOpen = false; // Status valve
bool motorOn = false; // Status motor
bool button1PressedStatus = false; // Status push button 1
bool button2Pressed = false; // Status push button 2

SharpDistSensor sensor(proximitySensorPin, 25); // Inisialisasi proximity sensor
LiquidCrystal_I2C lcd(0x27, 16, 2); // Inisialisasi LCD

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(flowSensorPin, INPUT);
  pinMode(valvePin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  digitalWrite(valvePin, LOW);
  digitalWrite(relayPin, LOW);
  digitalWrite(motorPin, LOW);
  attachInterrupt(digitalPinToInterrupt(flowSensorPin), flowSensorInterrupt, RISING);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  lcd.init(); // Inisialisasi LCD
  lcd.backlight(); // Hidupkan backlight LCD
}

void loop() {
  int urineCapacity = sensor.getDist(); // Baca jarak dari proximity sensor
  lcd.setCursor(0, 0); // Set cursor pada baris pertama, kolom pertama
  lcd.print("Urine Capacity:"); // Tampilkan teks
  lcd.setCursor(0, 1); // Set cursor pada baris kedua, kolom pertama
  lcd.print(urineCapacity); // Tampilkan hasil pengukuran
  lcd.print(" cm");
  if (button1PressedStatus) {
    valveOpen = true;
    digitalWrite(valvePin, HIGH);
    Serial.println("Valve opened");
    while (waterVolume < waterCapacity) {
      // Tunggu hingga kapasitas air terpenuhi
    }
    digitalWrite(valvePin, LOW);
    
    Serial.println("Valve closed");
valveOpen = false;
waterVolume = 0;
}
if (button2Pressed) {
motorOn = true;
digitalWrite(motorPin, HIGH);
digitalWrite(relayPin, HIGH);
Serial.println("Motor started");
while (urineCapacity < waterCapacity/2) {
// Tunggu hingga urine capacity mencapai setengah kapasitas air yang diinginkan
}
digitalWrite(motorPin, LOW);
digitalWrite(relayPin, LOW);
Serial.println("Motor stopped");
motorOn = false;
button2Pressed = false;
}
if (urineCapacity >= waterCapacity) {
if (!valveOpen && !motorOn) {
digitalWrite(motorPin, HIGH);
digitalWrite(relayPin, HIGH);
delay(2000);
digitalWrite(relayPin, LOW);
delay(2000);
digitalWrite(motorPin, LOW);
waterVolume = 0;
}
}
button1PressedStatus = false;
}

void flowSensorInterrupt() {
waterVolume++;
}

void button1Pressedfunc() {
button1PressedStatus = true;
}

void button2Pressedfunc() {
button2Pressed = true;
}
