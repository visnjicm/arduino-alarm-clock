#include <TimeLib.h>
#include <LiquidCrystal.h>

#define ARR_SIZE 15
#define LOOP_SIZE 20
#define ARR_ALARM_SIZE 2

#define PRINT_CLOCK 0
#define SET_CLOCKTIME 1
#define ADD_ALARMS 2
#define DELETE_ALARMS 3
#define ALARM_TRIGGERED 4



const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte alarmChar[] = {
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B00000,
  B00100,
  B00000
};

typedef struct alarm
{
  int hours;
  int minutes;
  
} Alarm;


Alarm arr_alarm[ARR_ALARM_SIZE] = {};
int arr_alarmState[ARR_ALARM_SIZE] = {0,0};
int index_alarmTriggered;


int prev_deviceState = NULL;
int current_deviceState = PRINT_CLOCK; 

char startDate[ARR_SIZE] = __DATE__;
char startTime[ARR_SIZE] = __TIME__;
char myDate[3][ARR_SIZE];
char myTime[3][ARR_SIZE];
char prev_char;

String hoursString = "";
String minutesString = "";
String secondsString = "";

int hours = hoursString.toInt();
int minutes = minutesString.toInt();
int seconds = secondsString.toInt();
int alarmHours;
int alarmMinutes;
int addSeconds;
int prev_addSeconds;
int toggleState = 0;
int prev_toggleState = 0;
int hoursState = 0;
int minutesState = 0;
int alarmState = 0;
int prev_alarmState = 0;
int alarmSet = 0;
int alarmTriggered = 0;

const int togglePin = 9;
const int hoursPin = 6;
const int minutesPin = 8;
const int motorPin = 10;
const int alarmPin = 13;







void setup() {
  
  lcd.begin(16, 2);
  lcd.createChar(0, alarmChar);
  
  Serial.begin(9600);
  
  pinMode(togglePin, INPUT);
  pinMode(hoursPin, INPUT);
  pinMode(minutesPin, INPUT);
  pinMode(alarmPin, INPUT);
  pinMode(motorPin, OUTPUT);

  parse_startDate();
  parse_startTime();
  hoursString.concat(myTime[0][0]);
  hoursString.concat(myTime[0][1]);
  minutesString.concat(myTime[1][0]);
  minutesString.concat(myTime[1][1]);
  secondsString.concat(myTime[2][0]);
  secondsString.concat(myTime[2][1]);
  hours = hoursString.toInt();
  minutes = minutesString.toInt();
  seconds = secondsString.toInt();

  alarmState = digitalRead(alarmPin);
  prev_alarmState = alarmState;

  toggleState = digitalRead(togglePin);
  prev_toggleState = toggleState;


  Alarm alarm;
  alarm.hours = -1;
  alarm.minutes = -1;
  arr_alarm[0] = alarm;
  arr_alarm[1] = alarm;

}










void loop() 
{

  toggleState = digitalRead(togglePin);
  if ((toggleState == LOW) && (toggleState != prev_toggleState) && current_deviceState != ALARM_TRIGGERED)
  {
    current_deviceState = (current_deviceState + 1)%4;
    lcd.clear();
  }
  prev_toggleState = toggleState;

  Clock();
  
  switch (current_deviceState)
  {
    case PRINT_CLOCK:
      print_Clock();
      break;

    case SET_CLOCKTIME:
      print_setClockTime();
      break;

    case ADD_ALARMS:
      //print_AddAlarms();
      break;

    case DELETE_ALARMS:
      //print_DeleteAlarms();
      break;
  }
  
}


//SUBROUTINES
void Clock()
{
  addSeconds = millis()/1000;
  if (addSeconds != prev_addSeconds)
  {
    seconds = seconds + 1;
  }
  prev_addSeconds = addSeconds;

  if (seconds == 60)
  {
    seconds = 0;
    minutes = minutes + 1;
  }

  if (minutes == 60)
  {
    minutes = 0;
    hours = hours + 1;
  }

  if (hours == 24)
  {
    hours = 0;
  }
}



void setClockTime()
{ 

  hoursState = digitalRead(hoursPin);
  minutesState = digitalRead(minutesPin);
  
  if (hoursState == LOW)
    {
      hours++;
      delay(100);  
    }

    if (hours == 24)
    {
      hours = 0;
    }

    if (minutesState == LOW)
    {
      minutes++;
      delay(100);
    }

    if (minutes == 60)
    {
      minutes = 0;
    }

    seconds = 0;
}


