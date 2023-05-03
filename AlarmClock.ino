#include <TimeLib.h>
#include <LiquidCrystal.h>

#define ARR_SIZE 15
#define LOOP_SIZE 20


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


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
int addSeconds;
int prev_addSeconds;



void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);

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
  Serial.println("Done");
}



void loop() {

  addSeconds = millis()/1000;
  if (addSeconds != prev_addSeconds){
    seconds ++;
  }
  prev_addSeconds = addSeconds;


  if (seconds == 60){
    seconds = 0;
    minutes ++;
  }

  if (minutes == 60){
    minutes = 0;
    hours++;
  }

  if (hours == 24){
    hours = 0;
  }


  lcd.setCursor(0, 0);
  lcd.print(myDate[0]);
  lcd.print(" ");
  lcd.print(myDate[1]);
  lcd.print(" ");
  lcd.print(myDate[2]);
  lcd.print(" ");
  lcd.setCursor(0, 1);
  lcd.print(myTime[0]);
  lcd.print("h");
  lcd.print(String(minutes));
  lcd.print("m");
  lcd.print(String(seconds));
  lcd.print("s ");

}





//User Functions
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
