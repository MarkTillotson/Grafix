#include "GrafComms595.h"

#include <pins_arduino.h>

GrafComms595::GrafComms595 () : GrafComms::GrafComms ()
{
}

void GrafComms595::begin (byte reset, byte dc, byte cs, byte wr, byte sr_cs)
{
  pinRES = reset ;
  pinDC = dc ;  DC_PORT = portOutputRegister (digitalPinToPort (dc)) ; DC_BIT = digitalPinToBitMask (dc) ;
  pinCS = cs ;  CS_PORT = portOutputRegister (digitalPinToPort (cs)) ; CS_BIT = digitalPinToBitMask (cs) ;
  pinWR = wr ;  WR_PORT = portOutputRegister (digitalPinToPort (wr)) ; WR_BIT = digitalPinToBitMask (wr) ;
  WR_PIN = portInputRegister (digitalPinToPort (wr)) ;
  pinSR_CS = sr_cs ; SR_CS_PORT = portOutputRegister (digitalPinToPort (sr_cs)) ; SR_CS_BIT = digitalPinToBitMask (sr_cs) ;

  digitalWrite (pinRES, HIGH) ; pinMode (pinRES, OUTPUT) ;
  digitalWrite (pinCS, HIGH) ;  pinMode (pinCS, OUTPUT) ;
  digitalWrite (pinSR_CS, HIGH) ;  pinMode (pinSR_CS, OUTPUT) ;
  digitalWrite (pinDC, LOW) ;   pinMode (pinDC, OUTPUT) ;
  digitalWrite (pinWR, HIGH) ;  pinMode (pinWR, OUTPUT) ;

  // Hardware SPI setup, 8 MHz
  pinMode (SS, OUTPUT) ;  digitalWrite (SS, HIGH) ;
  pinMode (SCK, OUTPUT) ; digitalWrite (SCK, LOW) ;
  pinMode (MOSI, OUTPUT) ; digitalWrite (MOSI, LOW) ;
  SPSR = 1 ;
  SPCR = (1<<SPE) | (1<<MSTR) ;
  byte junk = SPSR ;
  junk |= SPDR ;

  delay (10) ;
}

void  GrafComms595::resetChip ()
{
  digitalWrite (pinRES, LOW) ;
  delay (2) ;
  digitalWrite (pinRES, HIGH) ;
}

void  GrafComms595::wr_comm (byte reg)
{
  * DC_PORT &= ~DC_BIT ; // digitalWrite (pinDC, LOW) ;
  wr_data_16 (reg) ; // wr_data_16 (reg) ;
}


void  GrafComms595::wr_data (byte val)
{
  volatile byte * wrpin = WR_PIN ;
  volatile byte * csport = SR_CS_PORT ;
  byte wrbit = WR_BIT ;
  byte csbit = SR_CS_BIT ;

  * csport &= ~csbit ;
  SPDR = val ;
  while (!(SPSR & (1<<SPIF)))
    {}
  * csport |= csbit ;
  asm ("nop") ;
  * wrpin = wrbit ;
  asm ("nop") ;
  * wrpin = wrbit ;
  * csport &= ~csbit ;
}


void  GrafComms595::wr_data_16 (unsigned int data)
{
  /*
    wr_data (data >> 8) ;
    wr_data (data) ;
  */

  volatile byte * wrpin = WR_PIN ;
  volatile byte * csport = SR_CS_PORT ;
  byte wrbit = WR_BIT ;
  byte csbit = SR_CS_BIT ;
  * csport &= ~csbit ;
  SPDR = data>>8 ;
  while (!(SPSR & (1<<SPIF)))
    {}
  * csport |= csbit ;
  asm ("nop") ;
  * wrpin = wrbit ;
  asm ("nop") ;
  * wrpin = wrbit ;
  * csport &= ~csbit ;
  SPDR = data ;
  while (!(SPSR & (1<<SPIF)))
    {}
  * csport |= csbit ;
  asm ("nop") ;
  * wrpin = wrbit ;
  asm ("nop") ;
  * wrpin = wrbit ;
}

void  GrafComms595::wr_data_16xN (unsigned int data, byte count)
{
  volatile byte * wrpin = WR_PIN ;
  volatile byte * csport = SR_CS_PORT ;
  byte wrbit = WR_BIT ;
  byte csbit = SR_CS_BIT ;
  for (byte i = 0 ; i < count ; i++)
  {
    /*
    wr_data_16 (data) ;
    */

    * csport &= ~csbit ;
    SPDR = data>>8 ;
    while (!(SPSR & (1<<SPIF)))
      {}
    * csport |= csbit ;
    asm ("nop") ;
    * wrpin = wrbit ;
    asm ("nop") ;
    * wrpin = wrbit ;
    * csport &= ~csbit ;
    SPDR = data ;
    while (!(SPSR & (1<<SPIF)))
      {}
    * csport |= csbit ;
    asm ("nop") ;
    * wrpin = wrbit ;
    asm ("nop") ;
    * wrpin = wrbit ;
  }
}

void  GrafComms595::write_reg (byte reg, unsigned int val)
{
  * CS_PORT &= ~CS_BIT ; // digitalWrite (pinCS, LOW) ;
  wr_comm (reg) ;
  * DC_PORT |= DC_BIT ; // digitalWrite (pinDC, HIGH) ;
  wr_data_16 (val) ;
  * CS_PORT |= CS_BIT ; // digitalWrite (pinCS, HIGH) ;
}

unsigned int  GrafComms595::rd_data_16 ()
{
  return 0 ;
}


unsigned int GrafComms595::read_reg (byte reg) // not used if databus unidirectional
{
  * CS_PORT &= ~CS_BIT ; // digitalWrite (pinCS, LOW) ;
  wr_comm (reg) ;
  * DC_PORT |= DC_BIT ; // data mode
  unsigned int res = rd_data_16 () ;
  * CS_PORT |= CS_BIT ; // digitalWrite (pinCS, HIGH) ;
  return res ;
}


void  GrafComms595::gen_start_ram (byte ram_comm)
{
  * CS_PORT &= ~CS_BIT ; // digitalWrite (pinCS, LOW) ;
  * DC_PORT &= ~DC_BIT ; // digitalWrite (pinDC, LOW) ;
  wr_data_16 (ram_comm) ; // wr_data_16 (ram_comm) ;
  * DC_PORT |=  DC_BIT ; // digitalWrite (pinDC, HIGH) ;
}

void GrafComms595::start_ram () //???????
{
}

void GrafComms595::write_ram (unsigned int data)
{
  wr_data (data >> 8) ;
  wr_data (data) ;
}  

void  GrafComms595::stop_ram ()
{
  * CS_PORT |= CS_BIT ; //  digitalWrite (pinCS, HIGH) ;
}
