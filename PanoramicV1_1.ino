// Arduino Panoramic V 1.0
// Tihomir Nedev March.2013

// DA OPRAVQ ADDRESS FIND, DIGIT CHECK!

#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Stepper.h>
#include <EEPROM.h>

const int stepsPerRevolution=200;
const float angle=1.8;
const byte gear_ratio=1;
Stepper motor(stepsPerRevolution,7,8,9,10);

/* Original Values for all 6 modes
Those are all original values for the specs of each module. 
*/
unsigned int ts[6]={10,10,10,10,10,10};
byte nf[6]={6,6,6,10,10,10};
unsigned int tb[6]={1,1,0,1,1,0};
boolean fb[6]={0,0,0,0,0,0};
unsigned int fa[6]={0,0,0,0,0,0};
unsigned int np[6]={1,1,1,1,1,0};
unsigned int tp[6]={0,0,0,0,0,0};
unsigned int nh[6]={0,3,0,0,3,0};
unsigned int ta[6]={1,1,0,1,1,0};
unsigned int pw[6]={0,0,0,120,120,0};
boolean ri[6]={0,0,0,1,1,0};
unsigned int bm[6]={0,0,0,0,0,0};
unsigned int If[6]={0,0,0,0,0,10};



byte focus_button;
byte shutter_button;
char keypad_input[5];
char memory_input[4];

int panorama_counter, exposure_counter, shot_counter, hdr_counter,i,current_value, address_index,p;
int current_mode=0;
/*
EEPROM Start bits of addresses for each value
Each value is saved 4 bytes for in the EEPROM, so that we could read
the four bytes. The values show the indices for starting bytes of each value
Therefore, read firt byte and 3 after. "0" means the value does not exist for that mode.
*/
byte ts_adr[6]={1,29,61,97,133,173};
byte nf_adr[6]={5,33,69,105,141,193};
byte tb_adr[6]={9,37,0,109,145,0};
byte fb_adr[6]={13,41,0,113,149,185};
byte fa_adr[6]={17,0,0,0,0,0};
byte np_adr[6]={21,53,85,121,161,0};
byte tp_adr[6]={25,57,89,0,165,0};
byte nh_adr[6]={0,45,0,0,153,189};
byte ta_adr[6]={185,49,0,117,157,0};
byte pw_adr[6]={0,0,0,101,137,0};
byte ri_adr[6]={0,0,0,129,169,0};
byte bm_adr[6]={0,0,0,0,0,177};
byte If_adr[6]={0,0,0,0,0,181};

byte value_contain[6][13] = {
  {1,2,3,4,9,6,7,0,0,0,0,0,0},
  {1,2,3,4,8,9,6,7,0,0,0},
  {1,2,6,7,0,0},
  {1,10,2,3,4,9,6,7,11,0,0},
  {1,10,2,3,4,8,9,6,7,11,0,0},
  {1,12,13,8,2,0,0}
  };


byte currentMode=0; // Store the value for the current mode 
boolean start_once=false;
boolean was_edited=false;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);



const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {A3,A2,A1,A0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, A4, A5};
//initialize an instance of class NewKeypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 



void setup() {
  Serial.begin(9600); 
  lcd.begin(16, 2);
  motor.setSpeed(60);
}  


