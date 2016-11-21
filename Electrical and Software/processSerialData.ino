/*
  processSerialData
  Author: Shane Williams
  Author Contact: smw0031@auburn.edu
  Last Edit: 16 November 2016 by Eric Van Horn

  Description:
    processSerialData is a preliminary Arduino sketch used for the S.C.O.R.E. crane machine project.
    Ultimately, this program's purpose is to make the Arduino read serial input from a
    Raspberry Pi or computer, and interpret that serial data as values used to drive digital output pins. The digital output pins shall be connected to motor controllers and servos to drive the crane machine.

  Present State of Development:
    Currently, functions are being developed to convert data received from the serial port into a form that can
    be used to control digital ouput pins.
    Future points of development for the arduino side of the code include:
      --Verify ability to make claw move up/down...left/right...open/close
      ----Axes' values will need to be converted to frequency (based on magnitude) and direction (based on sign)
      ------in order to tell the stepper motors "what to do"
      ------We will also need to keep track of the claw's x/y position (motors step every 2 pulses of square wave)
      ------
      --Using button to "Start" game mode
      --Using button command to make claw DROP, GRAB, RISE, and RETURN HOME
      --Setting limits of claw's range of motion
*/

/*
  = === NOTES === =
  FOR LOOPS   https://www.arduino.cc/en/Reference/For
  SWITCH CASE   https://www.arduino.cc/en/Reference/SwitchCase
  digitalWrite()   https://www.arduino.cc/en/Reference/DigitalWrite
*/
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void printLCD(String input, int col, int row) {
  lcd.setCursor(col, row);
  lcd.print(input);
}


void setup() {
  lcd.begin(16,2);        // set up the LCD's number of columns and rows
  Serial.begin(9600);     // open serial port. Baud rate is 9600
  for (int i=2;i<=9;i++)  // loop to initialize pins
  {
    pinMode(i, OUTPUT);
  }
  pinMode(13, OUTPUT);    // adds pin 13 (built-in LED) for testing
}


int hexToInt(String hexString) {
  int len = hexString.length();
  int value = 0;
  int tempValue;
  int power;
  byte tempBit;
  for (int i = 0; i < len; i++) {
    tempBit = byte(hexString[i]);
    if (tempBit >= '0' and tempBit <= '9') {
      tempValue = int(tempBit - '0');
    }
    else if (tempBit >= 'A' and tempBit <= 'F') {
      tempValue = int(tempBit - 'A' + 10);
    }
    else if (tempBit >= 'a' and tempBit <= 'f') {
      tempValue = int(tempBit - 'a' + 10);
    }
    value += tempValue*bit((len-i-1)*4);
  }
  return value;
}


void hexToAxes(String byteString, int ax1, int ax2) {
/* hexToAxes takes a string representing a string containing the hexadecimal
   representation of integers corresponding to two signed integers and
   converts them to integers for the individual axes.
*/
  String byteString1 = byteString.substring(0, 3);
  ax1 = hexToInt(byteString1.substring(1));
  if (byteString.charAt(0) == '1') {
    ax1 *= -1;
  }

  String byteString2 = byteString.substring(4, 7);
  ax2 = hexToInt(byteString2.substring(1));
  if (byteString.charAt(4) == '1') {
    ax2 *= -1;
  }
}


void bitsToDigitalWrite(char buttons[], int len) {
/* bitsToDigitalWrite is meant to take a char array of ones and zeros (like 10110100)
   and convert it into an array of integers, which can be passed directly to digitalWrite.
*/
  String bitString;
  int pins[len] = {0};
  for (int i=0; i<len; i++) {       // while loop iterates through each input string index
    bitString = buttons[i];
    pins[i] = bitString.toInt();    // parses character to integer
    // digitalWrite(i+2, pins[i]);     // sets pin value to high or low
  }
  digitalWrite(13, pins[0]);        // test to determine if button 1 is pressed
}


void loop() {
  String input;
  String axes;
  String buttons;
  if (Serial.available() > 0) {
    input = Serial.readStringUntil('\n'); // reads specific number of bytes to buttons buffer
    if (input.length() > 0) {             // only calls function if bytes have been read
      if (input.charAt(0) == 'a') {
        axes = input.substring(1, input.length());
        printLCD(axes, 5, 0);
      }
      else if (input.charAt(0) == 'b') {
        buttons = input.substring(1, input.length());
        printLCD(buttons, 0, 1);
      }
      // bitsToDigitalWrite(buttons, numberOutputs);  // passes buffer (char array) to function for parsing
    }
  }
}
