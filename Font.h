#ifndef __FONT_H__
#define __FONT_H__

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


class Font
{
 public:
  Font () ;

  virtual  byte  width () = 0 ;
  virtual  byte  height () = 0 ;
  virtual  byte * findglyph (char c) = 0 ;
  virtual  byte  readbyte (byte * addr) ;
};

#endif
