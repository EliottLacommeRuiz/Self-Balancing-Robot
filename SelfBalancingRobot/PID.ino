void setupPID() {
  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(-10000, 10000); // Increase output limits for higher speed
  pid.SetSampleTime(10);
}