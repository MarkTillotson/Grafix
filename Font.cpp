#include "Font.h"

#include <avr/pgmspace.h>

Font::Font ()
{
}

byte  Font::readbyte (byte * addr)
{
  return pgm_read_byte (addr) ;
}

