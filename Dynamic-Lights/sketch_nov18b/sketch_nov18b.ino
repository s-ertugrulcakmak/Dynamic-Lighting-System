const int kirmizi1 = 2, sari1 = 3, yesil1 = 4;  
const int kirmizi2 = 5, sari2 = 6, yesil2 = 7;  

const int yayaKirmizi1 = A0, yayaYesil1 = A1; 
const int yayaKirmizi2 = A2, yayaYesil2 = A3;  

const int buton1 = 8, buton2 = 9; 
int sayac1 = 0, sayac2 = 0; 
unsigned long sonButonBasma = 0;

// Gerçek trafik süreleri
const int anaYolYesil = 60 * 1000; // 60 saniye
const int taliYolYesil = 30 * 1000; // 30 saniye
const int sariSure = 3000; // 3 saniye
const int maxYesilSure = 90 * 1000; // 90 saniye

void setup() {
  pinMode(kirmizi1, OUTPUT);
  pinMode(sari1, OUTPUT);
  pinMode(yesil1, OUTPUT);
  pinMode(kirmizi2, OUTPUT);
  pinMode(sari2, OUTPUT);
  pinMode(yesil2, OUTPUT);
  pinMode(yayaKirmizi1, OUTPUT);
  pinMode(yayaYesil1, OUTPUT);
  pinMode(yayaKirmizi2, OUTPUT);
  pinMode(yayaYesil2, OUTPUT);
  pinMode(buton1, INPUT_PULLUP);
  pinMode(buton2, INPUT_PULLUP);
}

void loop() {
  butonlariKontrolEt();

  int oncelikliYol = (sayac1 > sayac2) ? 1 : 2;
  trafikSistemi(oncelikliYol);
}

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

void trafikSistemi(int oncelikliYol) {
  int yesilSure1 = (sayac1 > sayac2) ? maxYesilSure : anaYolYesil;
  int yesilSure2 = (sayac2 > sayac1) ? maxYesilSure : taliYolYesil;

  if (oncelikliYol == 1) {
    isikDegistir(kirmizi1, sari1, yesil1, HIGH, HIGH, LOW);
    delay(sariSure);
    isikDegistir(kirmizi1, sari1, yesil1, LOW, LOW, HIGH);
    isikDegistir(kirmizi2, sari2, yesil2, HIGH, LOW, LOW);
    digitalWrite(yayaKirmizi1, HIGH);
    digitalWrite(yayaYesil1, LOW);
    digitalWrite(yayaKirmizi2, LOW);
    digitalWrite(yayaYesil2, HIGH);
    oncelikKontrolluBekle(yesilSure1);
    isikDegistir(kirmizi1, sari1, yesil1, LOW, HIGH, LOW);
    delay(sariSure);
    isikDegistir(kirmizi1, sari1, yesil1, HIGH, LOW, LOW);
  } 
  else {
    isikDegistir(kirmizi2, sari2, yesil2, HIGH, HIGH, LOW);
    delay(sariSure);
    isikDegistir(kirmizi2, sari2, yesil2, LOW, LOW, HIGH);
    isikDegistir(kirmizi1, sari1, yesil1, HIGH, LOW, LOW);
    digitalWrite(yayaKirmizi1, LOW);
    digitalWrite(yayaYesil1, HIGH);
    digitalWrite(yayaKirmizi2, HIGH);
    digitalWrite(yayaYesil2, LOW);
    oncelikKontrolluBekle(yesilSure2);
    isikDegistir(kirmizi2, sari2, yesil2, LOW, HIGH, LOW);
    delay(sariSure);
    isikDegistir(kirmizi2, sari2, yesil2, HIGH, LOW, LOW);
  }
}

void isikDegistir(int kirmizi, int sari, int yesil, bool kirmiziDurum, bool sariDurum, bool yesilDurum) {
  digitalWrite(kirmizi, kirmiziDurum);
  digitalWrite(sari, sariDurum);
  digitalWrite(yesil, yesilDurum);
}

void oncelikKontrolluBekle(unsigned long sure) {
  unsigned long baslangicZamani = millis();
  
  while (millis() - baslangicZamani < sure) {
    butonlariKontrolEt();
    if ((sayac1 > sayac2 && digitalRead(yesil2) == HIGH) || (sayac2 > sayac1 && digitalRead(yesil1) == HIGH)) {
      return;
    }
  }
}
