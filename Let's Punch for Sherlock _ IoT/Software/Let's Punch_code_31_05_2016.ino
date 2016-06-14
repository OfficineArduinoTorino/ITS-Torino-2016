#include <ThreadController.h>

#include <Adafruit_Thermal.h>

Adafruit_Thermal printer(&Serial1);
#define TX_PIN 14;


// declaring constants

const int fc = A0;  // control photoresistor
const int f1 = A1;  // photoresistor 1
const int f2 = A2;  // photoresistor 2
const int f3 = A3;  // photoresistor 3
const int f4 = A4;  // photoresistor 4
const int ledFeedback = 0;  // feedback led for the cards
// leds for the readings
const int ledLett1 = 1;
const int ledLett2 = 2;
const int ledLett3 = 3;
const int ledLett4 = 4;
const int ledLett5 = 5;

const int lungCodice = 4;   // code length
const int threshold = 50;  // light threshold to discriminate a 0 or a 1

int fcValue;  // control photoresistor value
int f1Value;
int f2Value;
int f3Value;
int f4Value;
int letture[lungCodice];  // array to store the reading of the photoresistors
String codice = "";   // card identifying code
boolean flag = false;  // flag "card inserted or not"

// blinking lamp
const int lamp = 6;
int lampState = LOW;  // starting state of the lamp
unsigned long previousMillis = 0;  // saves the last time the lamp's status changed
int interval = 400;  // blinking interval
unsigned long currentMillis;  // elapsed time


// function that receives an analog value and returns a digital value, based on the threshold
int Digitalizza(int lettAnalog){
  
  int lettDigit;

  if(lettAnalog>threshold)
    lettDigit = 1;
  else
    lettDigit = 0;

  return lettDigit;
}

// function to let the lamp blink
void blinkingLamp(long interval)
{
  currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    
    previousMillis = currentMillis;  // save the last time you blinked the lamp

    // if the lamp is off turn it on and vice-versa
    if (lampState == LOW) {
      lampState = HIGH;
    }
    else {
      lampState = LOW;
    }
    
    // set the lamp with the lampState of the variable
    digitalWrite(lamp, lampState);
  }
}


void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
  printer.begin(9600);

  // set all the reading leds to HIGH
  pinMode(ledLett1, OUTPUT);
  digitalWrite(ledLett1, HIGH);
  pinMode(ledLett2, OUTPUT);
  digitalWrite(ledLett2, HIGH);
  pinMode(ledLett3, OUTPUT);
  digitalWrite(ledLett3, HIGH);
  pinMode(ledLett4, OUTPUT);
  digitalWrite(ledLett4, HIGH);
  pinMode(ledLett5, OUTPUT);
  digitalWrite(ledLett5, HIGH);

  // set the feedback led to OUTPUT and LOW
  pinMode(ledFeedback, OUTPUT);
  digitalWrite(ledFeedback, LOW);
  
  // set the blinking lamp to OUTPUT
  pinMode(lamp, OUTPUT);

  // wait 2 seconds to start
  delay(2000);
}



void loop() {

  delay(50);

  // read the control photoresistor value
  fcValue=Digitalizza(analogRead(fc));
  // read the other photoresistors values
  f1Value=Digitalizza(analogRead(f1));
  f2Value=Digitalizza(analogRead(f2));
  f3Value=Digitalizza(analogRead(f3));
  f4Value=Digitalizza(analogRead(f4));
  Serial.println("\n");
  
  // print in console --> check the analogic value of the control photoresistor
Serial.println(analogRead(fc));
  
  // print in console --> check the digital value of all the photoresistors
  Serial.println(fcValue);
  Serial.println(f1Value);
  Serial.println(f2Value);
  Serial.println(f3Value);
  Serial.println(f4Value);


  // empty the card code
  codice = "";
  
// insert the digital values of the photoresistors readings into the readings array

if(fcValue==0 && !flag){
    
    flag = true;  // flag becomes true
    
    // when the user inserts the card, light the feedback led
    digitalWrite(ledFeedback, HIGH);
    
    // wait for the card to be fully inserted
    delay(1000);

      letture[0]=Digitalizza(analogRead(f1));
      delay(7);
      letture[1]=Digitalizza(analogRead(f2));
      delay(7);
      letture[2]=Digitalizza(analogRead(f3));
      delay(7);
      letture[3]=Digitalizza(analogRead(f4));
      delay(7);
      for (int i = 0; i < 4; i++) {
        Serial.print(letture[i]);
      }
      Serial.println();

      for(int i=0; i<lungCodice; i++)
      codice = (codice + letture[i]);

    // print in console --> card code
      Serial.println("\nCODICE SCHEDA:");
      Serial.println(codice);


  // print a different message for each card
  
  if(codice=="1111"){
    Serial.println("\nVITTORIA!");

    // set the lamp to blink slower
    for(int i=0; i<7; i++){
      delay(1200);
      if (lampState == LOW) {
      lampState = HIGH;
      }
      else {
      lampState = LOW;
      }

      digitalWrite(lamp, lampState);
    }

    // set the blinking lamp to LOW
    digitalWrite(lamp, LOW);
    // set the feedback led to LOW
    digitalWrite(ledFeedback, LOW);

    // set oll the other led to LOW
    digitalWrite(ledLett1, LOW);
    digitalWrite(ledLett2, LOW);
    digitalWrite(ledLett3, LOW);
    digitalWrite(ledLett4, LOW);
    digitalWrite(ledLett5, LOW);

    // print the final hint
    printer.println("\n\n''Tra cemento e calcestruzzo\nsi erige la facciata sud\ndell'edificio,\nincastrata tra le armature,\nLei ancora vive''\n\n");
    printer.println("Questo messaggio\nva ri-consegnato\na chi, per lungo tempo,\nla memoria ne ha celato.\n\n\n");
    
    // stop the program, blocked in an infinite loop
    while(true){}
  }

  else if(codice=="0100"){
    printer.println("\n\nMongolfiera\n\n\n");
  }
  
  else if(codice=="1010"){
    printer.println("\n\nScheda perforata\n\n\n");
  }
  
  else if(codice=="0001"){
    printer.println("\n\nAccademia\n\n\n");
  }

  else if(codice=="1110"){
    printer.println("\n\nMenabrea\n\n\n");
  }

  else if(codice=="0011"){
    printer.println("\n\nFesta di gala\n\n\n");
  }
  
  else if(codice=="0111"){
    printer.println("\n\nItalia '800\n\n\n");
  }
  
  else if(codice=="1000"){
    printer.println("\n\nTorino '800\n\n\n");
  }
  
  else if(codice=="1101"){
    printer.println("\n\nSpanu\n\n\n");
  }

  else if(codice=="1001"){
    printer.println("\n\nPorcheddu\n\n\n");
  }

  else if(codice=="0010"){
    printer.println("\n\nIl professore\n\n\n");
  }

  // read again the control photoresistor
  fcValue=Digitalizza(analogRead(fc));
  }
  
  if(fcValue==1){
    flag = false;  // flag becomes false when the user removes the card
    
    // after the reading, set the feedback led to LOW
    digitalWrite(ledFeedback, LOW);
   }

  // blink the lamp
  blinkingLamp(interval);
}
  

