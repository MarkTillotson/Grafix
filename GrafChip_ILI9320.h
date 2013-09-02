#ifndef __GRAFCHIP_ILI9320_H__
#define __GRAFCHIP_ILI9320_H__

#include <GrafComms.h>
#include <GrafChip.h>

class GrafChip_ILI9320 : public GrafChip
{
 public:
  explicit GrafChip_ILI9320 (GrafComms & comms) ;

  void  begin (byte orient) ;
  int   max_col () ;
  int   max_row () ;

  void  start_ram () ;
  void  dot (int col, int row, int foreground) ;
  void  move_to (int col, int row) ;

  boolean has_fast_fill () ;
  void  fast_fill (int x0, int y0, int x1, int y1, int foreground) ;
  void  charline (byte line, byte mult, int foreground, int background, int col) ;

 protected:
  void  init_regs (byte orient) ;
  virtual void  set_gate_scan (byte orient) ;
  byte  orient ;
  int  old_row ;
  int  old_col ;
  byte  col_comm ;
  byte  row_comm ;
};

#endif
