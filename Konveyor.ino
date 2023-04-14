#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

RTC_DS1307 rtc; //kontrol RTC

LiquidCrystal_I2C lcd(0x27, 16, 2); // inisialisasi pin lcd I2c yaitu Pin A4 dan A5
int relayPin = 3; // Pin Relay D3
unsigned long previousMillis = 0; // Waktu sebelumnya motor diaktifkan
unsigned long interval = 180000; // waktu motor aktif selama 3 menit


void setup() {
//  set relay pin sebagai output
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

//inisialisasi koneksi RTC
  Wire.begin();
  rtc.begin();

// set waktu pada RTC
  if(!rtc.isrunning()){
    rtc.adjust(DateTime(2023, 5, 7, 8, 30, 0)); //waktu diubah sesuai dengan pelaksanaan besok
  } 

// inisialisasi koneksi LCD
  lcd.begin();
  lcd.backlight();

// tampilkan pesan selamat datang di LCD
  lcd.print("Instalasi Pemisah Kotoran Dan Urine Domba!");
}

void loop() {
// Baca waktu dari RTC
  DateTime now = rtc.now();

// Tampilkan waktu RTC pada LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waktu: ");
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
  lcd.print(":");
  lcd.print(now.second(), DEC);

// Jika waktu sudah 8 jam sejak motor pertama kali diaktifkan
  if ((now.hour() % 8 == 0)&&(now.minute()== 0)&&(now.second()== 0)&&(millis() - previousMillis >= interval)){
    digitalWrite(relayPin, HIGH); // relay menyala
    previousMillis = millis(); // Simpan waktu sebelumnya
  }

// Jika waktu sudah 3 menit sejak motor aktif
  if (digitalRead(relayPin) == HIGH && millis() - previousMillis >= interval) {
    digitalWrite(relayPin, LOW); // Matikan relay untuk menghentikan motor
    previousMillis = millis(); // Simpan waktu sebelumnya
  }
}
