#include <Keypad.h>
//////ПИНЫ МОТОРОВ///////
int pump_1 = 17;
int pump_2 = 16;
int pump_3 = 18;
int pump_4 = 15;
int pumps[] = {pump_1, pump_2, pump_3, pump_4};
/////////////////////////

///////ПИНЫ RGB СВЕТОДИОДА//////////
int RED = 9;
int BLUE = 10;
int GREEN = 11;
int colors[] = {RED, BLUE, GREEN};

double firstIntens = 255;
double secondIntens = 0;

int pulseIter = 0;
double intensOffset = 0.001;
////////////////////////////////////


const byte ROWS = 4; // 4 строки
const byte COLS = 4; // 4 столбца
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
}; 
byte rowPins[ROWS] = {12, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//Старые переменные
/*double mixingTimer = 0;
double timeOfTest = 20;
bool mixingTest = false;*/

//Новые переменные
bool mixing = false;
double mixingTimer = 0;
double maxMixingTime = 0;



//////////// ПРОВЕРКА МОТОРОВ ///////////
bool pump_test = false;
bool pump_switcher = false;
/////////////////////////////////////////

void mix(double t0, double t1, double t2, double t3){  
  
  maxMixingTime = t0;
  
  if(t1 > maxMixingTime){
    maxMixingTime = t1;
  }else if(t2 > maxMixingTime){
    maxMixingTime = t2;
  }else if(t3 > maxMixingTime){
    maxMixingTime = t3;
  }
  mixingTimer = maxMixingTime;
  Serial.print("Start mixing. Time: "); Serial.print(maxMixingTime); Serial.println("sec.");
  if(t0 != 0){
    digitalWrite(pumps[0], HIGH);
  }
  if(t1 != 0){
    digitalWrite(pumps[1], HIGH);
  }
  if(t2 != 0){
    digitalWrite(pumps[2], HIGH);
  }
  if(t3 != 0){
    digitalWrite(pumps[3], HIGH);
  }
  
  
  while(mixingTimer > 0.1){
      mixingTimer -= 0.1;
      Serial.print("Remaining ");
      Serial.print(mixingTimer);
      Serial.println(" sec.");

      if(mixingTimer <= (maxMixingTime - t0)){
        Serial.println("Pump[0]: OFF");
        digitalWrite(pumps[0], LOW);
      }
      if(mixingTimer <= (maxMixingTime - t1)){
        digitalWrite(pumps[1], LOW);
      }
      if(mixingTimer <= (maxMixingTime - t2)){
        digitalWrite(pumps[2], LOW);
      }
      if(mixingTimer <= (maxMixingTime - t3)){
        digitalWrite(pumps[3], LOW);
      }
      
      delay(100);
  }

  for(int i = 0; i < 4; i++){
    digitalWrite(pumps[i], LOW);
  }
  Serial.println("Mixing success!");
  mixing = false;
}

/*void mix(double t0, double t1, double t2, double t3){
  if(mixingTest == true) {  
      
      while (mixingTimer > 0){
        mixingTimer-=0.1;     
        Serial.print("Remaining ");
        Serial.print(mixingTimer);
        Serial.println(" sec.");
        if (t0 == mixingTimer) digitalWrite(pumps[0], LOW);
        if (t1 == mixingTimer) digitalWrite(pumps[1], LOW);
        if (t2 == mixingTimer) digitalWrite(pumps[2], LOW);
        if (t3 == mixingTimer) digitalWrite(pumps[3], LOW);
      }
      if (mixingTimer <= 0){
        Serial.println("Mixing test success!");
        mixingTest = false; 
        mixingTimer = 0;
        digitalWrite(pumps[0], LOW);
        digitalWrite(pumps[1], LOW);
        digitalWrite(pumps[2], LOW);
        digitalWrite(pumps[3], LOW);
      }
     } 
}*/
void setup(){
    Serial.begin(9600);
  //////////НАСТРОЙКА НАСОСОВ/////////////
    for(int i = 0; i < 4; i++){
      pinMode(pumps[i], OUTPUT);
      digitalWrite(pumps[i], LOW);
    }
  //////////////////////////////////////////
  
  ////////////НАСТРОЙКА RGB СВЕТОДИОДА//////////
    for(int i = 0; i < 2; i++){
      pinMode(colors[i], OUTPUT);
      if(i == 0){
        analogWrite(colors[i], 255);
      }else{
        analogWrite(colors[i], 0);
      }
    }
  //////////////////////////////////////////////
}

void loop(){

  if(!mixing){
    char key = keypad.getKey();
  
    switch(key){
      case '1':
          mixing = true;
          mix(30, 20, 12, 6); // test mix
          break;
      case '2':
          mixing = true;
          mix(30, 0, 0, 0); // mojito
          break;
      case '3':
          mixing = true;
          mix(30, 20, 12, 6);
          break;
      case '*':
          Serial.println(".......DEBUG MODE ON.......");
          pump_test = true;
          break;
      case '#':
          Serial.println(".......DEBUG MODE OFF.......");
          pump_test = false;
          break;
      
      default:
         break;  
    }

    if(pump_test){
      if(key == '0' || key == '1' || key == '2' || key == '3'){
        pump_switcher = !pump_switcher;
        Serial.print(key); Serial.print(" motor set "); Serial.println(pump_switcher);
        digitalWrite(pumps[String(key).toInt()], pump_switcher);
      }
    }
  }
  
  /*if(key){
    if (key == '1'){      
      
        if(mixingTimer == 0 && mixingTest == false && !pump_test){
          mixingTimer = timeOfTest * 5;
          mixingTest = true;
          digitalWrite(pumps[0], HIGH);
          digitalWrite(pumps[1], HIGH);
          digitalWrite(pumps[2], HIGH);
          digitalWrite(pumps[3], HIGH); 
          Serial.println("Start mix 1");
          double t0 = 25;
          double t1 = 50;
          double t2 = 75;
          double t3 = 100;
          mix(20,t0,t1,t2,t3);
        }        
    }else if(key == '*'){
       Serial.println(".......DEBUG MODE ON.......");
       pump_test = true;
    }else if(key == '#'){
       Serial.println(".......DEBUG MODE OFF.......");
       pump_test = false;
    }
  }*/

  
  

    
///////////////////// ПУЛЬСАЦИЯ /////////////////////////// 

  if(firstIntens > 0 && secondIntens < 255){ 
    firstIntens-=intensOffset; 
    secondIntens+=intensOffset; 
    analogWrite(colors[pulseIter], firstIntens); 
    if(pulseIter < 2){ 
      analogWrite(colors[pulseIter+1], secondIntens); 
    }else{ 
      analogWrite(colors[0], secondIntens); 
    } 
  }else{ 
    if(pulseIter == 2){ 
      pulseIter = 0; 
    }else{ 
      pulseIter++; 
    } 
    firstIntens = 255; 
    secondIntens = 0; 
  } 

/////////////////////////////////////////////////////////// 
}
