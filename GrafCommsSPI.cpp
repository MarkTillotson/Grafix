#include <HardSPI.h>

#include "GrafCommsSPI.h"

#include <pins_arduino.h>

GrafCommsSPI::GrafCommsSPI () : GrafComms::GrafComms()
{
}

void  GrafCommsSPI::begin (byte power, byte reset, byte dc, byte cs)
{
  pinPOW = power ;
  pinRES = reset ;
  pinDC = dc ;  DC_PORT = portOutputRegister (digitalPinToPort (dc)) ; DC_BIT = digitalPinToBitMask (dc) ;
  pinCS = cs ;  CS_PORT = portOutputRegister (digitalPinToPort (cs)) ; CS_BIT = digitalPinToBitMask (cs) ;
  // currently we assume cs is pin 10, due to HardSPI issue

  digitalWrite (pinPOW, LOW) ;
  pinMode (pinPOW, OUTPUT) ;
  digitalWrite (pinRES, HIGH) ; pinMode (pinRES, OUTPUT) ;
  digitalWrite (pinCS, HIGH) ;  pinMode (pinCS, OUTPUT) ;
  digitalWrite (pinDC, LOW) ;   pinMode (pinDC, OUTPUT) ;
  delay (10) ;

  digitalWrite (pinRES, LOW) ;  // reset
  delay (2) ;
  digitalWrite (pinRES, HIGH) ;

  spi.begin () ;  // force initialization of SPI modes.
  spi.end () ;
}


// Define an inline SPI routine to speed transfers up.  Still limited by
// speed of digitalWrite on CS and DC pins of course.
#define  spitrans(data)       \
  SPDR = (data) ;	      \
  while (!(SPSR & (1<<SPIF))) \
  {}                          \
  SPDR ;
#define  spibegin()    * CS_PORT &= ~CS_BIT
#define  spiend()      * CS_PORT |=  CS_BIT

 
void  GrafCommsSPI::wr_comm (byte command)
{
  * DC_PORT &= ~DC_BIT ; // digitalWrite (pinDC, LOW) ;
  spibegin () ;
  spitrans (command) ;
  spiend () ;
}

void  GrafCommsSPI::wr_data (byte data)
{
  * DC_PORT |= DC_BIT ; // digitalWrite (pinDC, HIGH) ;
  spibegin () ;
  spitrans (data) ;
  spiend () ;
}

byte  GrafCommsSPI::rd_data ()
{
  * DC_PORT |= DC_BIT ; // digitalWrite (pinDC, HIGH) ;
  spibegin () ;
  byte res = spitrans (0) ;
  spiend () ;
  return res ;
}

void  GrafCommsSPI::gen_start_ram (byte ram_comm)
{
  * DC_PORT &= ~DC_BIT ; // digitalWrite (pinDC, LOW) ;
  spibegin () ;
  spitrans (ram_comm) ;
  * DC_PORT |= DC_BIT ; // digitalWrite (pinDC, HIGH) ;
}

void  GrafCommsSPI::write_ram (unsigned int data)
{
  spitrans (data >> 8) ;
  spitrans (data) ;
} 

void  GrafCommsSPI::stop_ram ()
{
  spiend () ;
}
