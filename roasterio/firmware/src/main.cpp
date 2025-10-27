#include <RBDdimmer.h>
#include <Adafruit_MAX31855.h>

//AC Dimmer Pins
const int DIMMER_PWM = 18;
const int DIMMER_ZEROCROSS = 19;

//Thermocouple Pins
const int THERMO_CLK = 12;
const int THERMO_CS = 5;
const int THERMO_MISO = 13;

//Initializing dimmer and thermocouple
dimmerLamp dimmer(DIMMER_PWM, DIMMER_ZEROCROSS);
Adafruit_MAX31855 thermocouple(THERMO_CLK, THERMO_CS, THERMO_MISO);

//Current dimmer settings
int power = 0;
bool isDimmerOn = false;


void setup()
{
  Serial.begin(115200);

  //Setup dimmer
  dimmer.begin(NORMAL_MODE, OFF);
  dimmer.setPower(0);

  //Setup thermocouple
  bool init = thermocouple.begin();

  if (!init)
  {
    Serial.println("Error: sensor could not be connected");
    exit(1);
  }
}


void loop()
{

  //Read temperature from thermocouple
  uint8_t error = thermocouple.readError();

  switch (error)
  {
  case MAX31855_FAULT_NONE:
  {
    //Reading temperature from thermocouple
    double temp_cel = thermocouple.readCelsius();
    Serial.print("Temperature: ");
    Serial.print(temp_cel);
    Serial.println(" °C");

    //Set dimmer power based on temperature (example logic)
    if (temp_cel < 25)
    {
      power = 20;
    }
    else if (temp_cel < 30)
    {
      power = 50;
    }
    else
    {
      power = 80;
    }

    dimmer.setPower(power);
    dimmer.setState(ON); // Ensure the dimmer is ON
    Serial.print("Dimmer Power: ");
    Serial.print(power);
    Serial.println("%");

    break;
  }
  case MAX31855_FAULT_OPEN:
  {
    Serial.println("Error Fault OPEN");
    exit(1);
  }
  case MAX31855_FAULT_SHORT_GND:
  {
    Serial.println("Error Fault SHORT GND");
    exit(1);
  }
  case MAX31855_FAULT_SHORT_VCC:
  {
    Serial.println("Error Fault SHORT VCC");
    exit(1);
  }
  case MAX31855_FAULT_ALL:
  {
    Serial.println("Error Fault ALL");
    exit(1);
  }
  default:
    break;
  }

}