// -- Smart Home Automation System (CAPSTONE PROJECT)

// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPL1CArseRR"
#define BLYNK_DEVICE_NAME "Capstone Project"
#define BLYNK_AUTH_TOKEN "viP_ztmqiHn5qEsRd_lNl_XaBIRT0AS7"
#define BLYNK_PRINT Serial

//Including Libraries and Defining Pins and types of module used
#include <ESP8266WiFi.h> // ESP8266 Library
#include <BlynkSimpleEsp8266.h> //Blynk Library -> Activating the ESP8266 board
#include "DHT.h" // DHT 22 Library
#define DHTPIN 4 // Digital Pin 2 for DHT22
#define DHTTYPE DHT22 // DHT22 -> Type of temperature sensor used

// Defining the pins of the four (4) channel relay (GPIO = D Pin in NodeMCU)
#define relay1_pin 5 // GPIO 5 == D1 Pin in NodeMCU
#define relay2_pin 14 // GPIO 14 == D5 Pin in NodeMCU
#define relay3_pin 12 // GPIO 12 == D6 Pin in NodeMCU
#define relay4_pin 13 // GPIO 13 == D7 Pin in NodeMCU

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

//Inserting Credentials
char auth[] = BLYNK_AUTH_TOKEN; //AUTH token
char ssid[] = "Marcelo Mesa"; //Insert Wifi Name
char pass[] = "5673266372"; // Insert Password

// -- Code for DHT22
void sendSensor(){
  float h = dht.readHumidity(); // Storing the value of Humidity
  float t = dht.readTemperature(); // Storing the value of Temperature

  // Checking if the sensor works or not
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Sending the value read of the double or float variables "t" and "h" to the blynk server
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
}

// -- Code for Fire Sensor
int flag = 0;
void notifyOnFire() {
  int isButtonPressed = digitalRead(D0); // Storing the value of the sensor ( 1 or 0 || True or False)
  if (isButtonPressed == 1 && flag == 0){ 
    Serial.println("Fire in the House");
    Blynk.logEvent("fire_alert", "Fire Alert!"); // Sending the event log that to the blynk server when a fire wasa detected
    flag=1;
  }
  else if (isButtonPressed == 0){
    flag=0;
  }
 Blynk.virtualWrite(V0, isButtonPressed);
}

// -- Code for Relay Module
// State of the relay will be set to HIGH because we want the relay to be off when starting the device
int relay1_state = HIGH;
int relay2_state = HIGH;
int relay3_state = HIGH;
int relay4_state = HIGH;
// Defining the Virtual Pins of the Relay Module
#define button1_vpin V1
#define button2_vpin V2
#define button3_vpin V3
#define button4_vpin V4

// Syncing the button virtual pins to the blynk server
BLYNK_CONNECTED () {
  Blynk.syncVirtual(button1_vpin);
  Blynk.syncVirtual(button2_vpin);
  Blynk.syncVirtual(button3_vpin);
  Blynk.syncVirtual(button4_vpin);
}

// Logic for the relay module
BLYNK_WRITE(button1_vpin){
  relay1_state = param.asInt();
  if(relay1_state == 1) {
    digitalWrite(relay1_pin, LOW); // Low means the current is flowing
  }
  else {
    digitalWrite(relay1_pin, HIGH); // High means the current is cut
  }
}

BLYNK_WRITE(button2_vpin){
  relay2_state = param.asInt();
  if(relay2_state == 1) {
    digitalWrite(relay2_pin, LOW); // Low means the current is flowing
  }
  else {
    digitalWrite(relay2_pin, HIGH); // High means the current is cut
  }
}

BLYNK_WRITE(button3_vpin){
  relay3_state = param.asInt();
  if(relay3_state == 1) {
    digitalWrite(relay3_pin, LOW); // Low means the current is flowing
  }
  else {
    digitalWrite(relay3_pin, HIGH); // High means the current is cut
  }
}

BLYNK_WRITE(button4_vpin){
  relay4_state = param.asInt();
  if(relay4_state == 1) {
    digitalWrite(relay4_pin, LOW); // Low means the current is flowing
  }
  else {
    digitalWrite(relay4_pin, HIGH); // High means the current is cut
  }
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Baud rate
  // Setting the pins of the relay module to OUTPUT
  pinMode(relay1_pin, OUTPUT); 
  pinMode(relay2_pin, OUTPUT);
  pinMode(relay3_pin, OUTPUT);
  pinMode(relay4_pin, OUTPUT);
  // Setting the pin of the fire sensor to OUTPUT
  pinMode(D0, INPUT_PULLUP);
  // Writing a HIGH or a LOW value to a digital pin
  digitalWrite(relay1_pin, relay1_state);
  digitalWrite(relay2_pin, relay2_state);
  digitalWrite(relay3_pin, relay1_state);
  digitalWrite(relay4_pin, relay2_state);
  // Starting the blynk application
  Blynk.begin(auth, ssid, pass);
  // Starting the DHT22
  dht.begin();
  // Time interval for the DHT22 and Fire sensor
  timer.setInterval(2500L, sendSensor);
  timer.setInterval(1000L, notifyOnFire);

  // Sending the read value of the state of the relay to the blynk server
  Blynk.virtualWrite(button1_vpin, relay1_state);
  Blynk.virtualWrite(button2_vpin, relay2_state);
  Blynk.virtualWrite(button3_vpin, relay1_state);
  Blynk.virtualWrite(button4_vpin, relay2_state);

}

void loop() {
  // put your main code here, to run repeatedly:
  // running the blynk and the timer in a loop
  Blynk.run();
  timer.run();
}
