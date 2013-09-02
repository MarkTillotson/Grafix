#include "Grafix.h"

Grafix::Grafix (GrafChip & disp) : _disp (disp), _comms (disp.get_comms ())
{
  foreground = 0xffff ; // assume FFFF is white
  background = 0x0000 ; // black
  old_row = 0 ;
  old_col = 0 ;
}

void  Grafix::setFont (Font * f)
{
  font = f ;
}


void  Grafix::set_colours (int fore, int back)
{
  foreground = fore ;
  background = back ;
}


void  Grafix::dot (int col, int row)
{
  _disp.dot (col, row, foreground) ;
}

void  Grafix::move_to (int col, int row)
{
  _disp.move_to (col, row) ;
  old_col = col ;
  old_row = row ;
}

void  Grafix::draw_glyph (char c)
{
  if (font == 0)
    return ;
  byte width = font->width () ;
  byte bytes = (width + 7) >> 3 ;
  byte height = font->height () ;
  byte * fontptr = font->findglyph (c) ;
  for (byte i = 0 ; i < height ; i++)
  {
    int col = old_col ;
    _disp.move_to (old_col, old_row) ;
    for (byte j = 0 ; j < bytes ; j++)
    {
      byte row = font->readbyte (fontptr++) ; // pgm_read_byte (fontptr++) ;
      _disp.charline (row, 1, foreground, background, col) ;
      col += 8 ;
    }
    old_row ++ ;
  }
  old_row -= height ;
  old_col += width ;
}

void  Grafix::draw_string (char * s)
{
  char c ;
  while (c = *s++)
  {
    draw_glyph (c) ;
  }
}

//#include "font.h"

void  Grafix::draw_small_glyph (char c)
{
  if (font == 0)
    return ;
  byte width = font->width () ;
  byte bytes = (width + 7) >> 3 ;
  byte height = font->height () ;
  byte * fontptr = font->findglyph (c) ;
  for (byte i = 0 ; i < height ; i+=2)
  {
    _disp.move_to (old_col, old_row) ;
    _disp.start_ram () ;
    for (byte j = 0 ; j < bytes ; j++)
    {
      byte row = font->readbyte (fontptr++) ; // pgm_read_byte (fontptr++) ;
      _comms.write_ram (row & 1 ? foreground : background) ;
      _comms.write_ram (row & 4 ? foreground : background) ;
      _comms.write_ram (row & 0x10 ? foreground : background) ;
      _comms.write_ram (row & 0x40 ? foreground : background) ;
    }
    _comms.stop_ram () ;
    old_row ++ ;
    fontptr += bytes ;
  }
  old_row -= height>>1 ;
  old_col += width>>1 ;
}

void  Grafix::draw_small_string (char * s)
{
  char c ;
  while (c = *s++)
  {
    draw_small_glyph (c) ;
  }
}


void  Grafix::draw_big_glyph (char c, byte mult)
{
  if (font == 0)
    return ;
  byte width = font->width () ;
  byte bytes = (width + 7) >> 3 ;
  byte height = font->height () ;
  byte * fontptr = font->findglyph (c) ;
  int rowcount = mult * height ;
  for (int i = 0 ; i < rowcount ; i++)
  {
    int  col = old_col ;
    _disp.move_to (old_col, old_row) ;
    for (byte j = 0 ; j < bytes ; j++)
    {
      byte row = font->readbyte (fontptr++) ; // pgm_read_byte (fontptr++) ;
      _disp.charline (row, mult, foreground, background, col) ;
      col += mult << 3 ;
    }
    old_row ++ ;
    if ((i + 1) % mult != 0)
      fontptr -= bytes ;
  }
  old_row -= rowcount ;
  old_col += mult * width ;
}

void  Grafix::draw_big_string (char * s, byte mult)
{
  char c ;
  while (c = *s++)
    draw_big_glyph (c, mult) ;
}


