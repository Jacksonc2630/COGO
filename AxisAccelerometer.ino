#include <Wire.h>
#include <MPU6050_tockn.h>

MPU6050 mpu(Wire);

// Timing variables
unsigned long lastTime = 0;
float deltaTime = 0;

// Acceleration variables
float accelX, accelY, accelZ;
float velocityX = 0, velocityY = 0, velocityZ = 0;
// Decay factor accounting for drift
const float VELOCITY_DECAY = 0.98;
const float NOISE_THRESHOLD = 0.05;
// Calibration variables
float accelXoffset = 0, accelYoffset = 0, accelZoffset = 0;
const int CALIBRATION_SAMPLES = 200;
//Starting Position
float positionX = 0, positionY = 0, positionZ = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  Serial.println("Initializing MPU6050");
  mpu.begin();

  Serial.println("Calibrating accelerometer... Keep the sensor still!");
  delay(1000); 
  calibrateAccelerometer();

  Serial.println("Calibration complete!");
  Serial.println("X offset: " + String(accelXoffset) + 
                 ", Y offset: " + String(accelYoffset) + 
                 ", Z offset: " + String(accelZoffset));
  
  lastTime = millis();
}

void calibrateAccelerometer() {
  float sumX = 0, sumY = 0, sumZ = 0;
  
  for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
    mpu.update();
    sumX += mpu.getAccX();
    sumY += mpu.getAccY();
    sumZ += mpu.getAccZ();
    delay(5);
  }
 // Calculate average offsets
  accelXoffset = sumX / CALIBRATION_SAMPLES;
  accelYoffset = sumY / CALIBRATION_SAMPLES;
  accelZoffset = sumZ / CALIBRATION_SAMPLES;
}


void loop() {
  // Update MPU6050 readings
  mpu.update();


 unsigned long currentTime = millis();
  deltaTime = (currentTime - lastTime) / 1000.0; //convert to seconds
  lastTime = currentTime;

  accelX = mpu.getAccX() - accelXoffset;
  accelY = mpu.getAccY() - accelYoffset;
  accelZ = mpu.getAccZ() - accelZoffset - 1.0;

// Accounting for gravity

  accelX *= 9.81;
  accelY *= 9.81;
  accelZ *= 9.81;

 // Apply noise threshold to reduce drift
  accelX = (abs(accelX) < NOISE_THRESHOLD) ? 0 : accelX;
  accelY = (abs(accelY) < NOISE_THRESHOLD) ? 0 : accelY;
  accelZ = (abs(accelZ) < NOISE_THRESHOLD) ? 0 : accelZ;
  

  velocityX = velocityX * VELOCITY_DECAY + accelX * deltaTime;
  velocityY = velocityY * VELOCITY_DECAY + accelY * deltaTime;
  velocityZ = velocityZ * VELOCITY_DECAY + accelZ * deltaTime;
  
  
  velocityX = (abs(velocityX) < NOISE_THRESHOLD) ? 0 : velocityX;
  velocityY = (abs(velocityY) < NOISE_THRESHOLD) ? 0 : velocityY;
  velocityZ = (abs(velocityZ) < NOISE_THRESHOLD) ? 0 : velocityZ;
  
  
  positionX += velocityX * deltaTime;
  positionY += velocityY * deltaTime;
  positionZ += velocityZ * deltaTime;
  
  // Print data
  Serial.println("---------------------------");
  Serial.print("Accel (m/s²)  X: "); Serial.print(accelX, 3);
  Serial.print("  Y: "); Serial.print(accelY, 3);
  Serial.print("  Z: "); Serial.println(accelZ, 3);
  
  Serial.print("Velocity (m/s) X: "); Serial.print(velocityX, 3);
  Serial.print("  Y: "); Serial.print(velocityY, 3);
  Serial.print("  Z: "); Serial.println(velocityZ, 3);
  
  Serial.print("Position (m)  X: "); Serial.print(positionX, 3);
  Serial.print("  Y: "); Serial.print(positionY, 3);
  Serial.print("  Z: "); Serial.println(positionZ, 3);
  
  delay(50);
}
