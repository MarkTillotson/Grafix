#include "GrafChip_ILI9325.h"


GrafChip_ILI9325::GrafChip_ILI9325 (GrafComms & comms) : GrafChip_ILI9320::GrafChip_ILI9320 (comms)
{
}

void GrafChip_ILI9325::begin (byte ori)
{
  orient = ori ;
  _comms.resetChip () ;
  init_regs (orient) ;
}

void GrafChip_ILI9325::init_regs (byte orient)
{
  _comms.write_reg (0xE3, 0x3008) ;
  _comms.write_reg (0xE7, 0x0012) ;
  _comms.write_reg (0xEF, 0x1231) ;

  GrafChip_ILI9320::init_regs (orient) ;

  _comms.write_reg (0x10, 0x17B0) ;
  _comms.write_reg (0x11, 0x0137) ; delay (50) ;
  _comms.write_reg (0x12, 0x001A) ; delay (50) ;
  _comms.write_reg (0x13, 0x1800) ;
  _comms.write_reg (0x29, 0x002A) ; delay (50) ;

  _comms.write_reg (0x93, 0x0003) ;
  _comms.write_reg (0x95, 0x0110) ;
  _comms.write_reg (0x97, 0x0000) ;
  _comms.write_reg (0x98, 0x0000) ;

}

void GrafChip_ILI9325::set_gate_scan (byte orient)
{
  if (orient == 9 || orient == 12)
    _comms.write_reg (0x60, 0x2700) ; // gate scan direction bit 8000
  else
    _comms.write_reg (0x60, 0xA700) ; // gate scan direction bit 8000
}