void addAlarms()
{

  hoursState = digitalRead(hoursPin);
  minutesState = digitalRead(minutesPin);
  
  if (hoursState == LOW)
    {
      hours++;
      delay(100);  
    }

    if (hours == 24)
    {
      hours = 0;
    }

    if (minutesState == LOW)
    {
      minutes++;
      delay(100);
    }

    if (minutes == 60)
    {
      minutes = 0;
    }

    seconds = 0;

  
  alarmState = digitalRead(alarmPin);
  if ((alarmState == LOW) && (alarmState != prev_alarmState))
  {
    Alarm alarm = {hours, minutes};

    for (int i = 0; i<ARR_ALARM_SIZE; i++)
    {
      if (arr_alarm[i].hours == -1)
      {
        arr_alarm[i].hours = hours;
        arr_alarm[i].minutes = minutes;
        break;
      }
    }
  }

  for (int i = 0; i<ARR_ALARM_SIZE; i++)
  {
    if (arr_alarm[i].hours != -1)
    {
      lcd.setCursor(i+12,0);
      printlcd_specialChar(0);
    }
  }


  prev_alarmState = alarmState;
  
  
  lcd.setCursor(0, 0);
  lcd.print(String(hours));
  lcd.print("h");
  lcd.print(String(minutes));
  lcd.print("m");
  lcd.print(String(seconds));
  lcd.print("s  ");

  lcd.setCursor(0,1);
  lcd.print("Add Alarms");
}


void deleteAlarms()
{
  hoursState = digitalRead(hoursPin);
  minutesState = digitalRead(minutesPin);
  
  lcd.setCursor(0,0);

  for (int i = 0; i<ARR_ALARM_SIZE; i++)
  {
    lcd.setCursor(0,i);
    if (arr_alarm[i].hours != -1)
    {
      printlcd_specialChar(0);
      lcd.print(i);
      lcd.print(":");
      lcd.print(arr_alarm[i].hours);
      lcd.print("h");
      lcd.print(arr_alarm[i].minutes);
      lcd.print("m");
      lcd.print("   ");
    }
  }
  

  if (digitalRead(hoursPin) == LOW)
  {
    lcd.clear();
    arr_alarm[0].hours = -1;
    arr_alarm[0].minutes = -1;
  }
  if (digitalRead(minutesPin) == LOW)
  {
    lcd.clear();
    arr_alarm[1].hours = -1;
    arr_alarm[1].minutes = -1;
  }
  
  //lcd.setCursor(0,1);
  //lcd.print("Delete Alarms");
}

/*
void alarmTriggered()
{
  alarmState = digitalRead(alarmPin);
  if ((alarmState == LOW) && (alarmState != prev_alarmState))
  {
    
  }
  prev_alarmState = alarmState;

  lcd.setCursor(0,0);
  lcd.print("ALARM TRIGGERED!");
  lcd.setCursor(0,1);
  lcd.print("PRESS ALARM BUTT");
  
}
*/


//PRINT FUNCTIONS

void print_Clock()
{
  lcd.setCursor(0, 0);
  lcd.print(String(hours));
  lcd.print("h");
  lcd.print(String(minutes));
  lcd.print("m");
  lcd.print(String(seconds));
  lcd.print("s      ");

  lcd.setCursor(0,1);
  lcd.print("Clock");
}

void printlcd_specialChar(int index)
{
  lcd.write((uint8_t) index);
}

void print_setClockTime()
{
  lcd.setCursor(0, 0);
  lcd.print(String(hours));
  lcd.print("h");
  lcd.print(String(minutes));
  lcd.print("m");
  lcd.print(String(seconds));
  lcd.print("s  ");

  lcd.setCursor(0,1);
  lcd.print("Set Time");
}



//PRE PROCESSING FUNCTIONS
void parse_startTime()
{
  int i = 0;
  int count = 0;
  for (int j = 0; j < LOOP_SIZE; j++)
  {
     
    if (i==3)
    {
      break;
    }
    
    if (startTime[j] != ':' && startTime[j] != NULL) 
    {
      myTime[i][count] = startTime[j];
      count++;
    }
    
    if ((startTime[j] == ':' || startTime[j] == NULL) && (prev_char != ':' && prev_char != NULL))
    {
      i++;
      count = 0;
    }
    
    prev_char = startTime[j];
    
  }
}

void parse_startDate()
{
  int i = 0;
  int count = 0;
  for (int j = 0; j < LOOP_SIZE; j++)
  {
     
    if (i==3)
    {
      break;
    }
    
    if (startDate[j] != ' ' && startDate[j] != NULL) 
    {
      myDate[i][count] = startDate[j];
      count++;
    }
    
    if ((startDate[j] == ' ' || startDate[j] == NULL) && (prev_char != ' ' && prev_char != NULL))
    {
      i++;
      count = 0;
    }
    
    prev_char = startDate[j];
    
  }
}
