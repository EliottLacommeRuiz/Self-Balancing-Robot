/********** Stepper **********/
#include <AccelStepper.h>


/********** MPU **********/
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

/* Stepper */
AccelStepper stepper1(1, 6, 5); // (# of phases, ST red, DIR blue) 
AccelStepper stepper2(1, 10, 9); // (# of phases, ST red, DIR blue) 

int pot = A0;

MPU6050 mpu;
#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

float pitch;

float trimPot;
float trimAngle;

void dmpDataReady() {
  mpuInterrupt = true;
}

/********** SETUP **********/
void setup() {
  setupMPU();

  stepper1.setMaxSpeed(5000); // Increase max speed
  stepper1.setAcceleration(1000); // Increase acceleration
  stepper1.setCurrentPosition(0);

  stepper2.setMaxSpeed(5000); // Increase max speed
  stepper2.setAcceleration(1000); // Increase acceleration
  stepper2.setCurrentPosition(0);
  
  pinMode(pot, INPUT);
}

void loop() {
  
  mpuData();
  // Get the current pitch angle
  pitch = map(pot, 0, 1023, -10, 10) + (ypr[1] * 180 / M_PI); // adjust to degrees

  //if (pitch <= 50 && pitch >= -50) {
  // Use PID output to control stepper motor speeds
  stepper1.setSpeed(-1 * map(pitch, -90, 90, -5000, 5000));
  stepper2.setSpeed(-1 * map(pitch, -90, 90, -5000, 5000));

  // Run the steppers to execute the motion
  stepper1.run();
  stepper2.run();
  //}
  printData();
}
