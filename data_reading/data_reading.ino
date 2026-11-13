#include <Adafruit_LSM6DSOX.h>
#include <math.h>

#define CS1 10 //for gyro
#define CS2 9 // for sd
#define SCK 13
#define MISO 12
#define MOSI 11

bool crash_event = false;
Adafruit_LSM6DSOX sox;

void writeFile(std::string filename, float deg, float gs)
{
  //does nothing rn
}

void appendFile(std::string filename, float deg, float gs)
{
  //does nothing rn
}

//converts gyroscope coordinates to the roll of the gyroscope
float coordToDeg(float x, float y, float z)
{
  //x coordinate is unused
  float rads = atan2f(y,z); //equivalent to arctan(y/z), in rads
  float angle = rads * RAD_TO_DEG; //convert rad to deg using predefined macro
  return angle;
}

//converts accelerometer coordinates to G-forces 
float coordToGs(float x, float y, float z) 
{
  return 1.0;
}
float getReading(Adafruit_LSM6DSOX &sensor)
{
  return 1.0;
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

  Serial.print("The roll (side-to-side angle; phi): %f");
  Serial.print(angle,2);

}
