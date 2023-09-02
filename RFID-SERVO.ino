/*SMARTLOCKER: ENHANCING PUP CONVENIENCE WITH COIN-OPERATED &
SMS-NOTIFICATION LOCKERS */

//PEDROSA SOURCE CODES (ARUDUINO)


#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>

#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);
Servo s1;
bool bukas = true;
 #define SS_PIN 10
 #define RST_PIN 9
 #define LED_G 5
 #define LED_R 7
 #define BUZZER 6
 MFRC522 mfrc522(SS_PIN, RST_PIN);
 void setup()
 {
  Serial.begin(9600);
  SPI.begin();
   lcd.init();
  lcd.backlight();
 ;
  mfrc522.PCD_Init();
  s1.attach(8);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  Serial.println("Put your card to the reader...");
  Serial.println();
  lcd.setCursor(0,0);
  lcd.print(" Put your card   ");
 }
 void loop()
 {

  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
   return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial())
  {
   return;
  }

  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "24 4B 11 2B" && bukas)
  {
   Serial.println("Authorized access");
   Serial.println();
     lcd.setCursor(0,0);
     lcd.print(" CARD IS VALID      ");
     lcd.setCursor(0,1);
     lcd.print("Opening the Door   ");
     digitalWrite(LED_G, HIGH);
     s1.write(0);
     delay(3000);
     lcd.setCursor(0,1);
     lcd.print("closing the Door   ");
     digitalWrite(LED_G, LOW);
     delay(2000);
     lcd.setCursor(0,0);
     lcd.print(" Put your card   ");
     lcd.setCursor(0,1);
     lcd.print("              ");
     bukas = false;
  }

   else if( content.substring(1) == "24 4B 11 2B" || !bukas){
    s1.write(90);
    lcd.print("closing the Door   ");
     digitalWrite(LED_G, LOW);
     delay(2000);
    bukas = true;
  }

  else
  {
   Serial.println("CARD IS INVALID");
    lcd.setCursor(0,1);
    lcd.print("CARD IS INVALID   ");
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 300);
    delay(2000);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
    lcd.setCursor(0,1);
    lcd.print("              ");
  }
 }