void mode_1(){ //  360PANO 
   Serial.println("Mode 1");
     lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Shooting");
   delay(ts[0]*1000);  
   for(panorama_counter=0; panorama_counter<np[0]; panorama_counter++ ){
     Serial.println("Panorama");
       for(exposure_counter=0; exposure_counter<nf[0];exposure_counter++){
         if(fb[0]=true){
             digitalWrite(focus_button, HIGH);
             delay(1000);
             digitalWrite(focus_button, LOW);
       }
           digitalWrite(shutter_button,HIGH);
           Serial.println("Shoot");
           delay(100);
           digitalWrite(shutter_button,LOW);
           
         delay(ta[0]*1000);
         motor.step((360*gear_ratio)/(nf[0]*angle));
         delay(tb[0]*1000);
       }
       if(panorama_counter<np[0]){
          delay(tp[0]*1000);
       }  
   }
   digitalWrite(7,LOW);
   digitalWrite(8,LOW);
   digitalWrite(9,LOW);
   digitalWrite(10,LOW);
}
void mode_2(){ //  360PANO HDR
   Serial.println("Mode 2");
     lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Shooting");
   delay(ts[1]*1000);  
   for(panorama_counter=0; panorama_counter<np[1]; panorama_counter++ ){
       Serial.println("Panorama");
       for(exposure_counter=0; exposure_counter<nf[1];exposure_counter++){
         if(fb[1]=true){
             digitalWrite(focus_button, HIGH);
             delay(1000);
             digitalWrite(focus_button, LOW);
       }
       for(shot_counter=0;shot_counter<nh[1];shot_counter++){
           digitalWrite(shutter_button,HIGH);
           Serial.println("Shoot");
           delay(100);
           digitalWrite(shutter_button,LOW);
       }
         delay(ta[1]*1000);
         motor.step((360*gear_ratio)/(nf[1]*angle));
         delay(tb[1]*1000);
       }
       if(panorama_counter<np[1]){
          delay(tp[1]*1000);
       }  
   }
   digitalWrite(7,LOW);
   digitalWrite(8,LOW);
   digitalWrite(9,LOW);
   digitalWrite(10,LOW);
}

void mode_3(){  //  360-FR
  Serial.println("Mode 3");
  lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Shooting");
 delay(ts[2]*1000);  
   for(panorama_counter=0; panorama_counter<np[2]; panorama_counter++ ){
       for(exposure_counter=0; exposure_counter<nf[2];exposure_counter++){
           digitalWrite(shutter_button,HIGH);
           Serial.println("Shoot");
           delay(50);
           digitalWrite(shutter_button,LOW);
           motor.step((360*gear_ratio)/(nf[2]*angle));
       }
       if(panorama_counter<np[2]){
          delay(tp[2]*1000);
       }  
   }
   digitalWrite(7,LOW);
   digitalWrite(8,LOW);
   digitalWrite(9,LOW);
   digitalWrite(10,LOW);
}

void mode_4(){        // PARTIAL
  delay(ts[3]*1000);  
  Serial.println("Mode 4");
  lcd.clear();
   lcd.setCursor(0,0);
  lcd.print("Shooting");
   for(panorama_counter=0; panorama_counter<np[3]; panorama_counter++ ){
       Serial.println("Panorama");
       for(exposure_counter=0; exposure_counter<nf[3];exposure_counter++){
         if(fb[3]=true){
             digitalWrite(focus_button, HIGH);
             delay(1000);
             digitalWrite(focus_button, LOW);
       }
           digitalWrite(shutter_button,HIGH);
           Serial.println("Shoot");
           delay(100);
           digitalWrite(shutter_button,LOW);
           
         delay(ta[3]*1000);
         motor.step((pw[3]*gear_ratio)/(nf[3]*angle));
         delay(tb[3]*1000);
       }
       if(panorama_counter<np[3]){
          motor.step((360-pw[3])*gear_ratio/angle);
          delay(tp[3]*1000);
       }  
   }
   if(ri){
         motor.step((360-pw[3])*gear_ratio/angle);
   }
   digitalWrite(7,LOW);
   digitalWrite(8,LOW);
   digitalWrite(9,LOW);
   digitalWrite(10,LOW);
}

void mode_5(){           // PARTIAL HDR 
  Serial.println("Mode 5");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Shooting");
  delay(ts[4]*1000);  
   for(panorama_counter=0; panorama_counter<np[4]; panorama_counter++ ){
       Serial.println("Panorama");
       for(exposure_counter=0; exposure_counter<nf[4];exposure_counter++){
         if(fb[4]=true){
             digitalWrite(focus_button, HIGH);
             delay(1000);
             digitalWrite(focus_button, LOW);
       }
           for(shot_counter=0;shot_counter<nh[4];shot_counter++){
           digitalWrite(shutter_button,HIGH);
           Serial.println("Shoot");
           delay(100);
           digitalWrite(shutter_button,LOW);
       }
           
         delay(ta[4]*1000);
         motor.step((pw[4]*gear_ratio)/(nf[4]*angle));
         delay(tb[4]*1000);
       }
       if(panorama_counter<np[4]){
          motor.step((360-pw[4])*gear_ratio/angle);
          delay(tp[4]*1000);
       }  
   }
   if(ri){
         motor.step((360-pw[3])*gear_ratio/angle);
   }
   digitalWrite(7,LOW);
   digitalWrite(8,LOW);
   digitalWrite(9,LOW);
   digitalWrite(10,LOW);
}

