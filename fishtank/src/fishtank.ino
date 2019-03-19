#include "OneWire.h"
#include"DS18.h"
#include "ledmatrix-max7219-max7221.h"


// temperature sensor variables
OneWire ds = OneWire(D4);  // 1-wire signal on pin D4

unsigned long lastUpdate = 0;

float lastTemp=0;//previous celsius
int turn_into=0;
byte i;
byte present = 0;
byte type_s;
byte data[12];
byte addr[8];
float celsius, fahrenheit;
//-------------------------------------------
//-------------------------------------------
//RGB LED display
String current_color="";
// List of CheerLights color names
String colorName[] = {"none","red","pink","green","blue"};
// Map of RGB values for each of the Cheerlight color names
int colorRGB[][3] = {     0,  0,  0,  // "none"
                        255,  0,  0,  // "red"
                        255,192,203,  // "pink"
                          0,255,  0,  // "green"
                          0,  0,255,  // "blue"
};
LEDMatrix *led;

//4 matrix LED display
int bitmapWidth = 8;   // 8 is default

String text = "";   // default string for display

int textLength = text.length();
// default position of the text is outside and then scrolls left
int textX = bitmapWidth;
int fontWidth = 5, space = 1;

//-------------------------------------------
//-------------------------------------------
//basic function variables
bool start=false;//start the whole detection timer flag
int currentRGB[3]={0,0,0}; // cureent color
int change_current[3]={0,0,0}; //change to color
bool first_alert=false; //when the alert is on
bool stop_warm_flag=false;//stop the alert
bool i_know=false;//if i know, the buzzer should not on again
String topic="cse222/final_project";//topic for the whole data
String topic_temp="cse222/final_project/temperature";//only for thingspeak temperature
Timer time_change_color (100,set_RGB);
Timer timer_whole_detection(2000,whole_detection);


