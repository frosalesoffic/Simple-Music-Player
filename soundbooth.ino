#include "Timer.h"
#include "Adafruit_LiquidCrystal.h"
// Depending on the LiquidCrystal library you are able to install, it might be:
// #include "LiquidCrystal.h"
#include "pitches.h"

// Sound Variables  
int buzzer = 8;

int sw = 10;
int col = 0;
int row = 0;
int p1 = 0;
int p2 = 0;
int swap = 0;
int input;
int altin;
int select;
int end = 0;
int loopp = 1;
int preS = 0;
int PFlag = 0;

// == Song 1 ==
int song1[] = {
NOTE_E4, NOTE_C5, NOTE_B1, NOTE_F3, NOTE_C4, 
NOTE_A4, NOTE_A4, NOTE_GS5, NOTE_C5, NOTE_CS4, 
NOTE_AS4, NOTE_C5, NOTE_DS4, NOTE_CS5, NOTE_GS4, 
NOTE_C3, NOTE_E3, NOTE_DS5, NOTE_D4, NOTE_D3
};
int song1_time[] = {
2, 1, 2, 1, 1, 4, 8, 16, 8, 4, 4, 1, 8, 4, 2, 4, 4, 16, 4, 2
};

// == Song 2 ==

int song2[] = {
  NOTE_FS5, NOTE_D2, NOTE_DS5, NOTE_G2, NOTE_B3, 
  NOTE_C2, NOTE_G5, NOTE_D6, NOTE_CS5, NOTE_AS4, 
  NOTE_DS6, NOTE_D3, NOTE_CS4, NOTE_E5, NOTE_DS6,
   NOTE_E4, NOTE_B4, NOTE_F4, NOTE_E6, NOTE_DS4
};

int song2_time[] = {
  2, 2, 4, 8, 1, 8, 4, 4, 16, 8, 2, 4, 16, 8, 2, 4, 16, 4, 8, 1
};

// == Song 3 == 

int song3[] = {
  NOTE_A5, NOTE_D4, NOTE_D6, NOTE_DS3, NOTE_G4, 
  NOTE_B2, NOTE_F2, NOTE_A3, NOTE_AS2, NOTE_B5, 
  NOTE_C6, NOTE_C3, NOTE_GS3, NOTE_G2, NOTE_FS5, 
  NOTE_AS4, NOTE_GS2, NOTE_CS3, NOTE_C3, NOTE_AS2
};

int song3_time[] = {
  1, 2, 16, 4, 16, 2, 16, 1, 1, 2, 1, 8, 2, 16, 8, 1, 16, 4, 1, 2
};


// LCD variables
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
Adafruit_LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// Depending on the LiquidCrystal library you are able to install, it might be:
// LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



// Task Structure Definition
typedef struct task {
   int state;                  // Tasks current state
   unsigned long period;       // Task period
   unsigned long elapsedTime;  // Time elapsed since last task tick
   int (*TickFct)(int);        // Task tick function
} task;


const unsigned char tasksNum = 4;
task tasks[tasksNum]; // We have 4 tasks

// Task Periods

const unsigned long periodLCDOutput = 100;
const unsigned long periodJoystickInput = 100;
const unsigned long periodSoundOutput = 100;
const unsigned long periodController = 500;


// GCD 
const unsigned long tasksPeriodGCD = 100;

// Task Function Definitions
int TickFct_LCDOutput(int state);
int TickFct_JoystickInput(int state);
int TickFct_SoundOutput(int state);
int TickFct_Controller(int state);

// Task Enumeration Definitions
enum LO_States {LO_init, LO_MenuOptionA, Song1, Song2, Song3};
enum JI_States {JI_init, JI_Sample, JI_M2};
enum SO_States {SO_init, SO_SoundOn, SO_SoundOff, S1, S2, S3, pause};
enum C_States {C_init, C_T1, M2};



void TimerISR() { // TimerISR actually defined here
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) { // Heart of the scheduler code
  //Serial.println(tasks[i].elapsedTime);
     if ( tasks[i].elapsedTime >= tasks[i].period ) { // Ready
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = 0;
     }
     tasks[i].elapsedTime += tasksPeriodGCD;
  }
}


void LCDWriteLines(String line1, String line2) {
  lcd.clear();          
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
}

// Task Function Definitions

int menuOption = 0;

