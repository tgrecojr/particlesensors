// This #include statement was automatically added by the Particle IDE.
#include <Si7020.h>
#if defined(ARDUINO)
#include "Wire.h"
#include "Si7020.h"
#elif defined(SPARK)
#endif

Si7020 sensor;
const String temperatureTopicName = "temperature";
const String humidityTopicName = "humidity";
long DELAY_BETWEEN_READINGS = 60000;
const long STARTUP_DELAY = 2000;

double rh_round;
double tc_round;
double th_round;

STARTUP(WiFi.selectAntenna(ANT_EXTERNAL));

void setup(){
    
    Particle.variable("temperature", th_round);
    Particle.variable("humidity", rh_round);
    Particle.variable("delay", DELAY_BETWEEN_READINGS);
    Particle.function("getTemp", getTemperature);
    Particle.function("getHumid", getHumidity);
    Particle.function("setDelay", setDelay);

    
    sensor.begin();
    delay(STARTUP_DELAY);
  
}

void loop(){
  
    if (Particle.connected() == false) {
        Particle.connect();
    }
    float rh = sensor.getRH();
    float tc = sensor.readTemp();
    float th = celciusToFarenheit(tc);
    rh_round = round_to_decimal(rh);
    tc_round = round_to_decimal(tc) ;
    th_round = round_to_decimal(th) ;
    Particle.publish(humidityTopicName, String(rh_round), 60, PRIVATE);
    Particle.publish(temperatureTopicName, String(th_round), 60, PRIVATE);
    delay(DELAY_BETWEEN_READINGS);
    
}

float celciusToFarenheit(float cTemp){
  return (cTemp * (9.0f/5.0f)) + 32.0f;
}

double round_to_decimal(float f) {
    char buf[42];
    sprintf(buf, "%.7g", f); // round to 7 decimal digits
    return atof(buf);
}



int getTemperature(String command){
  double temp = th_round + .5;
  return (int)temp;
}
int getHumidity(String command){
    double humid = rh_round + .5;
    return (int)humid;
}

int setDelay(String delay){
    long theDelay = atol(delay);
    DELAY_BETWEEN_READINGS = theDelay;
    return 1;
}
