#include <Adafruit_LSM6DSOX.h>
#include <math.h>

#define CS1 10 //for gyro
#define CS2 9 // for sd
#define SCK 13
#define MISO 12
#define MOSI 11

#define MAX_TILT_ANGLE 30 //in degrees //subject to change
#define MAX_G_VALUE 6.5 //in gs, subject to change

//states for dictating crash events and what to log
bool crash_state = false; //crash
bool idle_state = true; //not moving
bool normal_state = false; //moving

float angle_history;
float g_history;
int history_count;

//gyro/accelerometer
Adafruit_LSM6DSOX sox;

//does nothing rn
void writeFile(std::string filename, float deg, float gs)
{

}

//does nothing rn
void appendFile(std::string filename, float deg, float gs)
{

}

//converts gyroscope coordinates to the roll of the gyroscope
float coordToDeg(float x, float y, float z)
{
  //may have to add/sub pi/2 depending on sigh of x,y; not sure if that is the case.
  //if so, add conditional later for that

  float rads = atan2f(y,z); //equivalent to arctan(y/z), in rads
  float angle = rads * RAD_TO_DEG; //convert rad to deg using predefined macro
  return angle;
}

//converts accelerometer coordinates to G-forces 
float coordToGs(float x, float y, float z) 
{
  x = std::pow(x,2);
  y = std::pow(y,2);
  z = std::pow(z,2);

  float magnitude = std::sqrt(x+y+z); //magnitude of acceleration
  float gs = magnitude/9.80665; //div by force of gravity
  return gs;
}

// can probably delete this, easier to just get readings in main loop()
float getReading(Adafruit_LSM6DSOX &sensor)
{
  return 1.0;
}

//updates motorcycle state based on readings
void updateState(float angle, float gs)
{
  /*
  continously add last ten readings together into one float?
  this way, the current reading/ change in value can be checked/compared
  idle only when history stays as 0?
  cannot be in multiple states at once
  */
  int history_size = 10; //can be made larger/smaller to control accuracy
  // readings are very frequent, maybe this can be much larger

  angle_history += std::abs(angle);
  g_history += std::abs(gs);

  if (history_count > history_size)
  {
    //reset history to prevent infinite growth
    //otherwise, we can never come back to 0
    angle_history = 0;
    g_history = 0;
    history_count = 0;
  }

  //basic conditionals, will probably change
  // not sure if checks should be AND or OR conditions
  if (angle_history <= 0 and g_history <= 0)
  {
    idle_state = true;
    normal_state = false;
    crash_state = false;
  }
  else if (angle_history < MAX_TILT_ANGLE and g_history < MAX_G_VALUE)
  {
    idle_state = false;
    normal_state = true;
    crash_state = false;
  }
  //crashes more critical; compare immediate readings instead of history?
  else if (angle_history > MAX_TILT_ANGLE and g_history > MAX_G_VALUE)
  {
    //we cant go back to normal after crash(?) so might need something to fix that idk
    idle_state = false;
    normal_state = true;
    crash_state = false;
  }

  history_count++;

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial)
  {
    delay(10);
  }

  if (!sox.begin_I2C())
  {
    if (!sox.begin_SPI(CS1, SCK, MISO, MOSI))
      {
        Serial.println("Failed to find LSM6DSOX chip");
        while (1){
          delay(10);
        }
      }
  }

  Serial.println("LSM6DSOX Found!");

  sox.setAccelRange(LSM6DS_ACCEL_RANGE_8_G);
  Serial.println("Accelerometer range set to: +-8G");

  sox.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );
  Serial.println("Gyro range set to: 250 deg/s");

  sox.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.println("Accelerometer data rate set to: 12.5 Hz");

  sox.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.println("Gyro data rate set to: 12.5 Hz");
}


void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  sox.getEvent(&accel, &gyro, &temp);

  float gForce = coordToGs(accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);
  float angle = coordToDeg(gyro.gyro.x, gyro.gyro.y, gyro.gyro.z);

  Serial.print("The roll (side-to-side angle; phi): ");
  Serial.println(angle,2);
  Serial.print("The current G-force: ");
  Serial.println(gForce, 2);

  //based on state, change logging behavior directly in loop or delegate to other functions (not implemented yet)
  // ex: if crash --> report in log; if idle--> stop logging
  updateState(angle, gForce);

  //state check
  Serial.print("Idle: ");
  Serial.println(idle_state);
  Serial.print("Normal: ");
  Serial.println(normal_state);
  Serial.print("Crash: ");
  Serial.println(crash_state);

  Serial.print("------------------------------------------");

}
