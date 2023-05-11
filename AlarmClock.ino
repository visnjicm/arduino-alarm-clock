//INITIALIZATIONS//
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
//INITIALIZATIONS//



//CUSTOM LCD CHAR VARIABLES//
byte AlarmChar[] = {
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B00000,
  B00100,
  B00000
};
//CUSTOM LCD CHAR VARIABLES//




//ALARM DEFINITION & ALARM ARRAY VARIABLES//
typedef struct alarm
{
  int hours;
  int minutes;
  
} Alarm;

Alarm arr_Alarm[ARR_ALARM_SIZE] = {};
int arr_Button0State[ARR_ALARM_SIZE] = {0,0};
int index_AlarmTriggered;
//ALARM DEFINITION & ALARM ARRAY VARIABLES//




//DEVICE STATE VARIABLES//
int prev_deviceState = NULL;
int current_deviceState = PRINT_CLOCK; 
//DEVICE STATE VARIABLES//


//GREP CURRENT COMPUTER TIME VARIABLES//
char startDate[ARR_SIZE] = __DATE__;
char startTime[ARR_SIZE] = __TIME__;
char myDate[3][ARR_SIZE];
char myTime[3][ARR_SIZE];
char prev_char;

String hoursString = "";
String minutesString = "";
String secondsString = "";
//GREP CURRENT COMPUTER TIME VARIABLES//



//CLOCK SUBROUTINE VARIABLES//
int ClockHours = hoursString.toInt();
int ClockMinutes = minutesString.toInt();
int ClockSeconds = secondsString.toInt();
int ClockCount;
int prev_ClockCount;
//CLOCK SUBROUTINE VARIABLES//


//SET CLOCK SUBROUTINE VARIABLES//
int SetClockHours;
int SetClockMinutes;
int SetClockSeconds;
//SET CLOCK SUBROUTINE VARIABLES//



//ADD ALARM RELATED VARIABLES//
int AddAlarmHours;
int AddAlarmMinutes;
int AddAlarmSeconds;
int AlarmSet = 0;
int AlarmTriggered = 0;
int AlarmTriggeredHours;
int AlarmTriggeredMinutes;
int TurnAlarmOff = 0;
//ADD ALARM RELATED VARIABLES//


//BUTTON VARIABLES//
int Button0State = 1;
int prev_Button0State = 1;
int Button1State = 1;
int Button2State = 1;
int Button3State = 1;
int prev_Button3State = 1;
//BUTTON VARIABLES//



//PINS VARIABLES//
const int Button3Pin = 9;
const int Button1Pin = 6;
const int Button2Pin = 8;
const int MotorPin = 10;
const int Button0Pin = 13;
//PINS VARIABLES//


//LCD STORE WHAT TO PRINT VARIABLES//
String ROW0 = "";
String ROW1 = "";
//LCD STORE WHAT TO PRINT VARIABLES//

//CUSTOM DELAY VARIABLES
int SavedMillis = -1;
int CurrentMillis = millis()/1000;
//CUSTOM DELAY VARIABLES

void setup() {
  
  lcd.begin(16, 2);
  lcd.createChar(0, AlarmChar);
  
  Serial.begin(9600);
  
  pinMode(Button3Pin, INPUT);
  pinMode(Button1Pin, INPUT);
  pinMode(Button2Pin, INPUT);
  pinMode(Button0Pin, INPUT);
  pinMode(MotorPin, OUTPUT);

  parse_startDate();
  parse_startTime();
  hoursString.concat(myTime[0][0]);
  hoursString.concat(myTime[0][1]);
  minutesString.concat(myTime[1][0]);
  minutesString.concat(myTime[1][1]);
  secondsString.concat(myTime[2][0]);
  secondsString.concat(myTime[2][1]);
  ClockHours = hoursString.toInt();
  ClockMinutes = minutesString.toInt();
  ClockSeconds = secondsString.toInt();

  Button0State = digitalRead(Button0Pin);
  prev_Button0State = Button0State;

  Button3State = digitalRead(Button3Pin);
  prev_Button3State = Button3State;


  Alarm alarm;
  alarm.hours = -1;
  alarm.minutes = -1;
  arr_Alarm[0] = alarm;
  arr_Alarm[1] = alarm;

}










