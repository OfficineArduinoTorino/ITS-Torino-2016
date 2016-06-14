#include <Base64.h>
#include <TwitterApplicationOnly.h>
#include <ArduinoJson.h>

#define N_QUERIES 6
#define N_PWM_VALUES 3
#define ENCODED_OUTPUT_SIZE 7 // In bytes
#define MAX_INPUT_ARGUMENTS 3

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
const String twitterQueries[N_QUERIES] = {
  "%40MarcoRizzo1959", // Rizzo
  "%23IoStoConOsvaldoNapoli%20OR%20%23NapoliSindacoPerTorino", // Napoli
  "%40c_appendino%20OR%20%23TorinoRiparte%20OR%20%23ChiaraAppendino", // Appendino
  "%40OnRobertoRosso", // Rosso
  "%40pierofassino%20OR%20%23PeramorediTorino%20OR%20%23LaTorinocheverr%C3%A0%20OR%20%23NoiSiamoTorino%20OR%20%23pierofassino", // Fassino
  "%40GAiraudo%20OR%20%23torinoinComune" // Airaudo
};
String oldTweetIDs[N_QUERIES]; // Used to check if a tweet is new
bool queriesFlags[N_QUERIES]; // Used to check if dataCounters values need updates
tweet t; // Variable for tweet data

// Data variables
double dataCounters[N_QUERIES] = {
  0, // Rizzo
  0, // Napoli
  0, // Appendino
  0, // Rosso
  0, // Fassino
  0  // Airaudo
};
double dataCountersSum = 0;
double percentageMilestones[N_PWM_VALUES-1] = { 7, 20 };
double percentageValues[N_QUERIES];

// PWM Values
byte PWMValues[N_QUERIES][N_PWM_VALUES] = { // Light, medium, strong - Custom calibrated
  {145, 165, 205}, // Rizzo
  {140, 195, 240}, // Napoli
  {135, 180, 215}, // Appendino
  {135, 175, 215}, // Rosso
  {130, 190, 230}, // Fassino
  {185, 235, 255}  // Airaudo
};
byte currentPWMValues[N_QUERIES]; // Depends from PWMValues and percentageMilestones
byte PWMValuesOverlays[N_QUERIES]; // Emergency values

// Encoded Output variables
byte encodedOutputArray[ENCODED_OUTPUT_SIZE];
const int minimumValidOutputValue = 1; // Values less than this are reserved for output encoding

// Serial Input
String serialInputString = "";
String inputCommandString = "";
int inputArguments[MAX_INPUT_ARGUMENTS];
int commaIndexs[MAX_INPUT_ARGUMENTS];

void setup() {

  for (int i = 0; i < N_QUERIES; i++) {
    oldTweetIDs[i] = ""; // Empty string for all old Twitter IDs
    queriesFlags[i] = false; // False value for all queries flags
    PWMValuesOverlays[i] = 0; // Neutral value
  }

  Serial1.begin(9600); // Serial for Leonardo
  Serial.begin(9600); // Serial for serial monitor
  while (!Serial) { // Waiting for serial monitor to be opened
    ;
  }

  updateStatus(); // Calculate needed variables values
  sendStatus(); // Send needed data through Serial1
  printStatus(); // Prints status variables values

  RequestTokens();  // Twitter needed token
  
}

