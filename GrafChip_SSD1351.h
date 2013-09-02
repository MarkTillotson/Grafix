#ifndef __GRAFCHIP_SSD1351_H__
#define __GRAFCHIP_SSD1351_H__

#include <GrafComms.h>
#include <GrafChip.h>

class GrafChip_SSD1351 : public GrafChip
{
 public:
  explicit GrafChip_SSD1351 (GrafComms & comms) ;

  void  begin (byte orient) ;
  int   max_col () ;
  int   max_row () ;

  void  start_ram () ;
  void  dot (int col, int row, int foreground) ;
  void  move_to (int col, int row) ;

  boolean has_fast_fill () ;
  void  fast_fill (int x0, int y0, int x1, int y1, int foreground) ;
  void  charline (byte line, byte mult, int foreground, int background) ;

 protected:
  void  init_regs (byte orient) ;
  void  set_bounds (byte left, byte right, byte top, byte bot) ;
  byte  row_comm ;
  byte  col_comm ;
  byte  orient ;
};

#endif
