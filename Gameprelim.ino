#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

AsyncDelay Delaytime_1s; AsyncDelay Delaygametime; AsyncDelay Windelay;
int Rannum = 0;
int BopitIN = 0;
int Bopem = 0;
int Life = 3;
int Score = 0;
int Gamertime = 10000;

float midi[127];
int A_four = 440;

const int APin = 4;
volatile bool AFlag = false;
int ABgame = 1;

const int SwitchPin = 7;
volatile bool Switchflag = false;
int Switchgame = 1;

const int BPin =5;
volatile bool BFlag = false;
int BBgame = 1;

int Soundlvl;
int Sgame = 1;

int CorrectANS = 0;

int c_major[8] = {60, 62, 64, 65, 67, 69, 71, 72};


void AISr(){
  AFlag = true;
}
void BISr(){
  BFlag = true;
}
void SwitchISR(){
  Switchflag = true;
}

void setup(){
  Serial.begin(9600);
  CircuitPlayground.begin();
  attachInterrupt(digitalPinToInterrupt(APin), AISr, RISING);
  attachInterrupt(digitalPinToInterrupt(BPin), BISr, RISING);
  attachInterrupt(digitalPinToInterrupt(SwitchPin), SwitchISR, CHANGE);
  Delaytime_1s.start(1000, AsyncDelay::MILLIS);
  randomSeed(CircuitPlayground.lightSensor());
  Serial.println("Game Begin");
  generateMIDI();
  

}

void loop(){
  Soundlvl = CircuitPlayground.mic.soundPressureLevel(50);
  CircuitPlayground.clearPixels();
  

  if(Life <= 0){
    Serial.println("You lose *womp womp*");
    CircuitPlayground.playTone(midi[69], 200);
    delay(1);
    CircuitPlayground.playTone(midi[69], 200);
    delay(1);
    CircuitPlayground.playTone(midi[64], 500);
    Life = 3;
    Score = 0;
    Serial.println("Game reset!");
  }

  if(Score <= 3){
    Serial.println("Level 1");
    Gamertime = 10000;
  }else if(Score > 3 && Score <= 6){
    Serial.println("Level 2");
    Gamertime = 5000;
  }else if(Score > 6 && Score < 10){
    Serial.println("Level 3");
    Gamertime = 3000;
  }else if(Score = 10){
    Windelay.start(5000, AsyncDelay::MILLIS);
    while(!Windelay.isExpired()){
    Crisscross(60, 185, 25, 5);
    }
    Serial.println("You win!!");
    Serial.println("Game reset!");
    Gamertime = 10000;
    Score = 0;
    CircuitPlayground.clearPixels();
  }
  



  if(Delaytime_1s.isExpired()){
    Rannum = random(0,4);
    Delaytime_1s.repeat();
  }
  delay(2000); 

 InputTopress(Rannum);

 if(Bopem){

  Delaygametime.start(Gamertime, AsyncDelay::MILLIS);
  ABgame = 0;
  BBgame = 0;
  Switchgame = 0;

  while(!Delaygametime.isExpired()){

  Soundlvl = CircuitPlayground.mic.soundPressureLevel(50);
  
  if(Soundlvl > 100){
    Sgame = 1;
    break;
  }

  if(Switchflag){
    delay(5);
    Switchgame = 1;
    Switchflag = false;
    break;
  }
  
  if(AFlag){
    delay(5);
   ABgame = 1;
   AFlag = false;
   break;
  } 

  if(BFlag){
   delay(5);
   BBgame = 1;
   BFlag = false;
   break;
  } 

  } 
    switch(Rannum){
      case 0:
        if(ABgame){
          Serial.println("You pressed the A button!");
          CorrectANS = 1;
          Bopem = 0;
          ABgame = 0;
          Delaygametime.expire();
         
        }else if(Delaygametime.isExpired() || BBgame || Switchgame || Sgame &! ABgame){
          Serial.println("Better luck next time!");
          Bopem = 0;
          BBgame = 0;
          Switchgame = 0;
          Sgame = 0;
          Delaygametime.expire();
        }
        break;
      case 1:
        if(BBgame){
          Serial.println("You pressed the B button!");
          CorrectANS = 1;
          Bopem = 0;
          BBgame = 0;
          Delaygametime.expire();
        }else if(Delaygametime.isExpired() || ABgame || Switchgame || Sgame &!BBgame){
          Serial.println("Better luck next time!");
          Bopem = 0;
          ABgame = 0;
          Switchgame = 0;
          Sgame = 0;
          Delaygametime.expire();
        }
        break;
      case 2:
        if(Switchgame){
          Serial.println("You switched it!");
          CorrectANS = 1;
          Bopem = 0;
          Switchgame = 0;
          Delaygametime.expire();
        }else if(Delaygametime.isExpired() || ABgame || BBgame || Sgame &!Switchgame){
          Serial.println("Better luck next time!");
          Bopem = 0;
          ABgame = 0;
          BBgame = 0;
          Sgame = 0;
          Delaygametime.expire();
        }
        break;
      case 3:
        if(Sgame){
          Serial.println("Woah, watch the volume");
          CorrectANS = 1;
          Bopem = 0;
          Sgame = 0;
          Delaygametime.expire();
        }else if(Delaygametime.isExpired() || ABgame || BBgame || Switchgame){
          Serial.println("Better luck next time!");
          Bopem = 0;
          ABgame = 0;
          BBgame = 0;
          Switchgame = 0;
          Delaygametime.expire();
        }
        break;

      default:
        break;
    }   
 }

  if(CorrectANS){
    Score = Score +1;
    Serial.print("Score =");
    Serial.println(Score);
    CorrectANS = 0;
  } else{
    Life = Life -1;
    Serial.println("Life -1");
    Serial.print("Life = ");
    Serial.println(Life);

  }
}