//detect whether or not there is an alert
void alert_detect(){
  //set the alert of not
  if(i_know==true&&current_color!="red"){
    i_know=false;
  }else if(i_know==false&&first_alert==false && current_color=="red"){
    Serial.println("start the alert");
    first_alert=true;
    digitalWrite(D3,HIGH);
  }
  if(first_alert==true&&(current_color!="red"|| stop_warm_flag==true)){
    Serial.println("stop the alert");
    Serial.println(digitalRead(D3));
    first_alert=false;
    i_know=true;
    stop_warm_flag=false;
    digitalWrite(D3,0);
    Serial.println(digitalRead(D3));
    publishState("nothing");
  }
}
//stop the buzzer from UI (I know)
int stop_warm(String nothing){
  if(first_alert==true){
    stop_warm_flag=true;
  }
}
// draw the text in 4-matrix LED
void drawText(String s, int x)
{
  int y = 0;
  for(int i = 0; i < s.length(); i++) {
    // Adafruit_GFX method
    led->drawChar(x + i*(fontWidth+space), y, s[i], true, false, 1);
  }
}
//publish state to UI
int publishState(String temp) {
  String data = "{";
  data+="\"light_color\":";
  data+="\"";
  data+=current_color;
  data+="\"";
  data+=",";
  data+="\"temp\":";
  data+=String(lastTemp);
  data+=",";
  if(first_alert==false){
    data+="\"alert\": false";
  }else{
    data+="\"alert\": true";
  }
  data +="}";

  Particle.publish(topic, data, 60, PRIVATE);
  return 0;
}
//fade the RGB LED
void set_RGB(){
      if(turn_into==50){
        Serial.println("Finish Set the Color");
        analogWrite(A3,255-change_current[0]);
        analogWrite(D0,255-change_current[1]);
        analogWrite(D1,255-change_current[2]);
        Serial.print(change_current[0]);
        Serial.print(" ");
        Serial.print(change_current[1]);
        Serial.print(" ");
        Serial.print(change_current[2]);
        Serial.println(" ");
        currentRGB[0]=change_current[0];
        currentRGB[1]=change_current[1];
        currentRGB[2]=change_current[2];

        turn_into=0;
        time_change_color.stop();
        publishState("nothing");
      }
      Serial.println("set the color");
      analogWrite(A3,255-(change_current[0]-currentRGB[0])*turn_into/100+currentRGB[0]);
      analogWrite(D0,255-(change_current[1]-currentRGB[1])*turn_into/100+currentRGB[1]);
      analogWrite(D1,255-(change_current[2]-currentRGB[2])*turn_into/100+currentRGB[2]);
      turn_into+=1;
}
//set the color based on the color that we want to change
void setColor(String color){
  // Look through the list of colors to find the one that was requested
  for(int iColor = 0; iColor <= 5; iColor++)
  {
    if(color == colorName[iColor])
    {
      Serial.println("start to set the color");
      current_color=color;
	    change_current[0]=colorRGB[iColor][0];
      change_current[1]=colorRGB[iColor][1];
      change_current[2]=colorRGB[iColor][2];
      Serial.print("change_current: ");
      Serial.print(change_current[0]);
      Serial.print(" ");
      Serial.print(change_current[1]);
      Serial.print(" ");
      Serial.print(change_current[2]);
      Serial.println(" ");
      time_change_color.start();
      return;
    }
  }
}
//determine whether we should change color
void change_color(){
  if(celsius<20 && current_color!="pink"){
    Serial.println("pink color is set");
    setColor("pink");
  }else if(celsius>20 && celsius<25 &&current_color!="green"){
    Serial.println("green color is set");
    setColor("green");
  }else if(celsius>25 && celsius<30 &&current_color!="blue"){
    Serial.println("blue color is set");
    setColor("blue");
  }else if(celsius>30 &&current_color!="red"){
    Serial.println("red color is set");
    setColor("red");
  }
}
// the whole detection of temperature process
void whole_detection(){

  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    return;
  }

  // The order is changed a bit in this example
  // first the returned address is printed

  Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  // second the CRC is checked, on fail,
  // print error and just return to try again

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println();

  // we have a good address at this point
  // what kind of chip do we have?
  // we will set a type_s value for known types or just return

  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS1820/DS18S20");
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    case 0x26:
      Serial.println("  Chip = DS2438");
      type_s = 2;
      break;
    default:
      Serial.println("Unknown device type.");
      return;
  }

  // this device has temp so let's read it

  ds.reset();               // first clear the 1-wire bus
  ds.select(addr);          // now select the device we just found
  // ds.write(0x44, 1);     // tell it to start a conversion, with parasite power on at the end
  ds.write(0x44, 0);        // or start conversion in powered mode (bus finishes low)

  // first make sure current values are in the scratch pad

  present = ds.reset();
  ds.select(addr);
  ds.write(0xB8,0);         // Recall Memory 0
  ds.write(0x00,0);         // Recall Memory 0

  // now read the scratch pad

  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE,0);         // Read Scratchpad
  if (type_s == 2) {
    ds.write(0x00,0);       // The DS2438 needs a page# to read
  }

  // transfer and print the values

  Serial.print("  Data = ");
  Serial.print(present, HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s == 2) raw = (data[2] << 8) | data[1];
  byte cfg = (data[4] & 0x60);

  switch (type_s) {
    case 1:
      raw = raw << 3; // 9 bit resolution default
      if (data[7] == 0x10) {
        // "count remain" gives full 12 bit resolution
        raw = (raw & 0xFFF0) + 12 - data[6];
      }
      celsius = (float)raw * 0.0625;
      break;
    case 0:
      // at lower res, the low bits are undefined, so let's zero them
      if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
      if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
      if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
      // default is 12 bit resolution, 750 ms conversion time
      celsius = (float)raw * 0.0625;
      break;

    case 2:
      data[1] = (data[1] >> 3) & 0x1f;
      if (data[2] > 127) {
        celsius = (float)data[2] - ((float)data[1] * .03125);
      }else{
        celsius = (float)data[2] + ((float)data[1] * .03125);
      }
  }

  fahrenheit = celsius * 1.8 + 32.0;
  //check the error information
  if(celsius==0 ||celsius<0||celsius>60){
    Serial.print("error information: ");
    Serial.println(celsius);
    return;
  }
  //the temperature cannot change more than 10 at once(impossible)
  if(celsius-lastTemp>10&&lastTemp!=0){
    Serial.println(celsius);
    Serial.println("error message received");
    return;
  }
  lastTemp = celsius;
  //change the change_color
  change_color();
  //set the alert of not
  alert_detect();
  //print all the information to serial monitor
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
  Serial.print("current corlor: ");
  Serial.println(current_color);
  Serial.print("currentRGB: ");
  Serial.print(currentRGB[0]);
  Serial.print(" ");
  Serial.print(currentRGB[1]);
  Serial.print(" ");
  Serial.print(currentRGB[2]);
  Serial.println(" ");
  Serial.print("change_current: ");
  Serial.print(change_current[0]);
  Serial.print(" ");
  Serial.print(change_current[1]);
  Serial.print(" ");
  Serial.print(change_current[2]);
  Serial.println(" ");
  Serial.println(digitalRead(D3));
  Serial.println(analogRead(D3));
  //display the led
  text="";
  text+=String((int)celsius);
  text+=" C";
  if(led != NULL) {
    led->fillScreen(false);
    drawText(text, 4);
    led->drawCircle(20,1,1,0xFFFF);
    led->flush();
  }
  Particle.publish(topic_temp,String(celsius),60,PRIVATE);
  publishState("nothing");
}
void setup() {
  //set up the 4 matrix led
  Serial.begin(9600);
  led = new LEDMatrix(4, 1, A0, A1, A2);
  led->addMatrix(3, 0, 0, false, false);
  led->addMatrix(2, 0, 0, false, false);
  led->addMatrix(1, 0, 0, false, false);
  led->addMatrix(0, 0, 0, false, false);
  //set the cloud function
  Particle.function("publishState",publishState);
  Particle.function("stop_warm",stop_warm);
  //set A3,D0,D1,D3 to output
  pinMode(A3,OUTPUT);
  pinMode(D0,OUTPUT);
  pinMode(D1,OUTPUT);
  pinMode(D3,OUTPUT);
}

void loop(void) {
  // start the whole detection timer
  if(start==false){
    timer_whole_detection.start();
    start=true;
  }
}