// Task 1
int TickFct_LCDOutput(int state) {
  switch (state) { // State Transitions
    case LO_init:
      state = LO_MenuOptionA;
      LCDWriteLines(" Song 1   Song 2", " Song 3   Start");
    break;

    case LO_MenuOptionA:
       if(swap == 1){
         state = Song1;
         LCDWriteLines("Playing Song 1", " Pause    Play");
       }else if(swap == 2){
         state = Song2;
         LCDWriteLines(" Playing Song 2", " Pause    Play");

       }else if(swap == 3){
         state = Song3;
         LCDWriteLines(" Playing Song 3", " Pause    Play");
       }
    break;
    case Song1:
    
      if(menuOption == 1) {
        lcd.blink();
        lcd.setCursor(col, row);
      }
      if(menuOption == 2) {
        lcd.blink();
        lcd.setCursor(col, row);
      }
      if(swap == 0){
        state = LO_init;
      }
    break;
    case Song2:
      if(menuOption == 1) {
        lcd.blink();
        lcd.setCursor(col, row);
      }
      if(menuOption == 2) {
        lcd.blink();
        lcd.setCursor(col, row);
      }
      if(swap == 0){
        state = LO_init;
      }
    break;
    case Song3:
      if(menuOption == 1) {
        lcd.blink();
        lcd.setCursor(col, row);
      }
      if(menuOption == 2) {
        lcd.blink();
        lcd.setCursor(col, row);
      }
      if(swap == 0){
        state = LO_init;
      }
    break;
  }

  switch (state) { // State Actions
    case LO_MenuOptionA:
          if(menuOption == 1) {
        lcd.blink();
        lcd.setCursor(col, row);
      }
      if(menuOption == 2) {
        lcd.blink();
        lcd.setCursor(col, row);
      }
      if(menuOption == 3) {
        lcd.blink();
        lcd.setCursor(col, row);
      }
      if(menuOption == 4) {
        lcd.blink();
        lcd.setCursor(col, row);
      }
    break;
    case Song1:
    break;
  }
  return state;
}
// Task 2
int TickFct_JoystickInput(int state) {
  switch (state) { // State Transitions
    case JI_init:
    state = JI_Sample;
    break;

    case JI_Sample:
    if(swap != 0){
      //Serial.println("green");
       state = JI_M2;
     }
    break;

    case JI_M2:
    if(swap == 0){
      state = JI_Sample;
    }
    //Serial.println("whyyy");
    /*int altin = analogRead(A0);
    //Serial.println("in");
    if(altin > 550){
       menuOption = 1;
       col = 0;
       row = 1;
     }else if(altin < 500){
       menuOption = 2;
       col = 9;
       row = 1;
     }*/
    break;
  }
//Serial.println(state);
//Serial.println(JI_M2);
   switch (state) { // State Actions
  
    case JI_Sample:
      input = analogRead(A1);
      altin = analogRead(A0);
      select = digitalRead(sw);
      //Serial.println(input);
      //Serial.println(altin);
     // Serial.println(select);
     if(input > 550){
       menuOption = 1;
       col = 0;
       row = 0;
     }else if(altin < 500){
       menuOption = 2;
       col = 9;
       row = 0;
     }else if(altin > 550){
       menuOption = 3;
       col = 0;
       row = 1;
     }else if(input < 500){
       menuOption = 4;
       col = 9;
       row = 1;
     }
     
     // menuOption = input > 550 ? 1 : 0;
      //menuOption = altin < 500 ? 2 : 0;
      //menuOption = altin > 550 ? 3 : 0;
     // Serial.println(menuOption);
    break;
  //Serial.println("egg");
    case JI_M2:
    //altin = analogRead(A0);
    //Serial.println("in");
    if(analogRead(A0) > 550){
       menuOption = 1;
       col = 9;
       row = 1;
     }else if(analogRead(A0) < 500){
       menuOption = 2;
       col = 0;
       row = 1;
     }
    break;

    default:
    Serial.println(state);
    break;
  }

  
  return state;
}
// Sound Output
int counter = 0;
int note = 0;
int TickFct_SoundOutput(int state) {
  switch (state) { // State Transitions
    case SO_init:
      state = SO_SoundOn;
    break;
    case SO_SoundOn:
    if(swap == 1){
      loopp = 1;
      state = S1;
    }else if(swap == 2){
      loopp = 1;
      state = S2;
    }else if(swap == 3){
      loopp = 1;
      state = S3;
    }
    break;
    case S1:
      
      if(counter >= song1_time[note]) {
         preS = state;
         state = SO_SoundOff;
         note++;
         counter = 0;
         loopp = 0;
         //swap = 0;
         note = note % 20;
        // Serial.println(note);
      }
      if(note == 0 && loopp == 0){
        state = SO_SoundOn;
        swap = 0;
      }
      if(PFlag == 1){
        state = pause;
      }

    break;
    case S2:
    
    if(counter >= song2_time[note]) {
         preS = state;
         state = SO_SoundOff;
         note++;
         counter = 0;
         loopp = 0;
         //swap = 0;
         note = note % 20;
        // Serial.println(note);
      }
      if(note == 0 && loopp == 0){
        state = SO_SoundOn;
        swap = 0;
      }
      if(PFlag == 1){
        state = pause;
      }
    break;
    case S3:
    if(counter >= song3_time[note]) {
          preS = state;
         state = SO_SoundOff;
         note++;
         counter = 0;
         loopp = 0;
         //swap = 0;
         note = note % 20;
        // Serial.println(note);
      }
      if(note == 0 && loopp == 0){
        state = SO_SoundOn;
        swap = 0;
      }
      if(PFlag == 1){
        state = pause;
      }
    break;
    case pause:
    if(PFlag == 0){
        state = preS;
      }
    break;
    case SO_SoundOff:
      state = preS;
    break;
    
  }
   switch (state) { // State Actions
    case SO_SoundOn:
    break;
    case S1:
      tone(buzzer, song1[note], periodSoundOutput * song1_time[note]);
      counter++;
    break;

    case S2:
    Serial.println("apppppple");
      tone(buzzer, song2[note], periodSoundOutput * song2_time[note]);
      counter++;
    break;
    case S3:
      tone(buzzer, song3[note], periodSoundOutput * song3_time[note]);
      counter++;
    break;
    case pause:
    noTone(buzzer);
    break;
    case SO_SoundOff:
      noTone(buzzer);
    break;

  }
  return state;
}

