#include <LiquidCrystal_I2C.h>
#include <Wire.h>

int timeStart = 0;
int buttonMorse = 13;
int buttonEnd = 12;
int buttonMorseState = 0;
int buttonEndState = 0;
int timeButtonPressed = 0;
int buttonStartEnd = 11;
int buttonStartEndState= 0;
int cont = 0;
int timeButtonNotPressed_Start = 0;
int timeButtonNotPressed = 0;
int buzzer = 10;
int lcdPos = 0;

LiquidCrystal_I2C lcd(0x3F,16,2);

// morse character
String character = "";
// list of letters and numbers
String lettersAndNumbers[36]={"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","0","1","2","3","4","5","6","7","8","9"};
// respective morse code for each letter and number
String morseCode[36]={".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--..",
"-----",".----","..---","...--","....-",".....","-....","--...","---..","----."};

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonMorse, INPUT);
  pinMode(buttonEnd, INPUT);
  pinMode(buttonStartEnd, INPUT);
  pinMode(buzzer, OUTPUT);

  lcd.init(); // Inicialize communication with the display
  lcd.backlight(); // Turn the display lights on
  lcd.clear(); // Clean the display
  lcd.setCursor(0,0);
  Serial.begin(9600);

}

void loop() {
  if (lcdPos%32==0) {
    // Everytime all the 32 positions of the display are being used, clean it up
    lcd.clear(); 
  }
  buttonStartEndState = digitalRead(buttonStartEnd);
  // Everytime the button thats start and pauses the program is pressed, the program increases 'cont' in one
  if(buttonStartEndState){
    cont++;
    delay(500);
  }
  // If 'cont' is even, the program starts, if it's odd, the program pauses
  if(cont%2==1){
      // When the program is running, the time that the button of the morse code is not pressed is count
      timeButtonNotPressed = millis();
      // If the button is not pressed for 3 seconds, a space is done
      if (timeButtonNotPressed % 3000 == 0){
      Serial.print(" ");
      lcd.print(" ");
      lcdPos ++;
      delay(300);
      }
  buttonMorseState = digitalRead(buttonMorse);
  buttonEndState = digitalRead(buttonEnd);
  // timeStart restart before press the button
  timeStart = millis();
// while the button is pressed the time is count
  while (buttonMorseState){
    buttonMorseState = digitalRead(buttonMorse);
    tone(buzzer, 800);
  }
  noTone(buzzer);
// finalize the time and store in timeButtonPressed
  timeButtonPressed = millis() - timeStart;
// if the button is pressed for less than 200 miliseconds and more than 5 miliseconds, so the character is "."
  if (timeButtonPressed < 200 and timeButtonPressed > 5){
    character = character + ".";
    // the time that the button is not pressed is turned to 0
    timeButtonNotPressed = timeButtonNotPressed - timeButtonNotPressed;
  }
  // if the button is pressed for more than 200 seconds, so the character is "-"
  if (timeButtonPressed > 200) {
    character = character + "-";
    // the time that the button is not pressed is turned to 0
    timeButtonNotPressed = timeButtonNotPressed - timeButtonNotPressed;
  }
  // if the button that ends the character is pressed
  if (buttonEndState){
    // compare the list of characters with the list of morse
     for(int i = 0;i < 36; i++){
       // if the character made is equal to the respective morse code, print the character that have the same index
       if(morseCode[i]==character){
         Serial.print(lettersAndNumbers[i]);
         lcd.print(lettersAndNumbers[i]);
         // Position of the display's cursor is increased in one
         lcdPos ++;
         // If the position is in the end of the first line, everytime lcdPos is divisible by 16, the cursor go to the second line, position 0,1
         if(lcdPos%16==0){
          lcd.setCursor(0,1);
         }
       }
     }
     // character is restarted
    character = "";
    delay(500);
  }
  }
}