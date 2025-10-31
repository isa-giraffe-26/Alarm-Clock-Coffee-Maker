#include <Wire.h>
#include <RTClib.h>
#include <TM1637Display.h>


#define CLK 8
#define DIO 9
#define BUZZER 6
#define BTN_HOUR 3
#define BTN_MIN 2
#define SWITCH 10


RTC_DS1307 rtc;
TM1637Display display(CLK, DIO);


int alarmHour = 0;
int alarmMinute = 0;
int alarmTime = alarmHour * 100 + alarmMinute;
bool alarmOn = false;


unsigned long lastHourPress = 0;
unsigned long lastMinPress = 0;
const unsigned long debounceDelay = 250;


void setup() {
  Wire.begin();
  rtc.begin();
  display.setBrightness(5);


  pinMode(BUZZER, OUTPUT);
  pinMode(BTN_HOUR, INPUT_PULLUP);
  pinMode(BTN_MIN, INPUT_PULLUP);


  Serial.begin(9600);
  Serial.println("Alarm clock starting...");
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Uncomment once if needed
  //alarmHour = 0;
  //alarmMinute = 0;
}


void loop() {
  DateTime now = rtc.now();




  int currentHour = now.hour();
  int currentMinute = now.minute();
  int timeToDisplay = currentHour * 100 + currentMinute;
  display.showNumberDecEx(timeToDisplay, 0b11100000, true); // HH:MM 

  if (currentHour == alarmHour && currentMinute == alarmMinute) {
    alarmOn = true;
  }
  if (alarmOn && digitalRead(SWITCH) == LOW) {
    digitalWrite(BUZZER, HIGH);
  }
  else if (alarmOn && digitalRead(SWITCH) == HIGH) {
    digitalWrite(BUZZER, LOW);
    alarmOn = false;
    alarmHour = (0);
    alarmMinute = (0);
  }
  else {
    digitalWrite(BUZZER, LOW);
  }

  handleButtons();
  delay(200); // Display update interval
}


void handleButtons() {
  //int alarmDisplay = alarmHour * 100 + alarmMinute;
  // Handle hour button
  if (digitalRead(BTN_HOUR) == LOW && millis() - lastHourPress > debounceDelay) {
    alarmHour = (alarmHour + 1) % 24;
    int alarmDisplay = alarmHour * 100 + alarmMinute;
    //display.showNumberDecEx(alarmHour * 100 + 100, 0b11100000, true);
    display.showNumberDecEx(alarmDisplay, 0b11100000, true);
    lastHourPress = millis();
    Serial.print("Alarm Hour Set To: ");
    Serial.println(alarmHour);
    delay(500);
  }

  if (digitalRead(BTN_MIN) == LOW && millis() - lastMinPress > debounceDelay) {
    alarmMinute = (alarmMinute + 5) % 60;
    int alarmDisplay = alarmHour * 100 + alarmMinute;
    display.showNumberDecEx(alarmDisplay, 0b11100000, true);
    lastMinPress = millis();
    Serial.print("Alarm Minute Set To: ");
    Serial.println(alarmMinute);
    delay(500);
  }
}


















