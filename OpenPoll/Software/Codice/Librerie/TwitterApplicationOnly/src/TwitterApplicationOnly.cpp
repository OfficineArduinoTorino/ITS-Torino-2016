/*
Copyright (C) 2016 Arturo Guadalupi. All right reserved.

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

---

Custom version modified by Francesco Brandozzi for OpenPoll Project at Officine Arduino Turin.
*/

#include <TwitterApplicationOnly.h>

bool TwitterApplicationOnlyClass::begin(String networkName, String password, String server) {
  SSID = networkName;
  pwd = password;

  char ssid[networkName.length()];
  sprintf(ssid, "%s", networkName.c_str());

  char pass[password.length()];
  sprintf(pass, "%s", password.c_str());

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    return false;
  }

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000); // VALUTARE SE DIMINUIRE
  }

  printWifiStatus();
  Serial.println();

  APIserver = server;

  return true;
}

void TwitterApplicationOnlyClass::setCredentials(String oauthConsumerKey, String oauthSecretKey, String accessToken, String accessTokenSecret) {
  OauthConsumerKey = oauthConsumerKey;
  OauthSecretKey = oauthSecretKey;
  AccessToken = accessToken;
  AccessTokenSecret = accessTokenSecret;
}

void TwitterApplicationOnlyClass::printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void TwitterApplicationOnlyClass::generateBase64bearerToken() {
  String tobeEncodedString = OauthConsumerKey + ":" + OauthSecretKey;

  int tobeEncodedLength = tobeEncodedString.length();
  int encodedLen = Base64.encodedLength(tobeEncodedLength);
  char encodedString[encodedLen];
  char tobeEncodedCstring[tobeEncodedString.length()];
  sprintf(tobeEncodedCstring, "%s", tobeEncodedString.c_str());
  Base64.encode(encodedString, tobeEncodedCstring, tobeEncodedLength);

  BearerToken = String(encodedString);
}

String TwitterApplicationOnlyClass::getBearer() {

  generateBase64bearerToken();

  char server[APIserver.length()];
  sprintf(server, "%s", APIserver.c_str());

  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();
  // if there's a successful connection:
  if (client.connectSSL(server, 443)) // 443 standard port for https requests
  {
    String authString = "Authorization: Basic ";
    authString += BearerToken;

    // send the HTTP POST request:
    client.println("POST /oauth2/token HTTP/1.1");
    client.println("Host: api.twitter.com");
    client.println(authString);
    client.println("Content-Type: application/x-www-form-urlencoded;charset=UTF-8");
    client.println("Content-Length: 29");
    client.println();
    client.println("grant_type=client_credentials");

    extractBearer();
  }
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
	Serial.flush();
  }

  return BearerToken;
}

void TwitterApplicationOnlyClass::extractBearer() {
  int startIndex, stopIndex;
  bool start;
  String accessTokenString;

  unsigned long startTimeout = millis();
  while(millis() - startTimeout < 5000){
    while (client.available()){
      char c = client.read();

	  if (c == '{') //useful data starts from '{'
        start = true;

        if (start && isPrintable(c)) // if useful data start and significant data is received
          clientBufferString += c;   // add it to the buffer
    }
  }

  startIndex = clientBufferString.indexOf("\"token_type\":\"bearer\",\"access_token\":\"");
  stopIndex = clientBufferString.lastIndexOf("\"");

  BearerToken = clientBufferString.substring(startIndex + strlen("\"token_type\":\"bearer\",\"access_token\":\""), stopIndex);
  clientBufferString = ""; // clear the buffer;
}

void TwitterApplicationOnlyClass::getCustomTimeline(int numberOfTweets, String query) {
	String authString = "Authorization: Bearer ";
	authString += BearerToken;

  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  char server[APIserver.length()];
  sprintf(server, "%s", APIserver.c_str());

  bool success = false;
  while (!success) {
    // if there's a successful connection:
    if (client.connectSSL(server, 443)){ // 443 standard port for https requests
	  success = true;
	  client.print("GET /1.1/search/tweets.json?q=");
	  client.print(query);
	  client.print("&count=");
	  client.print(numberOfTweets);
	  client.print("&result_type=recent");
	  client.println(" HTTP/1.1");
	  client.print("Host: ");
      client.println(server);
	  client.println("User-Agent: Arduino");
	  client.println(authString);
	  client.println();

	  listen();
	  }
	else {
      // if you couldn't make a connection:
      Serial.println("connection failed");
	  Serial.flush();
	  delay(5000);
    }
  }
}

void TwitterApplicationOnlyClass::listen() {
  String payload ="";

  unsigned long startTimeout = millis();

  while(millis() - startTimeout < 1000){
    while (client.available()){
      payload = client.readStringUntil('\n');
      if (payload == "\r") {
        break;
      }
    }
  }
  
  int payloadLastPosition  = 0;
  for (int i = 0; i < 4; i++) {
    payloadLastPosition =  payload.indexOf(',', payloadLastPosition + 1);
  }
  
  payload= payload.substring(0, payloadLastPosition);
  if (payload[payloadLastPosition-1] != '"') {
    payload += '"';
  }
  payload += "}]}";
  
  if (payload != "") {
    StaticJsonBuffer<JSON_BUFF_SIZE> jsonBuffer;
    JsonObject & root = jsonBuffer.parseObject(payload);

    String tweet_id = root["statuses"][0]["id_str"];
    String text = root["statuses"][0]["text"];
    String date = root["statuses"][0]["created_at"];
    t.tweet_id = tweet_id;
    t.text = text;
    t.date = date;

    if (!root.success()) {
      Serial.println("parseObject() failed\n");
	  Serial.flush();
      return;
    }
  }
}

void TwitterApplicationOnlyClass::getTweet(tweet *myTweet){
  myTweet->tweet_id= t.tweet_id;
  myTweet->text=t.text;
  myTweet->date=t.date;
}

TwitterApplicationOnlyClass TwitterApplicationOnly;
