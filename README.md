# 🚦 Dynamic Lighting and Control System

This project is a dynamic lighting/signaling system developed using the ATmega328P (Arduino) microcontroller. The circuit design and simulation were carried out using Proteus.

## 🛠️ Technologies & Components
* **Software:** Arduino IDE (C/C++)
* **Simulation:** Proteus Design Suite
* **Hardware:** * ATmega328P Microcontroller
  * LEDs (Red, Yellow, Green, Bi-color)  
  * Resistors (220Ω, 10kΩ)  
  * Push Buttons

## 📸 Circuit Diagram

<img width="902" height="690" alt="circuit-diagram" src="https://github.com/user-attachments/assets/58e855f7-641e-40b0-b567-01ddc44c318e" />

## 💻 Source Code & Logic
The system features a non-blocking delay structure using the `millis()` function instead of standard `delay()`. This allows the system to continuously monitor traffic density (via push buttons) and dynamically alter the green light duration for the priority road without freezing the loop.

<details>
  <summary><b>🔍 Click here to view the annotated Arduino Code</b></summary>
  
  ```cpp
  // --- PIN CONFIGURATIONS ---
  const int kirmizi1 = 2, sari1 = 3, yesil1 = 4;   // Route 1 (Main Road) Vehicle Lights
  const int kirmizi2 = 5, sari2 = 6, yesil2 = 7;   // Route 2 (Side Road) Vehicle Lights

  const int yayaKirmizi1 = A0, yayaYesil1 = A1;    // Route 1 Pedestrian Lights
  const int yayaKirmizi2 = A2, yayaYesil2 = A3;    // Route 2 Pedestrian Lights

  const int buton1 = 8, buton2 = 9; // Buttons to simulate traffic density
  int sayac1 = 0, sayac2 = 0;       // Counters for road density
  unsigned long sonButonBasma = 0;  // Timer for button debouncing

  // --- TRAFFIC TIMINGS ---
  const int anaYolYesil = 60 * 1000;  // Default main road green duration (60s)
  const int taliYolYesil = 30 * 1000; // Default side road green duration (30s)
  const int sariSure = 3000;          // Yellow light duration (3s)
  const int maxYesilSure = 90 * 1000; // Maximum green duration under heavy traffic (90s)

  void setup() {
    pinMode(kirmizi1, OUTPUT); pinMode(sari1, OUTPUT); pinMode(yesil1, OUTPUT);
    pinMode(kirmizi2, OUTPUT); pinMode(sari2, OUTPUT); pinMode(yesil2, OUTPUT);
    pinMode(yayaKirmizi1, OUTPUT); pinMode(yayaYesil1, OUTPUT);
    pinMode(yayaKirmizi2, OUTPUT); pinMode(yayaYesil2, OUTPUT);
    
    // Using internal pull-up resistors for buttons
    pinMode(buton1, INPUT_PULLUP); 
    pinMode(buton2, INPUT_PULLUP);
  }

  void loop() {
    butonlariKontrolEt(); // Continuously check traffic density

    // Determine the priority route based on the counters
    int oncelikliYol = (sayac1 > sayac2) ? 1 : 2;
    trafikSistemi(oncelikliYol);
  }

  // Prevents button bounce and increases the density counter for the respective road
  void butonlariKontrolEt() {
    if (millis() - sonButonBasma > 300) { 
      if (digitalRead(buton1) == LOW) {
        sayac1++;
        sonButonBasma = millis();
      }
      if (digitalRead(buton2) == LOW) {
        sayac2++;
        sonButonBasma = millis();
      }
    }
  }

  // Main traffic logic deciding light sequences and durations
  void trafikSistemi(int oncelikliYol) {
    int yesilSure1 = (sayac1 > sayac2) ? maxYesilSure : anaYolYesil;
    int yesilSure2 = (sayac2 > sayac1) ? maxYesilSure : taliYolYesil;

    if (oncelikliYol == 1) {
      isikDegistir(kirmizi1, sari1, yesil1, HIGH, HIGH, LOW);
      delay(sariSure);
      isikDegistir(kirmizi1, sari1, yesil1, LOW, LOW, HIGH);
      isikDegistir(kirmizi2, sari2, yesil2, HIGH, LOW, LOW);
      digitalWrite(yayaKirmizi1, HIGH); digitalWrite(yayaYesil1, LOW);
      digitalWrite(yayaKirmizi2, LOW); digitalWrite(yayaYesil2, HIGH);
      
      oncelikKontrolluBekle(yesilSure1); // Dynamic wait based on priority
      
      isikDegistir(kirmizi1, sari1, yesil1, LOW, HIGH, LOW);
      delay(sariSure);
      isikDegistir(kirmizi1, sari1, yesil1, HIGH, LOW, LOW);
    } 
    else {
      isikDegistir(kirmizi2, sari2, yesil2, HIGH, HIGH, LOW);
      delay(sariSure);
      isikDegistir(kirmizi2, sari2, yesil2, LOW, LOW, HIGH);
      isikDegistir(kirmizi1, sari1, yesil1, HIGH, LOW, LOW);
      digitalWrite(yayaKirmizi1, LOW); digitalWrite(yayaYesil1, HIGH);
      digitalWrite(yayaKirmizi2, HIGH); digitalWrite(yayaYesil2, LOW);
      
      oncelikKontrolluBekle(yesilSure2); // Dynamic wait based on priority
      
      isikDegistir(kirmizi2, sari2, yesil2, LOW, HIGH, LOW);
      delay(sariSure);
      isikDegistir(kirmizi2, sari2, yesil2, HIGH, LOW, LOW);
    }
  }

  // Helper function to easily change traffic light states
  void isikDegistir(int kirmizi, int sari, int yesil, bool kirmiziDurum, bool sariDurum, bool yesilDurum) {
    digitalWrite(kirmizi, kirmiziDurum);
    digitalWrite(sari, sariDurum);
    digitalWrite(yesil, yesilDurum);
  }

  // Non-blocking wait function: waits for the given time but instantly interrupts if priority changes
  void oncelikKontrolluBekle(unsigned long sure) {
    unsigned long baslangicZamani = millis();
    
    while (millis() - baslangicZamani < sure) {
      butonlariKontrolEt();
      // If the other road gets higher priority suddenly, cut the green light short
      if ((sayac1 > sayac2 && digitalRead(yesil2) == HIGH) || (sayac2 > sayac1 && digitalRead(yesil1) == HIGH)) {
        return; 
      }
    }
  }

</details>
```

⚠️ Important Notes / Çalıştırmak İçin Önemli Notlar
Prerequisites (Gerekli Kurulumlar): Ensure that Proteus Design Suite and Arduino IDE are installed on your computer. (Projenin çalışabilmesi için bilgisayarınızda Proteus ve Arduino IDE kurulu olmalıdır.)

ATmega328P Configuration: To properly configure the ATmega328P microcontroller in Proteus and upload the compiled .hex file, please watch this tutorial video. (Proteus ayarları ve koda ait .hex dosyasını yüklemek için bu videoyu izleyebilirsiniz.)

Proteus Components List (Devre Elemanları): If you want to build the circuit from scratch, search for the following keywords in the Proteus library (Sıfırdan kurmak isterseniz Proteus kütüphanesinde aratmanız gereken kelimeler):

ATMEGA328P (Microcontroller / Mikrodenetleyici)

LED-RED, LED-YELLOW, LED-GREEN (Standard LEDs / Standart LED'ler)

LED-BIRG (Bi-color LED / Çift Renkli LED)

RES (Resistors / Dirençler)

BUTTON (Push Button / Buton)
