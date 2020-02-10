
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include <WebServer.h>
#include <WiFiUdp.h>


//Avec Roballs
//const char* ssid = "ENSIM'Elec";                // SSID du Wifi (si avec téléphone damien: Honor 7X)
//const char* password = "milharausecours!2018";  // mot de passe du WiFi (si avec téléphone damien: 69f73dc8fd79)
//Avec Kiroulpa
//const char* ssid = "Kiroulpa";
//const char* password = "Milharausecours!2018";
//Avec MilhabotGaming
//const char* ssid = "MilhabotGaming";
//const char* password = "Milharausecours!2018";
//Pour avec téléphone Abdou
//const char* ssid = "iPhone de Abderrahman";
//const char* password = "989awn3esgghq";
//Pour avec PC Abdou-VR
//const char* ssid = "abdou-VR";
//const char* password = "989awn3esgghq";

//Pour avec Tel David
const char* ssid = "OnePlus7Pro_DavidHPW";
const char* password = "!YouKnowWhoI4m";

//const char* ssid="Your SSID";
//const char* password = "Your Password";

//LED strip declaration
const int NB_LED = 4;
const int PIN_LED = 26;



Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(NB_LED, PIN_LED); //number of LEDs, pin


Servo myservo_1;  // create servo object to control a servo
Servo myservo_2;  // create servo object to control a servo
const int SERVO_1 = 14;
const int SERVO_2 = 27;
int valAngle_1 = 50;    // angle
int valAngle_2 = 180;    // angle


char packetBuffer[512]; //buffer to hold incoming and outgoing packets
unsigned int localPort = 4214;      // local port to listen for UDP packets
WiFiUDP Udp;
bool Bonmsg = false;

void setup() {

  /*led strip initialisation*/
  pinMode(PIN_LED, OUTPUT);
  ledStrip.begin();
  ledStrip.setBrightness(125); //0 to 255

  delay(1000);
  myservo_1.attach(SERVO_1);  // attaches the servo on pin 13 to the servo object
  myservo_2.attach(SERVO_2);  // attaches the servo on pin 10 to the servo object



  Serial.begin(9600);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid,password);

  Serial.println();
  Serial.print("Connecting");

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      WiFi.begin(ssid,password);
      Serial.print(".");        
  }

 
  Serial.println();

  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );


   Serial.print("Udp server started at port ");
  Serial.println(localPort);
  Udp.begin(localPort);
 


   /*
   // on definit ce qui doit etre fait lors de l'appel des routes (route = /on)

  // activation du relais
  server.on("/on", [](){
    Serial.write(relON, sizeof(relON));
    server.send(200, "text/plain", "motor is ON");
    Serial.println("motor is ON");
  }); 

  // désactivation du relais
  server.on("/off", [](){
    Serial.write(relOFF, sizeof(relOFF));
    server.send(200, "text/plain", "motor is OFF");
    Serial.println("motor is OFF");
  });
  
  // on commence a ecouter les requetes venant de l'exterieur
  server.begin();
  Serial.println("HTTP server started");
    */

  

}
  
void loop() {
 


 int noBytes = Udp.parsePacket();

  
  if ( noBytes ) {

    Serial.print(":Packet of ");
    Serial.print(noBytes);
    Serial.print(" received from ");
    Serial.print(Udp.remoteIP());
    Serial.print(":");
    Serial.println(Udp.remotePort());

    
    // We've received a packet, read the data from it
    Udp.read(packetBuffer,noBytes); // read the packet into the buffer
    
    if(strcmp("/on",packetBuffer) == 0)
    {
      // display the packet contents
      Serial.println(packetBuffer);
      Bonmsg = true;
      
    }
    else
    {
      Serial.println("Je recois le mauvais message");
      
    }
    for (int i=0; i<512;i++)
      {packetBuffer[i]=NULL;} 
      
  }

  if(Bonmsg){
    //Programme de déploiement
    
    //servo_1
    delay(1000);
    myservo_1.write(valAngle_1);              // tell servo to go to position in variable 'pos'
                  // sets the servo position according to the scaled value
    delay(1000);                           // waits for the servo to get there
    //myservo_1.detach();

  //servo_2
  myservo_2.write(valAngle_2);                 // sets the servo position according to the scaled value
  delay(1000);                           // waits for the servo to get there
  //myservo_2.detach();
  
  //animations

   while(true) {
      alternateColor4(0xFFFFFF, 0x000000, 0x000000, 0x000000, 500);
      Serial.println();
      delay(0);  
    } // En raison de rangement du phare à enlever apres

    //Rangement du phare  
    delay(5000);
    
    for(int i=valAngle_2;i>=0;i=i-1){
      myservo_2.write(i);
      delay(50);
    }
    for(int i=valAngle_1;i<=180;i=i+1){
      myservo_1.write(i);
       delay(50);
    }
    myservo_1.detach();
    myservo_2.detach();
    
     while(1){}
    
    
    
  }
   
}

// alternate between four colors on even and odd LEDs
void alternateColor4(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4, uint8_t wait) {
  for(uint16_t i=0; i<ledStrip.numPixels(); i++) {
    if(i%4 == 0) { // set even LED to color 1
      ledStrip.setPixelColor(i, c1);
    }
    if(i%4 == 1) { // set even LED to color 2
      ledStrip.setPixelColor(i, c2);
    }
    if(i%4 == 2) { // set even LED to color 3
      ledStrip.setPixelColor(i, c3);
    }
    if(i%4 == 3) { // set even LED to color 4
      ledStrip.setPixelColor(i, c4);
    }
    
  }
 
  ledStrip.show(); // apply the colors
  delay(wait);

  for(uint16_t i=0; i<ledStrip.numPixels(); i++) {
     if(i%4 == 0) { // set even LED to color 2
      ledStrip.setPixelColor(i, c2);
    }
    if(i%4 == 1) { // set even LED to color 3
      ledStrip.setPixelColor(i, c3);
    }
    if(i%4 == 2) { // set even LED to color 4
      ledStrip.setPixelColor(i, c4);
    }
    if(i%4 == 3) { // set even LED to color 1
      ledStrip.setPixelColor(i, c1);
    }
  }
 
  ledStrip.show(); // apply the colors
  delay(wait);

  for(uint16_t i=0; i<ledStrip.numPixels(); i++) {
     if(i%4 == 0) { // set even LED to color 3
      ledStrip.setPixelColor(i, c3);
    }
    if(i%4 == 1) { // set even LED to color 4
      ledStrip.setPixelColor(i, c4);
    }
    if(i%4 == 2) { // set even LED to color 1
      ledStrip.setPixelColor(i, c1);
    }
    if(i%4 == 3) { // set even LED to color 2
      ledStrip.setPixelColor(i, c2);
    }
  }
 
  ledStrip.show(); // apply the colors
  delay(wait);
 
  for(uint16_t i=0; i<ledStrip.numPixels(); i++) {
     if(i%4 == 0) { // set even LED to color 4
      ledStrip.setPixelColor(i, c4);
    }
    if(i%4 == 1) { // set even LED to color 1
      ledStrip.setPixelColor(i, c1);
    }
    if(i%4 == 2) { // set even LED to color 2
      ledStrip.setPixelColor(i, c2);
    }
    if(i%4 == 3) { // set even LED to color 3
      ledStrip.setPixelColor(i, c3);
    }
  }
 
  ledStrip.show(); // apply the colors
  delay(wait);


  
  
}
