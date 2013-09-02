#include "GrafChip_SSD1351.h"

#define  NUM_COLS 128
#define  NUM_ROWS 128

int GrafChip_SSD1351::max_col () { return NUM_COLS-1 ; }
int GrafChip_SSD1351::max_row () { return NUM_ROWS-1 ; }

#define  RAM_COMM 0x5C

#define  COL_COMM 0x15
#define  ROW_COMM 0x75

GrafChip_SSD1351::GrafChip_SSD1351 (GrafComms & comms) : GrafChip::GrafChip (comms)
{
}

void  GrafChip_SSD1351::begin (byte ori)
{
  orient = ori ;
  init_regs (orient) ;
}

void  GrafChip_SSD1351::start_ram ()
{
  _comms.gen_start_ram (RAM_COMM) ;
}

void  GrafChip_SSD1351::dot (int col, int row, int foreground)
{
  if (col < 0 || col >= NUM_COLS || row < 0 || row >= NUM_ROWS) 
    return ;
  move_to (col, row) ;
  start_ram () ;
  _comms.write_ram (foreground) ;
  _comms.stop_ram () ;
}

void  GrafChip_SSD1351::charline (byte line, byte mult, int foreground, int background)
{
  start_ram () ;
  for (byte i = 0 ; i < 8 ; i++)
  {
    for (byte j = 0 ; j < mult ; j++)
      _comms.write_ram (line & 1 ? foreground : background) ;
    line >>= 1 ;
  }
  _comms.stop_ram () ;
}

boolean GrafChip_SSD1351::has_fast_fill ()
{
  return true ;
}
/*
// Define an inline SPI routine to speed transfers up.  Still limited by
// speed of digitalWrite on CS and DC pins of course.
#define  spitrans(data)       \
  SPDR = (data) ;	      \
  while (!(SPSR & (1<<SPIF))) \
  {}                          \
  SPDR ;
#define  spibegin()    * CS_PORT &= ~CS_BIT
#define  spiend()      * CS_PORT |=  CS_BIT
*/
void  GrafChip_SSD1351::set_bounds (byte left, byte right, byte top, byte bot)
{
  _comms.wr_comm (col_comm) ;
  _comms.wr_data (left) ;
  _comms.wr_data (right) ;
  _comms.wr_comm (row_comm) ;
  _comms.wr_data (top) ;
  _comms.wr_data (bot) ;
}

void  GrafChip_SSD1351::move_to (int col, int row)
{
  set_bounds (col, NUM_COLS-1, row, NUM_ROWS-1) ;
}

void  GrafChip_SSD1351::fast_fill (int x0, int y0, int x1, int y1, int foreground)
{
  set_bounds (x0, x1, y0, y1) ;
  start_ram () ;
  byte yiters = (y1-y0+1) ;
  byte xiters = (x1-x0+1) ;
  for (byte i = 0 ; i < yiters ; i++)
    for (byte j = 0 ; j < xiters ; j++)
      _comms.write_ram (foreground) ;
  _comms.stop_ram () ;
}

void GrafChip_SSD1351::init_regs (byte orient)
{
  boolean rotate = (orient == 3 || orient == 9) ;
  boolean reverse_gates = (orient == 3 || orient == 6) ;
  boolean reverse_sources = (orient == 3 || orient == 12) ;
  if (rotate)
  {
    row_comm = COL_COMM ;
    col_comm = ROW_COMM ;
  }
  else
  {
    col_comm = COL_COMM ;
    row_comm = ROW_COMM ;
  }
  byte  flags = 0x64 ;
  if (reverse_gates)  flags |= 0x10 ;
  if (reverse_sources) flags |= 0x02 ;
  if (rotate)          flags |= 0x01 ;

  delay (10) ;

  _comms.wr_comm (0xFD) ; _comms.wr_data (0xB1) ;  // command lock
  _comms.wr_comm (0xAE) ;                   // sleep
  _comms.wr_comm (0xB3) ; _comms.wr_data (0xF1) ;  // divide ratio / osc freq   (default D1).  Change to F6 for v slow
  _comms.wr_comm (0xA2) ; _comms.wr_data (0x00) ;  // vertical scroll display offset
  _comms.wr_comm (0xA1) ; _comms.wr_data (0x00) ;  // vertical scroll start line
  _comms.wr_comm (0xA0) ; _comms.wr_data (flags) ;
  // _comms.wr_comm (0xA0) ; _comms.wr_data (0x66) ;  // - 64K colours, even/odd split, reverse RGB, rev cols      - tab at 12 o'clock
  // _comms.wr_comm (0xA0) ; _comms.wr_data (0x74) ;  // - 64K colours, even/odd split, reverse RGB, rev rows  - tab at 6 o'clock
  // _comms.wr_comm (0xA0) ; _comms.wr_data (0x77) ;  // - 64K colours, even/odd split, reverse RGB, rev col, rows - tab at 3 o'clock
  // _comms.wr_comm (0xA0) ; _comms.wr_data (0x65) ;  // - 64K colours, even/odd split, reverse RGB                - tab at 9 o'clock

  _comms.wr_comm (0xB5) ; _comms.wr_data (0x00) ;  // GPIO

  //  delay (10) ;
  //  digitalWrite (pinPOW, HIGH) ;
  delay (10) ;

  _comms.wr_comm (0xB8) ;
  for (byte i = 0 ; i < 0x12 ; i++)
    _comms.wr_data (i+2) ;
  for (byte i = 0x12 ; i < 0x1C ; i++)
    _comms.wr_data (i+i-0x0F) ;
  for (byte i = 0x1C ; i < 0x26 ; i++)
    _comms.wr_data (i+i+i-0x2A) ;
  for (byte i = 0x26 ; i < 0x32 ; i++)
    _comms.wr_data ((i<<2)-0x50) ;
  for (byte i = 0x32 ; i < 0x37 ; i++)
    _comms.wr_data (i*5 - 0x82) ;

  _comms.wr_comm (0xA6) ; // display mode normal (A7 = inverse)
  _comms.wr_comm (0xAF) ; // unsleep
  delay (10) ;
}
