#include <NimBLEDevice.h>
#include <BleMouse.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Initialize the BLE Mouse and MPU6050
BleMouse bleMouse;
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Mouse and MPU6050");

  // Start BLE Mouse
  bleMouse.begin();

  // Start MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to start MPU6050!");
    while (1);
  }
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  if (bleMouse.isConnected()) {
    // Reading MPU6050 data
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    // Map accelerometer data to mouse movement (this is just an example)
    int16_t mouseX = map(a.acceleration.x, -8, 8, -127, 127);
    int16_t mouseY = map(a.acceleration.y, -8, 8, -127, 127);

    // Move the mouse
    bleMouse.move(mouseX, mouseY);

    // Print the values for debugging
    Serial.print("Mouse X: "); Serial.print(mouseX); Serial.print(" ");
    Serial.print("Mouse Y: "); Serial.println(mouseY);
  }
  delay(100);
}