void loop() {
  
  for (int i = 0; i < N_QUERIES; i++) { // For every query
    checkSerialInput(); // Checks and eventually manages user input
    checkTwitterQuery(i); // Check last tweet
    manageLastTweet(i); // Consume the tweet if it's new

    delay(queriesDelay);

    if (millis()-timer > tokenDelay || millis() < timer) { // Timer for token refresh
      RequestTokens();
      timer = millis();
    }
  }

  updateStatus();
  sendStatus();
  printStatus();

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

void checkTwitterQuery(int i) {
  
  TwitterApplicationOnly.getCustomTimeline(1, twitterQueries[i]); // Get the last 1 tweet with custom query
  TwitterApplicationOnly.getTweet(&t); // Stores retrevied data in tweet variable

  if (!t.tweet_id.equals("")) { // Extracted a valid tweet
    Serial.print(t.date);
    Serial.print(" - ");
    Serial.print(t.tweet_id);
    Serial.print(" - ");
    Serial.println(t.text);
    Serial.println();
    Serial.flush();
    
    if (!t.tweet_id.equals(oldTweetIDs[i])) { // New tweet
      queriesFlags[i] = true;
      oldTweetIDs[i] = t.tweet_id;
    }
  }
    
}

void checkSerialInput() {
  
  if (Serial.available()) { // User input available
    serialInputString = Serial.readString();

    commaIndexs[0] = serialInputString.indexOf(','); // First comma
    for (int i = 1; i < MAX_INPUT_ARGUMENTS; i++) {
      commaIndexs[i] = serialInputString.indexOf(',', commaIndexs[i-1]+1); // Commas after last comma
    }
    
    inputCommandString = serialInputString.substring(0, commaIndexs[0]);

    for (int i = 0; i < MAX_INPUT_ARGUMENTS-1; i++) { // Arguments parsing
      inputArguments[i] = serialInputString.substring(commaIndexs[i]+1, commaIndexs[i+1]).toInt();
    }
    inputArguments[MAX_INPUT_ARGUMENTS-1] = serialInputString.substring(commaIndexs[MAX_INPUT_ARGUMENTS-1]+1).toInt();

    Serial.print("Serial input received: ");
    Serial.print(serialInputString);
    Serial.print(" - ");
    Serial.print(inputCommandString);
    for (int i = 0; i < MAX_INPUT_ARGUMENTS; i++) {
      Serial.print(" ");
      Serial.print(inputArguments[i]);
    }
    Serial.println();
    Serial.flush();
    
    if (inputCommandString == "counter") { // Change a counter variable
      dataCounters[inputArguments[0]] = inputArguments[1];
    }
    else if (inputCommandString == "milestone") { // Change a milestone variable 
      percentageMilestones[inputArguments[0]] = inputArguments[1];
    }
    else if (inputCommandString == "pwmvalue") { // Change a pwmvalue variable
      PWMValues[inputArguments[0]][inputArguments[1]] = inputArguments[2];
    }
    else if (inputCommandString == "pwmoverlay") { // Change a pwmoverlay variable
      PWMValuesOverlays[inputArguments[0]] = inputArguments[1];
    }
    else {
      Serial.println("Serial input error\n");
      Serial.flush();
    }

    updateStatus();
    sendStatus();
    printStatus();
  }
  
}

void manageLastTweet(int i) {
  
  if (queriesFlags[i]) { // New tweet
    Serial.println("BLINK!\n");
    Serial.flush();
    dataCounters[i]++; // Update data counter
    queriesFlags[i] = false; // New tweet was consumed
  }
  
}

void printStatus() {
  
  Serial.println("Data variables: ");
  Serial.println("Percentages");
  for (int i = 0; i < N_QUERIES; i++) {
    Serial.print(percentageValues[i]);
    Serial.print(" ");
  }
  Serial.print("| ");
  for (int i = 0; i < N_PWM_VALUES-1; i++) {
    Serial.print(percentageMilestones[i]);
    Serial.print(" ");
  }
  
  Serial.println("\nCounters");
  for (int i = 0; i < N_QUERIES; i++) {
    Serial.print(dataCounters[i]);
    Serial.print(" ");
  }
  Serial.print("| ");
  Serial.print(dataCountersSum);

  Serial.println("\nOld tweet IDs");
  for (int i = 0; i < N_QUERIES; i++) {
    if (oldTweetIDs[i] != "") {
      Serial.print(oldTweetIDs[i]);
    }
    else {
      Serial.print("empty");
    }
    Serial.print(" ");
  }

  Serial.println("\nPWM values");
  for (int i = 0; i < N_QUERIES; i++) {
    Serial.print(currentPWMValues[i]);
    Serial.print(" ");
  }

  Serial.println("\nPWM values overlays");
  for (int i = 0; i < N_QUERIES; i++) {
    Serial.print(PWMValuesOverlays[i]);
    Serial.print(" ");
  }

  Serial.println("\nEncoded output");
  for (int i = 0; i < N_QUERIES; i++) {
    Serial.print(encodedOutputArray[i]);
    Serial.print(" ");
  }
  
  Serial.println("\n");
  Serial.flush();
  
}

void updateStatus() {

  dataCountersSum = 0;
  for (int i = 0; i < N_QUERIES; i++) {
    dataCountersSum+= dataCounters[i]; // Update data counter sum
  }

  for (int i = 0; i < N_QUERIES; i++) { // Calculate current percentages and PWM values
    if (dataCountersSum > 0) { 
      percentageValues[i] = (dataCounters[i] / dataCountersSum) * 100;
    }
    else {
      percentageValues[i] = 0;
    }
    
    int j = calculatePWMValuesColumn(percentageValues[i]);
    currentPWMValues[i] = PWMValues[i][j];
  }

  for (int i = 0; i < N_QUERIES; i++) {
    if (PWMValuesOverlays[i] == 0) { // If there is a neutral PWM overlay value
      encodedOutputArray[i] = currentPWMValues[i]; // Output will be calculated PWM value 
    }
    else {
      encodedOutputArray[i] = PWMValuesOverlays[i]; // Otherwise it will be user's choosed value
    }
  }
  encodedOutputArray[ENCODED_OUTPUT_SIZE-1] = minimumValidOutputValue-1; // End of the output array value
  
}

void sendStatus() {
  
  Serial1.write(encodedOutputArray, ENCODED_OUTPUT_SIZE);
  Serial1.flush();
  
}

int calculatePWMValuesColumn(double percentageValue) {
  
  int PWMValuesColumnIndex = N_PWM_VALUES-1; // Max value index
  
  for (int i = 0;
       i < N_PWM_VALUES-1 && PWMValuesColumnIndex == N_PWM_VALUES-1; // Will continue until second to last value, or until value is changed
       i++) {
    if (percentageValue < percentageMilestones[i]) { // Considered value is less than a milestone
      PWMValuesColumnIndex = i;
    }
  }

  return PWMValuesColumnIndex;
}

// TODO
// User input control
// Cloud saving of counters

