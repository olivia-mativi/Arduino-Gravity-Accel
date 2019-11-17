typedef const int PIN;

PIN bottomSensor  = 2;    
PIN topSensor     = 3;    
PIN LED_Ready     = 4;
PIN LED_Not_Ready = 5;
PIN reset_sw      = 6;    

const double distance = 90.1; // cm between end sensor and edge of sphere closest to end sensor

void setup() {
  pinMode(topSensor, INPUT_PULLUP);
  pinMode(bottomSensor, INPUT);
  pinMode(reset_sw, INPUT_PULLUP);
  pinMode(LED_Ready, OUTPUT);
  pinMode(LED_Not_Ready, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(!systemReady){
    digitalWrite(LED_Ready, LOW);
    digitalWrite(LED_Not_Ready, HIGH);
  } else {
    digitalWrite(LED_Ready, HIGH);
    digitalWrite(LED_Not_Ready, LOW);
  }
  while (digitalRead(reset_sw)) {}
  Serial.println("Reset in progress");
  delay(500);

  // Check setup validity

  bool flagSetup = true;

  Serial.print("topSensor state: ");
  Serial.println(digitalRead(topSensor));
  if (digitalRead(topSensor)) flagSetup = false;
  delay(1000);

  Serial.print("bottomSensor state:");
  Serial.println(digitalRead(bottomSensor));

  if (!digitalRead(bottomSensor)) flagSetup = false;

  if (flagSetup) {
    systemReady=true;
    digitalWrite(LED_Ready, HIGH);
    digitalWrite(LED_Not_Ready, LOW);
    while (!digitalRead(topSensor)) {}
    // Start time
    double stime = millis();
    while (digitalRead(bottomSensor)) {}
    // Stop time, calc diff
    stime = millis() - stime;
    digitalWrite(LED_Ready, LOW);
    digitalWrite(LED_Not_Ready, HIGH);
    // Calc acceleration
    double acceleration = 20000 * distance / (stime * stime);

    Serial.println();
    Serial.println("---------------------------");
    Serial.print("dt: ");
    Serial.print(stime);
    Serial.println(" ms");
    Serial.print("a: ");
    Serial.print(acceleration);
    Serial.println(" m/s^2");
    Serial.println("---------------------------");
    systemReady=false;
  } else {
    systemReady=false;
    Serial.println("ERROR: CHECK SETUP");
    Serial.println();
  }

}
