#include <Wire.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold24pt7b.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);
const int buttonPin1 = 15;
const int buttonPin2 = 16;
const int buzzerPin = 14;
const unsigned long timeDebounce = 10;
const unsigned long timeTimer = 1000;

int counter = 0;
int counterHalf = 0;
unsigned long timeCounter = 0;
bool flagTimerActive = false;

unsigned long timeButtonDebounce1 = 0;
int lastButtonState1 = HIGH;
int buttonState1 = HIGH;
bool buttonIgnoreEdge1 = false;
unsigned long timeButtonDebounce2 = 0;
int lastButtonState2 = HIGH;
int buttonState2 = HIGH;
bool buttonIgnoreEdge2 = false;

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

void makeSound(int frequency, int duration) {
  pinMode(buzzerPin, OUTPUT);
  tone(buzzerPin, frequency);
  delay(duration);
  noTone(buzzerPin);
  pinMode(buzzerPin, INPUT);
}
void makeSound(int duration) {
  makeSound(1000, duration);
}

void setup() {
  //Serial.begin(115200);

  pinMode(buttonPin1, INPUT | INPUT_PULLUP);
  pinMode(buttonPin2, INPUT | INPUT_PULLUP);
     
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
          counterHalf = counter / 2;
          printNumber(counter);
          flagTimerActive = true;
          timeCounter = timeNow;
          makeSound(1000, 100);
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
          counterHalf = counter / 2;
          printNumber(counter);
          flagTimerActive = true;
          timeCounter = timeNow;
          makeSound(150);
        }
      }
    }
  }
  lastButtonState2 = buttonReading2;

  if (buttonState1 == LOW && buttonState2 == LOW) {
    if (counter != 0) {
       makeSound(200);
    }
    counter = 0;
    printNumber(counter);
    flagTimerActive = false;
    buttonIgnoreEdge1 = true;
    buttonIgnoreEdge2 = true;
  }

  if (flagTimerActive) {
    if (timeNow - timeCounter >= timeTimer) {
      timeCounter = timeNow;
      counter--;
      printNumber(counter);
      if (counter == counterHalf) {
        makeSound(100);
      }
      if (counter <= 0) {
        flagTimerActive = false;
        //makeSound(300, 200);
        makeSound(50);
        delay(50);
        makeSound(200);
      }
    }
  }
}
