#include <Wire.h>
#include <LiquidCrystal.h>

const float voltaje = 120.0;
const float costo = 2.86;
const int ID = 3;

int potKwh = A0;
int potMenu = A1;

float kwhFijo = 0.0;
float energiaConsumida = 0.0;
float valor = 0.0;
float corriente = 0.0;

unsigned long lastMillis = 0;
unsigned long startMillis = 0;
int currentScreen = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  lcd.begin(16, 2);
  lcd.clear();

  Wire.begin(ID);   
  Wire.onRequest(requestEvent); 

  lastMillis = millis();
  startMillis = millis();
}

void loop() {
  unsigned long now = millis();
  float deltaT = (now - lastMillis) / 1000.0;
  lastMillis = now;

  unsigned long elapsedMillis = now - startMillis;
  unsigned long totalSeconds = elapsedMillis / 1000;
  int hours = totalSeconds / 3600;
  int minutes = (totalSeconds % 3600) / 60;
  int seconds = totalSeconds % 60;
  int kwhRaw = analogRead(potKwh);
  kwhFijo = map(kwhRaw, 0, 1023, 0, 3000);
  energiaConsumida += (kwhFijo * deltaT) / 3600.0; 
  valor = energiaConsumida * costo;
  corriente = kwhFijo / voltaje;

  int menuRaw = analogRead(potMenu);
  currentScreen = map(menuRaw, 0, 1023, 0, 4);

  lcd.setCursor(0, 0);
  lcd.print("Valor: Q");
  lcd.print(valor, 2);
  lcd.print("     ");

  lcd.setCursor(0, 1);
  lcd.print("                "); 

  switch (currentScreen) {
    case 0:
      lcd.setCursor(0, 1);
      lcd.print("Voltaje: ");
      lcd.print(voltaje, 0);
      lcd.print("V");
      break;
    case 1:
      lcd.setCursor(0, 1);
      lcd.print("Corriente: ");
      lcd.print(corriente, 2);
      lcd.print("A");
      break;
    case 2:
      lcd.setCursor(0, 1);
      lcd.print("Costo: Q");
      lcd.print(costo, 2);
      break;
    case 3:
      lcd.setCursor(0, 1);
      lcd.print("kWh: ");
      lcd.print(energiaConsumida, 2);
      break;
    case 4:
      lcd.setCursor(0, 1);
      lcd.print("Tiempo ");
      if (hours < 10) lcd.print('0');
      lcd.print(hours); lcd.print(':');
      if (minutes < 10) lcd.print('0');
      lcd.print(minutes); lcd.print(':');
      if (seconds < 10) lcd.print('0');
      lcd.print(seconds);
      break;
  }

  delay(250);
}

void requestEvent() {
  Wire.write((byte*)&energiaConsumida, sizeof(energiaConsumida));
  Wire.write((byte*)&valor, sizeof(valor));
}
