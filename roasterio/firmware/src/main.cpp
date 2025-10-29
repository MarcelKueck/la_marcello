#include <RBDdimmer.h>
#include <Adafruit_MAX31855.h>
#include <PID_v1.h>

//AC Dimmer Pins
const int DIMMER_PWM = 18;
const int DIMMER_ZEROCROSS = 19;

//Thermocouple Pins
const int THERMO_CLK = 12;
const int THERMO_CS = 5;
const int THERMO_MISO = 13;

//PID Parameters
double TargetTemp, CurrentTemp, Output; // TODO: Implement Web Interface to set temperature
double Kp = 2.0;
double Ki = 0.0;
double Kd = 0.0;
unsigned long lastPID = 0; //For Update Interval
const unsigned long PID_INTERVAL = 1000; //For Update Interval

//Initializing dimmer, thermocouple and PID Controller
dimmerLamp dimmer(DIMMER_PWM, DIMMER_ZEROCROSS);
Adafruit_MAX31855 thermocouple(THERMO_CLK, THERMO_CS, THERMO_MISO);
PID controller(&CurrentTemp, &Output, &TargetTemp, Kp, Ki, Kd, DIRECT);

//Dimmer settings
int power = 0;

//Safety Limits
const double MAX_TEMP = 150.0; // Maximum allowable temperature in °C

void setup()
{
  Serial.begin(115200);

  //Setup PID Controller
  CurrentTemp = thermocouple.readCelsius();
  TargetTemp = 100;
  controller.SetMode(AUTOMATIC);
  controller.SetOutputLimits(0.0, 100.0);

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
  unsigned long now = millis();
  if(now - lastPID > PID_INTERVAL){
    //update timestamp to current interval
    lastPID = now;

    //Read temperature from thermocouple
    uint8_t error = thermocouple.readError();

    switch (error)
    {
    case MAX31855_FAULT_NONE:
    {
      //Reading temperature from thermocouple
      CurrentTemp = thermocouple.readCelsius();
      Serial.print("Temperature: ");
      Serial.print(CurrentTemp);
      Serial.println(" °C");

      if(CurrentTemp >= MAX_TEMP){
        dimmer.setPower(0);
        dimmer.setState(OFF); // Turn off the dimmer if temperature exceeds MAX_TEMP
        Serial.println("Maximum temperature exceeded! Dimmer turned OFF.");
        break;
      }

      //Set dimmer power based on temperature using PID Controller
      controller.Compute();
      Serial.print("PID Output: ");
      Serial.println(Output);
      power = (int) Output;

      //Set Dimmer to PID calculated Power Level
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

}