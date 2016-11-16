/*
 * processSerialData
 * Author: Shane Williams
 * Author Contact: smw0031@auburn.edu
 * Last Edit: 16 November 2016 by Eric Van Horn
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
  Serial.begin(9600);     // open serial port. Baud rate is 9600
  for (int i=2;i<=9;i++)  // loop to initialize pins
  {
    pinMode(i, OUTPUT);
  }
  pinMode(13, OUTPUT);    // adds pin 13 (built-in LED) for testing
}

void bitsToDigitalWrite(char buttons[], int len)
/*
 * bitsToDigitalWrite is meant to take a char array of ones and zeros (like 10110100)
 * and convert it into an array of integers, which can be passed directly to digitalWrite.
 */
{
  String bitString;
  int pins[len] = {0};
  for (int i=0; i<len; i++)         // while loop iterates through each input string index
  {
    bitString = buttons[i];
    pins[i] = bitString.toInt();    // parses character to integer
    digitalWrite(i+2, pins[i]);     // sets pin value to high or low
  }
  digitalWrite(13, pins[0]);        // test to determine if button 1 is pressed
}


void loop() {
  int numberButtons = 16;
  int numberOutputs = 8;
  int bytes;                                            // number of bytes read
  char buttons[numberButtons];
  if (Serial.available() > numberButtons)
  {
    bytes = Serial.readBytes(buttons, numberButtons);   // reads specific number of bytes to buttons buffer
    if (bytes > 0) {                                    // only calls function if bytes have been read
      bitsToDigitalWrite(buttons, numberOutputs);       // passes buffer (char array) to function for parsing
    }
  }
}