void  Grafix::draw_line (int x0, int y0, int x1, int y1)
{
  unsigned int dx = (x1 > x0 ? x1 - x0 : x0 - x1)  ;
  char      xstep =  x1 > x0 ? 1 : -1 ;
  unsigned int dy = (y1 > y0 ? y1 - y0 : y0 - y1) ;
  char      ystep =  y1 > y0 ? 1 : -1 ;
  int col = x0, row = y0 ;
  if (dx < dy)
  {
    int t = - (dy >> 1) ;
    while (true)
    {
      _disp.dot (col, row, foreground) ;
      if (row == y1)
        return ;
      row += ystep ;
      t += dx ;
      if (t >= 0)
      {
        col += xstep ;
        t   -= dy ;
      }
    } 
  }
  else
  {
    int t = - (dx >> 1) ;
    while (true)
    {
      _disp.dot (col, row, foreground) ;
      if (col == x1)
        return ;
      col += xstep ;
      t += dy ;
      if (t >= 0)
      {
        row += ystep ;
        t   -= dx ;
      }
    } 
  }
}


void  Grafix::fill_rect (int x0, int y0, int x1, int y1)
{
  if (x0 < 0)               x0 = 0 ;
  if (x1 > _disp.max_col()) x1 = _disp.max_col() ;
  if (y0 < 0)               y0 = 0 ;
  if (y1 > _disp.max_row()) y1 = _disp.max_row() ;
  if (x0 > x1 || y0 > y1)
    return ;
  if (_disp.has_fast_fill ())
  {
    _disp.fast_fill (x0, y0, x1, y1, foreground) ;
  }
  else
  {
    while (y0 <= y1)
    {
      _disp.move_to (x0, y0) ;
      _disp.start_ram () ;
      for (int i = x0 ; i <= x1 ; i++)
        _comms.write_ram (foreground) ;
      _comms.stop_ram () ;
      y0 ++ ;
    }
  }
}

void  Grafix::draw_rect (int x0, int y0, int x1, int y1)
{
  draw_line (x0, y0, x1, y0) ;
  draw_line (x1, y0, x1, y1) ;
  draw_line (x1, y1, x0, y1) ;
  draw_line (x0, y1, x0, y0) ;
}


void  Grafix::clear_screen ()
{
  int f = foreground ;
  foreground = background ;
  fill_rect (0, 0, _disp.max_col(), _disp.max_row()) ;
  foreground = f ;
}


void Grafix::draw_circle (int x, int y, int radius)
{
  do_circle (false, x, y, radius) ;
}

void Grafix::fill_circle (int x, int y, int radius)
{
  do_circle (true, x, y, radius) ;
}

void Grafix::safe_draw_hline (int xl, int xh, int y)
{
  if (y >= 0 && y < _disp.max_row())
  {
    if (xl < 0)
      xl = 0 ;
    if (xh >= _disp.max_col())
      xh = _disp.max_col()-1 ;
    if (xh > xl)
      draw_line (xl, y, xh, y) ;
  }
}

void Grafix::do_circle (boolean fill, int x, int y, int radius)
{
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x1 = 0;
  int y1 = radius;

  _disp.dot (x, y+radius, foreground) ;
  _disp.dot (x, y-radius, foreground) ;
  if (fill)
  {
    safe_draw_hline (x-radius, x+radius, y) ;
  }
  else
  {
    _disp.dot (x+radius, y, foreground) ;
    _disp.dot (x-radius, y, foreground) ;
  }
 
  while(x1 < y1)
  {
    if (f >= 0) 
    {
      y1-- ;
      ddF_y += 2 ;
      f += ddF_y ;
    }
    x1++ ;
    ddF_x += 2 ;
    f += ddF_x ;    
    circle_slice (fill, x-x1, x+x1, y+y1) ;
    circle_slice (fill, x-x1, x+x1, y-y1) ;
    circle_slice (fill, x-y1, x+y1, y+x1) ;
    circle_slice (fill, x-y1, x+y1, y-x1) ;
  }
}

void Grafix::circle_slice (boolean fill, int xl, int xh, int y)
{
  if (fill)
  {
    safe_draw_hline (xl, xh, y) ;
  }
  else
  {
    _disp.dot (xl, y, foreground) ;
    _disp.dot (xh, y, foreground) ;
  }
}