void mode_6(){        // INTERVL
  Serial.println("Mode 6");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Shooting");
 delay(ts[5]*1000);  
       for(exposure_counter=0; exposure_counter<nf[5];exposure_counter++){
         if(fb[0]=true){
             digitalWrite(focus_button, HIGH);
             delay(1000);
             digitalWrite(focus_button, LOW);
       }
         if(nh[5]){
            for(hdr_counter=0;hdr_counter<nh[5];hdr_counter++){         
             digitalWrite(shutter_button,HIGH);
             Serial.println("Shoot");
             delay(100);
             digitalWrite(shutter_button,LOW);
           }
         }
         if(bm[5]){
           digitalWrite(shutter_button,HIGH);
           delay(bm[5]*1000);
           digitalWrite(shutter_button,LOW);
         }
         if(bm==false && nh==false){
           digitalWrite(shutter_button,HIGH);
           delay(100);
           digitalWrite(shutter_button,LOW);
         }
         delay(If[5]*1000);
             
  }
  digitalWrite(7,LOW);
   digitalWrite(8,LOW);
   digitalWrite(9,LOW);
   digitalWrite(10,LOW);
}

void value_edit(byte pointer_value){
// This Function edits a value. The pointer argument ranges for 1-13 for all parameters
   Serial.println("EDIT");
 byte pointer;
 pointer=value_contain[current_mode-1][pointer_value];
              Serial.print("current mode: ");
              Serial.println(current_mode); 
              Serial.print("Pointer: ");
              Serial.println(pointer);  
              Serial.print("Pointer_value: ");
              Serial.println(pointer_value);
              
  lcd.setCursor(0,0);
  value_print(pointer); 
  lcd.print("]:");
  lcd.setCursor(0,1);
  lcd.print("*:Next #:Clear"); 
  lcd.setCursor(3,0);
  while(pointer!=0){
    while( i<=4 && keypad_input[i-1]!='*'){ // Take four readings from the keypad    
     if(i==0){
    lcd.setCursor(0,0);
    value_print(pointer); 
    lcd.print("]:        ");
    lcd.setCursor(0,1);
    lcd.print("*:Next #:Clear"); 
    lcd.setCursor(3,0);
       
     }
       
       keypad_input[i]=keypad.waitForKey();
       if(keypad_input[i]!=NO_KEY){ // If there is a reading, store it it memmory
       
          
           lcd.print(keypad_input[i]);
            lcd.print("]:       ");
            lcd.setCursor((4+i),0);
            //Serial.println(i);
            i++;
      }
     
      if( (keypad_input[i-1]=='#') || ((keypad_input[4]>='0') && keypad_input[4]<='9')  ){ // If you press * or # you go back. The minutes cannnot be more than 59
          
          lcd.setCursor(0,0);
          value_print(pointer); 
          lcd.print("]:     ");
          lcd.setCursor(0,1);
          lcd.print("*:Next #:Clear"); 
          lcd.setCursor(3,0);
  
          keypad_input[0]=0;
          keypad_input[1]=0;
          keypad_input[2]=0;
          keypad_input[3]=0;
          keypad_input[4]=0;
          i=0;     
      }
     if(keypad_input[0]=='*') {
       
       
       i=0;
       pointer_value++;
       pointer=value_contain[current_mode-1][pointer_value];
       if(pointer==0)
         i=5;
        Serial.println("EDIT 0");
        Serial.println(pointer_value);
        was_edited=false;
     }
     
     if(keypad_input[i-1]=='*' && i>1){
       
       if(i==2){
         current_value =((int)keypad_input[0]-(int)'0');
         was_edited=true;
         pointer_value++;
       }
       else if(i==3){
         current_value =( ((int)keypad_input[0]-(int)'0')*10 + (int)keypad_input[1]-(int)'0' );
         was_edited=true;
         pointer_value++;
       }
       else if(i==4){
          current_value =( ((int)keypad_input[0]-(int)'0')*100 + ((int)keypad_input[1]-(int)'0')*10 + (int)keypad_input[2]-(int)'0' );
          was_edited=true;
          pointer_value++;
       }
       else if(i==5){
          current_value =( ((int)keypad_input[0]-(int)'0')*1000 + ((int)keypad_input[1]-(int)'0')*100 + ((int)keypad_input[2]-(int)'0')*10 + (int)keypad_input[3]-(int)'0' );
          was_edited=true;
          pointer_value++;
       }
      
       if(pointer==1 || pointer==3  || pointer==5 || pointer==9 || pointer==12 || pointer==13){
           if(current_value>=0 && current_value<=9999){
         i=5;
           }
           else {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Wrong Value");
            delay(2000);
            pointer_value--;
            Serial.println("EDIT POINTER 1");
            i=0;

           }
         }  
         
       if(pointer==6 || pointer==7  || pointer==8){
           if(current_value>0 && current_value<=9999){
              
               i=5;
           }
           else {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Wrong Value");
            delay(2000);
            pointer_value--;
            i=0;

           }
         }  
         
          if(pointer==4 || pointer==11){
           if(current_value>-1 && current_value<2){
            
               i=5;
           }
           else {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Wrong Value");
            pointer_value--;
            delay(2000);
            i=0; 
    
           }
         } 
        if(pointer==10){
           if(current_value>=2 && current_value<=358){
            
               i=5;
              
           }
           else {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Wrong Value");
            delay(2000);
            pointer_value--;
            i=0;
 
           }
         } 
         if(pointer==2){
           if(current_value>=2 && current_value<=180){
            
               i=5;
           }
           else {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Wrong Value");
            delay(2000);
            pointer_value--;
            i=0;

           }
         }
           
              pointer=value_contain[current_mode-1][pointer_value]; 
              Serial.print("Pointer: ");
              Serial.println(pointer);  
              Serial.print("Pointer_value: ");
              Serial.println(pointer_value);
     }   
           
       }
     
          if(was_edited) {
            Serial.print("WRITTEN TO EEPROM: ");
            if(keypad_input[0]!='*'){
            EEPROM.write(address_index, keypad_input[0]); 
            Serial.print(keypad_input[0]);
            EEPROM.write(address_index+1, 0);
            EEPROM.write(address_index+2, 0);
            EEPROM.write(address_index+3, 0);
            }
            if(keypad_input[1]!='*'){
            EEPROM.write((address_index +1 ), keypad_input[1]);
            Serial.print(keypad_input[1]);
            EEPROM.write(address_index+2, 0);
            EEPROM.write(address_index+3, 0);
            }
           if(keypad_input[2]!='*') {
            EEPROM.write((address_index +2 ), keypad_input[2]); 
            Serial.print(keypad_input[2]);
            EEPROM.write(address_index+3, 0);
           }
            if(keypad_input[3]!='*'){
            EEPROM.write((address_index +3 ), keypad_input[3]);
            Serial.println(keypad_input[3]);
            }
            pointer=value_contain[current_mode-1][pointer_value-1];
            EEPROM.write( (500+pointer), '1'); 
            Serial.print("Read from EEPROM: ");   
            Serial.print(EEPROM.read(address_index)-(int)'0');
            Serial.print(EEPROM.read(address_index+1)-(int)'0');
            Serial.print(EEPROM.read(address_index+2)-(int)'0');
            Serial.println(EEPROM.read(address_index+3)-(int)'0');
            Serial.print("Value:"); 
            Serial.print(pointer);
            Serial.print(" from mode:");
            Serial.print(current_mode);
            Serial.println(" changed ");  
            pointer=value_contain[current_mode-1][pointer_value];  
          }
   
          keypad_input[0]=0;
          keypad_input[1]=0;
          keypad_input[2]=0;
          keypad_input[3]=0;
          keypad_input[4]=0;
          i=0;   
        Serial.print("current value: ");  
        Serial.println(current_value); 
  }
}

