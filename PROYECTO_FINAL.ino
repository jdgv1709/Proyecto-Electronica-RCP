//Juan David Galan Vargas- Adriana Maria Velasquez Medina
//Protecto Electronica para Ciencias 2023-1
//Entrenador Electronico de Reanimación Cardio-Pulmonar

// incluir librerias necesarias:

#include <LiquidCrystal.h> //Libreria para el LCD
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most accurate BPM math
#include <PulseSensorPlayground.h>     //Libreria para el XD58C

//Confuguración XD-58C
const int PulseWire = 0;       // Señal RCP al A0 medida por el XD58C
const int LED13 = 13;          // on-board Arduino LED
int Threshold1 = 550;           // Determine which Signal to "count as a beat" and which to ignore                               
PulseSensorPlayground pulseSensor;  // Crea objeto para medir el RCP

//Configuracion LCD
// Crea LCD objeto. Parametros: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//Configuración AD8232
/*
 * VARIABLES
 * count: variable to hold count of rr peaks detected in 10 seconds
 * flag: variable that prevents multiple rr peak detections in a single heatbeat
 * hr: HeartRate (initialised to 72)
 * hrv: Heart Rate variability (takes 10-15 seconds to stabilise)
 * instance1: instance when heart beat first time
 * interval: interval between second beat and first beat
 * timer: variable to hold the time after which hr is calculated
 * value: raw sensor value of output pin
 */
long instance1=0, timer;
double hrv =0, hr = 72, interval = 0;
int value = 0, count = 0;  
bool flag = 0;
#define shutdown_pin 10 
#define threshold 100 // to identify R peak
#define timer_value 10000 // 10 seconds timer to calculate hr

void setup() 
{
  // set up the LCD's number of columns and rows:
	lcd.begin(16, 2);
	// Clears the LCD screen
	lcd.clear();

  Serial.begin(9600);

  pinMode(8, INPUT); // Setup for leads off detection LO +
  pinMode(9, INPUT); // Setup for leads off detection LO -

  pinMode (7, OUTPUT);    // Input
  pinMode (6, OUTPUT);    // Input conectados a los motores

  // Configure the PulseSensor object, by assigning our variables to it
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       // Blink on-board LED with heartbeat
  pulseSensor.setThreshold(Threshold1);   

  if (pulseSensor.begin()) {
    Serial.println("PulseSensor object created!");
  }
}
void loop() 
{
    int myBPM = pulseSensor.getBeatsPerMinute();      // Calculates RCP
    if((digitalRead(8) == 1)||(digitalRead(9) == 1))
    {
      Serial.println("leads off!");
      digitalWrite(shutdown_pin, LOW); //standby mode
      instance1 = micros();
      timer = millis();
    }
    else 
      {
      digitalWrite(shutdown_pin, HIGH); //normal mode
      value = analogRead(A1); //Pin A1 recibe la entrada del AD8232
      value = map(value, 250, 400, 0, 100); //to flatten the ecg values a bit
      if((value > threshold) && (!flag))
       {
        count++;  
        Serial.println("in");
        flag = 1;
        interval = micros() - instance1; //RR interval
        instance1 = micros(); 
      }
      else if((value < threshold))
       {
        flag = 0;
       }
      if ((millis() - timer) > 10000)
       {
       hr = count*6;
       timer = millis();
       count = 0; 
      }
      hrv = hr/60 - interval/1000000;
     Serial.println("BPM:");
      Serial.print(hr);
      Serial.print(",");
      Serial.println("♥:");
      Serial.println(value);
      delay(5);
    }  
  if (pulseSensor.sawStartOfBeat())
   {               // Constantly test to see if a beat happened
    Serial.print(",");
    Serial.println("RCP:"); 
    digitalWrite (6, HIGH); //Activar motor para el corazon simulado
    delay(500);
    digitalWrite (6, LOW);
    delay(500);
    Serial.println(myBPM);                      // Mostrar frecuencia RCP
    if (myBPM < 100) //Comparar RCP con la frecuencia estandar recomendada
    {
      lcd.print("FASTER!")
      delay(500):
      lcd.clear()
    }
    else if (myBPM > 120)
    {
      lcd.print("SLOWER!")
      delay(500):
      lcd.clear()
    }
    else
    {
       lcd.print("KEEP!")
      delay(500):
      lcd.clear()
    }
    lcd.print(myBPM);
    delay(5);
    lcd.clear();
  }
  delay(20);
}

