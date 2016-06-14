/*
 * Sketch di Test per le 6 ventole comandate dalla Leonardo
 * Si può testare anche con meno di 6 ventole, a condizione che le ventole non siano attaccate a pin diversi da quelli definiti in PWMOutputPins
 *
 * Per testare un nuovo valore per una ventola inserire x,y come serial input, dove x è l'indice della ventola e y è il valore desiderato
 */

#define N_PWM_OUTPUTS 6

const int PWMOutputPins[N_PWM_OUTPUTS] = { 3, 5, 6, 9, 10, 11 }; // Pins degli output PWM

byte PWMValues[N_PWM_OUTPUTS] = { // Leggero, medio, forte
  155, // Rizzo
  150, // Napoli
  215, // Appendino
  135, // Rosso
  230, // Fassino
  160  // Airaudo
};

int PWMOutputPinsIndex;
int PWMValue;

String serialInputString;
int commaIndex;

void setup() {

  for (int i = 0; i < N_PWM_OUTPUTS; i++) { // Settiamo il pinMode per i vari pin output
    pinMode(PWMOutputPins[i], OUTPUT);
  }

  Serial.begin(9600); // Predisposizione e configurazione della frequenza per la comunicazione seriale (Visionabile dalla lente in alto a destra dell'IDE di Arduino)
    
  for (int i = 0; i < N_PWM_OUTPUTS; i++) { // Settiamo i valori scelto per tutti pin PWM
    analogWrite(PWMOutputPins[i], PWMValues[i]);
  }
  
}

void loop() {

  while (Serial.available()) { // Input utente presente
    
    //Parsing dell'input
    serialInputString = Serial.readString();
    commaIndex = serialInputString.indexOf(',');
    PWMOutputPinsIndex = serialInputString.substring(0, commaIndex).toInt();
    PWMValue = serialInputString.substring(commaIndex+1).toInt();
    
    Serial.print("Stiamo testando la ventola ");
    Serial.print(PWMOutputPinsIndex);
    Serial.print(" con una potenza di ");
    Serial.println(PWMValue);

    analogWrite(PWMOutputPins[PWMOutputPinsIndex], PWMValue);
  }
  
  delay(10); // Evitiamo il ciclo attivo (Potrebbe far sforzare troppo la Leonardo)

}
