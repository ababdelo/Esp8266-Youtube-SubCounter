/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Arduino-Youtube-SubCounter.ino                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ababdelo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 10:12:42 by ababdelo          #+#    #+#             */
/*   Updated: 2023/07/12 17:45:51 by ababdelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address may vary, adjust accordingly

const int buzzerPin = 2; // Pin for the buzzer
String user = "";
String subscribers = "";
String views = "";
long subs = 0;
long viewsCount = 0;
long prevSubs = 0;
long prevViews = 0;
bool fetchingData = true;
unsigned long lastDataTime = 0;
const unsigned long dataTimeout = 1000; // Timeout in milliseconds

void displayFetchingData(void);
void displayhome();
void displayData();
void triggerAlarm();

byte prt1[8] = {
  B00000,
  B00111,
  B01110,
  B01110,
  B01110,
  B01110,
  B00111,
  B00000
};

byte prt2[8] = {
  B00000,
  B11100,
  B01110,
  B00110,
  B00110,
  B01110,
  B11100,
  B00000
};

byte t1[8] = {
  B00000,
  B11111,
  B11111,
  B01110,
  B00100,
  B01010,
  B10001,
  B11111
};

byte t2[8] = {
  B00000,
  B11111,
  B11011,
  B01110,
  B00100,
  B01010,
  B10101,
  B11111
};

byte t3[8] = {
  B00000,
  B11111,
  B10011,
  B01110,
  B00100,
  B01010,
  B11101,
  B11111
};

byte t4[8] = {
  B00000,
  B11111,
  B10001,
  B01110,
  B00100,
  B01010,
  B11111,
  B11111
};

byte t5[8] = {
  B00000,
  B11111,
  B10001,
  B01010,
  B00100,
  B01110,
  B11111,
  B11111
};

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.createChar(0, prt1);
  lcd.createChar(1, prt2);
  lcd.createChar(2, t1);
  lcd.createChar(3, t2);
  lcd.createChar(4, t3);
  lcd.createChar(5, t4);
  lcd.createChar(6, t5);
  pinMode(buzzerPin, OUTPUT);
  displayFetchingData();
  fetchingData = true; // Reset fetchingData flag when serial data is available
}
void loop() {
  if (Serial.available()) {
    fetchingData = false; // Reset fetchingData flag when serial data is available
    String data = Serial.readStringUntil('\n');
    int sep1 = data.indexOf(',');
    int sep2 = data.indexOf(',', sep1 + 1);

    if (sep1 != -1 && sep2 != -1) {
      subscribers = data.substring(0, sep1);
      views = data.substring(sep1 + 1, sep2);
      user = data.substring(sep2 + 1);
      subs = subscribers.toInt();
      viewsCount = views.toInt();
      displayhome();
      if (subs > prevSubs || viewsCount > prevViews) {
        triggerAlarm();
      }
      displayData();
      prevSubs = subs;
      prevViews = viewsCount;
      lastDataTime = millis(); // Update the last data time

    }
  }
  else {
    // Check for data timeout
    if (!fetchingData && millis() - lastDataTime > dataTimeout) {
      fetchingData = true;
      displayFetchingData();
    }
    else {
      if (fetchingData) {
        displayFetchingData();
      }
    }
  }
}


void displayFetchingData(void) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Fetching Data ");
  Serial.println(" Fetching Data ... ");
  lcd.setCursor(7, 1);
  lcd.write(byte(2));
  delay(200);
  lcd.setCursor(7, 1);
  lcd.write(byte(3));
  delay(200);
  lcd.setCursor(7, 1);
  lcd.write(byte(4));
  delay(200);
  lcd.setCursor(7, 1);
  lcd.write(byte(5));
  delay(200);
  lcd.setCursor(7, 1);
  lcd.write(byte(6));
  delay(200);
  lcd.setCursor(7, 1);
  lcd.write(byte(2));
  delay(200);
  lcd.setCursor(7, 1);
  lcd.write(byte(3));
  delay(200);
  lcd.setCursor(7, 1);
  lcd.write(byte(4));
  delay(200);
  lcd.setCursor(7, 1);
  lcd.write(byte(5));
  delay(200);
  lcd.setCursor(7, 1);
  lcd.write(byte(6));
  delay(200);
}

void displayhome()
{
  fetchingData = false;
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.write(byte(0));
  lcd.setCursor(2, 0);
  lcd.write(byte(1));
  lcd.setCursor(((16 - user.length()) / 2), 1);
  lcd.print(user);
  lcd.setCursor(4, 0);
  lcd.print("  YOUTUBE :");
  delay(2500);
}

void displayData() {
  fetchingData = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SUBS: ");
  lcd.print(subscribers);
  Serial.print("SUBS: ");
  Serial.print(subscribers);
  lcd.setCursor(0, 1);
  lcd.print("VIEWS: ");
  lcd.print(views);
  Serial.print("\tVIEWS: ");
  Serial.println(views);
  delay(5000);
}

void triggerAlarm() {
  tone(buzzerPin, 250); // Turn on the buzzer for a short duration
  delay(250);
  noTone(buzzerPin);
}
