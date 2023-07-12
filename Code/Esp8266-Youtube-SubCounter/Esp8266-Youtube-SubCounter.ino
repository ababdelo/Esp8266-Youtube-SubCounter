/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Esp8266-Youtube-SubCounter.ino                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ababdelo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 10:13:26 by ababdelo          #+#    #+#             */
/*   Updated: 2023/07/12 17:45:45 by ababdelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "JsonStreamingParser.h"
#include <ArduinoJson.h>
#include <YoutubeApi.h>

const char* ssid     = "XXXXXXXXXXX";      // SSID of local network
const char* password = "XXXXXXXXXXXXXXXXXX";                // Password on network
String USER = "XXXXXXXX";

#define API_KEY "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" //Your Youtube API-Key 
#define CHANNEL_ID "XXXXXXXXXXXXXXXXXXXXXXXXXX" // Your youtube channel ID. 

String nbrformat(long num);

WiFiClientSecure client; //For ESP8266 boards

YoutubeApi api(API_KEY, client);

void setup() {
  // Connecting to wifi
  Serial.begin(115200);
  Serial.println("Connecting to WiFi ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("."); delay(500);
  }
  Serial.println("");
  Serial.print("Connected !");
  Serial.println(WiFi.localIP());
  client.setInsecure();
}

void loop() {
  long subs, views;
  int cnt = 0;
  if (api.getChannelStatistics(CHANNEL_ID)) {
    subs = api.channelStats.subscriberCount;
    views = api.channelStats.viewCount;

    String subsCount = nbrformat(subs);
    String viewCount = nbrformat(views);

    Serial.print(subsCount);
    Serial.print(",");
    Serial.print(viewCount);
    Serial.print(",");
    Serial.print((String)USER);
    Serial.print("\n");
    Serial.flush();
    delay(1000);  // Wait for data to be transmitted completely
  }
  delay(3000);  // Wait for 2.5 seconds before retrying
}

String nbrformat(long num) {
  String num_s;
  long num_original = num;
  if (num > 99999 && num <= 999999) {
    num = num / 1000;
    long fraction = num_original % 1000;
    String num_fraction = String(fraction);
    String decimal = num_fraction.substring(0, 1);
    num_s = String(num) + "." + decimal + "K";
  }
  else if (num > 999999) {
    num = num / 1000000;
    long fraction = num_original % 1000000;
    String num_fraction = String(fraction);
    String decimal = num_fraction.substring(0, 1);
    if (num_original < 100000000) {
      num_s = " " + String(num) + "." + decimal + "M";
    }
    else {
      num_s = String(num) + "." + decimal + "M";
    }
  }
  else {
    int num_l = String(num).length();
    char num_f[15];
    int blankDigits = 6 - num_l;
    for (int i = 0; i < blankDigits; i++) {
      num_f[i] = ' ';
    }
    num_f[blankDigits] = '\0';
    num_s = num_f + String(num);
  }
  return num_s;
}
