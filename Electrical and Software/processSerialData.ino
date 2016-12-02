/*
  processSerialData
  Author: Shane Williams, Eric Van Horn, and Jacob Van Horn
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
  Pwm frequency http://playground.arduino.cc/Code/PwmFrequency
*/
int xDir = 9;
int xEn = 10;
int xStep = 8;

int yDir = 12;
int yEn = 13;
int yStep = 11;

int freqDivisor = 31250;

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}


void setup() {
  //lcd.begin(16,2);        // set up the LCD's number of columns and rows
  Serial.begin(9600);     // open serial port. Baud rate is 9600
  for (int i=8;i<=13;i++)  // loop to initialize pins
  {
    pinMode(i, OUTPUT);
  }
  setPwmFrequency(xStep, freqDivisor);
  setPwmFrequency(yStep, freqDivisor);
}

void dir(int sign, int axis){
  //digitalWrite(axis, sign);
  switch (sign) {
    case 0:
      digitalWrite(axis, LOW);
      break;
    case 1:
      digitalWrite(axis, HIGH);
      break;
  }
  delay(1);
}

void moov(int input, int aEN, int aST, int aDI, int sign){  //mispelled move on purpose because move is an reserved word
  //int in = input.toInt();
  if (input > 50){
    digitalWrite(aEN, HIGH);
    delay(1);
    dir(sign, aDI);
    analogWrite(aST, 2.5);
  }else{
    digitalWrite(aEN, LOW);
    analogWrite(aST, 0);
  }
}

void loop() {
  String input;
  int sign;
  int button;
  int value;
  int axis;
  int aEn;
  int aSt;
  int aDir;
  if (Serial.available() > 0) {
    input = Serial.readStringUntil('\n'); // reads specific number of bytes to buttons buffer
    if (input.length() > 0) {             // only calls function if bytes have been read
      switch (input.charAt(0)) {
        case 'a':
          switch (input.charAt(1)) {
            case '0':
              axis = 0;
              break;
            case '1':
              axis = 3;
              break;
          }
          sign = int(input.charAt(2));
          //sign = String (input.charAt(2));
          aEn = xEn + axis;
	        aSt = xStep + axis;
	        aDir = xDir + axis;
          value = input.substring(3, input.length()).toInt();
          moov(value, aEn, aSt, aDir, sign);
          break;
        case 'b':
          button = input.substring(1, 3).toInt();
          value = input.substring(3, 4).toInt();
          // digitalWrite(button, value);  // passes buffer (char array) to function for parsing
          break;
      }
    }
  }
}
