

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPLZfLkcE0V"
#define BLYNK_DEVICE_NAME "Zahanati"


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h> // LCD I2C library
#include <dht11.h>


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] ="LR9wqtsH61H1KKEG5NHOP3uglGYp-29H";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ADANIAN";
char pass[] = "!ARL2021@";

// This function creates the timer object. It's part of Blynk library 
BlynkTimer timer; 

 int timerValue = 1000L;

  const int sensorPin = A6; 
 
 
 float tempF ;
 float bodyTemp2;
 int hearbeat;



////////////LM35////////////////////////////////////////////////////////////

#define ADC_VREF_mV    5000.0 // in millivolt
#define ADC_RESOLUTION 1024.0
#define PIN_LM35 36 // pin connected to LM35 temperature sensor

/////////////////////////////////////////////////////////////////////////////

////////////////////pulse sensor/////////////////////////////////////////////

#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

//  Variables
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int heartBeatLED = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 
int DHT11Temp = 0;
int DHT11Hum = 0;
const int DHT11pin = 23;
const int buttonPin = 3;

bool seeData = false; 

dht11 DHT;                  //Note:DHT on behalf of the temperature and humidity from first sensor                 
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
LiquidCrystal_I2C lcd(0x27,16,2);  // LCD I2C address 0x27, 16 column and 2 rows


void myTimer() 
{
  // This function describes what will happen with each timer tick
  // e.g. writing sensor value to datastream V5
  Blynk.virtualWrite(V0, DHT11Temp);
  Blynk.virtualWrite(V1, DHT11Hum);
  Blynk.virtualWrite(V2, PIN_LM35);
  Blynk.virtualWrite(V3, tempF);
  Blynk.virtualWrite(V4, hearbeat);
  

}



void setup() {
  Serial.begin(9600);

  lcd.init();      // initialize the lcd
  lcd.backlight(); // open the backlight
  lcd.clear();
  pinMode(PIN_LM35, INPUT); // input for temperature sensor
  pinMode(DHT11pin, INPUT); 
  pinMode (buttonPin, INPUT_PULLUP);


   Blynk.begin(auth, ssid, pass, "blynk.cloud", 80); //https://github.com/blynkkk/blynk_Issues/issues/114

       // Setting interval to send data to Blynk Cloud to 1000ms. 
  // It means that data will be sent every second
  timer.setInterval(timerValue, myTimer); 

  lcd.setCursor(2, 0); // start to print at the first row
  lcd.print("WELCOME TO");    // print the temperature in Celsius
  lcd.setCursor(3, 1); // start to print at the first row
  lcd.print("ZAHANATI");    // print the temperature in Celsius
  delay(10000);
    lcd.clear();
    
  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(heartBeatLED);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  

  
  
  }
  
}
void loop() {
  
  int adcVal = analogRead(PIN_LM35);
  // convert the ADC value to voltage in millivolt
  float milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION);
  // convert the voltage to the temperature in Celsius
  float tempC = milliVolt / 10;
  float tempF = tempC * 9 / 5 + 32; // convert Celsius to Fahrenheit
  
  int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 

 ////////////////////////// READ Temperature & Humidity DATA from first upper DHT11 sensor//////////////////////////
  DHT.read(DHT11pin);
  DHT11Temp = DHT.temperature;
  DHT11Hum = DHT.humidity;

 
 int buttonState = digitalRead(buttonPin);

  // print the temperature to Serial Monitor
  Serial.print(tempC);
  Serial.print("°C ~ ");
  Serial.print(tempF);
  Serial.println("°F");


 
  //lcd.print("°C");
  //lcd.setCursor(0, 1); // start to print at the second row
 // lcd.print(tempF);    // print the temperature in Fahrenheit
  //lcd.print("°F");


 

 //lcd.setCursor(0, 1); // start to print at the first row
if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
 Serial.println("  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
 Serial.print("BPM: ");                        // Print phrase "BPM: " 
 Serial.println(myBPM);                        // Print the value inside of myBPM.                                            
}
else {
  
}

//if (buttonState == LOW)

{
  
  lcd.setCursor(0, 0); // start to print at the first row
  lcd.print("tempC");    // print the temperature in Celsius
  lcd.setCursor(0, 1); // start to print at the first row
  lcd.print(tempC);    // print the temperature in Celsius

  lcd.setCursor(8, 0); // start to print at the first row
  lcd.print("tempF");    // print the temperature in Celsius
  lcd.setCursor(8, 1);
  lcd.print(tempF);
 
  
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0); // start to print at the first row
  lcd.print("DHTtemp");    // print the temperature in Celsius
  lcd.setCursor(0, 1); // start to print at the first row
  lcd.print(DHT11Temp);    // print the temperature in Celsius  


  lcd.setCursor(8, 0); // start to print at the first row
  lcd.print("DHThum");    // print the temperature in Celsius
  lcd.setCursor(8, 1);
  lcd.print(DHT11Hum);
  
  delay(2000);
  lcd.clear();
  

} 
/*
else {
 
   lcd.setCursor(0, 0); // start to print at the first row
  lcd.print("Hold Button To");    // print the temperature in Celsius
  lcd.setCursor(0, 1); // start to print at the first row
  lcd.print("See sensor data");    // print the temperature in Celsius

}
*/


    // Runs all Blynk stuff
  Blynk.run(); 
  
  // runs BlynkTimer
  timer.run(); 
  

    delay(1000); 

 }
