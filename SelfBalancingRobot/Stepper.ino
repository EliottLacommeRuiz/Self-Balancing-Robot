void setupStepper() {
  stepper1.setMaxSpeed(10000); // Increase max speed
  stepper1.setAcceleration(5000); // Increase acceleration
  stepper1.setCurrentPosition(0);

  stepper2.setMaxSpeed(10000); // Increase max speed
  stepper2.setAcceleration(5000); // Increase acceleration
  stepper2.setCurrentPosition(0);
}


void moveStepper() { 
  trimPot = map(analogRead(pot), 0, 1023, 0, -5000); // read the pot and scaled to adjust based on output
  trimAngle = (trimPot / 100) + 5; // adjust to degrees (5 to -45 degrees)

  // Get the current pitch angle
  pitch = (ypr[1] * 180 / M_PI); // adjust to degrees

  // Update PID inputs
  setpoint = trimAngle;
  input = pitch;

  // Compute PID output
  pid.Compute();

  
  if ( (pitch <= 50 && pitch >= -50) && (output >= 100 || output <= -100) ) {
  // Use PID output to control stepper motor speeds
  stepper1.setSpeed(output);
  stepper2.setSpeed(output);

  // Run the steppers 1 step
  stepper1.run();
  stepper2.run();
  }
}