// Task 4 (Menu Cursor/ select)
int TickFct_Controller(int state) {
  
  switch (state) { // State Transitions
    case C_init:
    state = C_T1;
    break;

    case C_T1:
    if(digitalRead(sw) == 0 && menuOption != 4){
      lcd.setCursor(p1, p2);
      lcd.print(" ");
      lcd.setCursor(col, row);
      lcd.print("*");
      p1 = col;
      p2 = row;
      Serial.println(p1);
      Serial.println(p2);
    }
    if(digitalRead(sw) == 0 && menuOption == 4){
      state = M2;
      if(p1 == 0 && p2 == 0){
        lcd.noBlink();
        swap = 1;
        p1 = 0;
        p2 = 1;
      }else if(p1 == 9 && p2 == 0){
        lcd.noBlink();
        swap = 2;
        p1 = 0;
        p2 = 1;
      }else if(p1 == 0 && p2 == 1){
        lcd.noBlink();
        swap = 3;
        p1 = 0;
        p2 = 1;
      }
    }
    break;

    case M2:
  Serial.println(swap);
    if(digitalRead(sw) == 0){
      lcd.setCursor(p1, p2);
      lcd.print(" ");
      lcd.setCursor(col, row);
      lcd.print("*");
      p1 = col;
      p2 = row;
    }
    if(digitalRead(sw) == 0){
      if(p1 == 0 && p2 == 1){
        lcd.noBlink();
        PFlag = 1;
      }else if(p1 == 9 && p2 == 1){
        lcd.noBlink();
        PFlag = 0;
      }
    }
    if(swap == 0){
      //Serial.println("");
        state = LO_init;
      }
    break;
  }

   switch (state) { // State Actions
    case C_init:
    break;
    case M2:
   // Serial.println("apple");
    break;
  }
  return state;
}



void InitializeTasks() {
  unsigned char i=0;
  tasks[i].state = LO_init;
  tasks[i].period = periodLCDOutput;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].TickFct = &TickFct_LCDOutput;
  ++i;
  tasks[i].state = JI_init;
  tasks[i].period = periodJoystickInput;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].TickFct = &TickFct_JoystickInput;
  ++i;
  tasks[i].state = SO_init;
  tasks[i].period = periodSoundOutput;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].TickFct = &TickFct_SoundOutput;
  ++i;
  tasks[i].state = C_init;
  tasks[i].period = periodController;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].TickFct = &TickFct_Controller;

}

void setup() {
  // put your setup code here, to run once:
  InitializeTasks();
  pinMode(sw, INPUT_PULLUP);

  TimerSet(tasksPeriodGCD);
  TimerOn();
  Serial.begin(9600);
  // Initialize Outputs
  lcd.begin(16, 2);
  // Initialize Inputs

}

void loop() {
  // put your main code here, to run repeatedly:
  // Task Scheduler with Timer.h
  
}
