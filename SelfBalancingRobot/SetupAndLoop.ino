void setup() {
  setupMPU();
  setupStepper();
  setupPID();
  
  pinMode(pot, INPUT);
}

void loop() {
  runMPU();
  moveStepper();
}