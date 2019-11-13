typedef const int PIN;


/////////////////////
// Pin Definitions //
/////////////////////

PIN topSensor = 2;    // Switch that detects start of fall
PIN bottomSensor = 8; // Switch that detects end of fall
PIN runReset = 13;    // Switch that resets the system

const double distFall = 65; // cm between sensors (or rather, bottom of sphere to bottom sensors)

void setup() {
  pinMode(topSensor, INPUT_PULLUP);
  pinMode(bottomSensor, INPUT);
  pinMode(runReset, INPUT_PULLUP);

  Serial.begin(9600); // Start the serial port to output data
  
  Serial.println("Setup the hardware and press reset (button @ pin 2) when ready.");
  Serial.println();
}

void loop() {
  // Wait for reset button to be pressed

  while (digitalRead(runReset)) {}
  Serial.println("Reset...checking setup");
  delay(500);

  // Reset was pressed, so check the setup

  boolean flagSetup = true; // set this to false if there is a problem

  Serial.print("Upper sensor is ");
  Serial.println(digitalRead(topSensor));
  if (digitalRead(topSensor)) flagSetup = false;
  delay(1000);

  Serial.print("Lower sensor is ");
  Serial.println(digitalRead(bottomSensor));

  if (!digitalRead(bottomSensor)) flagSetup = false;

  if (flagSetup) { // The setup is ok, proceed

    Serial.println("Ready, proceed.");

    // wait for the object to leave the upper sensor

    while (!digitalRead(topSensor)) {}

    // the object left the top sensor, capture the current time
    double timeDrop = millis();

    // wait for the object to hit the bottom sensor

    while (digitalRead(bottomSensor)) {}

    // the object hit the bottom sensor, stop the time and calculate the acceleration

    timeDrop = millis() - timeDrop;

    double resultAcc = 20000 * distFall / (timeDrop * timeDrop);

    Serial.println();
    Serial.println();
    Serial.print("Calculated acceleration based on a fall time of ");
    Serial.print(timeDrop);
    Serial.println(" ms");
    Serial.print("and a drop distance of ");
    Serial.print(distFall);
    Serial.println(" cm");
    Serial.print("is ");
    Serial.print(resultAcc);
    Serial.println(" m/s^2");
    Serial.println();
    Serial.println();
    Serial.println("Reset the hardware and press reset for another run.");
    Serial.println();
  } else { // There is a problem with the setup
    Serial.println("Check physical setup and press reset.");
    Serial.println();
  }

}