void value_print(int pointer){
  // This functions is a slave of the edit function. It prints the right parameter and points at its EEPPROM address
  
  if(pointer==1)
  {
    lcd.print("Ts[");
    lcd.print(ts[current_mode-1]);
    address_index=ts_adr[current_mode-1];
    Serial.print("At ts address ");
    Serial.println(address_index);
  }
  else if(pointer==2)
  {
    lcd.print("Nf[");
    lcd.print(nf[current_mode-1]);
    address_index=nf_adr[current_mode-1];
    Serial.print("At Nf address ");
    Serial.println(address_index);
  }
  else if(pointer==3)
   {
    lcd.print("Tb[");
    lcd.print(tb[current_mode-1]);
    address_index=tb_adr[current_mode-1];
    Serial.print("At Tb address ");
    Serial.println(address_index);
  }
  else if(pointer==4)
   {
    lcd.print("Fb[");
    lcd.print(fb[current_mode-1]);
    address_index=fb_adr[current_mode-1];
    Serial.print("At Fb address ");
    Serial.println(address_index);
    
  }
   else if(pointer==5)
    {
    lcd.print("Fa[");
    lcd.print(fa[current_mode-1]);
    address_index=fa_adr[current_mode-1];
    Serial.print("At Fa address ");
    Serial.println(address_index);
  }
  else if(pointer==6)
   {
    lcd.print("Np[");
    lcd.print(np[current_mode-1]);
    address_index=np_adr[current_mode-1];
    Serial.print("At Np address ");
    Serial.println(address_index);
  }
  else if(pointer==7)
    {
    lcd.print("Tp[");
    lcd.print(tp[current_mode-1]);
    address_index=tp_adr[current_mode-1];
    Serial.print("At Tp address ");
    Serial.println(address_index);
  }
  else if(pointer==8)
    {
    lcd.print("Nh[");
    lcd.print(nh[current_mode-1]);
    address_index=nh_adr[current_mode-1];
    Serial.print("At Nh address ");
    Serial.println(address_index);
  }
  else if(pointer==9)
  {
    lcd.print("Ta[");
    lcd.print(ta[current_mode-1]);
    address_index=ta_adr[current_mode-1];
    Serial.print("At Ta address ");
    Serial.println(address_index);
  }
  else if(pointer==10)
    {
    lcd.print("Pw[");
    lcd.print(pw[current_mode-1]);
    address_index=pw_adr[current_mode-1];
    Serial.print("At Pw address ");
    Serial.println(address_index);
  }
 else if(pointer==11)
    {
    lcd.print("Ri[");
    lcd.print(ri[current_mode-1]);
    address_index=ri_adr[current_mode-1];
    Serial.print("At Ri address ");
    Serial.println(address_index);
  }
  else if(pointer==12)
   {
    lcd.print("Bm[");
    lcd.print(bm[current_mode-1]);
    address_index=bm_adr[current_mode-1];
    Serial.print("At Bm address ");
    Serial.println(address_index);
  }
  else if(pointer==13)
   {
    lcd.print("If[");
    lcd.print(If[current_mode-1]);
    address_index=If_adr[current_mode-1];
    Serial.print("At If address ");
    Serial.println(address_index);
  }
  
}

