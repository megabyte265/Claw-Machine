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
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void setup()
{
  Serial.begin(9600);     // open serial port. Baud rate is 9600
  lcd.begin(16, 2);
}
/*
 * bitsToDigitalWrite is meant to take a char array of ones and zeros (like 10110100)
 * and convert it into an array of integers, which can be passed directly to digitalWrite.
 */
void bitsToDigitalWrite(char buttons[], int len){
  
  String bitString;
  int pins[len] = {0};
  for (int i=0; i<len; i++)         // while loop iterates through each input string index
  {
    bitString = buttons[i];
    pins[i] = bitString.toInt();    // parses character to integer
    lcd.setCursor(i, 1);
    lcd.print("1");
  }
  digitalWrite(13, pins[0]);        // test to determine if button 1 is pressed
  
}
void hexToDigitalWrite(char axles[], int len){
    String hexString = String(0x8f, DEC);
  for (int j=0; j<len; j++){
    hexString = axles[j];
    lcd.setCursor(j, 0);
    lcd.print(hexString);
    }
}

void loop() {
  int numberButtons = 16;
  int numberOutputs = 8;
  int bytes;                                            // number of bytes read
  char buttons[numberButtons];
  int numberAxles = 2;
  char axles[numberAxles];
  if (Serial.available() > numberButtons)
  {
    bytes = Serial.readBytes(buttons, numberButtons);   // reads specific number of bytes to buttons buffer
    if (bytes > 0) {                                    // only calls function if bytes have been read
      bitsToDigitalWrite(buttons, numberOutputs);       // passes buffer (char array) to function for parsing
    }
  }
}
