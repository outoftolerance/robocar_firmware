#include <Servo.h>

//Servo objects for outputs
Servo throttle;
Servo steering;

//Define pins for inputs and outputs
const int steering_in_pin = 2;
const int throttle_in_pin = 3;
const int steering_out_pin = 4;
const int throttle_out_pin = 5;

//Serial settings
const int serial_buffer_length = 128;
const int serial_baud = 9600;

//Global variables
int mode = 0; //0 = manual, 1 = guided, 
int throttle_millis = 0;  //Commanded throttle
int steering_millis = 0;  //Commanded steering
int throttle_trim_millis = 0; //Commanded throttle trim
int steering_trim_millis = 0; //Commanded steeting trim
int throttle_millis_output = 0; //Calculated throttle output
int steering_millis_output = 0; //Calculated steeting output

//Default PWM values for different states
const int max_millis = 2000;
const int center_millis = 1500;
const int min_millis = 1000;

//Vehicle control settings
const bool enable_reverse = FALSE;

//Serial protocol defines
#define MSG_START_CHAR = '$';
#define MSG_END_CHAR = '\n';
#define MSG_TYPE_SET_MODE = 'm';
#define MSG_TYPE_SET_STEERTING = 's';
#define MSG_TYPE_SET_TROTTLE = 't';
#define MSG_TYPE_TRIM_STEETING = 'd';
#define MSG_TYPE_TRIM_THROTTLE = 'y';

//Setup and init objects
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

//Main program loop
void loop() 
{
  //Switch based on modes
  switch(mode)
  {
    //Manual mode
    case 0:
      //Get the input PWM
      throttle_millis = pulseIn(throttle_in_pin, HIGH);
      steering_millis = pulseIn(steering_in_pin, HIGH);

      //Get serial input, but only allow mode and trim changes
      while(serial.available())
      {
        char c = serial.read();

        //Detect start of message
        if(c == '$')
        {
          //Get message type
          c = serial.read();

          //Switch to correct action
          switch(c)
          {
            case MSG_TYPE_SET_MODE:
              mode = serial.parseInt();
              break;

            case MSG_TYPE_TRIM_STEERING:
              steering_trim_millis += serial.parseInt();
              break;

            case MSG_TYPE_TRIM_TROTTLE:
              throttle_trim_millis += serial.parseInt();
              break;
          }
        }
      }
      break;

    //Guided Mode
    case 1:
      //Get serial input and allow all message types
      while(serial.available())
      {
        char c = serial.read();

        //Detect start of message
        if(c == '$')
        {
          //Get message type
          c = serial.read();

          //Switch to correct action
          switch(c)
          {
            case MSG_TYPE_SET_MODE:
              mode = serial.parseInt();
              break;

            case MSG_TYPE_SET_STEERTING:
              steering_millis = serial.parseInt();
              break;

            case MSG_TYPE_SET_TROTTLE:
              throttle_millis = serial.parseInt();
              break;

            case MSG_TYPE_TRIM_STEERING:
              steering_trim_millis += serial.parseInt();
              break;

            case MSG_TYPE_TRIM_TROTTLE:
              throttle_trim_millis += serial.parseInt();
              break;
          }
        }
      }
      break;

    //Default mode (do nothing)
    default:
      //Set the steering and throttle to nothing
      throttle_millis = center_millis;  //Center is zero throttle since ESC has reverse mode
      steering_millis = center_millis;

      //Reset trims to zero
      throttle_trim_millis = 0;
      steering_trim_millis = 0;
      break;
  }

  //Calculate output values
  throttle_millis_output = throttle_millis + throttle_trim_millis;
  steering_millis_output = steering_millis + steering_trim_millis;

  //Validate throttle output is sane
  if(throttle_millis_output < min_millis)
  {
    throttle_millis_output = min_millis;
  }
  else if(throttle_millis_output > max_millis)
  {
    throttle_millis_output = max_millis;
  }

  //Validate steering output is sane
  if(steering_millis_output < min_millis)
  {
    steering_millis_output = min_millis;
  }
  else if(steering_millis_output > max_millis)
  {
    steering_millis_output = max_millis;
  }

  //Write output PWM values
  throttle.writeMicroseconds(throttle_millis_output);
  steering.writeMicroseconds(steering_millis_output);
}