void value_read(){
  // This function checks if there was a change in parameters from the original values. If there was such a change, it takes the value
  // and stores it at the operating memory. It works under current mode, so it has to be runnerd every time the mode is changed. 
 int digit_check=9999;
 int pointer;
 
 for(p=0;p<13;p++){ // changed with pointer!
   pointer=value_contain[current_mode-1][p];
 if (EEPROM.read(500+pointer)!=0 && EEPROM.read(500+pointer)!=255 && pointer!=0){ // 501-513 are storage cells for parameters 1-13. If they are true, means that there was a change in the value. 
   address_index=address_find(pointer);
   Serial.print("ADDRESS OF CHANGED: ");
   Serial.println(address_index);
   
   if(EEPROM.read(address_index)==0 || EEPROM.read(address_index)==255){
    digit_check=0; 
   }
   
   if(EEPROM.read(address_index)){
   memory_input[0]=EEPROM.read(address_index);
   digit_check=1;
   }
    else memory_input[0]=0;
   
   if(EEPROM.read(address_index+1)){
   memory_input[1]=EEPROM.read(address_index+1);
   digit_check=2;
   }
    else memory_input[1]=0;
   if(EEPROM.read(address_index+2)){
   memory_input[2]=EEPROM.read(address_index+2);
   digit_check=3;
   }
    else memory_input[2]=0;
   if(EEPROM.read(address_index+3)){
   digit_check=4;
   memory_input[3]=EEPROM.read(address_index+3);
   }
   else memory_input[3]=0;
     // CALCULATE THE VALUE AND STORE IT IN current_value

   
    if (digit_check==0) {
      was_edited=0;
      current_value=0;
    }
    else if(digit_check==1 ){
       current_value =((int)memory_input[0]-(int)'0');
    }
    else if(digit_check==2){
       current_value =( ((int)memory_input[0]-(int)'0')*10 + (int)memory_input[1]-(int)'0' );
    }
    else if(digit_check==3){
        current_value =( ((int)memory_input[0]-(int)'0')*100 + ((int)memory_input[1]-(int)'0')*10 + (int)memory_input[2]-(int)'0' );
    }
     else if(digit_check==4){
        current_value =( ((int)memory_input[0]-(int)'0')*1000 + ((int)memory_input[1]-(int)'0')*100 + ((int)memory_input[2]-(int)'0')*10 + (int)memory_input[3]-(int)'0' );
     }
     
      if(digit_check){
       Serial.print("Changed Value READ FROM EEPROM: ");
       Serial.println(current_value);
       Serial.print("digit_check: ");
       Serial.println(digit_check);
       value_put(pointer);
      }
 }  
  
 } 
}

