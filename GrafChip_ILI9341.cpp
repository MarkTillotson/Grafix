#include "GrafChip_ILI9341.h"

#define  NUM_COLS 240
#define  NUM_ROWS 320

int GrafChip_ILI9341::max_col () 
{
  return (orient & 1 ? NUM_ROWS : NUM_COLS) - 1 ;
}
int GrafChip_ILI9341::max_row ()
{
  return (orient & 1 ? NUM_COLS : NUM_ROWS) - 1 ;
}

#define  COL_COMM 0x2A
#define  ROW_COMM 0x2B  

#define  RAM_COMM 0x2C
#define  RE_RAM_COMM 0x3C

GrafChip_ILI9341::GrafChip_ILI9341 (GrafComms & comms) : GrafChip::GrafChip (comms)
{
  old_col = 0 ;
  old_row = 0 ;
}

void GrafChip_ILI9341::begin (byte ori)
{
  orient = ori ;
  _comms.resetChip () ;
  init_regs (orient) ;
}


void GrafChip_ILI9341::start_ram ()
{
  _comms.gen_start_ram (RAM_COMM) ;
}


void  GrafChip_ILI9341::dot (int col, int row, int foreground)
{
  if (col < 0 || col > max_col() || row < 0 || row > max_row()) 
    return ;
  if (col != old_col)
  {
    old_col = col ;
    _comms.write_reg (col_comm, col) ;
    _comms.wr_data_16 (col) ;
  }
  if (row != old_row)
  {
    old_row = row ;
    _comms.write_reg (row_comm, row) ;
    _comms.wr_data_16 (row) ;
  }
  _comms.write_reg (RAM_COMM, foreground) ;
}

