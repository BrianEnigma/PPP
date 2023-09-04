#include <Adafruit_CircuitPlayground.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

#define SERVO_MIN -90
#define SERVO_MAX 90
#define SERVO_STEP 5

void setup() 
{
    CircuitPlayground.begin();
    myservo.attach(A2, SERVO_MIN, SERVO_MAX);
    myservo.write(SERVO_MAX);
    CircuitPlayground.setPixelColor(0, 255, 0, 255);
}

void doServo()
{
    CircuitPlayground.setPixelColor(0, 255, 0, 0);
    for (pos = SERVO_MAX; pos >= SERVO_MIN; pos -= SERVO_STEP) 
    {
        myservo.write(pos);
        delay(15);
    }
    for (pos = SERVO_MIN; pos <= SERVO_MAX; pos += SERVO_STEP) {
        myservo.write(pos);
        delay(15);
    }
    CircuitPlayground.setPixelColor(0, 0, 255, 255);
}

void loop() {
    bool leftButtonPressed = CircuitPlayground.leftButton();
    if (leftButtonPressed)
    {
        doServo();
    }
    delay(100);
}
