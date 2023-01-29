//required library 
#include "AnalogFrequency.h" 
#include #include "ArduinoJson.h" 
//communication variable 
String message = ""; 
bool messageReady = false; 
//frequency period library use for sensor 1 
double lfrq;
long int pp;
double hb1;
//global variable define for sensor number 2
// Below: pin number for FOUT 
#define PIN_NUMBER 4
 // Below: number of samples for averaging 
#define AVERAGE 4 unsigned int doppler_div = 19.46;
unsigned int samples[AVERAGE];
unsigned int x;
double hb2;
uint32_t displayTimer = 0;
//analog library use for sensor 3 
#define ADCPin A1
// Incoming data is summed, so fetching the results every second // will indicate speed over the previous second
// How often in mS to display the results ( 0 = print all results if possible)
#define printDelay 0
double hb3;
//*****************************************/ 
// RCWL-0516 Sensor (Sensor 4)
int Sensor = 12;
int LED = 3;
int flg;
void setup(){
Serial.begin(115200);
pinMode (Sensor, INPUT);
Serial.println("Waiting for motion");
pinMode(PIN_NUMBER, INPUT);
setupADC(ADCPin);
//frequency period 
FreqPeriod::begin(); 
Serial.println("FreqPeriod Library Test"); 
}
void loop() { 
pp = FreqPeriod::getPeriod(); 
  //Monitor serial communication 
//frequency period for sensor 1 
if (pp) { 
Serial.print ("period: "); 
Serial.print(pp); 
Serial.print(" 1/16us / frequency: "); 
lfrq = 16000400.0 /pp;
hb1 = lfrq/19.46;
Serial.print(hb1);
Serial.println( " kmh ");
} 
//for sensor 2
noInterrupts();
pulseIn(PIN_NUMBER, HIGH);
unsigned int pulse_length = 0;
for (x = 0; x < AVERAGE; x++) {
pulse_length = pulseIn(PIN_NUMBER, HIGH);
pulse_length += pulseIn(PIN_NUMBER, LOW);
samples[x] = pulse_length;
} 
interrupts();
// Check for consistency 
bool samples_ok = true;
unsigned int nbPulsesTime = samples[0];
for (x = 1; x < AVERAGE; x++) {
nbPulsesTime += samples[x];
if ((samples[x] > samples[0] * 2) || (samples[x] < samples[0] / 2)) {
samples_ok = false;
}
} 
if (samples_ok) {
unsigned int Ttime = nbPulsesTime / AVERAGE;
unsigned int Freq = 1000000 / Ttime;
hb2 = Freq/doppler_div;
Serial.print(hb2);
Serial.print("km/h\r\n"); 
}


//for sensor 3
if( fAvailable() && millis() - displayTimer > printDelay ){
displayTimer = millis();
uint32_t frequency = getFreq();
hb3 = frequency/19.49;
Serial.print(hb3);
Serial.print("km/h\r\n"); 
} 
//RCWL-0516 Sensor4
int val = digitalRead(Sensor);
//Read Pin as input
if((val > 0) && (flg==0)) {
Serial.println("Motion Detected"); 
flg = 1; 
} 
if(val == 0) { 
Serial.println("NO Motion");
flg = 0;
}
//for sending data to esp32
while(Serial.available()){
message = Serial.readString();
messageReady = true;
} 
//Only process message if there's one 
if(messageReady){ 
//The only messages we'll parse will be formatted in JSON 
DynamicJsonDocument doc(1024);  // ArduinoJson version 6+ 
//Attempt to deserialize the message 
DeserializationError error = deserializeJson(doc,message); 
if(error){ 
Serial.print(F("deserializeJson() failed. :"));
Serial.println(error.c_str()); messageReady = false; return; 
} 
if(doc["type"] == "request"){ 
doc["type"] = "response"; 
//Get data from analog sensors 
doc["hb1"] = hb1; 
doc["hb2"] = hb2; 
doc["hb3"] = hb3; 
doc["RCWL"] = flg; 
serializeJson(doc,Serial); 
} 
messageReady = false; 
} 
}
