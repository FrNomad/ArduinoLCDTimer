#include <LiquidCrystal.h>
#include "pitches.h"
#define RESET 10
#define START 9
#define PAUSE 12
#define BUZZER 28
#define analogX_Pin A0
#define analogY_Pin A1
#define analogX_deadZone 400
#define analogY_deadZone 400
int X_position = 1;
int Y_position = 1;
int loopnumber = 0;
int startstop = 0;
boolean time_end = false;
int trash;

int readanalogX();
int readanalogY();
boolean toggle(byte pin);
void line();
void positioning();
void breakwhiledelay(int a);
void Stopwatch();
void Timer();


int state = LOW;      
int reading;          
int previous = LOW;  
long time = 0;         
long debounce = 100;   

const int rs = 7, en = 8, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
float timer_s = 0.0;
int timer_m = 0;
int timer_h = 0;
float stopwatch_s = 0.0;
int stopwatch_m = 1;
int stopwatch_h = 0;

                     

int readanalogX(void){                                                          //조이스틱의 X좌표 값이 변함에 따라 -1, 0, 1을 배출하는 함수
  if(analogRead(analogX_Pin) >= 490 + analogX_deadZone) return 1;
  else if(analogRead(analogX_Pin) <= 490 - analogX_deadZone) return -1;
  else return 0;
}

int readanalogY(void){                                                          //조이스틱의 Y좌표 값이 변함에 따라 -1, 0, 1을 배출하는 함수
  if(analogRead(analogY_Pin)>= 510 + analogY_deadZone) return 1;
  else if(analogRead(analogY_Pin) <= 510 - analogY_deadZone) return -1;
  else return 0;
}

void line(void){                                                                //LCD의 끝부분에 세로줄을 생성하는 함수
  lcd.setCursor(15,0);
  lcd.print("|");
  lcd.setCursor(15,1);
  lcd.print("|");
  lcd.setCursor(0,0);
  lcd.print("|");
  lcd.setCursor(0,1);
  lcd.print("|");
}


boolean toggle(byte pin){                                                       //괄호 안의 핀 번호의 신호를 입력받고 버튼이 눌러졌을 때 신호가 켜져있으면 끄고, 꺼져있으면 키는 함수 
  reading = digitalRead(pin);  
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (state == HIGH) state = LOW;   
    else state = HIGH;
 
    time = millis();
  }
 
 
  previous = reading;
  return state;
}






void positioning(void){                                                          //조이스틱 함수의 신호를 받고 페이지를 올리거나 내리거나 왼쪽이나 오른쪽으로 넘겨주는 함수
  if(readanalogX() == 1){
    X_position++;
    lcd.clear();
    delay(200);
  }
  else if(readanalogX() == -1){
    X_position--;
    lcd.clear();
    delay(200);
  }
  else;
  if(readanalogY() == 1){
   // loopnumber = 0;
    Y_position++;
   
    delay(200);
  }
  else if(readanalogY() == -1){
   // loopnumber = 0;
    Y_position--;
   
    delay(200);
  }
  else;
}



void breakwhiledelay(int a){                                                   //기능이 실행되고 있는 도중에 리셋이 입력되면 기능을 취소하는 함수
  for(int i = 0; i < a; i++){
    if(digitalRead(RESET) == HIGH){
      break;
    }
    else delay(1);
  }
}










void Timer(void){                                                             //스톱워치 기능 함수(실수로 타이머라 지정함)

      
      
      if(loopnumber = 0){
        timer_s = 0.0;
        timer_m = 0;
        timer_h = 0;
        loopnumber++;
      }
      lcd.setCursor(0, 0);
      lcd.print("Timer : ");
      
      if(toggle(START) == HIGH){
        timer_s += 0.1;
      }
      else if(toggle(START) == LOW && timer_s != 0){
        lcd.setCursor(12, 0);
        lcd.print("STOP");
      }
      else;
      if(digitalRead(RESET) == LOW && timer_s != 0 ){
        while(digitalRead(RESET) == LOW){
          timer_s = 0.0;
          timer_m = 0;
          timer_h = 0;
          reading = HIGH;
         lcd.setCursor(11, 0);
         lcd.print("RESET");
         delay(300);
        }
      }
      if(timer_s >= 60.0){
        timer_s = 0.0;
        timer_m++;
      }
      if(timer_m >= 60){
        timer_m = 0;
        timer_h++;
      }
      lcd.setCursor(0, 1);
      if(timer_h < 10) lcd.print("0");
      
      lcd.print(timer_h);
      lcd.print("h ");
       if(timer_m < 10) lcd.print("0");
      
      lcd.print(timer_m);
      lcd.print("m ");
       if(timer_s < 10) lcd.print("0");
      
      lcd.print(timer_s);
      lcd.print("s");
      delay(100);
      lcd.clear();
      
}









