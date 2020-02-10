#include <CytronMotorDriver.h>
#include <Wire.h>

//Moteur Gauche
#define PWM_left 9   
#define DIR_left 11 

//Moteur Droite
#define PWM_Right 6  
#define DIR_Right 7 

#define ADDR_ARDUINO 8

CytronMD motor_right(PWM_DIR, PWM_Right, DIR_Right);  // PWM 1 = Pin 3, DIR 1 = Pin 4.
CytronMD motor_left(PWM_DIR, PWM_left, DIR_left);  // PWM 1 = Pin 3, DIR 1 = Pin 4.

int PWM_Gauche, PWM_Droite; // 0 ... 255
int sens_Gauche, sens_Droite; // 0 = avant / 1 = arriere
int signel = 0;
int signer = 0;
int vitr = 0;
int vitl = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(ADDR_ARDUINO);
  Wire.onReceive(recv);

  PWM_Gauche = 0; PWM_Droite = 0;
  sens_Gauche = 0; sens_Droite = 0;  
  Serial.println("STARTING");
}

void loop() { delay(1); }

void recv(int numBytes) {
  if(numBytes == 4) {
    PWM_Gauche = Wire.read();
    PWM_Droite = Wire.read();
    sens_Gauche = Wire.read();
    sens_Droite = Wire.read();

    Serial.print("Gauche -> (");
    Serial.print(sens_Gauche);
    Serial.print(") ");
    switch(sens_Gauche) {
      case 1: //Roue Gauche Marche Avant
        signel =1;
        break;
      case 2: //Roue Gauche Marche Arriere
        signel =-1;
        break;
      default: //Arrêt du moteur (frein)
        signel =0;
        break;
    }
    //Serial.println(PWM_Gauche);
    
    //Serial.print("Droite -> (");
    //Serial.print(sens_Droite);
    //Serial.print(") ");
    switch(sens_Droite) {
      case 1: //Roue Droite Marche Avant
        signer=1;
        break;
      case 2: //Roue Droite Marche Arriere
        signer=-1;
        //Serial.print("marche arriere : ");
        break;
      default: //Arrêt du moteur (frein)
        signer=0;
        break;
    }
    //Serial.println(PWM_Droite);
    //Serial.print("\n");
    vitl=signel*PWM_Gauche;
    vitr=signer*PWM_Droite;

    motor_left.setSpeed(vitl);
    motor_right.setSpeed(vitr);
  }
  //Stream.flush();
  while(Wire.available()) Wire.read();
}
