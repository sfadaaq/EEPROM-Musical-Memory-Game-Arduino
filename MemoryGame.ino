/* Memory Game Algorithm
==================================
By Amal, Salma, and Yosr
==================================
  1. click start (button + code)
    2. start round
      2a. all led blink 
    3. Generate sequence (seq array of 100; max num of round)
      3a. turn on note led + note tone (for around 250-500ms)
        3b. (for multiple notes) delay 500ms between note sequences
    4. promote player to insert note(s) as sequence
    5. check sequence
      5a. if correct, go to 3a (delay 1-1.5 second between rounds)
        5b. otherwise, gameover
        
  Note: notes buttons are not in-use till user is promote to click 
*/
#include <LiquidCrystal.h>
#include <EEPROM.h>

//Using anaglog pins A1-A4 as digital (15-18)
LiquidCrystal lcd(12,13,15,16,17,18); //(RS, EN, D4, D5, D6, D7)

int gameSeq[100], noteSeq[100], playerSeq[100];
byte nRound = 1, highScore, myScore;
int note;
bool gameOver = 0; //gameOver flag

void setup() {
  
 // EEPROM.write(0, 0); //highScore in address 0 was initialized to 0 early on
  highScore = EEPROM.read(0); //read from address 0

  lcd.begin(16, 2); //LCD size (c,r)
  lcd.print("Press Button");
  lcd.setCursor(0,1);
  lcd.print("to Start");
  
  pinMode(2, OUTPUT); //yellow led 
  pinMode(3, OUTPUT); //blue led
  pinMode(4, OUTPUT); //red led
  pinMode(5, OUTPUT); //green led

  pinMode(6, INPUT); //button 1 Y 
  pinMode(7, INPUT); //button 2 B
  pinMode(8, INPUT); //button 3 R
  pinMode(9, INPUT); //button 4 G
  pinMode(11, INPUT); //start button
}

void loop() {
 if(gameOver == 0){ //Game goes on if player did not trigger gameOver flag
  if (nRound == 1) {
    generateSeq();
   
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    delay(250);

    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    delay(250);
  }

  if (digitalRead(11) == LOW || nRound != 1) { //start button is on or game is going on
    lcd.clear();
 
 //for(int i=nRound; i<=nRound;i++){
     
      lcd.clear();
      myScore=nRound-1;
      lcd.setCursor(0, 0);  //(Column,Row)
      lcd.print("Round ");
      lcd.print(nRound);
      lcd.setCursor(0, 1);  //(Column,Row)
      lcd.print("Your Score: ");
      lcd.setCursor(12, 1); 
      lcd.print(myScore);

      delay(1000);
    displaySeq(); //display sequence
    getSeq(); //get sequence from player
  
 }
}
}


void generateSeq() {
  randomSeed(analogRead(A0)); //random generation from noise

  for (int i = 0; i < 100; i++) {
    gameSeq[i] = random(2, 6);// (min, max-1)

    switch (gameSeq[i]) { //convert color to sound
      case 2:
        note = 262; //C
        break;
      case 3:
        note = 294; //D
        break;
      case 4:
        note = 330; //E
        break;
      case 5:
        note = 349; //F
        break;
    }
    noteSeq[i] = note;
  }
}

void displaySeq() {
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);

  for (int i = 0; i < nRound; i++) {
    digitalWrite(gameSeq[i], HIGH);
    tone(10, noteSeq[i]);
    delay(1000);
    digitalWrite(gameSeq[i], LOW);
    noTone(10);
    delay(200);
  }
}

void getSeq() {
  bool flag; //flag correct sequence
  
  for (int i = 0; i < nRound; i++) {
    flag = 0;

    while (flag == 0) {

      if (digitalRead(6) == LOW) {
        digitalWrite(2, HIGH);
        tone(10, 262); //C
        delay(500);
        noTone(10);
        playerSeq[i] = 2;
        flag = 1;

        if (playerSeq[i] != gameSeq[i]) {
          wrongSeq();
          return;
        }
        digitalWrite(2, LOW);
      }

      if (digitalRead(7) == LOW) {
        digitalWrite(3, HIGH);
        tone(10, 294); //D
        delay(500);
        noTone(10);
        playerSeq[i] = 3;
        flag = 1;
        //delay(200);

        if (playerSeq[i] != gameSeq[i]) {
          wrongSeq();
          return;
        }
        digitalWrite(3, LOW);
      }

      if (digitalRead(8) == LOW) {
        digitalWrite(4, HIGH);
        tone(10, 330); //E
        delay(500);
        noTone(10);
        playerSeq[i] = 4;
        flag = 1;

        if (playerSeq[i] != gameSeq[i]) {
          wrongSeq();
          return;
        }
        digitalWrite(4, LOW);
      }

      if (digitalRead(9) == LOW) {
        digitalWrite(5, HIGH);
        tone(10, 349); //F
        delay(500);
        noTone(10);
        playerSeq[i] = 5;
        flag = 1;

        if (playerSeq[i] != gameSeq[i]) {
          wrongSeq();
          return;
        }
        digitalWrite(5, LOW);
      }
    }
  }
  correctSeq();
}

void correctSeq() {
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delay(250);

  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  delay(500);

  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delay(500);

  if (nRound < 100) //below max
    nRound++;
}
  
  void endgameDis(){
  if(myScore>highScore){
    EEPROM.update(0, myScore); //update the new highscore value
    highScore = EEPROM.read(0); //take it
    lcd.setCursor(0, 0); 
    lcd.print("NEW HIGHSCORE!!!");
    lcd.setCursor(0, 1);
    lcd.print("High Score: ");
    lcd.setCursor(12, 1);
    lcd.print(highScore); //and display it
  }
  else{
     highScore = EEPROM.read(0); //read from highscore in internal EEPROM
     lcd.setCursor(0, 0);
     lcd.print("High Score: ");
   lcd.print(highScore); //display it
   lcd.setCursor(0, 1);
   lcd.print("Your Score: ");
   lcd.print(myScore); //display player score
  }
}

void wrongSeq() {
  for (int i = 0; i < 3; i++) { //gameOver blinking and display
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    tone(10, 233);
    delay(250);

    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    noTone(10);
    delay(250);
   
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Game Over");
  }
 
  delay(2000);
  lcd.clear();//clear it
  
 gameOver=1; //trigger gameOver flag
  endgameDis(); //endgame display
}
