#include "GrafCommsPar8.h"

#include <pins_arduino.h>

GrafCommsPar8::GrafCommsPar8 () : GrafComms::GrafComms ()
{
}

void GrafCommsPar8::begin (byte reset, byte dc, byte cs, byte wr, byte rd, volatile byte * port)
{
  data_port = port ;
  pinRES = reset ;
  pinDC = dc ;  DC_PORT = portOutputRegister (digitalPinToPort (dc)) ; DC_BIT = digitalPinToBitMask (dc) ;
  pinCS = cs ;  CS_PORT = portOutputRegister (digitalPinToPort (cs)) ; CS_BIT = digitalPinToBitMask (cs) ;
  pinWR = wr ;  WR_PORT = portOutputRegister (digitalPinToPort (wr)) ; WR_BIT = digitalPinToBitMask (wr) ;
  WR_PIN = portInputRegister (digitalPinToPort (wr)) ;
  pinRD = rd ;  RD_PORT = portOutputRegister (digitalPinToPort (rd)) ; RD_BIT = digitalPinToBitMask (rd) ;


  digitalWrite (pinRES, HIGH) ; pinMode (pinRES, OUTPUT) ;
  digitalWrite (pinCS, HIGH) ;  pinMode (pinCS, OUTPUT) ;
  digitalWrite (pinDC, LOW) ;   pinMode (pinDC, OUTPUT) ;
  digitalWrite (pinWR, HIGH) ;  pinMode (pinWR, OUTPUT) ;
  digitalWrite (pinRD, HIGH) ;  pinMode (pinRD, OUTPUT) ;
  DDRD |= 0xFF ;   // assumes databuse is PORTD
  delay (10) ;

  digitalWrite (pinRES, LOW) ;
  delay (2) ;
  digitalWrite (pinRES, HIGH) ;
}

void  GrafCommsPar8::wr_comm (byte reg)
{
  * DC_PORT &= ~DC_BIT ; // digitalWrite (pinDC, LOW) ;
  wr_data_16 (reg) ; // wr_data_16 (reg) ;
}


void  GrafCommsPar8::wr_data (byte data)
{
}  // ????????



void  GrafCommsPar8::wr_data_16 (unsigned int data)
{
  volatile byte * port = data_port ;
  volatile byte * wrpin = WR_PIN ;
  byte wrbit = WR_BIT ;
  * port = data >> 8 ;
  * wrpin = wrbit ; //  digitalWrite (pinWR, LOW) ;  
  * wrpin = wrbit ; //  digitalWrite (pinWR, HIGH) ;
  * port = data ;
  * wrpin = wrbit ; //  digitalWrite (pinWR, LOW) ;  
  * wrpin = wrbit ; //  digitalWrite (pinWR, HIGH) ;
}

void  GrafCommsPar8::wr_data_16xN (unsigned int data, byte count)
{
  volatile byte * port = data_port ;
  volatile byte * wrpin = WR_PIN ;
  byte wrbit = WR_BIT ;
  for (byte i = 0 ; i < count ; i++)
  {
    * port = data >> 8 ;
    * wrpin = wrbit ; //  digitalWrite (pinWR, LOW) ;  
    * wrpin = wrbit ; //  digitalWrite (pinWR, HIGH) ;
    * port = data ;
    * wrpin = wrbit ; //  digitalWrite (pinWR, LOW) ;  
    * wrpin = wrbit ; //  digitalWrite (pinWR, HIGH) ;
  }
}

void  GrafCommsPar8::write_reg (byte reg, unsigned int val)
{
  * CS_PORT &= ~CS_BIT ; // digitalWrite (pinCS, LOW) ;
  wr_comm (reg) ;
  * DC_PORT |= DC_BIT ; // digitalWrite (pinDC, HIGH) ;
  wr_data_16 (val) ;
  * CS_PORT |= CS_BIT ; // digitalWrite (pinCS, HIGH) ;
}

unsigned int  GrafCommsPar8::rd_data_16 ()
{
  // make data bus readable,
  * RD_PORT &= ~RD_BIT ; // digitalWrite (nRD, LOW) ;
  byte high = * data_port ; // read the bus
  * RD_PORT |= RD_BIT ; // digitalWrite (nRD, HIGH) ;
  delayMicroseconds (1) ;
  * RD_PORT &= ~RD_BIT ; // digitalWrite (nRD, LOW) ;
  byte low = * data_port ; // read the bus
  * RD_PORT |= RD_BIT ; // digitalWrite (nRD, HIGH) ;
  // make data bus writeable
  return (high << 8) | low ;
}

unsigned int GrafCommsPar8::read_reg (byte reg) // not used if databus unidirectional
{
  * CS_PORT &= ~CS_BIT ; // digitalWrite (pinCS, LOW) ;
  wr_comm (reg) ;
  * DC_PORT |= DC_BIT ; // data mode
  unsigned int res = rd_data_16 () ;
  * CS_PORT |= CS_BIT ; // digitalWrite (pinCS, HIGH) ;
  return res ;
}


void  GrafCommsPar8::gen_start_ram (byte ram_comm)
{
  * CS_PORT &= ~CS_BIT ; // digitalWrite (pinCS, LOW) ;
  * DC_PORT &= ~DC_BIT ; // digitalWrite (pinDC, LOW) ;
  wr_data_16 (ram_comm) ; // wr_data_16 (ram_comm) ;
  * DC_PORT |=  DC_BIT ; // digitalWrite (pinDC, HIGH) ;
}

void GrafCommsPar8::start_ram () //???????
{
  * CS_PORT &= ~CS_BIT ; // digitalWrite (pinCS, LOW) ;
  * DC_PORT &= ~DC_BIT ; // digitalWrite (pinDC, LOW) ;
}
void GrafCommsPar8::write_ram (unsigned int data)
{
  volatile byte * port = data_port ;
  volatile byte * wrpin = WR_PIN ;
  byte wrbit = WR_BIT ;
  * port = data >> 8 ;
  * wrpin = wrbit ; //  digitalWrite (pinWR, LOW) ;  
  * wrpin = wrbit ; //  digitalWrite (pinWR, HIGH) ;
  * port = data ;
  * wrpin = wrbit ; //  digitalWrite (pinWR, LOW) ;  
  * wrpin = wrbit ; //  digitalWrite (pinWR, HIGH) ;
}  

void  GrafCommsPar8::stop_ram ()
{
  * CS_PORT |= CS_BIT ; //  digitalWrite (pinCS, HIGH) ;
}
