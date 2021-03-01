#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold24pt7b.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);
const int buttonPin1 = 15;
const int buttonPin2 = 16;
//const int buttonPin3 = 15;
const int buzzerPin = 14;
const unsigned long timeDebounce = 10;
const unsigned long timeTimer = 1000;

int counter = 0;
unsigned long timeCounter = 0;

unsigned long timeButtonDebounce1 = 0;
int lastButtonState1 = HIGH;
int buttonState1 = HIGH;
bool buttonIgnoreEdge1 = false;
unsigned long timeButtonDebounce2 = 0;
int lastButtonState2 = HIGH;
int buttonState2 = HIGH;
bool buttonIgnoreEdge2 = false;
/*unsigned long timeButtonDebounce3 = 0;
int lastButtonState3 = HIGH;
int buttonState3 = HIGH;*/

bool flagTimerActive = false;

//======================================================
void printNumber(int number) {
  String numberString = String(number);
  if (numberString.length() < 2) {
    display.setCursor(50, 47);
  } else if (numberString.length() < 3) {
    display.setCursor(37, 47);
  } else {
    display.setCursor(23, 47);
  }
  display.clearDisplay();
  display.print(numberString);
  display.display();
}

void setup() {
  //Serial.begin(115200);

  pinMode(buttonPin1, INPUT | INPUT_PULLUP);
  pinMode(buttonPin2, INPUT | INPUT_PULLUP);
  //pinMode(buttonPin3, INPUT | INPUT_PULLUP);
    
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    //Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setFont(&FreeMonoBold24pt7b);
  
  printNumber(counter);
}

void loop() {
  unsigned long timeNow = millis();
  int buttonReading1 = digitalRead(buttonPin1);
  int buttonReading2 = digitalRead(buttonPin2);
  //int buttonReading3 = digitalRead(buttonPin3);

  // Button add 20
  if (buttonReading1 != lastButtonState1) {
    timeButtonDebounce1 = timeNow;
  }
  if ((timeNow - timeButtonDebounce1) > timeDebounce) {
    if (buttonReading1 != buttonState1) {
      buttonState1 = buttonReading1;
      if (buttonState1 == HIGH) {
        if (buttonIgnoreEdge1) {
          buttonIgnoreEdge1 = false;
        } else {
          counter += 20;
          printNumber(counter);
          flagTimerActive = true;
          timeCounter = timeNow;
        }
      }
    }
  }
  lastButtonState1 = buttonReading1;
   
  // Button add 30
  if (buttonReading2 != lastButtonState2) {
    timeButtonDebounce2 = timeNow;
  }
  if ((timeNow - timeButtonDebounce2) > timeDebounce) {
    if (buttonReading2 != buttonState2) {
      buttonState2 = buttonReading2;
      if (buttonState2 == HIGH) {
         if (buttonIgnoreEdge2) {
          buttonIgnoreEdge2 = false;
        } else {
          counter += 30;
          printNumber(counter);
          flagTimerActive = true;
          timeCounter = timeNow;
        }
      }
    }
  }
  lastButtonState2 = buttonReading2;

  if (buttonState1 == LOW && buttonState2 == LOW) {
    counter = 0;
    printNumber(counter);
    flagTimerActive = false;
    buttonIgnoreEdge1 = true;
    buttonIgnoreEdge2 = true;
    //timeCounter = timeNow;
  }

  /*// Button clear
  if (buttonReading3 != lastButtonState3) {
    timeButtonDebounce1 = timeNow;
  }
  if ((timeNow - timeButtonDebounce3) > timeDebounce) {
    if (buttonReading3 != buttonState3) {
      buttonState3 = buttonReading3;
      if (buttonState3 == HIGH) {

      }
    }
  }
  lastButtonState3 = buttonReading3;*/

  if (flagTimerActive) {
    if (timeNow - timeCounter >= timeTimer) {
      timeCounter = timeNow;
      counter--;
      printNumber(counter);
      if (counter <= 0) {
        flagTimerActive = false;
        pinMode(buzzerPin, OUTPUT);
        tone(buzzerPin, 300);
        delay(200);
        noTone(buzzerPin);
        pinMode(buzzerPin, INPUT);
      }
    }
  }
}
