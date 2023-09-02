/*SMARTLOCKER: ENHANCING PUP CONVENIENCE WITH COIN-OPERATED &
SMS-NOTIFICATION LOCKERS */

//PEDROSA SOURCE CODES (ARUDUINO)

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial SIM900A(12, 11);


const int monitoredArduinoVoltagePin = A0; // Connect the analog pin A0 to the voltage you want to monitor


const int thresholdVoltage = 300; // TO CHECK OTHER ARDUINO POWER
bool timesUpSent = false;




// Keypad connection
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','!'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String mobileNumber = "";
int cursorPosition = 0;

// LCD display configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

  lcd.init();
  lcd.backlight();
  SIM900A.begin(9600);
  Serial.begin(9600);
  pinMode(monitoredArduinoVoltagePin, INPUT);
  lcd.begin(16, 2);
  lcd.print("Enter Number:       ");
  lcd.print("Press # to skip");
  lcd.setCursor(0, 1);
  lcd.blink();


  mobileNumber = "";
  cursorPosition = 0;
}

void loop() {
  char key = keypad.getKey();

  if (key != NO_KEY) {
    handleKeypadInput(key);
  }

  int voltageValue = analogRead(monitoredArduinoVoltagePin);
  if (voltageValue < thresholdVoltage && !timesUpSent) {
    timesUpSent = true;
    Serial.println("Arduino 2 is shut down or out of voltage!" );
    lcd.clear();
    SendTimesUp();
    lcd.print("Time is expired,");
    lcd.setCursor(0, 1);
    lcd.print("Insert Coin to");
    lcd.setCursor(0, 2);
    lcd.print("ACCESS!");

    unsigned long startTime = millis();

    while (millis() - startTime < 10000) {

      voltageValue = analogRead(monitoredArduinoVoltagePin);
      if (voltageValue >= thresholdVoltage) {

        break;
      }
    }

    if (voltageValue < thresholdVoltage) {

      lcd.clear();
      SendCoops();
      lcd.print("Please see Admin,");
      lcd.setCursor(0, 1);
      lcd.print("to retrieve your");
      lcd.setCursor(0, 2);
      lcd.print("Belongings!");
      lcd.noBlink();

    }
  }

  if (voltageValue >= thresholdVoltage && timesUpSent) {
    lcd.clear();
    lcd.print("This rental locker");
    lcd.setCursor(0, 1);
    lcd.print("is occupied!");
    lcd.noBlink();
    timesUpSent = false;
  }
}

void handleKeypadInput(char key) {
  if (key == '!') {

    if (mobileNumber.length() != 11) {
      lcd.clear();
      lcd.print("Invalid Number");
      delay(2000);
      mobileNumber = "";
      cursorPosition = 0;
      lcd.clear();
  lcd.print("Enter Number:       ");
  lcd.print("Press # to skip");
  lcd.setCursor(0, 1);
  lcd.blink();
      return;
    }

    lcd.clear();
    lcd.print("Sending SMS:");
    lcd.setCursor(0, 1);
    lcd.print(mobileNumber);

    SendMessage();

    delay(2000);



    lcd.clear();
    lcd.print("This rental locker");
    lcd.setCursor(0, 1);
    lcd.print("is occupied");
     lcd.noBlink();
  }

else if (key == 'B') {

    if (cursorPosition > 0) {
      mobileNumber = mobileNumber.substring(0, cursorPosition - 1) + mobileNumber.substring(cursorPosition);
      cursorPosition--;
      lcd.setCursor(cursorPosition, 1);
      lcd.print(" ");
      lcd.setCursor(cursorPosition, 1);
    }
  }
  else if (key == 'C') {

    mobileNumber = "";
    cursorPosition = 0;
    lcd.clear();
      lcd.print("Enter Number:       ");
  lcd.print("Press # to skip");
  lcd.setCursor(0, 1);
    lcd.blink();

  }
  else if (key == 'D') {

    mobileNumber = "";
    cursorPosition = 0;
    lcd.clear();
    lcd.print("Change Number");
    lcd.setCursor(0, 1);
    lcd.blink();
  }
   else if (key == '#') {

    mobileNumber = "";
    cursorPosition = 0;
    lcd.clear();
    lcd.print("This rental locker");
    lcd.setCursor(0, 1);
    lcd.print("is occupied");
    lcd.noBlink();

  }
  else {

    if (mobileNumber.length() < 16) {
      mobileNumber = mobileNumber.substring(0, cursorPosition) + String(key) + mobileNumber.substring(cursorPosition);
      cursorPosition++;
      lcd.print(key);
      lcd.setCursor(cursorPosition, 1);
    }
  }
}

void SendMessage() {
  lcd.clear();
  lcd.print("Your Number");
  lcd.setCursor(0, 1);
  lcd.print("Have Been Registered");
  delay(4000);
    lcd.clear();
  lcd.print("Sending Message");
  SIM900A.println("AT+CMGF=1");
  delay(1000);
  SIM900A.print("AT+CMGS=\"");
  SIM900A.print(mobileNumber);
  SIM900A.println("\"");
  delay(1000);
  SIM900A.println("Your locker belongings have a limited time! Retrieve your items before the expiry date to avoid forfeiture. We will send an alert message"); // Message content
  delay(1000);
  lcd.clear();
  SIM900A.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
  lcd.clear();
  lcd.print("Message has been");
  lcd.setCursor(0, 1);
  lcd.print("sent via SMS ");
}

void SendTimesUp() {


  SIM900A.println("AT+CMGF=1");  // Sets the GSM Module in Text Mode
  delay(1000);
  SIM900A.print("AT+CMGS=\"");
  SIM900A.print(mobileNumber);
  SIM900A.println("\"");
  delay(1000);
  SIM900A.println("Your locker time has expired! Please retrieve your belongings as soon as possible to avoid forfeiture. Thank you!"); // Message content
  delay(1000);
  lcd.clear();
  SIM900A.println((char)26);
 // ASCII code of CTRL+Z
}

void SendCoops() {


  SIM900A.println("AT+CMGF=1");  // Sets the GSM Module in Text Mode
  delay(1000);
  SIM900A.print("AT+CMGS=\"");
  SIM900A.print("09217871467");
  SIM900A.println("\"");
  delay(1000);
  SIM900A.println("This Locker A exceed the time Limit! Go and reset the locker"); // Message content
  delay(1000);
  lcd.clear();
  SIM900A.println((char)26);
 // ASCII code of CTRL+Z
}
