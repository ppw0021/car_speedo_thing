# GPS Speed Monitor
This was a small project completed in a day at the request of one of my friends. His car does not have a working speedometer, and he wanted a means to see how fast he was going.

## Description
This project uses an Arduino Uno, an I2C 16x2 Display, and a ublox NEO 7m GPS module. The GPS module has been programmed by the software ublox to update its position twice a second, the arduino then communicates with the module to determine the calculated speed and displays it on the screen.

## Reprogramming of the ublox module
The GPS module update speed was too slow, to fix this, simply plug in the module via a USB cable, and change the update speed parameters onboard the device using U-Center.
![ublox](https://github.com/user-attachments/assets/10e85a76-9d1d-4b7a-87a7-b9ef753c948a)
This tool also shows the different satellites overhead and more information about the module.

## Diagram
Connect the wires as shown below.
![diagram](https://github.com/user-attachments/assets/219eb633-9fb5-42a4-8053-a1cde01bbf8c)

## Code
This is the code I created. It was written in VSC using the PlatformIO extension.
```cpp
#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <LiquidCrystal_I2C.h>

TinyGPS gps;
SoftwareSerial ss(9, 8);

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);

  lcd.init();
  lcd.backlight();
  // Setup GPS
  ss.begin(9600);
}

void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 200;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c);    // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    double speed = gps.f_speed_kmph();
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    Serial.println();
    lcd.setCursor(0,0);
    lcd.print("Speed: ");
    int rounded_speed = round(speed);
    if (rounded_speed < 2)
    {
      rounded_speed = 0;
    }
    lcd.print(rounded_speed);
    lcd.print(" KPH");
  }

  gps.stats(&chars, &sentences, &failed);
  /*Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");*/

  // display.clearDisplay();
  // display.setCursor(0, 0);
  // display.println(F("sentences: "));
  // display.println(sentences);
  // display.display();
}
```
