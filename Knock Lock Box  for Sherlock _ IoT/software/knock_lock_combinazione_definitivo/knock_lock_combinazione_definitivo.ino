#include <Servo.h>
Servo myServo;

// Pin definitions
const int piezo = A0;         // Piezo sensor on pin 0.
const int photoresist = A5;   //fotoresistenza
const int blueLed = 4;
const int redLed = 5;
const int greenLed = 3;

// Tuning constants.
const int threshold = 15;           // Minimum signal from the piezo to register as a knock
const int rejectValue = 15;        // If an individual knock is off by this percentage of a knock we don't unlock..
const int knockFadeTime = 150;     // milliseconds we allow a knock to fade before we listen for another one. (Debounce timer.)
const int maximumKnocks = 4;       // Maximum number of knocks to listen for.
unsigned long timer;
unsigned long lastKnock;

// Variables.
int knockSensorValue = 0;           // Last reading of the knock sensor.
int photoresistVal = 0;             // Initial Value for the photoresistance
boolean locked = false;             // The program begn with the box open
int nrOfKnocks = 0;


void setup() {
  myServo.attach(6);
  pinMode(photoresist, INPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  Serial.begin(9600);
  myServo.write(0);     // the servo is in vertical mode
  Serial.println("Program Start");

  //led RGB
  digitalWrite(greenLed, HIGH);
  digitalWrite(redLed, HIGH);
  digitalWrite(blueLed, HIGH);
}


void loop() {

  /*Lock the box . Used a photoresistence. When we close the box, it actives the lock system (with the dark)*/
  if (locked == false) {
    photoresistVal = analogRead(photoresist);
    if (photoresistVal < 50) {
      locked = true;
      analogWrite(greenLed, 0);
      analogWrite(redLed, 255);
      analogWrite(blueLed, 0);
      myServo.write(0);
      Serial.println("Locked");
      delay(2000);
    }
  }

  /*Unlock the box*/
  if (locked == true) {
  knockSensorValue = analogRead(A0);
 
    if (KnockSensorValue > 50) {
      nrOfKnocks ++;
      Serial.print("Colpo numero: ");
      Serial.println(nrOfKnocks);
      timer = millis();
      yellowLight();
      
    //secret code: 2 knocks -- wait 3 seconds -- 1 knock
      if (nrOfKnocks == 3 &&  millis() - lastKnock > 2000) {
        Serial.println("SBLOCCA TUTTO");
        nrOfKnocks = 0;
        locked = false;
        myServo.write(90);  //the servo turns 
        delay(500);
        analogWrite(greenLed, 255);
        analogWrite(redLed, 0);
        analogWrite(blueLed, 0);
        Serial.println("Unlocked!");
        delay(7000);
      }
      
      lastKnock = millis();
    }
  }

  //condition: if the secret code isn't right
  if (nrOfKnocks > 0 && millis() - timer > 5000 && locked == true) {
    yellowLight();
    timer = millis();
    nrOfKnocks = 0;
    analogWrite(greenLed, 0);
    analogWrite(redLed, 255);
    analogWrite(blueLed, 0);
  }
}

//check for knocks: For each knock the led become yellow
void yellowLight()
{
  analogWrite(redLed, 80);
  analogWrite(greenLed, 255);
  analogWrite(blueLed, 0);
  delay(140);
  analogWrite(redLed, 0);
  analogWrite(greenLed, 0);
  Serial.println("Colpo Valido");

}


