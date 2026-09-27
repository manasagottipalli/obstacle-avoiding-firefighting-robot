#include <Servo.h>

#define trigPin 9        // Trigger pin of ultrasonic sensor
#define echoPin 10        // Echo pin of ultrasonic sensor
#define flameSensorPin 7  // Flame sensor pin
#define servoPin 11        // Servo motor control pin
#define pumpPin 3          // Pin to control water pump (optional)

Servo waterServo;  // Servo for controlling water nozzle
long duration;
int distance;
int flameDetected;

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);

  // Set the ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set the flame sensor pin
  pinMode(flameSensorPin, INPUT);

  // Set the pump pin (if using a pump)
  pinMode(pumpPin, OUTPUT);

  // Attach the servo motor to pin 11
  waterServo.attach(servoPin);

  // Initialize servo position
  waterServo.write(0);  // Start with the nozzle at 0 degrees (rest position)

  // Optionally, turn off the water pump
  digitalWrite(pumpPin, LOW);
}

void loop() {
  // Measure distance using ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;  // Calculate distance in cm
  Serial.print("Distance: ");
  Serial.println(distance);  // Print the distance value

  // Fire Fighting Logic
  flameDetected = digitalRead(flameSensorPin);

  if (flameDetected == HIGH) {  // Flame detected
    Serial.println("Fire Detected!");
    digitalWrite(pumpPin, HIGH);  // Activate water pump

    // Sweep the servo motor to find the direction of the flame
    for (int angle = 0; angle <= 180; angle += 10) {
      waterServo.write(angle);
      delay(500);  // Wait for the servo to reach the position

      flameDetected = digitalRead(flameSensorPin);  // Check for flame in current direction
      if (flameDetected == HIGH) {
        Serial.print("Fire found at angle: ");
        Serial.println(angle);
        break;  // Exit the loop once the fire is detected
      }
    }

    delay(5000);  // Spray water for 5 seconds
    digitalWrite(pumpPin, LOW);  // Deactivate pump after spraying
    waterServo.write(0);  // Reset the nozzle to its resting position

  } else {
    Serial.println("No Fire Detected");
    waterServo.write(0);  // Reset nozzle position if no fire
  }

  delay(100);  // Short delay before repeating the loop
}