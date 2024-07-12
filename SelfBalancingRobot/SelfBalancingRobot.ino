/********** PID **********/
#include <PID_v1.h>

double kP = 100;
double kI = 225;
double kD = 0.07;

double setpoint, input, output;   // PID variables
PID pid(&input, &output, &setpoint, kP, kI, kD, DIRECT); // PID setup

#include <AccelStepper.h>


/********** MPU **********/
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

/********** Stepper **********/
AccelStepper stepper1(1, 6, 5); // (# of phases, ST red, DIR blue) 
AccelStepper stepper2(1, 10, 9); // (# of phases, ST red, DIR blue) 

/********** Globals **********/
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

/* -------- Function Prototypes --------*/
/*
  The point of this is to declare the function prototypes. 
  When arduino compiles into tabs, it creates a prototype that is the 
  name of the function as seen below and prevents a miscompilation of the functions.
*/
void setupParams();
void setupMPU();
void setupPID();

void runMPU();
void moveStepper();

void dmpDataReady();

void prindData();

