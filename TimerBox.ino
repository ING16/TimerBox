/*
TimerBox
*/
//Pin's
const int switchPin       = 11;
const int ledPin          = 13;
const int buttonPin       = 7;    // the number of the pushbutton pin
const int dataPin         = 10; 
const int latchPin        = 9;      
const int clockPin        = 8;     

//Botton Variables
int buttonState;                      // the current reading from the input pin
int lastButtonState = LOW;            // the previous reading from the input pin
unsigned long lastDebounceTime = 0;   // the last time the output pin was toggled
unsigned long debounceDelay = 5;      // the debounce time; increase if the output flickers
bool bottonState = true;

//Switch Variables
unsigned long int timeOn      = 60;     //The switch is on for this many sec/min
unsigned long int timeOnCnt;
unsigned long int timeOnDef   = 1000;   //Defines timeOn 1000=sec 60000=min
unsigned long int bottonTime  = 0;      //The botton wat pressed at this time
int ledStatus = LOW;                    //Status for the led
int switchState = HIGH;

//7 segment Variables
unsigned long previousMillis  = 0;        // will store last time LED was updated
byte segmentLight[10] = {63, 6, 91, 79, 102, 109, 125, 7 ,127, 103};
int ssDelayTime                 = 10;
unsigned long lastSsDelayTime   = 0;

void setup()
{
Serial.begin(9600);  
//initier pin som output
pinMode(ledPin, OUTPUT);
pinMode(switchPin, OUTPUT);
pinMode(dataPin, OUTPUT);
pinMode(clockPin, OUTPUT);
pinMode(latchPin, OUTPUT);
pinMode(buttonPin, INPUT);

digitalWrite(switchPin, HIGH);
switchState = HIGH;
bottonTime = millis();
timeOnCnt = timeOn; 
}

void loop(){
  //Serial.print("timeon="); Serial.println(millis() - oldSwTime,DEC);
  bottonHandling();
  sevenSegmentHandling();
  
  if (switchState == LOW){
    digitalWrite(switchPin, LOW);
  }else{
    digitalWrite(switchPin, HIGH);
    switchState = HIGH;
  }
}

void bottonHandling(){
   int reading = digitalRead(buttonPin);
  // check to see if you just pressed the button
  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // Reset timers and lights
      if (buttonState == HIGH) {
        bottonTime = millis();
        switchState = HIGH;
        timeOnCnt = timeOn;
      }
    }
  }
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;     
  }

void sevenSegmentHandling(){
  unsigned long currentMillis = millis();
//  Serial.print("timeOnDef="); Serial.println(timeOnDef,DEC);
  if (currentMillis - previousMillis >= timeOnDef and switchState == HIGH) {
//        // save the last time you blinked the LED
        previousMillis = currentMillis;
        timeOnCnt--;
        if (timeOnCnt <= 0){
         timeOnCnt = 00;
         switchState = LOW;
        }
     }
  displayNo(timeOnCnt);
  
}

void displayNo(int no) {

//  unsigned long currentMillis = millis();
  
  int digit1 = 0;
  int digit2 = 0;  
  if (no > 9){
    digit1 = no / 10;
    digit2 = no % 10;
  }
  else {
    digit1 = 0;
    digit2 = no;
  }
//  if (currentMillis - lastSsDelayTime >= ssDelayTime) {
//    lastSsDelayTime = millis();

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, B00000010);
    shiftOut(dataPin, clockPin, MSBFIRST, segmentLight[digit1]);
    digitalWrite(latchPin, HIGH);
    delay(5);
    
//  digitalWrite(latchPin, LOW);
//  shiftOut(dataPin, clockPin, MSBFIRST, B00000010);
////    shiftOut(dataPin, clockPin, MSBFIRST, B10000000); //punktum
//  digitalWrite(latchPin, HIGH);
//  delay(5);
    
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, B00000001);
  shiftOut(dataPin, clockPin, MSBFIRST, segmentLight[digit2]);
  digitalWrite(latchPin, HIGH);
  delay(5);
}


