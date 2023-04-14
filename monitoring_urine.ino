#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

// deklarasi variabel dengan nama "rtc" 
  RTC_DS1307 rtc; 
// inisialisasi pin LCD I2C
  LiquidCrystal_I2C lcd (0x27, 16, 4);

// inisialisasi pin
  int relayPin = 2; // pin relay 
  int proximityPin = 3; // [in proximity

// inisialisasi waktu
  unsigned long previousMillis = 0; // Waktu sebelumnya sanyo diaktifkan
  unsigned long interval = 190000; // waktu motor aktif selama 4 menit

// jumlah volume urine
  const float volumeMax = 40; // Volume maksimum tampungan urine dalam liter

void setup() {
// set Output dan Input
  pinMode (proximityPin, INPUT);
  pinMode (relayPin, OUTPUT);
// set kondisi awal relay
  digitalWrite (relayPin, LOW);
// inisialisasi LCD
  Wire.begin();
  lcd.begin();
  lcd.backlight();
// set waktu pada RTC
  if(!rtc.isrunning()){
    rtc.adjust(DateTime(2023, 5, 7, 8, 30, 0)); //waktu diubah sesuai dengan pelaksanaan besok
  } 
}

void loop() {
// baca rtc
  DateTime now = rtc.now();
  int hour = now.hour();
  int minute = now.minute();

// baca data proximity
   int proximityValue = digitalRead(proximityPin);
// Konversi nilai sensor proximity ke dalam liter
   proximityValue = proximityValue * 0.0049;

// hitung jumlah urine dalam tampungan
  int urineLevel = map(proximityValue, 0, 1023, 0, 40);

// tampilan dalam lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(now.toString("dd/MM/yyyy"));
  lcd.setCursor(0, 1);
  lcd.print(urineLevel);
  lcd.print("L");

// Jika volume sudah mencapai batas maksimum dan waktu sudah 4 menit sejak relay aktif
  if (proximityValue>= volumeMax && millis() - previousMillis >= interval) { 
    digitalWrite(relayPin, HIGH); // Aktifkan relay
    previousMillis = millis(); // Simpan waktu sebelumnya
  }

  // Jika relay sudah aktif selama 4 menit
  if (digitalRead(relayPin) == HIGH && millis() - previousMillis >= interval) {
    digitalWrite(relayPin, LOW); // Matikan relay
  }
  delay(1000); // tunggu selama 1 detik
}