void  GrafChip_ILI9341::charline (byte line, byte mult, int foreground, int background, int col)
{
  _comms.gen_start_ram (RE_RAM_COMM) ;  // note move_to now has to force a start_ram.
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

void  GrafChip_ILI9341::move_to (int col, int row)
{
  old_col = col ;
  _comms.write_reg (col_comm, col) ;
  _comms.wr_data_16 (319) ;
  old_row = row ;
  _comms.write_reg (row_comm, row) ;
  _comms.wr_data_16 (319) ;
  _comms.wr_comm (RAM_COMM) ;  // force start at this new position.
}

boolean GrafChip_ILI9341::has_fast_fill ()
{
  return true ;
}

// This duplicates whats in DisplayDriver, but here we avoid lots of indirection for
// calls to write_ram
void  GrafChip_ILI9341::fast_fill (int x0, int y0, int x1, int y1, int foreground)
{
  _comms.write_reg (col_comm, x0) ;
  _comms.wr_data_16 (x1) ;
  _comms.write_reg (row_comm, y0) ;
  _comms.wr_data_16 (y1) ;
  unsigned int pixels = (x1-x0+1)*(y1-y0+1) ;
  start_ram () ;
  while (pixels > 255)
  {
    _comms.wr_data_16xN (foreground, 255) ;
    pixels-=255 ;
  }
  if (pixels > 0)
    _comms.wr_data_16xN (foreground, pixels) ;
  _comms.stop_ram () ;
}


void GrafChip_ILI9341::init_regs (byte orient)
{
  col_comm = COL_COMM ;
  row_comm = ROW_COMM ;
  
  _comms.write_reg (0x01, 0) ; delay (10) ;


  _comms.wr_comm (0xCB);  
  _comms.wr_data (0x39); 
  _comms.wr_data (0x2C); 
  _comms.wr_data (0x00); 
  _comms.wr_data (0x34); 
  _comms.wr_data (0x02); 

  _comms.wr_comm (0xCF);  
  _comms.wr_data (0x00); 
  _comms.wr_data (0XC1); 
  _comms.wr_data (0X30); 

  _comms.wr_comm (0xE8);  
  _comms.wr_data (0x85); 
  _comms.wr_data (0x00); 
  _comms.wr_data (0x78); 

  _comms.wr_comm (0xEA);  
  _comms.wr_data (0x00); 
  _comms.wr_data (0x00); 

  _comms.wr_comm (0xED);  
  _comms.wr_data (0x64); 
  _comms.wr_data (0x03); 
  _comms.wr_data (0X12); 
  _comms.wr_data (0X81); 

  _comms.wr_comm (0xF7);  
  _comms.wr_data (0x20); 

  _comms.wr_comm (0xC0);    	//Power control 
  _comms.wr_data (0x23);   	//VRH[5:0] 

  _comms.wr_comm (0xC1);    	//Power control 
  _comms.wr_data (0x10);   	//SAP[2:0];BT[3:0] 

  _comms.wr_comm (0xC5);    	//VCM control 
  _comms.wr_data (0x3e);   	//Contrast
  _comms.wr_data (0x28); 

  _comms.wr_comm (0xC7);    	//VCM control2 
  _comms.wr_data (0x86);  	 //--

  //	_comms.wr_comm (0x36);    	// Memory Access Control 
  //	_comms.wr_data (0x48);  	//C8	   //48 68绔栧睆//28 E8 妯睆

  byte  memctl = 0x08 ; // BGR set
  if (orient & 1)
    memctl ^= 0x20 ;  // swap
  _comms.wr_comm (0x36); 
  _comms.wr_data (memctl) ;

  _comms.wr_comm (0x3A);    
  _comms.wr_data (0x55); 

  _comms.wr_comm (0xB1);    
  _comms.wr_data (0x00);  
  _comms.wr_data (0x18); 

  byte scanctl = 0x82 ;
  if (orient == 6 || orient == 9)
    scanctl ^= 0x40 ;  // gate scan reverse
  if (orient == 9 || orient == 12)
    scanctl ^= 0x20 ;  // source scan reverse
  _comms.wr_comm (0xB6);    	// Display Function Control 
  _comms.wr_data (0x08); 
  _comms.wr_data (scanctl);
  _comms.wr_data (0x27);  
 
  _comms.wr_comm (0xF2);    	// 3Gamma Function Disable 
  _comms.wr_data (0x00); 

  _comms.wr_comm (0x26);    	//Gamma curve selected 
  _comms.wr_data (0x01); 
  
  _comms.wr_comm (0xE0);    	//Set Gamma 
  _comms.wr_data (0x0F); 
  _comms.wr_data (0x31); 
  _comms.wr_data (0x2B); 
  _comms.wr_data (0x0C); 
  _comms.wr_data (0x0E); 
  _comms.wr_data (0x08); 
  _comms.wr_data (0x4E); 
  _comms.wr_data (0xF1); 
  _comms.wr_data (0x37); 
  _comms.wr_data (0x07); 
  _comms.wr_data (0x10); 
  _comms.wr_data (0x03); 
  _comms.wr_data (0x0E); 
  _comms.wr_data (0x09); 
  _comms.wr_data (0x00); 

  _comms.wr_comm (0XE1);    	//Set Gamma 
  _comms.wr_data (0x00); 
  _comms.wr_data (0x0E); 
  _comms.wr_data (0x14); 
  _comms.wr_data (0x03); 
  _comms.wr_data (0x11); 
  _comms.wr_data (0x07); 
  _comms.wr_data (0x31); 
  _comms.wr_data (0xC1); 
  _comms.wr_data (0x48); 
  _comms.wr_data (0x08); 
  _comms.wr_data (0x0F); 
  _comms.wr_data (0x0C); 
  _comms.wr_data (0x31); 
  _comms.wr_data (0x36); 
  _comms.wr_data (0x0F); 
  
  _comms.wr_comm (0x11);    	//Exit Sleep 
  delay(120); 

  _comms.wr_comm (0x29);    //Display on 
  _comms.wr_comm (0x2c);   

}

void GrafChip_ILI9341::set_gate_scan (byte orient) 
{
}
