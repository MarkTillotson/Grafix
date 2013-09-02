#ifndef __GRAFIX_H__
#define __GRAFIX_H__

#include <Grafix.h>
#include <Font.h>
#include <GrafChip.h>

class Grafix
{
 public:
  Grafix (GrafChip & disp) ;

  void setFont (Font * font) ;

  void dot (int col, int row) ;
  void move_to (int col, int row) ;
  void draw_line (int x0, int y0, int x1, int y1) ;
  void draw_rect (int x0, int y0, int x1, int y1) ;
  void fill_rect (int x0, int y0, int x1, int y1) ;
  void draw_glyph (char c) ;
  void draw_string (char * s) ;
  void draw_big_glyph (char c, byte mult) ;
  void draw_big_string (char * s, byte mult) ;
  void draw_small_glyph (char c) ;
  void draw_small_string (char * s) ;
  void clear_screen () ;
  void set_colours (int fore, int back) ;
  void draw_circle (int x, int y, int radius) ;
  void fill_circle (int x, int y, int radius) ;

 protected:
  void do_circle (boolean fill, int x, int y, int radius) ;
  void circle_slice (boolean fill, int xl, int xh, int y) ;
  void safe_draw_hline (int xl, int xh, int y) ;

 private:
  int old_row ;
  int old_col ;
  int foreground ;
  int background ;

  GrafChip & _disp ;
  GrafComms & _comms ;
  Font * font ;

};

#endif
