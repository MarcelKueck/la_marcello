#include <Adafruit_MAX31855.h>

int8_t CS_PIN = 5; // Chip select pin for MAX31855
Adafruit_MAX31855 sensor(CS_PIN);

void setup()
{
  Serial.begin(115200);

  bool init = sensor.begin();

  if(!init){
    Serial.println("Error: sensor could not be connected");
    exit(1);
  }
}

void loop()
{
  uint8_t error = sensor.readError();
  
  switch (error)
  {
  case MAX31855_FAULT_NONE:{
    double temp_cel = sensor.readCelsius();
    Serial.print("Temperature: ");
    Serial.print(temp_cel);
    Serial.println(" °C");
    delay(100);
    break;
  }
  case MAX31855_FAULT_OPEN:{
    Serial.println("Error Fault OPEN");
    exit(1);
  }
  case MAX31855_FAULT_SHORT_GND:{
    Serial.println("Error Fault SHORT GND");
    exit(1);
  }
  case MAX31855_FAULT_SHORT_VCC:{
    Serial.println("Error Fault SHORT VCC");
    exit(1);
  }
  case MAX31855_FAULT_ALL:{
    Serial.println("Error Fault ALL");
    exit(1);
  }
  default:
    break;
  }
}