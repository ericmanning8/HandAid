//
/// HandAid Software
/// created by Viterbi Dropouts
/// 2-8-20
//

#define INDEX 12
#define MIDDLE 11
#define RING 10
#define PINKY 9
#define THUMB 8

// external libraries
#include "rbtree.h"
#include "Vector.h"
#include "User.h"
#include "Game.h"
#include <LiquidCrystal_I2C.h>

// fingers
char P = 'P'; // pinky
char R = 'R'; // ring
char M = 'M'; // middle
char I = 'I'; // index

bool flag = 0;
bool flag_totaltimer = 0;
int count = 0;
bool index = 0;
bool middle = 0;
bool ring = 0;
bool pinky = 0;
bool thumb = 0;
bool pressed = 0;
bool buzz_input = 0;
unsigned char state = 0;
unsigned char question = 0;
unsigned char answer = 0;
unsigned char timer = 0;
int total_timer = 0;
unsigned char common_state = 0;

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

// function prototyping
void startUp(unsigned char&);
void generateNumber(unsigned char&, int&);
void letGo(unsigned char&, int&);
void gameOver(unsigned char&, int&);
void retry(unsigned char&, int&);

// build data structures 
Game game = Game();
User user = User();

void getAnswer(Game* game, unsigned char& state, int& count);

void setup()
{
  pinMode(INDEX, INPUT);
  pinMode(MIDDLE, INPUT);
  pinMode(RING, INPUT);
  pinMode(PINKY, INPUT);
  pinMode(THUMB, INPUT);
  digitalWrite(INDEX, HIGH);
  digitalWrite(MIDDLE, HIGH);
  digitalWrite(RING, HIGH);
  digitalWrite(PINKY, HIGH);
  digitalWrite(THUMB, HIGH);
  pinMode(7 , OUTPUT);

  Serial.begin(9600);

  randomSeed(analogRead(0));

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("HandAid");
  lcd.setCursor(0, 1);
  lcd.print("Viterbi Dropouts");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press any button");
  lcd.setCursor(0, 1);
  lcd.print("to start");

  cli();
  
  //set timer0 interrupt at 2kHz
  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
  
  sei();
}

void loop()
{

  // getting inputs for each finger, 1 -> pressed, 0 -> unpressed
  index = !digitalRead(INDEX);
  middle = !digitalRead(MIDDLE);
  ring = !digitalRead(RING);
  pinky = !digitalRead(PINKY);
  thumb = !digitalRead(THUMB);

  pressed = index || middle || ring || pinky || thumb;

  // get user pressed button answer
  if(index){answer = 0;}
  else if(middle){answer = 1;}
  else if(ring){answer = 2;}
  else if(pinky){answer = 3;}
  else if(thumb){answer = 4;}

  // time to respond to question in seconds 
  timer = 3;

  /***********************************

         STATES

  ***********************************/
  
  //START UP
  startUp(state);

  //GENERATE NUMBER
  generateNumber(state, count);

  //GET ANSWER
  getAnswer(&game, state, count);

  //LET GO
  letGo(state, count);

  //GAME OVER
  gameOver(state, count);

  //RETRY
  retry(state, count);

  //Game Timer
  if( ( (state == 2) || (state == 3) || (state == 1) ) && flag_totaltimer )
  {
    flag_totaltimer = 0;
    total_timer++;
  }

  //Light up LED when finger pressed
  if(pressed)
  {
    digitalWrite(7, HIGH);
  }
  else
  {
    digitalWrite(7, LOW);
  }
}

/***********************************

         STATE FUNCTIONS

***********************************/

void startUp(unsigned char& state)
{
  if(state == 0)
  {
    if(pressed)
    {
      common_state = 1;
      lcd.clear();
    }
    if(flag & common_state)
    {
      flag = 0;
      count++;
      Serial.println(count);
      if(count == 2000*0.2)
      {
        state = 1;
        common_state = 0;
      }
    }
    else
    {
      count = 0;
    }
    return;
  }
}

void generateNumber(unsigned char& state, int& count)
{
  if(state == 1)
  {
    question = random(5);
    //Serial.println(question);
    printQuestion(question);
    state = 2;
    count = 0;
  }

  return;
}

void printQuestion(unsigned char question)
{
  if(question == 0)
  {
    Serial.println("Yellow");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Yellow");
  }
  else if(question == 1)
  {
    Serial.println("Green");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Green");
  }
  else if(question == 2)
  {
    Serial.println("Red");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Red");
  }
  else if(question == 3)
  {
    Serial.println("Blue");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Blue");
  }
  else if(question == 4)
  {
    Serial.println("Black");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Black");
  }
}

void getAnswer(Game* game, unsigned char& state, int& count)
{
  if(state == 2)
  {
    if(total_timer > 7*2000)
    {
      lcd.clear();
      lcd.setCursor(0,0);

      // add data to user struct
      user.setCurrentScore(game.getScore());
      user.setNumPresses(game.getTotalPresses());
      user.setReactionTimes(game.getReactionTimes(), game.getFingersUsed());

      lcd.print("Game Over!");
      lcd.setCursor(0,1);
      lcd.print("Press any button");
      
      Serial.println("Game Over!");
      Serial.println("Press any button to continue");

      
      state = 4;
      total_timer = 0;
      count = 0;
    }
    if(flag == 1)
    {
      flag = 0;
      count++;
    }
    if(pressed)
    {
      if(answer == question)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Correct!");

        // insert data to data structure
        game.insertTime((double)count/2000);
        game.insertFinger(answer);

        
        Serial.println("yes");
        game->correctAnswer();
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sorry, wrong!");

        
        Serial.println("no");
        game->wrongAnswer();
      }
      
      state = 3;
      count = 0;
    }
    
    //timer = reaction time (s)
    else if(count == 2000*timer)
    {
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Sorry, too late!");
      
      Serial.println("too late");
      state = 3;
      count = 0;
    }
  }
  return;
}

void letGo(unsigned char& state, int& count)
{
  if(state == 3)
  {
    if(flag == 1)
    {
      flag = 0;
      count++;
      if(count == 2000)
      {
        state = 1;
        count = 0;
      }
    }
    if(pressed)
    {
      count = 0;
    }
  }
}

void gameOver(unsigned char& state, int& count)
{
  if(state == 4)
  {
    if(pressed)
    {
      state = 5;
      Serial.print("New Game:");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("New Game");
      
    }
  }
}

void retry(unsigned char& state, int& count)
{
  if(state == 5)
  {
    if(flag)
    {
      flag = 0;
      count++;
      if(count == 2000*1)
      {
        state = 1;

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Press any button");
        lcd.setCursor(0, 1);
        lcd.print("to start");
      }
    }
  }
}



/***********************************

         TIMER

***********************************/

ISR(TIMER0_COMPA_vect)
{
  //timer0 interrupt 2kHz
  flag = 1;
  flag_totaltimer = 1;
}

double average(Vector<double> v) {
  double avg;

  while () {

  }

  return avg;
}