int InputTopress(int Targetcase){

   switch(Targetcase){
    case 0:
      Serial.println("Press the A button!");
      Bopem = 1;
      break;
    
    case 1:
      Serial.println("Press the B button!");
      Bopem = 1;
      break;

    case 2:
      Serial.println("Switch!!");
      Bopem = 1; 
      break;
    case 3:
      Serial.println("Yell!");
      Bopem =1;
      break;

    default:
      break;
    }
  }

void generateMIDI(){
  for (int x = 0; x < 127; ++x)
  {
    midi[x] = (A_four / 32.0) * pow(2.0, ((x - 9.0) / 12.0));
    Serial.println(midi[x]);
  }
  }

void Crisscross(uint32_t color, uint32_t color2, int brightness, int delayTime) {
  CircuitPlayground.setBrightness(brightness);  // Set LED brightness
  
  // Define an array with the desired LED pins
  int ledS[] = {0, 2, 4, 5, 7, 9};
  int numLEDs = sizeof(ledS) / sizeof(ledS[0]);  // Calculate the number of LEDs
  
  for (int i = 0; i < numLEDs; i++) {
    CircuitPlayground.setPixelColor(ledS[i], CircuitPlayground.colorWheel(color));  // Set LED color
  }
  delay(40);
  CircuitPlayground.clearPixels();

  int ledT[] = {1, 3, 6, 8};
  int numLEDT = sizeof(ledT) / sizeof(ledT[0]); 
  for(int j = 0; j < numLEDT; j++) {
    CircuitPlayground.setPixelColor(ledT[j], CircuitPlayground.colorWheel(color2));  // Set LED color
  }
  delay(delayTime);
  CircuitPlayground.clearPixels();

  int ledA[] = {0, 2, 4, 5, 7, 9};
  int numLEDa = sizeof(ledA) / sizeof(ledA[0]);  // Calculate the number of LEDs
  
  for (int A = 0; A < numLEDa; A++) {
    CircuitPlayground.setPixelColor(ledA[A], CircuitPlayground.colorWheel(color2));  // Set LED color
  }
  delay(40);
  CircuitPlayground.clearPixels();

  int ledB[] = {1, 3, 6, 8};
  int numLEDb = sizeof(ledB) / sizeof(ledB[0]); 
  for(int B = 0; B < numLEDb; B++) {
    CircuitPlayground.setPixelColor(ledB[B], CircuitPlayground.colorWheel(color));  // Set LED color
  }
  
  }
