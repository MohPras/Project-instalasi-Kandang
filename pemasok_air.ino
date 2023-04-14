#include <LiquidCrystal_I2C.h>

// inisialisasi pin LCD I2C
  LiquidCrystal_I2C lcd (0x27, 16, 4);

// inisialisasi variabel 
  int relayPin = 2; // relay
  int valvePin = 3; // valve
  int flowSensor = 2; // flow sensor
  int waterVolume = 0; // save jumlah air yang masuk

void setup() {
//  Set Input dan Output
  pinMode(flowSensor, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode (valvePin, OUTPUT);

//  Set LCD
  lcd.clear();
  lcd.begin();
  lcd.backlight();
}

void loop() {
// Baca Flow Sensor
  int flow (digitalRead,flowSensor);

// Jika air masuk
  if (flow == HIGH){
    digitalWrite(relayPin, HIGH); // relay aktif
    digitalWrite(valvePin, HIGH); // valve aktif
    waterVolume += 1; // lakukan penambahan 1 liter
  } else { // jika air tidak mengalir
    digitalWrite(relayPin, LOW);
    digitalWrite(valvePin, LOW);
  }

// Jika air sudah 84 liter
  if (waterVolume <= 84){
    digitalWrite(relayPin, LOW);
    digitalWrite(valvePin, LOW);
  }

// Tampilkan data ke LCD
  lcd.setCursor(0, 0); // Pindahkan kursor ke baris 1 kolom 1
  lcd.print("Jumlah air (L):"); // Tampilkan label "Jumlah air (L):" pada LCD I2C
  lcd.setCursor(0, 1); // Pindahkan kursor ke baris 2 kolom 1
  lcd.print(waterVolume); // Tampilkan jumlah air yang telah mengalir pada LCD I2C

  delay(1000); //jeda sebelum alat mati
}
