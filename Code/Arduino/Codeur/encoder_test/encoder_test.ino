#include <FlexiTimer2.h>


/**
 * Test encodeur
 * 
 * Amélioration: 
 * Rajoute de digitalWriteFast pour permettre la lecture rapide lors de l'interpution (digitalWrite() 6280nS  vs  digitalWriteFast() 125nS.)
 *  permet d’améliorer la rapidité de lecture et d’écriture des entrées digitales pour minimiser le temps passé dans la routine d’interruption
 */
const int PULSE = 2;
const int DIR = 3;

volatile long temps; // déclaration variable volatile = stockée en RAM
volatile long tick; // tick codeur

#define CANDENCE_MS_LOG 2000    // 2s

void setup() {
  Serial.begin(9600);
  pinMode(PULSE, INPUT);
  pinMode(DIR, INPUT);
  attachInterrupt(digitalPinToInterrupt(PULSE), read_rotary, RISING);

  reset();

  //Logger debug purpose
  FlexiTimer2::set(CANDENCE_MS_LOG, 1/1000., loggerDebug);  // rés 1ms);
  FlexiTimer2::start();
}

void loop() {
  
  raspSerial();
}
// appel à cette fonction quand y a une pulse (interpution sur la broche 0 (D2)

// On regarde l'état de la direction lorsqu'il y a des impulsations, 
//  quand l'état de la direction est au niveau HAUT on décrémente le tick (sens inverse), 
//  et dans le cas contraire on inrémente

// read bit on DIR on D3
void read_rotary(void)   {
  if (digitalRead(DIR) == HIGH) tick--;
    else tick++;
}

// reset
void reset() {
  tick = 0;
  temps = micros();
}

// debugger
void loggerDebug() {
  Serial.println("===== DEBUG LOG =====");
  temps = micros();
  Serial.print("temps = "); 
  Serial.println(temps);
  Serial.print("tick = ");
  Serial.println(tick);
}

// récupérer des données par la rasp
void raspSerial() {
 if (Serial.available())  
  { 
    int data = Serial.read(); 
    switch (data) 
    { 
        case 'C' : 
          temps = micros(); 
          Serial.print("?"); 
          Serial.print(tick); 
          Serial.print(":"); 
          Serial.print(temps); 
          Serial.println(";"); 
          break; 
        case 'R' : 
           reset(); 
           break; 
     }    
  } 
}