void loop() 
{

  Clock();

  if (AlarmTriggered == 0 || ((TurnAlarmOff) == 1))
  {
  Button3State = digitalRead(Button3Pin);
  if ((Button3State == LOW) && (Button3State != prev_Button3State) && current_deviceState != ALARM_TRIGGERED)
  {
    current_deviceState = (current_deviceState + 1)%4;
    lcd.clear();
  }
  prev_Button3State = Button3State;
  
  switch (current_deviceState)
  {
    case PRINT_CLOCK:
      SetClockHours = ClockHours;
      SetClockMinutes = ClockMinutes;
      lcd.setCursor(0,0);
      ROW0 = String(ClockHours) + "h" + String(ClockMinutes) + "m" + String(ClockSeconds) + "s    ";
      lcd.print(ROW0);
      lcd.setCursor(0,1);
      ROW1 = "CLOCK";
      lcd.print(ROW1);
      break;

    case SET_CLOCKTIME:
      SetClockTime();
      lcd.setCursor(0,0);
      ROW0 = String(SetClockHours) + "h" + String(SetClockMinutes) + "m" + String(SetClockSeconds) + "s    ";
      lcd.print(ROW0);
      break;

    case ADD_ALARMS:
      AddAlarms();
      lcd.setCursor(0,0);
      ROW0 = String(AddAlarmHours) + "h" + String(AddAlarmMinutes) + "m" + String(AddAlarmSeconds) + "s  ";
      lcd.print(ROW0);
      lcd.setCursor(0,1);
      ROW1 = "ADD ALARMS";
      lcd.print(ROW1);
      break;

    case DELETE_ALARMS:
      DeleteAlarms();
      break;
  }
  }
  else
  {
    lcd.setCursor(0,0);
    ROW0 = "ALARM TRIGGERED!!!";
    lcd.print(ROW0);
    lcd.setCursor(0,1);
    ROW1 = "PRESS BUTTON0";
    lcd.print(ROW1);
    
    Button0State = digitalRead(Button0Pin);
    if ((Button0State == LOW) && (Button0State != prev_Button0State))
    {
      lcd.clear();
      TurnAlarmOff = 1;
    }
    prev_Button0State = Button0State;

  }

  if ((TurnAlarmOff == 1) && ((ClockMinutes != AlarmTriggeredMinutes) || (ClockHours != AlarmTriggeredHours)))
  {
    TurnAlarmOff = 0;
  }    

  if (TurnAlarmOff == 0)
  {
  for (int i = 0; i < ARR_ALARM_SIZE; i++)
  {
    if ((ClockHours == arr_Alarm[i].hours) && (ClockMinutes == arr_Alarm[i].minutes))
    {
      AlarmTriggered = 1;
      AlarmTriggeredHours = arr_Alarm[i].hours;
      AlarmTriggeredMinutes = arr_Alarm[i].minutes;
      break;
    }
    else
    {
      AlarmTriggered = 0;
    }
  }
  }
  
}



///////////////////////////////////////////////////////////////////////////
//  FUNCTIONS                                                            //
///////////////////////////////////////////////////////////////////////////


/////// CLOCK SUBROUTINE ///////
void Clock()
{
  ClockCount = millis()/1000;
  if (ClockCount != prev_ClockCount)
  {
    ClockSeconds = ClockSeconds + 1;
  }
  prev_ClockCount = ClockCount;

  if (ClockSeconds == 60)
  {
    ClockSeconds = 0;
    ClockMinutes = ClockMinutes + 1;
  }

  if (ClockMinutes == 60)
  {
    ClockMinutes = 0;
    ClockHours = ClockHours + 1;
  }

  if (ClockHours == 24)
  {
    ClockHours = 0;
  }
}
///////////////////////////////////







/////// SET CLOCK SUBROUTINE ///////
void SetClockTime()
{ 
  CurrentMillis = millis()/1000;
  
  Button1State = digitalRead(Button1Pin);
  Button2State = digitalRead(Button2Pin);
  
  if (Button1State == LOW)
    {
      SetClockHours++;
      delay(100);  
    }

    if (SetClockHours == 24)
    {
      SetClockHours = 0;
    }

    if (Button2State == LOW)
    {
      SetClockMinutes++;
      delay(100);
    }

    if (SetClockMinutes == 60)
    {
      SetClockMinutes = 0;
    }

    SetClockSeconds = 0;

    lcd.setCursor(0,1);
    ROW1 = "SET CLOCK";
    lcd.print(ROW1);
   
    Button0State = digitalRead(Button0Pin);
    if (Button0State == LOW && prev_Button0State != Button0State)
    {
      SavedMillis = (millis()/1000) + 2;
      ClockHours = SetClockHours;
      ClockMinutes = SetClockMinutes;
      ClockSeconds = 0;
    }
    prev_Button0State = Button0State;

    Serial.println(CurrentMillis);

    if ((SavedMillis - CurrentMillis) > 0)
    {
      lcd.setCursor(0,1);
      ROW1 = "Time Set!";
      lcd.print(ROW1);
    }
    else
    {
      SavedMillis = (millis()/1000);
    }
}
////////////////////////////////////




