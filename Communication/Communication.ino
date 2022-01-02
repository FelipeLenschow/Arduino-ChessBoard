#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

String EngineMove = "";
bool Send;
int count = 0;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  lcd.clear();
  
  lcd.setCursor(0, 0);
  for (int i = 0; i < 3; i++) {
    randomSeed(analogRead(A6));
    lcd.print(random(0, 60));
    lcd.print(" ");
  }


}

void loop() {
  while (Serial.available() > 0) {
    EngineMove += Serial.readString();
    lcd.setCursor(0, 0);
    lcd.print(EngineMove);
    Send = true;
    digitalWrite(13, HIGH);
    count++;
  }

  if (Send ){//&& EngineMove.substring(0, 3 == "e2e4")) {
    delay(3000);
    Serial.write("c7c5");
    Send = false;
  }
  
//  Serial.println(analogRead(A6));

}
