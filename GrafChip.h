#ifndef __GRAFCHIP_H__
#define __GRAFCHIP_H__

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <GrafComms.h>

class GrafChip
{
 public:
  explicit GrafChip (GrafComms & comms) ;

  virtual void  begin (byte orient) = 0 ;
  GrafComms & get_comms () ;
  virtual int   max_col () = 0 ;
  virtual int   max_row () = 0 ;

  virtual boolean has_fast_fill () ;

  virtual void  fast_fill (int x0, int y0, int x1, int y1, int foreground) ;
  virtual void  dot (int col, int row, int foreground) = 0 ;
  virtual void  move_to (int col, int row) = 0 ;
  virtual void  charline (byte data, byte mult, int foreground, int background, int col) = 0 ;
  virtual void  start_ram () = 0 ;

  virtual void  scroll_to (int x) ;
  virtual void  dim_to (byte x) ;

 protected:
  virtual void  init_regs (byte orient) = 0 ;
  GrafComms & _comms ;

};

#endif
