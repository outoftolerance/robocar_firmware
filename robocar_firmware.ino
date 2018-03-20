#include <Servo.h>

//Servo objects for outputs
Servo throttle;
Servo steering;

//Define pins for inputs and outputs
const int steering_in_pin = 2;
const int throttle_in_pin = 3;
const int steering_out_pin = 4;
const int throttle_out_pin = 5;

//Set serial baud rate
const int serial_baud = 57600;

//Global variables
int mode = 0;
int throttle_pwm = 0;
int steering_pwm = 0;
int throttle_trim_pwm = 0;
int steering_trim_pwm = 0;

void setup() 
{
  //Begin serial interface
  Serial.begin(serial_baud);

  //Set the pin modes correctly
  pinMode(throttle_in_pin, INPUT);
  pinMode(steering_in_pin, INPUT);
  pinMode(throttle_out_pin, OUTPUT);
  pinMode(steering_out_pin, OUTPUT);

  //Attach servo objects to correct pin
  throttle.attach(throttle_out_pin);
  steeting.attach(steering_out_pin);
}

void loop() 
{
  //Get serial input
  while(serial.available())
  {
    char c = Serial.read();

    if(c == 1)
    {
      
    }
  }
  
  //Do correct mode stuff
  switch(mode)
  {
    //Manual mode
    case 0:
      //Get the input PWM
      throttle_pwm = pulseIn(throttle_in_pin, HIGH);
      steering_pwm = pulseIn(steering_in_pin, HIGH);

      //Check if no input
      if(throttle_pwm = 0)
      {
        throttle_pwm = 1000 + throttle_trim_pwm;
      }
      else
      {
        throttle_pwm += throttle_trim;
      }
      
      if(steering_pwm = 0)
      {
        steering_pwm = 1500 + steering_trim_pwm;
      }
      else
      {
        steering_pwm += steering_trim;
      }
      
      //Set outputs
      throttle.writeMicroseconds(throttle_pwm);
      steering.writeMicroseconds(steering_pwm);
    //Guided Mode
    case 1:
      //Set outputs
      throttle.writeMicroseconds(throttle_pwm);
      steering.writeMicroseconds(steering_pwm);
  }
}
