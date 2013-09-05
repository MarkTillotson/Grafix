#include "GrafChip_ILI9320.h"

#define  NUM_COLS 240
#define  NUM_ROWS 320

int GrafChip_ILI9320::max_col () 
{
  return (orient & 1 ? NUM_ROWS : NUM_COLS) - 1 ;
}
int GrafChip_ILI9320::max_row ()
{
  return (orient & 1 ? NUM_COLS : NUM_ROWS) - 1 ;
}

#define  COL_COMM 0x20
#define  ROW_COMM 0x21  

#define  RAM_COMM 0x22

GrafChip_ILI9320::GrafChip_ILI9320 (GrafComms & comms) : GrafChip::GrafChip (comms)
{
  old_col = 0 ;
  old_row = 0 ;
}

void GrafChip_ILI9320::begin (byte ori)
{
  orient = ori ;
  _comms.resetChip () ;
  init_regs (orient) ;
}


void GrafChip_ILI9320::start_ram ()
{
  _comms.gen_start_ram (RAM_COMM) ;
}


void  GrafChip_ILI9320::dot (int col, int row, int foreground)
{
  if (col < 0 || col > max_col() || row < 0 || row > max_row()) 
    return ;
  if (col != old_col)
  {
    old_col = col ;
    _comms.write_reg (col_comm, col) ;
  }
  if (row != old_row)
  {
    old_row = row ;
    _comms.write_reg (row_comm, row) ;
  }
  _comms.write_reg (RAM_COMM, foreground) ;
}

void  GrafChip_ILI9320::charline (byte line, byte mult, int foreground, int background, int col)
{
  start_ram () ;
  byte count = mult ;
  byte prev = line & 1 ;
  for (byte i = 1 ; i < 8 ; i++)
  {
    line >>= 1 ;
    byte bit = line & 1 ;
    if (bit == prev)
    {
      count += mult ;
    }
    else
    {
      _comms.wr_data_16xN (prev == 1 ? foreground : background, count) ;
      count = mult ;
      prev = bit ;
    }
  }
  _comms.wr_data_16xN (prev == 1 ? foreground : background, count) ;
  _comms.stop_ram () ;
}

void  GrafChip_ILI9320::move_to (int col, int row)
{
  old_col = col ;
  _comms.write_reg (col_comm, col) ;
  old_row = row ;
  _comms.write_reg (row_comm, row) ;
}

boolean GrafChip_ILI9320::has_fast_fill ()
{
  return true ;
}

// This duplicates whats in DisplayDriver, but here we avoid lots of indirection for
// calls to write_ram
void  GrafChip_ILI9320::fast_fill (int x0, int y0, int x1, int y1, int foreground)
{
  while (y0 <= y1)
  {
    move_to (x0, y0) ;
    start_ram () ;
    int i = x0 ;
    while (i <= x1-255)
    {
      _comms.wr_data_16xN (foreground, 255) ;
      i+=255 ;
    }
    if (i <= x1)
      _comms.wr_data_16xN (foreground, x1-i+1) ;

    _comms.stop_ram () ;
    y0 ++ ;
  }
}


void GrafChip_ILI9320::init_regs (byte orient)
{
  if (orient & 1) // 3 or 9 o'clock
  {
    col_comm = ROW_COMM ;
    row_comm = COL_COMM ;
  }
  else // 6 or 12 o'clock
  {
    col_comm = COL_COMM ;
    row_comm = ROW_COMM ;
  }

  _comms.write_reg (0x00, 0x0001) ; delay (10) ;
  //  _comms.write_reg (0xA4, 0x0001) ; // is this needed?
  _comms.write_reg (0x07, 0x0000) ; delay (10) ;

  if (orient == 6 || orient == 9)
    _comms.write_reg (0x01, 0x0100) ; // Source scan direction bit 0100
  else 
    _comms.write_reg (0x01, 0x0000) ; // Source scan direction bit 0100

  _comms.write_reg (0x02, 0x0700) ;
  
  if (orient & 1)
    _comms.write_reg (0x03, 0x1038) ;
  else
    _comms.write_reg (0x03, 0x1030) ;

  set_gate_scan (orient) ;

  _comms.write_reg (0x04, 0x0000) ;
  _comms.write_reg (0x08, 0x0202) ;
  _comms.write_reg (0x09, 0x0000) ;
  _comms.write_reg (0x07, 0x0101) ;  // turn on?
  _comms.write_reg (0x17, 0x0001) ;
  _comms.write_reg (0x10, 0x0000) ;
  _comms.write_reg (0x11, 0x0007) ;
  _comms.write_reg (0x12, 0x0000) ;
  _comms.write_reg (0x13, 0x0000) ; delay (20) ;
  _comms.write_reg (0x10, 0x16B0) ;

  _comms.write_reg (0x11, 0x0037) ; delay (50) ;

  _comms.write_reg (0x12, 0x013E) ; delay (50) ;

  _comms.write_reg (0x13, 26 << 8) ;  // was 26
  _comms.write_reg (0x29, 0x000F) ; delay (50) ;

  _comms.write_reg (0x20, 0x0000) ;
  _comms.write_reg (0x21, 0x0000) ;

  _comms.write_reg (0x50, 0x0000) ;
  _comms.write_reg (0x51, 0x00EF) ;
  _comms.write_reg (0x52, 0x0000) ;
  _comms.write_reg (0x53, 0x013F) ;

  _comms.write_reg (0x61, 0x0001) ;
  _comms.write_reg (0x6A, 0x0000) ;

  _comms.write_reg (0x90, 0x0010) ;
  _comms.write_reg (0x92, 0x0000) ;
  _comms.write_reg (0x93, 0x0000) ;
  
  _comms.write_reg (0x30, 0x0505) ;
  _comms.write_reg (0x31, 0x0505) ;
  _comms.write_reg (0x32, 0x0505) ;
  _comms.write_reg (0x35, 0x0404) ;
  _comms.write_reg (0x36, 0x001F) ;
  _comms.write_reg (0x37, 0x0505) ;
  _comms.write_reg (0x38, 0x0505) ;
  _comms.write_reg (0x39, 0x0505) ;
  _comms.write_reg (0x3C, 0x0404) ;
  _comms.write_reg (0x3D, 0x1F10) ;
  /*
  _comms.write_reg (0x30, 0x0507) ;
  _comms.write_reg (0x31, 0x0404) ;
  _comms.write_reg (0x32, 0x0205) ;
  _comms.write_reg (0x35, 0x0000) ;
  _comms.write_reg (0x36, 0x1517) ;
  _comms.write_reg (0x37, 0x0507) ;
  _comms.write_reg (0x38, 0x0404) ;
  _comms.write_reg (0x39, 0x0205) ;
  _comms.write_reg (0x3C, 0x0000) ;
  _comms.write_reg (0x3D, 0x1517) ;
  */
  _comms.write_reg (0x07, 0x0173) ; delay (50) ;
}

void GrafChip_ILI9320::set_gate_scan (byte orient) 
{
  if (orient == 9 || orient == 12)
    _comms.write_reg (0x60, 0xA700) ; // gate scan direction bit 8000
  else
    _comms.write_reg (0x60, 0x2700) ; // gate scan direction bit 8000
}
