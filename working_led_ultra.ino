#define trigPin0 13
#define echoPin0 12
#define trigPin1 11
#define echoPin1 10
#define trigPin2 9
#define echoPin2 8
//pin for switch to change color mode
#define colorSwitch 2

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif


// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      60


int tasterStatus = LOW;           // aktuelles Signal vom Eingangspin
int tasterGedrueckt = 0;          // abfragen ob Taster gedrückt war
int lichtmodus = 0;               // festlegen der verschiedenen Lichtprogramme
int entprellZeit = 250;           // Zeit für Entprellung, anpassen!
 
unsigned long tasterZeit = 0;     // Zeit beim drücken des Tasters
unsigned long ledMillis = 0;      // Zeit für den Blinkrythmus der LED

int red_val, green_val, blue_val;
int color_set, leds;


// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 250; // delay 

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.

  Serial.begin (9600);

  //switch pin as input
  pinMode(colorSwitch, INPUT);
 
  pinMode(trigPin0, OUTPUT);
  pinMode(echoPin0, INPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  attachInterrupt(digitalPinToInterrupt(colorSwitch), changeColor, HIGH);
}

void loop() {
  float duration0, distance0;
  long duration1, distance1;
  long duration2, distance2;

    
  digitalWrite(trigPin0, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin0, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin0, LOW);
  duration0 = pulseIn(echoPin0, HIGH);
  distance0 = (duration0/2) / 29.1;
  //Serial.print(distance0);
  //Serial.println(" cm Ultrasonic 0");

digitalWrite(trigPin1, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1/2) / 29.1;
  //Serial.print(distance1);
  //Serial.println(" cm Ultrasonic 1");

digitalWrite(trigPin2, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2/2) / 29.1;
  //Serial.print(distance2);
  //Serial.println(" cm Ultrasonic 2");


/*  for(int i=0;i<NUMPIXELS;i++){
//    pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(distance1*2,distance0*2,distance2*2)); // Moderately bright green color.
  }*/


  if(distance2 > 100){
    distance2 = 100;
  }

  // if(distance1 > 100){
  //  distance2 = 100;
  //}

   int leds = distance0 / 1.75;
   int color_set = distance2 * 2.55;


/*************************************************************************************************/
  // Lesen des tasterPin
  tasterStatus = digitalRead(colorSwitch);

 
  // Wenn der Taster gedrückt ist...
  if (tasterStatus == HIGH)
  {
    tasterZeit = millis();      // aktualisiere tasterZeit
    tasterGedrueckt = 1;        // speichert, dass Taster gedrückt wurde
  }
 
  // Wenn die gewählte entprellZeit vergangen ist und der Taster gedrückt war...
  if ((millis() - tasterZeit > entprellZeit) && tasterGedrueckt == 1)
    {
      
      if (lichtmodus < 4){
        lichtmodus++;  // lichtmodus wird um +1 erhöht
      }

      if (lichtmodus == 4){
        lichtmodus = 0;
      }
                         
      tasterGedrueckt = 0;      // setzt gedrückten Taster zurück
    }

    //Serial.println(distance1);
/*************************************************************************************************/

    for(int i=0;i<NUMPIXELS;i++){
//    pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
    }

  /*if (distance0 < 100.0) {
   for(int i=0;i<leds;i++){
      pixels.setPixelColor(i, pixels.Color(color,0,0)); // Moderately bright green color.
   }

   // pixels.setPixelColor(10, pixels.Color(0,distance,0)); // Moderately bright green color
  }*/


Serial.println(lichtmodus);

//(red_val/(distance1 * 2.55),green_val/(distance1 * 2.55),blue_val/(distance1 * 2.55)));
pixels.setPixelColor(0, pixels.Color(255,0,0));

  if(lichtmodus==0){
    pixels.setPixelColor(0, pixels.Color(0,0,0));
    for(int i=1;i<leds; i++){
      pixels.setPixelColor(i, pixels.Color(red_val,green_val,blue_val));
    } 
  } else if  (lichtmodus == 1) {
    pixels.setPixelColor(0, pixels.Color(255,0,0));
    red_val = color_set;
    for(int i=1;i<leds; i++){
      pixels.setPixelColor(i, pixels.Color(red_val,green_val,blue_val));
    } 
  } else if(lichtmodus == 2){
    pixels.setPixelColor(0, pixels.Color(0,255,0));
    green_val = color_set;
      for(int i=1;i<leds; i++){
        pixels.setPixelColor(i, pixels.Color(red_val,green_val,blue_val));
      } 
  } else if(lichtmodus==3){
    pixels.setPixelColor(0, pixels.Color(0,0,255));
    blue_val = color_set;
     for(int i=0;i<leds; i++){
        pixels.setPixelColor(i, pixels.Color(red_val,green_val,blue_val));
      } 
  }
  
    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  
}

void changeColor(){
    tasterZeit = millis();      // aktualisiere tasterZeit
    tasterGedrueckt = 1;        // speichert, dass Taster gedrückt wurde
 
  // Wenn die gewählte entprellZeit vergangen ist und der Taster gedrückt war...
  if ((millis() - tasterZeit > entprellZeit) && tasterGedrueckt == 1)
    {
      
      if (lichtmodus < 4){
        lichtmodus++;  // lichtmodus wird um +1 erhöht
      }

      if (lichtmodus == 4){
        lichtmodus = 0;
      }
                         
      tasterGedrueckt = 0;      // setzt gedrückten Taster zurück
    }
}


/* leo_hanke@web.de
 *  alex.schwarz@hotmail.de
 *  goessling32@gmail.com
 *  leonaldejohann@gmail.com
 */
