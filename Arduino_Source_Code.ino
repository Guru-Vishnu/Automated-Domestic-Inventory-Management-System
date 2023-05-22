#include <Arduino.h>
#include <HX711_ADC.h>  // Header File for HX711 ADC Module
#include <EEPROM.h>     // Header File for Reading Calibration Value from EEPROM
#include <math.h>

// User Interface (0 for Laptop, 1 for Mobile Application)
const int ui = 1;

int text_instruction = 1;

// Pin Definitions

  // HX711 Module
  const int HX711_DT = 4;
  const int HX711_SCK = 5;
  
  // State Indication LEDs
  const int LED_R = 8;
  const int LED_Y = 9;
  const int LED_G = 10;
  
  const int BUTTON = 7;

// Load Cell Constructor
HX711_ADC LoadCell(HX711_DT, HX711_SCK);

const int calVal_eepromAddress = 0;
unsigned long t = 0; // Variable to Store Time

float empty_weight = 0;
float gross_weight = 0;
float net_weight = 0;
float current_weight = 0;
float threshold_percent = 0;
float threshold_weight = 0;
float pw, cw;
float current_percentage = 0;

int day = 1;

// Switch Debouncing Related Variables
boolean lastButton = LOW;
boolean currentButton = LOW;
boolean state = LOW;

boolean debounce(boolean);
void stabilize();           // Delay Function to Stabilize Load Cell Readings

void setup() 
{
  pinMode(BUTTON, INPUT);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_G, OUTPUT);
  
  Serial.begin(9600);
  delay(10);

  if (ui == 0)
    Serial.println("\nAutomated Domestic Inventory Management System\n");

  LoadCell.begin();
  float calibrationValue;
  EEPROM.get(calVal_eepromAddress, calibrationValue); // Obtaining Calibration Value from EEPROM

  unsigned long stabilizingtime = 2000;  // Stabilizing Time after Startup to Get More Precise Values
  LoadCell.start(stabilizingtime, true);
  
  if (LoadCell.getTareTimeoutFlag()) 
  {
    if (ui == 0)
      Serial.println("Timeout - Program Aborted");
    while (1);
  }
  
  else 
  {
    LoadCell.setCalFactor(calibrationValue); // Setting Calibration Value obtained from EEPROM
  }
  
  digitalWrite(LED_R, HIGH);

  pw = 5000.0, cw = 0.0;

  if (ui == 0)
    Serial.println("Place the EMPTY container on the scale and press the button once done.");
  else
  {
    Serial.write(text_instruction);
    text_instruction++;
  }
  while (digitalRead(BUTTON) == LOW);
  
  stabilize();
  while (!LoadCell.update());
  empty_weight = LoadCell.getData();

  if (ui == 0)
    Serial.println("Place the FULL container on the scale and press the button once done.");
  else
  {
    Serial.write(text_instruction);
    text_instruction++;
  }
  while (digitalRead(BUTTON) == LOW);
  
  stabilize();  
  while (!LoadCell.update());
  gross_weight = LoadCell.getData();  

  digitalWrite(LED_G, HIGH);

  net_weight = gross_weight - empty_weight;

  if (ui == 0)
    Serial.print("Enter threshold percentage: ");
  else
  {
    Serial.write(text_instruction);
    text_instruction++;
  }
  
  while(Serial.available() == 0)
  {
    t = millis();
    if (t % 500 == 0 && t % 1000 != 0)
      digitalWrite(LED_R, LOW);
    if (t % 1000 == 0)
      digitalWrite(LED_R, HIGH);
  }

  digitalWrite(LED_R, LOW);

  threshold_percent = Serial.parseFloat();
  threshold_weight = threshold_percent * 0.01 * net_weight;

  if (ui == 0)
  {
    Serial.println("\nStored Data:");
    Serial.print("Empty Weight: ");
    Serial.print(empty_weight);
    Serial.println(" g");
    Serial.print("Gross Weight: ");
    Serial.print(gross_weight);
    Serial.println(" g");
    Serial.print("Net Weight  : ");
    Serial.print(net_weight);
    Serial.println(" g");
    Serial.print("Threshold   : ");
    Serial.print(threshold_percent);
    Serial.println(" %");
    Serial.print("Threshold Weight: ");
    Serial.print(threshold_weight);
    Serial.println(" g");

    Serial.println("Startup Complete");
  }

  else
  {
    Serial.print(empty_weight);
    while(!Serial.available());
    Serial.read();
    Serial.print(gross_weight);
    while(!Serial.available());
    Serial.read();
    Serial.print(net_weight);
    while(!Serial.available());
    Serial.read();
    Serial.print(threshold_percent);
    while(!Serial.available());
    Serial.read();
    Serial.print(threshold_weight);
  }
}

void loop() 
{
  currentButton = debounce(lastButton);
  
  if (lastButton == LOW && currentButton == HIGH)
  {
    day++;
    if (day == 120)
      day = 0;

    while (!LoadCell.update());
    current_weight = LoadCell.getData();

    if (current_weight - empty_weight <= threshold_weight)
    {
      digitalWrite(LED_R, HIGH);
      digitalWrite(LED_G, LOW);
    }
    
    else
    {
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, HIGH);
    }

    Serial.read();
    Serial.print("BTN");
  }

  if (LoadCell.update()) 
  {
    if (millis() > t + 1000)
    {
      float weight = LoadCell.getData();
      current_percentage = (weight - empty_weight)/net_weight * 100;
      current_percentage = constrain(current_percentage, 0, 100);
      
      if (ui == 0)
      {
        Serial.print("Load Cell Output Value (g): ");
        Serial.print(weight);
        Serial.print("\tPercentage Remaining: ");
        Serial.println(current_percentage);
      }

      else
      {
        while(!Serial.available());
        Serial.read();
        Serial.print(current_percentage);
      }

      t = millis();
    }
  }

  lastButton = currentButton;
}

// Function to Stabilize Load Cell Readings
void stabilize()
{
  pw = 5000.0, cw = 0.0;
  
  while (1)
    {
      if (abs(pw - cw) > 1.0)
      {
        while (!LoadCell.update());
        pw = LoadCell.getData();
        digitalWrite(LED_Y, HIGH);
        delay(250);
        while (!LoadCell.update());
        cw = LoadCell.getData();
        digitalWrite(LED_Y, LOW);
        delay(250);
      }
      else
        break;  
    }
}

// Switch Debouncing Function
boolean debounce(boolean last)
{
  boolean current = digitalRead(BUTTON);
  if (last != current)
  {
    delay(5);
    current = digitalRead(BUTTON);
  }
  return current;
}
