#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_READ(base, mask)         (((*(base)) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)   ((*((base)+1)) &= ~(mask), (*((base)+2)) &= ~(mask))
#define DIRECT_MODE_OUTPUT(base, mask)  ((*((base)+1)) |= (mask))
#define DIRECT_WRITE_LOW(base, mask)    ((*((base)+2)) &= ~(mask))
#define DIRECT_WRITE_HIGH(base, mask)   ((*((base)+2)) |= (mask))
#define IO_REG_TYPE uint8_t

#define set_bit(REG, BIT) (REG |= (1<<BIT))
#define clr_bit(REG, BIT) (REG &= ~(1<<BIT))
#define tst_bit(REG, BIT) (REG & (1<<BIT))

int SquareValue[64];//[2];

// variables
int error;
unsigned long  leastTotal = 5150;
unsigned int   loopTimingFactor;
unsigned long  CS_Timeout_Millis;
//unsigned long  CS_AutocaL_Millis;
unsigned long  lastCal;
unsigned long  total;
IO_REG_TYPE sBit;   // send pin's ports and bitmask
volatile IO_REG_TYPE *sReg;
IO_REG_TYPE rBit;    // receive pin's ports and bitmask
volatile IO_REG_TYPE *rReg;
// methods
int SenseOneCycle(void);
int sendPin = 4, receivePin = 2;
int value;
int Read;




void setup()
{
  DDRB = 0b00111111;
  
  Serial.begin(9600);
  loopTimingFactor = 310;   // determined empirically -  a hack

  CS_Timeout_Millis = (2000 * (float)loopTimingFactor * (float)F_CPU) / 16000000;
  //CS_AutocaL_Millis = 20000;

  pinMode(sendPin, OUTPUT);            // sendpin to OUTPUT
  pinMode(receivePin, INPUT);           // receivePin to INPUT
  digitalWrite(sendPin, LOW);

  sBit = PIN_TO_BITMASK(sendPin);         // get send pin's ports and bitmask
  sReg = PIN_TO_BASEREG(sendPin);         // get pointer to output register

  rBit = PIN_TO_BITMASK(receivePin);        // get receive pin's ports and bitmask
  rReg = PIN_TO_BASEREG(receivePin);

  // get pin mapping and port for receive Pin - from digital pin functions in Wiring.c
  //  leastTotal = 0x0FFFFFFFL;   // input large value for autocalibrate begin
  //  lastCal = millis();         // set millis for start;
  value = ReadCapacitiveSensor(500);

}


void loop()
{
  Read = ReadCapacitiveSensor(50);
  if (Read < 430)
    value += (Read - value)/10;

  //if (value < 15000 && value > -100) 
    Serial.println(value);
  

  if (value > 140)
    digitalWrite(13, HIGH);
  else
    digitalWrite(13, LOW);

  delay(10);                             // arbitrary delay to limit data to serial port
}









long ReadCapacitiveSensor(uint8_t samples)
{
  total = 0;
  if (samples == 0) return 0;
  if (error < 0) return -1;

  for (uint8_t i = 0; i < samples; i++) {
    if (SenseOneCycle() < 0)  return -2;
  }


  //  if ( (millis() - lastCal > CS_AutocaL_Millis) && abs(total  - leastTotal) < (int)(.10 * (float)leastTotal) ) {
  //    leastTotal = 0x0FFFFFFFL;
  //    lastCal = millis();
  //  }

  //if (total < leastTotal) leastTotal = total;                 // set floor value to subtract from sensed value
  return (total - leastTotal);

}





int SenseOneCycle(void)
{
  //  clr_bit(PORTD,4);   //DIRECT_WRITE_LOW(sReg, sBit); // sendPin Register low
  //  clr_bit(PORTD,2);   //DIRECT_MODE_INPUT(rReg, rBit);  // receivePin to input (pullups are off)
  //  set_bit(PORTD,2);   //DIRECT_MODE_OUTPUT(rReg, rBit); // receivePin to OUTPUT
  //  clr_bit(PORTD,2);   //DIRECT_WRITE_LOW(rReg, rBit); // pin is now LOW AND OUTPUT
  DIRECT_WRITE_LOW(sReg, sBit); // sendPin Register low
  DIRECT_MODE_INPUT(rReg, rBit);  // receivePin to input (pullups are off)
  DIRECT_MODE_OUTPUT(rReg, rBit); // receivePin to OUTPUT
  DIRECT_WRITE_LOW(rReg, rBit); // pin is now LOW AND OUTPUT
  delayMicroseconds(10);
  DIRECT_MODE_INPUT(rReg, rBit);  // receivePin to input (pullups are off)
  DIRECT_WRITE_HIGH(sReg, sBit);  // sendPin High


  while ( !DIRECT_READ(rReg, rBit) && (total < CS_Timeout_Millis) ) {  // while receive pin is LOW AND total is positive value
    total++;
  }



  DIRECT_WRITE_HIGH(rReg, rBit);
  DIRECT_MODE_OUTPUT(rReg, rBit);  // receivePin to OUTPUT - pin is now HIGH AND OUTPUT
  DIRECT_WRITE_HIGH(rReg, rBit);
  DIRECT_MODE_INPUT(rReg, rBit);  // receivePin to INPUT (pullup is off)
  DIRECT_WRITE_LOW(sReg, sBit); // sendPin LOW


  while ( DIRECT_READ(rReg, rBit) && (total < CS_Timeout_Millis) ) {  // while receive pin is HIGH  AND total is less than timeout
    total++;
  }
}



void choseSquare(byte numbers) {
  for (int i = 0; i < 5; i++) {
    if (tst_bit(numbers, i)) set_bit(PORTB, i);
    else clr_bit(PORTB, i);
  }
}

void ReadSquares() {
  for (int square = 0; square < 64; square++) {
    choseSquare(square);
    //SquareValue[square] = ReadCapacitiveSensor(10);
    
  }
}
