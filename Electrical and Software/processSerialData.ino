/*
 * processSerialData
 * Author: Shane Williams
 * Author Contact: smw0031@auburn.edu
 * Last Edit: 8 November 2016
 *
 * Description:
 *    processSerialData is a preliminary Arduino sketch used for the S.C.O.R.E. crane machine project.
 *    Ultimately, this program's purpose is to make the Arduino read serial input from a
 *    Raspberry Pi or computer, and interpret that serial data as values used to drive digital output pins.
 *    The digital output pins shall be connected to motor controllers and servos to drive the crane machine.
 *
 * Present State of Development:
 *    Currently, functions are being developed to convert data received from the serial port into a form that can
 *    be used to control digital ouput pins.
 *    Future points of development for the arduino side of the code include:
 *       --Verify ability to make claw move up/down...left/right...open/close
 *       ----Axes' values will need to be converted to frequency (based on magnitude) and direction (based on sign)
 *       ------in order to tell the stepper motors "what to do"
 *       ------We will also need to keep track of the claw's x/y position (motors step every 2 pulses of square wave)
 *       ------
 *       --Using button to "Start" game mode
 *       --Using button command to make claw DROP, GRAB, RISE, and RETURN HOME
 *       --Setting limits of claw's range of motion
 *
 */

/*
 * = === NOTES === =
 FOR LOOPS   https://www.arduino.cc/en/Reference/For
 SWITCH CASE   https://www.arduino.cc/en/Reference/SwitchCase
 digitalWrite()   https://www.arduino.cc/en/Reference/DigitalWrite
*/

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);                // open serial port. Baud rate is 9600
  for (int i=2;i<=9;i++)
  {
    pinMode(i, OUTPUT);
  }
}

char bitsToDigitalWrite(String inputString)
/*
 * bitsToDigitalWrite is meant to take a STRING of Ones and Zeros (like 10110100)
 * and convert it into an array of strings. 1 becomes HIGH and 0 becomes LOW.
 * This array of HIGH and LOW will be used to write the values of digitalOutput pins.
 * The abundance of Serial.println() and delay() commands is for debugging.
 */
{
  Serial.println(inputString);
  delay(1000);
  char pinValues[8];                                // Trying to declare an array to be filled with strings (HIGH and LOW)
  int i=0;                                          // Initialize loop iteration value
  Serial.println(inputString.length());
  delay(1000);
  int x=inputString.length();                       // write variable x to limit # of loop iterations to length of function input
  while (i<x)                                       // while loop iterates through each input string index
  {
    Serial.println(i);
    delay(1000);
    if (inputString.charAt(i)=='0')                 // tried using switch/case, but it didn't work as well as if/else
    {
      Serial.println("Case 0");
        delay(1000);
        pinValues[i]="LOW";                         // having trouble writing string to index [i] of array
        Serial.println(pinValues);
        delay(1000);
    }
    else if (inputString.charAt(i)=='1')
    {
       Serial.println("Case 1");
        delay(1000);
        pinValues[i]="HIGH";
        Serial.println(pinValues);
        delay(1000);
    }
    else
    {
      Serial.println("Something went wrong");       // this should only run if while loop runs too many iterations
    }
    i++;
  }
  return pinValues;
}


void loop() {
  // put your main code here, to run repeatedly:
  /*
  if (Serial.available() > 0)                             // eventually (when rest is working) will uncomment this to
  {                                                       //     read joystick data and use that to drive output pins
    char joystickdata=                                    //     (instead of hard-coded variable used as of 8 Nov 2016)
    joystickdata = Serial.read();
    joystickdata
    Serial.print(joystickdata);
    */
    while(!Serial);
    {                                                     // pinValues is hard coded in format sent from python
        //String pinValues = Serial.readString();      // variable used during debugging. will be replaced by value read from joystick
        String pinValues = "0, 1, 0, 0, 1, 0, 1, 1";      // reading the buttons from the joystick python code will not
                                                          // trigger the digital write lines...must confirm format of Serial.readString()
        pinValues.replace(" ","");                        // formats string
        pinValues.replace(",","");                        // formats string
         Serial.println(pinValues);
        delay(1000);
        if (pinValues.charAt(0)=='1')
        {
          //Serial.println("Drop the Claw");
          digitalWrite(2, HIGH);
        }
        else
        {
          digitalWrite(2, LOW);
        }
        delay(1000);
        pinValues = "1, 0, 1, 1, 0, 1, 0, 0";      // variable used during debugging. will be replaced by value read from joystick
        pinValues.replace(" ","");                        // formats string
        pinValues.replace(",","");                        // formats string
        Serial.println(pinValues);
        delay(1000);
        if (pinValues.charAt(0)=='1')
        {
          Serial.println("Drop the Claw");
          digitalWrite(2, HIGH);
        }
        else
        {
          digitalWrite(2, LOW);
        }

        // char pins[8]={bitsToDigitalWrite(pinValues)};     // I want to store the array of HIGH and LOW in pins[]
                                                          //    will use this variable to set digitalOutput pins
        // Serial.println(pins);
    }
}
