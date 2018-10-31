#include <Keypad.h>
//////ПИНЫ МОТОРОВ///////
int pump_1 = 15;
int pump_2 = 17;
int pump_3 = 18;
int pump_4 = 16;
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

double mixingTimer = 0;
double timeOfTest = 5;
bool mixingTest = false;



//////////// ПРОВЕРКА МОТОРОВ ///////////
bool pump_test = false;
bool pump_switcher = false;
/////////////////////////////////////////



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
  char key = keypad.getKey();
  if(key){
    if (key == '1'){      
        if(mixingTimer == 0 && mixingTest == false && !pump_test){
          mixingTimer = timeOfTest * 5;
          mixingTest = true;
          Serial.println("Start mixing test");
        }        
    }else if(key == '*'){
       Serial.println(".......DEBUG MODE.......");
       pump_test = true;
    }else if(key == '#'){
       Serial.println(".......DEBUG MODE.......");
       pump_test = false;
    }
  }

  if(pump_test){
    if(key == '0' || key == '1' || key == '2' || key == '3'){
      pump_switcher = !pump_switcher;
      Serial.print(key); Serial.print(" motor set "); Serial.println(pump_switcher);
      digitalWrite(pumps[String(key).toInt()], pump_switcher);
    }
  }
  
  if(mixingTest == true){
    mixingTimer-=0.1;     
    Serial.print("Remaining ");
    Serial.print(mixingTimer);
    Serial.println(" sec.");
    if(mixingTimer <= 0){
      mixingTest = false; 
      mixingTimer = 0;
      digitalWrite(pumps[0], LOW);
      digitalWrite(pumps[1], LOW);
      digitalWrite(pumps[2], LOW);
      digitalWrite(pumps[3], LOW);
      Serial.println("Mixing test success!");
    }else{
    
        digitalWrite(pumps[0], HIGH);
        digitalWrite(pumps[1], HIGH);
        digitalWrite(pumps[2], HIGH);
        digitalWrite(pumps[3], HIGH);
     
     }         
  }
    
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
