#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Keypad.h>

#define FOOD_WEIGHT 0;
#define WATER_WEIGHT 0;

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};  // connect to the row pinouts of the keypad
byte colPins[COLS] = {6,7,8,9};     // connect to the column pinouts of the keypad

unsigned int addr = 0;              // Address to store time value in EEPROM

bool isSet = false;
bool toRefile = false;

unsigned int currentTime = 0;
unsigned int setTime = 0;

LiquidCrystal lcd();                // Pa set nalang

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setLCD() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Smart Pet Feeder");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(3000);
  lcd.clear();
}

void resetTimer() {
  setTime = 0;
  isTimeSet = false;
}

void refile() {
  float foodWeight = 0; // computation of the current weight of the food tray
  float waterWeight = 0; // computation of the current weight of the water tray

  if (foodWeight < FOOD_WEIGHT) {
    lcd.clear();
    lcd.print("Pouring food...");

    // include the actual code for refiling of food
  }

  if (waterWeight < WATER_WEIGHT) {
    lcd.clear();
    lcd.print("Pouring water...");

    // include the actual code for refiling of water
  }

  toRefile = false;
}

void displayCurrentStatus() {
  lcd.setCursor(0, 0);
  lcd.print("Food:");
  lcd.print(); // current food weight 
  lcd.setCursor(0, 1);
  lcd.print("Water:");
  lcd.print(); // current water weight
}

String getTimerData() {
  char key;
  String inputStr;
  
  do {
    key = keypad.getKey();
    
    if (key != NO_KEY) {
      inputStr += key;
    }
  
  } while (key != '#');
  
  return strtoul(inputStr.c_str(), NULL, 10);
}

void setTimer() {
  setTime = getTimerData();

  EEPROM.write(addr, setTime); // store time value in EEPROM
  lcd.clear();
  lcd.print("Time set to:");
  lcd.setCursor(0, 1);
  lcd.print(setTime);
  lcd.print(" seconds");
  delay(2000);
  lcd.clear();
  isTimeSet = true;
}
void setup() {
  setLCD();                        // Initialize LCD display
  Serial.begin(9600);
}

void loop() {
  // Check if time has already been set
  if (!isSet) {
    setTimer();
  }

  // Check if reset button has been pressed
  if (isSet && keypad.getKey() != NO_KEY && keypad.getKey() == '*') {
    resetTimer();
  }

  // Check if it's time to refile
  currentTime = millis() / 1000; // convert milliseconds to seconds
  if (currentTime >= setTime) {
    toRefile = true;
  }

  if (toRefile) {
    refile();
  }

  displayCurrentStatus();
  delay(1000); 
}