int address_find(int pointer){
 // This function find the EEPROM address the value in the current mode. 
      if(pointer==1)
        address_index=ts_adr[current_mode-1];
      else if(pointer==2)
        address_index=nf_adr[current_mode-1];
      else if(pointer==3)
        address_index=tb_adr[current_mode-1];
      else if(pointer==4)
       address_index=fb_adr[current_mode-1];
       else if(pointer==5)
        address_index=fa_adr[current_mode-1];
      else if(pointer==6)
        address_index=np_adr[current_mode-1];
      else if(pointer==7)
        address_index=tp_adr[current_mode-1];
      else if(pointer==8)
        address_index=nh_adr[current_mode-1];
      else if(pointer==9)
        address_index=ta_adr[current_mode-1];
      else if(pointer==10)
        address_index=pw_adr[current_mode-1];
     else if(pointer==11)
        address_index=ri_adr[current_mode-1];
      else if(pointer==12)
        address_index=bm_adr[current_mode-1];
      else if(pointer==13)
        address_index=If_adr[current_mode-1];  
}

void value_put(int pointer){ // This changes the original values with edited ones, for the current mode.

  if(pointer==1)
    {
      ts[current_mode-1]=current_value;
      Serial.print("ts was changed to ");
      Serial.println(ts[current_mode-1]);
    }
    else if(pointer==2)
    {
       nf[current_mode-1]=current_value;
       Serial.print("nf was changed to ");
       Serial.println(nf[current_mode-1]);
    }
    else if(pointer==3)
     {
       tb[current_mode-1]=current_value;
       Serial.print("tb was changed to ");
       Serial.println(tb[current_mode-1]);
    }
    else if(pointer==4)
     {
       fb[current_mode-1]=current_value;
       Serial.print("fb was changed to ");
       Serial.println(fb[current_mode-1]);
    }
     else if(pointer==5)
      {
      fa[current_mode-1]=current_value;
      Serial.print("fa was changed to ");
      Serial.println(fa[current_mode-1]);
    }
    else if(pointer==6)
     {
       np[current_mode-1]=current_value;
       Serial.print("np was changed to ");
       Serial.println(np[current_mode-1]);
    }
    else if(pointer==7)
      {
       tp[current_mode-1]=current_value;
       Serial.print("tp was changed to ");
       Serial.println(tp[current_mode-1]);
    }
    else if(pointer==8)
      {
       nh[current_mode-1]=current_value;
       Serial.print("nh was changed to ");
       Serial.println(nh[current_mode-1]);
    }
    else if(pointer==9)
    {
      ta[current_mode-1]=current_value;
      Serial.print("ta was changed to ");
      Serial.println(ta[current_mode-1]);
    }
    else if(pointer==10)
      {
       pw[current_mode-1]=current_value;
       Serial.print("pw was changed to ");
       Serial.println(pw[current_mode-1]);
    }
   else if(pointer==11)
      {
       ri[current_mode-1]=current_value;
       Serial.print("ri was changed to ");
       Serial.println(ri[current_mode-1]);
    }
    else if(pointer==12)
     {
      bm[current_mode-1]=current_value;
      Serial.print("bm was changed to ");
      Serial.println(bm[current_mode-1]);
    }
    else if(pointer==13)
     {
       If[current_mode-1]=current_value;
       Serial.print("if was changed to ");
       Serial.println(If[current_mode-1]);
    }
    
  }
  // Ostava da napravq funkciite koito da podkarvat Modovete i nulirat EEPROMA, kakto i main-a
   
   
  void value_reset() {       // resets all values to original 
  
    for(i=0;i<200;i++){
       EEPROM.write(i, 0); 
    }
    
    for(i=1;i<14;i++){
       EEPROM.write(500+i, 0); 
    }

}

