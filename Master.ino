#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int slaveIDs[] = {1,2,3};  
const int numSlaves = sizeof(slaveIDs) / sizeof(slaveIDs[0]);

float energiaTotal = 0.0;
float valorTotal = 0.0;
float energiaPorSlave[5];
float valorPorSlave[5];

int currentScreen = 0;

void setup() {
  Wire.begin(); // Maestro
  lcd.begin(16, 2);
}

void loop() {
  energiaTotal = 0;
  valorTotal = 0;

  for (int i = 0; i < numSlaves; i++) {
    Wire.requestFrom(slaveIDs[i], 8);
    if (Wire.available() == 8) {
      Wire.readBytes((byte*)&energiaPorSlave[i], 4);
      Wire.readBytes((byte*)&valorPorSlave[i], 4);

      energiaTotal += energiaPorSlave[i];
      valorTotal += valorPorSlave[i];
    }
  }

  float valorConDescuento = valorTotal * 0.85;

  lcd.clear();
  lcd.setCursor(0, 0);

  if (currentScreen == 0) {
    lcd.print("Total: Q");
    lcd.print(valorConDescuento, 2);
    lcd.setCursor(0, 1);
    lcd.print("kWh:");
    lcd.print(energiaTotal, 2);
  } else {
    int idx = currentScreen - 1;
    if (idx < numSlaves) {
      lcd.print("S" + String(slaveIDs[idx]) + " Q");
      lcd.print(valorPorSlave[idx], 2);
      lcd.setCursor(0, 1);
      lcd.print("kWh:");
      lcd.print(energiaPorSlave[idx], 2);
    }
  }

  currentScreen++;
  if (currentScreen > numSlaves) currentScreen = 0;

  delay(2000);
}