/////// ADD ALARMS SUBROUTINE ///////
void AddAlarms()
{

  Button1State = digitalRead(Button1Pin);
  Button2State = digitalRead(Button2Pin);
  
  if (Button1State == LOW)
  {
    AddAlarmHours++;
    delay(100);  
  }

  if (AddAlarmHours == 24)
  {
    AddAlarmHours = 0;
  }

  if (Button2State == LOW)
  {
    AddAlarmMinutes++;
    delay(100);
  }

  if (AddAlarmMinutes == 60)
  {
    AddAlarmMinutes = 0;
  }

  AddAlarmSeconds = 0;

  Button0State = digitalRead(Button0Pin);
  if ((Button0State == LOW) && (Button0State != prev_Button0State))
  { 
    Alarm alarm = {AddAlarmHours, AddAlarmMinutes};
    for (int i = 0; i<ARR_ALARM_SIZE; i++)
    {
      if ((arr_Alarm[i].hours == -1) && (AlarmArrayContains(arr_Alarm, alarm) == 0))
      {
        arr_Alarm[i].hours = alarm.hours;
        arr_Alarm[i].minutes = alarm.minutes;
        break;
      }
    }
  }
  prev_Button0State = Button0State;

  for (int i = 0; i<ARR_ALARM_SIZE; i++)
  {
    if (arr_Alarm[i].hours != -1)
    {
      lcd.setCursor(i+12,0);
      printlcd_specialChar(0);
    }
  }
  
}
////////////////////////////////////////



/////// DELETE ALARMS SUBROUTINE ///////
void DeleteAlarms()
{
  Button1State = digitalRead(Button1Pin);
  Button2State = digitalRead(Button2Pin);

  
  if (arr_Alarm[0].hours != -1)
  {
    lcd.setCursor(0,0);
    printlcd_specialChar(0);
    lcd.setCursor(1,0);
    ROW0 = String(1) + ":" + String(arr_Alarm[0].hours) + "h" + String(arr_Alarm[0].minutes) + "m" + "     ";
    lcd.print(ROW0);
  }

  if (arr_Alarm[1].hours != -1)
  {
    lcd.setCursor(0,1);
    printlcd_specialChar(0);
    lcd.setCursor(1,1);
    ROW1 = String(2) + ":" + String(arr_Alarm[1].hours) + "h" + String(arr_Alarm[1].minutes) + "m" + "     ";
    lcd.print(ROW1);
  }


  if ((arr_Alarm[0].hours == -1) && (arr_Alarm[1].hours == -1))
  {
    lcd.setCursor(0,0);
    ROW0 = "NO ALARMS SET";
    lcd.print(ROW0);
    lcd.setCursor(0,1);
    ROW1 = "                            ";
    lcd.print(ROW1);
  }

  
  if (digitalRead(Button1Pin) == LOW)
  {
    lcd.clear();
    arr_Alarm[0].hours = -1;
    arr_Alarm[0].minutes = -1;
  }
  if (digitalRead(Button2Pin) == LOW)
  {
    lcd.clear();
    arr_Alarm[1].hours = -1;
    arr_Alarm[1].minutes = -1;
  }

}
////////////////////////////////////////

/*
void alarmTriggered()
{
  Button0State = digitalRead(Button0Pin);
  if ((Button0State == LOW) && (Button0State != prev_Button0State))
  {
    
  }
  prev_Button0State = Button0State;

  lcd.setCursor(0,0);
  lcd.print("ALARM TRIGGERED!");
  lcd.setCursor(0,1);
  lcd.print("PRESS ALARM BUTT");
  
}
*/


//PRINT FUNCTIONS


void printlcd_specialChar(int index)
{
  lcd.write((uint8_t) index);
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

int AlarmArrayContains(Alarm arr_alarm[], Alarm alarm)
{
  for (int i = 0; i < ARR_ALARM_SIZE; i++)
  {
    if ((arr_alarm[i].hours == alarm.hours) && (arr_alarm[i].minutes == alarm.minutes))
    {
      return 1;
    }
  }
  return 0;
}