void Stopwatch(void){                                                               //타이머 기능 함수
  
      loopnumber = 0;
      Serial.println(Y_position);
      Serial.print(",");
      Serial.print(startstop);
      
      
      if(loopnumber = 0){
        stopwatch_h = 0;
        stopwatch_m = 1;
        stopwatch_s = 0.0;
        loopnumber++;
      }

      if(Y_position <= 0){
        Y_position = 6;
      }
      else if(Y_position >= 7){
        Y_position = 1;
      }
      else;
      
    
      if(startstop == 0 && digitalRead(PAUSE) == LOW){
          if(Y_position == 1){
            stopwatch_s++;
            if(stopwatch_s >= 60.0){
              stopwatch_s = 0.0;
            }
          }
          else if(Y_position == 2){
            stopwatch_s += 10.0;
            if(stopwatch_s >= 60.0){
              
              stopwatch_s = 0.0;
            }
          }
          else if(Y_position == 3){
            stopwatch_m++;
            if(stopwatch_m >= 60){
              stopwatch_m = 0;
            }
          }
          else if(Y_position == 4){
            stopwatch_m += 10;
             if(stopwatch_m >= 60){
              stopwatch_m = 0;
            }
          }
          else if(Y_position == 5){
            stopwatch_h++;
             if(stopwatch_h >= 100){
              stopwatch_h = 0;
            }
          }
          else if(Y_position == 6){
            stopwatch_h += 10;
            if(stopwatch_h >= 100){
              stopwatch_h = 0;
            }
          }
          else;
          delay(10);
        }
          
         
         
        
      else if(startstop == 0){
          if(Y_position == 1){
            lcd.setCursor(11, 0);
            lcd.print("    s");
            delay(1);
          }
          else if(Y_position == 2){
            lcd.setCursor(11, 0);
            lcd.print("  10s");
            delay(1);
          }
          else if(Y_position == 3){
            lcd.setCursor(11, 0);
            lcd.print("    m");
            delay(1);
          }
          else if(Y_position == 4){
            lcd.setCursor(11, 0);
            lcd.print("  10m");
            delay(1);
          }
          else if(Y_position == 5){
            lcd.setCursor(11, 0);
            lcd.print("    h");
            delay(1);
          }
          else if(Y_position == 6){
           lcd.setCursor(11, 0);
            lcd.print("  10h");
            delay(1);
          }
          else;
        }
        else;

      
      lcd.setCursor(0, 0);
      lcd.print("Stopwatch : ");
      
      if(stopwatch_h < 0){
        time_end = true;
        stopwatch_h = 0;
        stopwatch_m = 0;
        stopwatch_s = 0.0;
      }
      else;
      

     
      
      
      
      if(toggle(START) == HIGH && time_end == false){
        stopwatch_s -= 0.1;
      }
      else if(toggle(START) == LOW && startstop >= 1){
        lcd.setCursor(12, 0);
        lcd.print("STOP");
      }
      
      else;

      if(digitalRead(START) == HIGH){
        startstop++;
      }



      
     
      if(digitalRead(RESET) == LOW && (stopwatch_s != 0 || time_end == true)){
        while(digitalRead(RESET) == LOW){
          time_end = false;
          startstop = 0;
          stopwatch_s = 0.0;
          stopwatch_m = 1;
          stopwatch_h = 0;
          reading = HIGH;
         lcd.setCursor(11, 0);
         lcd.print("RESET");
         delay(300);
        }
      }
      if(stopwatch_s < 0.0){
        stopwatch_s = 60.0;
        stopwatch_m--;
      }
      if(stopwatch_m < 0){
        stopwatch_m = 59;
        stopwatch_h--;
      }
      lcd.setCursor(0, 1);
      if(stopwatch_h < 10) lcd.print("0");
      
      lcd.print(stopwatch_h);
      lcd.print("h ");
      if(stopwatch_m < 10) lcd.print("0");
      
      lcd.print(stopwatch_m);
      lcd.print("m ");
      if(stopwatch_s < 10.0) lcd.print("0");

      lcd.print(stopwatch_s);
      lcd.print("s");
      
      if(time_end){
        lcd.setCursor(12, 0);
        lcd.print("end!");
        tone(BUZZER, hihiC, 230);
        breakwhiledelay(250);
        tone(BUZZER, hihiC, 230);
        breakwhiledelay(500);
      }
      delay(100);
      lcd.clear();
}      
      
      





void setup() {
  timer_s = 0.0;
  int timer_m = 0;
  int timer_h = 0;
  pinMode(START, INPUT_PULLUP);
  pinMode(RESET, INPUT_PULLUP);
  pinMode(PAUSE, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(22,OUTPUT);
  digitalWrite(22, LOW);
  Serial.begin(9600);
  lcd.begin(16, 2);
  X_position = 4;
  Y_position = 1;

}


void loop() {
  
  positioning();
  
  if(X_position >= 5){
    X_position = 1;
    
  }
  else if(X_position <= 0){
    X_position = 4;
    
  }
  else;
  if(X_position == 1){
    loopnumber = 0;
    Timer();
  }
  else if(X_position == 2){
   
    loopnumber = 0;
   
    Stopwatch();
    
  }
  else if(X_position == 3){

    line();
    lcd.setCursor(5, 0);
    lcd.print("Credits");
    lcd.setCursor(2, 1);
    lcd.print("made by Sung");
  }
  else if(X_position == 4){

    line();
    lcd.setCursor(4, 0);
    lcd.print("LCD Timer");
    lcd.setCursor(6, 1);
    lcd.print("V3.0");
  }
  else;
  

} 
