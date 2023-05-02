// Time - Version: Latest 
#include <TimeLib.h>

// include the library code:
#include <LiquidCrystal.h>

#define SIZE 15
#define LOOP_SIZE 20

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
char date[SIZE] = __DATE__;
char time_at_upload = __TIME__;
char myDate[3][SIZE]; 
char prev_char;

String print_0;
String print_1;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);

  Serial.println("====================================================");
  Serial.println(date[SIZE] == NULL);


  //Store date at Arduino upload
  int i = 0;
  int count = 0;
  for (int j = 0; j < LOOP_SIZE; j++)
  {
    Serial.print("char = ");
    Serial.println(date[j]);
    Serial.print("isNull = ");
    Serial.println(date[j] == NULL);
    Serial.print("j = ");
    Serial.println(j);
    Serial.print("i = ");
    Serial.println(i);
    Serial.print("count = ");
    Serial.println(count);
    Serial.print("my monitor = ");
    Serial.println(myDate[i]);
    Serial.println("-------");
    
    if (i==3)
    {
      Serial.println("BREAK!");
      break;
    }
    
    if (date[j] != ' ' && date[j] != NULL) 
    {
      myDate[i][count] = date[j];
      count++;
    }
    
    if ((date[j] == ' ' || date[j] == NULL) && (prev_char != ' ' && prev_char != NULL))
    {
      i++;
      count = 0;
    }
    
    prev_char = date[j];
    
  }

  Serial.println(__TIME__);
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  lcd.print(myDate[0]);
  lcd.print(" ");
  lcd.print(myDate[1]);
  lcd.print(" ");
  lcd.print(myDate[2]);
  lcd.print(" ");
  lcd.setCursor(0, 1);
  lcd.print(__TIME__);
}

void ParseString(char input[], 
