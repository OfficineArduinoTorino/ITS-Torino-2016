#define N_PWM_OUTPUTS 6
#define ENCODED_INPUT_SIZE 7 // In bytes

// PWM related variables
const int PWMOutputPins[N_PWM_OUTPUTS] = { 3, 5, 6, 9, 10, 11 };
int PWMValues[N_PWM_OUTPUTS];
int oldPWMValues[N_PWM_OUTPUTS];

// Encoded input variables
byte encodedInputArray[ENCODED_INPUT_SIZE];
const int minimumValidInputValue = 1; // Values less than this are reserved for input encoding

// Delay values
const int loopDelayValue = 5;
const int analogWriteDelayValue = 5;

void setup() {

  // Setting pinMode for every output and minimum valid input value for old PWM values
  for (int i = 0; i < N_PWM_OUTPUTS; i++) {
    pinMode(PWMOutputPins[i], OUTPUT);
    oldPWMValues[i] = minimumValidInputValue;
    PWMValues[i] = minimumValidInputValue;
    analogWrite(PWMOutputPins[i], PWMValues[i]);
  }

  Serial1.begin(9600); // Serial for MRK1000
  Serial.begin(9600); // Serial for serial monitor
  
}

void loop() {

  readSerialInput(); // MKR1000 Input
  
  for (int i = 0; i < N_PWM_OUTPUTS; i++) {     
    if (PWMValues[i] != oldPWMValues[i]) { // New PWM value
      Serial.print("Received ");
      Serial.print(PWMValues[i]);
      Serial.print(" for PWM Output ");
      Serial.println(PWMOutputPins[i]);
      Serial.flush();
      
      analogWrite(PWMOutputPins[i], PWMValues[i]);
      delay(analogWriteDelayValue);
      
      oldPWMValues[i] = PWMValues[i]; // Debounce variable update
    }
  }

  delay(loopDelayValue);

}

void readSerialInput() {
  
  if (Serial1.available()) {
    for (int i = 0; i < ENCODED_INPUT_SIZE; i++) {
      encodedInputArray[i] = Serial1.read();
      delay(loopDelayValue);
    }

    for (int i = 0; i < N_PWM_OUTPUTS; i++) {
      PWMValues[i] = encodedInputArray[i];
    }
  }
  
}

// TODO:
// add 0 control
// add control for less then minimum value