void start_mode(byte mode){  // starts the mode

  if(mode==1)
    mode_1();
  else if (mode==2)
    mode_2();
  else if (mode==3)
    mode_3();
  else if (mode==4)
    mode_4();
  else if (mode==5)
    mode_5();
  else if (mode==6)
    mode_6(); 
  
}

void initialization_print(int mode){ // prints the mode iow prints -360pano...

if(mode==1)
    lcd.print("1-360PANO");
  else if (mode==2)
    lcd.print("2-360-HDR");
  else if (mode==3)
    lcd.print("3-360-CR");
  else if (mode==4)
    lcd.print("4-PARTIAL");
  else if (mode==5)
    lcd.print("5-PAR-HDR");
  else if (mode==6)
    lcd.print("6-INTERVL"); 
  
  
}

void loop(){ 
 
  keypad_input[0]=0;
  keypad_input[1]=0;
  keypad_input[2]=0;
  keypad_input[3]=0;
  keypad_input[4]=0;
  
  if(start_once==false){ // run only after it is turned on
  
    if( EEPROM.read(0)==0 || EEPROM.read(0)==255)
      EEPROM.write(0,1);
      
    current_mode=EEPROM.read(0);
    start_once=true;  
  }
  value_read();
  Serial.println("Values were read");
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Mode: ");
  initialization_print(current_mode);
  lcd.setCursor(0,1);
  lcd.print("1:OK *:Ch #:Ed");
  
  while(keypad_input[0]!='#' && keypad_input[0]!='*' && keypad_input[0]!='1' && keypad_input[0]!='3') {
    
    keypad_input[0]=keypad.waitForKey();
    // TO ADD REMOTE CONTROL HERE!!!
  }
  
    if(keypad_input[0]=='1') {
      if(current_mode!=EEPROM.read(0)){
        EEPROM.write(0, current_mode); // if you have started different mode, write that in the memory 
        Serial.println("Mode changed in memory");
      }
      Serial.println("Mode initialized"); 
      start_mode(current_mode); 
      
    }
    else if(keypad_input[0]=='#'){
     value_edit(0);
   }
    else if(keypad_input[0]=='3'){
      value_reset();
      Serial.println("Original values restored");
    } 
    else if(keypad_input[0]=='*') {      
      current_mode++;
      Serial.println("Next mode"); 
      value_read();
      Serial.println("Values were read AGAIN");
     if(current_mode>6){
        current_mode=1;
      } 
    }
   
   
}
