
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);                // open serial port. Baud rate is 9600
  for (int i=2;i<=9;i++)
  {
    pinMode(i, OUTPUT);
    pinMode(12, OUTPUT);
  }
  while(!Serial)
  {
//    digitalWrite(13, HIGH);
  }
}

void loop()
{
  // char  bitValues[8] = {1,0,1,1,0,1,0,0};
   char bitValues[8] = {0,1,0,0,1,0,1,1};
  // char bitValues[8];

  int it = 0;
//if(Serial.read().length() > 0) {
//  digitalWrite(12, HIGH);
//}


//    for (int i=0;i<8;i++)
//    {
//      bitValues[i]=Serial.read();
//      digitalWrite(12, bitValues[i] == '10100000');
//      delay(700);
//      digitalWrite(12, LOW);
//      delay(700);
//
//    }

// digitalWrite(13, Serial.read() == '1');


    // Serial.println(bitValues);
    while (it<8)
    {
      //char bitValues = {Serial.read()};
      int pin = 2 + it;
      //int strLen = bitValues[it].length()+1;
      //char charArray[strLen];
      //bitValues[it].toCharArray(charArray, strLen);
      //Serial.print(pin);
      //Serial.println(charArray);

      digitalWrite(pin, bitValues[it]);
      //digitalWrite(pin, bitValues[it]=='1'||bitValues[it]==1);
      Serial.println(bitValues[it]);
      it++;
      delay(500);

    }
    it=0;
    //delay(2500);

    while (it<8)
    {
      int pin=2+it;
      digitalWrite(pin, LOW);
      it++;
    }
    // Serial.println("Waiting...");
    //delay(2000);
}
