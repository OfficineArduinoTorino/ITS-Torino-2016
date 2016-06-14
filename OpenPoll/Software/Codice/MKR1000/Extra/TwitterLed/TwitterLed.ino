#include <Base64.h>
#include <TwitterApplicationOnly.h>
#include <ArduinoJson.h>

// Wifi needed parameters
const String ssid = "";
const String pass = "";
const int keyIndex = 0; // Your network key Index number (needed only for WEP)

// Used to make Oauth requests
const String OauthConsumerKey = ""; // Found in your application details
const String OauthSecretKey = ""; // Found in your application details
const String accessToken = ""; // Generate it in your application
const String accessTokenSecret = ""; // Generate it in your application

// Timers
const unsigned long queriesDelay = 10000; // 10 seconds in millis
const unsigned long tokenDelay = 300000; // 5 minutes in millis
unsigned long timer = 0; // Used for token refresh

// Tweets data
const String twitterQuery = "%40FablabTorino%20OR%20%40TOmakerFaire%20OR%20%40TeamOpenPoll%20OR%20%23TeamOpenPoll";
String oldTweetID = ""; // Used to check if a tweet is new
bool queryFlag = false; // Used to check if a blink is needed
tweet t; // Variable for tweet data

// Pins
const int outputLed = 5;

void setup() {

  Serial.begin(9600); // Serial for serial monitor
  while (!Serial) { // Waiting for serial monitor to be opened
    ;
  }

  pinMode(outputLed, OUTPUT);
  Serial.println("BEGIN!");

  RequestTokens();  // Twitter needed token  
  
}

void loop() {
  
  checkTwitterQuery(); // Check last tweet
  manageLastTweet(); // Consume the tweet with a blink if it's new

  delay(queriesDelay);

  if (millis()-timer > tokenDelay || millis() < timer) { // Timer for token refresh
    RequestTokens();
    timer = millis();
  }
  
}

void RequestTokens() {
  
  if (!TwitterApplicationOnly.begin(ssid, pass, "api.twitter.com")) {
    Serial.println("Twitter API connection problem");
    while (true); // Cannot continue
  }

  TwitterApplicationOnly.setCredentials(OauthConsumerKey, OauthSecretKey, accessToken, accessTokenSecret);

  Serial.println("-------\n");
  Serial.println("Requesting access token...");
  String bearer = TwitterApplicationOnly.getBearer();
  Serial.print("Your bearer token is: ");
  Serial.println(bearer);
  Serial.println("\n-------\n");
  Serial.flush();
  
}

void checkTwitterQuery() {
  
  TwitterApplicationOnly.getCustomTimeline(1, twitterQuery); // Get the last 1 tweet with custom query
  TwitterApplicationOnly.getTweet(&t); // Stores retrevied data in tweet variable

  if (!t.tweet_id.equals("")) { // Extracted a valid tweet
    Serial.print(t.date);
    Serial.print(" - ");
    Serial.print(t.tweet_id);
    Serial.print(" - ");
    Serial.println(t.text);
    Serial.println();
    Serial.flush();
    
    if (!t.tweet_id.equals(oldTweetID)) { // New tweet
      queryFlag = true;
      oldTweetID = t.tweet_id;
    }
  }
    
}

void manageLastTweet() {
  
  if (queryFlag) { // New tweet
    Serial.println("BLINK!\n");
    Serial.flush();
    customBlink();
    queryFlag = false; // New tweet was consumed
  }
  
}

void customBlink() {

  for (int i = 0; i < 6; i++) {
    digitalWrite(outputLed, HIGH);
    delay(500);
    digitalWrite(outputLed, LOW);
    delay(500);
  }
  
}

