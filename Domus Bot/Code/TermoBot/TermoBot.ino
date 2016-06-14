#include <WiFi101.h>
#include <SPI.h>
#include <TelegramBot.h>
#include "DHT.h"
#include <ArduinoJson.h>
#include <LiquidCrystal.h>


#define DHTPIN 6
#define DHTTYPE DHT22

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int brightnessPin = 10;
int brightnessValue = 100;
const char BotToken[] = "174631339:AAFz0D6gr2U2rSiHpR3X1uvfQoK3XccBlaU";
const char BotName[] = "Domusbot";
const char BotUsername[] = "Domusbot_bot";

char ssid[] = "InfostradaWiFi-004155";    // your network SSID (name)
char pass[] = "200E04D90C924";           // your network key
const int buttonPin = 7;     // the number of the pushbutton pin
int buttonState = 0;
bool alreadyOpen = false;
DHT dht(DHTPIN, DHTTYPE);
WiFiSSLClient client;
TelegramBot bot (BotToken, BotName, BotUsername, client);
TelegramKeyboard keyboard_one;
int chat_id_1 = 138780798;
//int chat_id_2 = 175593403;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(3000);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(brightnessPin, INPUT);

  lcd.begin(16, 2);
  //lcd.print("@DomusBot");


  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");

  const char* row_one[] = {"Temp", "Humidity"};
  keyboard_one.addRow(row_one, 2);

  bot.begin();
  dht.begin();

}

void loop() {


  analogWrite(brightnessPin, brightnessValue);
  lcd.setCursor(0, 1);
  buttonState = digitalRead(buttonPin);


  // put your main code here, to run repeatedly:


  message m = bot.getUpdates(); // Read new messages
  if ( m.chat_id != 0 && m.text.equals("Temp")) { // Checks if there are some updates
    String testo = String(getTemp());
    Serial.println(testo);
    Serial.println(m.chat_id);
    bot.sendMessage(m.chat_id, testo + " °C", keyboard_one, false, true); // Reply to the same chat with the same text
  }
  else if ( m.chat_id != 0 && m.text.equals("Humidity")) { // Checks if there are some updates
    String testo = String(getHumidity());
    Serial.println(testo);
    bot.sendMessage(m.chat_id, testo + " %", keyboard_one, false, true);
  }
  else {
    Serial.println("funziona!");
  }

  if (buttonState == HIGH && alreadyOpen == false) {
    Serial.println("Sending alarm message");
    bot.sendMessage(chat_id_1, "Finestra aperta!!");
    //bot.sendMessage(chat_id_2, "Finestra aperta!!"); 
    alreadyOpen=true;
  }
  else if( buttonState == LOW){
    alreadyOpen=false;
  }
 
  
  String te = String(getTemp());
  String hu = String(getHumidity());

  lcd.setCursor(0, 0);
  lcd.print(" T = " + te + " C ");
  lcd.setCursor(0, 1);
  lcd.print(" H = " + hu + " % ");

}


float getTemp() {
  float t = dht.readTemperature();
  if ( isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");

  }
  return t;
}


float getHumidity() {
  float h = dht.readHumidity();
  if ( isnan(h) ) {
    Serial.println("Failed to read from DHT sensor!");

  }
  return h;
}

