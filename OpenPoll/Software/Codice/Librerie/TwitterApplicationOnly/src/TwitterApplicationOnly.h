/*
Copyright (C) 2016 Arturo Guadalupi. All right reserved.

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

---

Custom version modified by Francesco Brandozzi for OpenPoll Project at Officine Arduino Turin.
*/

#ifndef TWITTER_APPLICATION_ONLY_H
#define TWITTER_APPLICATION_ONLY_H
#define JSON_BUFF_SIZE 10000

#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>
#include <Base64.h>
#include <ArduinoJson.h>

struct tweet{
  String text;
  String tweet_id;
  String date;
};

class TwitterApplicationOnlyClass
{
  public:
    WiFiClient client;

    int status = WL_IDLE_STATUS;
    bool begin(String networkName, String password, String server);
    void setCredentials(String oauthConsumerKey, String oauthSecretKey, String accessToken, String accessTokenSecret);
    String getBearer();
    void generateTimestamp(unsigned long epoch);

    void getCustomTimeline(int numberOfTweets, String query);

    void printWifiStatus(void);
    void getTweet(tweet *myTweet);

  private:
    String clientBufferString;
    String SSID;
    String pwd;
    String APIserver;
    String OauthConsumerKey;
    String OauthSecretKey;
    String BearerToken;
    String AccessToken;
    String AccessTokenSecret;
    String BaseString;
    tweet t;

    String StatusTwitt;

    void generateBase64bearerToken(void);
    void generateBaseString(void);

    void extractBearer(void);
    void listen(void);
};
extern TwitterApplicationOnlyClass TwitterApplicationOnly;

#endif
