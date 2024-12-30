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