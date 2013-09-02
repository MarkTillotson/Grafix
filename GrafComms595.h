#ifndef __GRAFCOMMS595_H__
#define __GRAFCOMMS595_H__

#include <GrafComms.h>

class GrafComms595 : public GrafComms
{
 public:
  GrafComms595 () ;

  void  begin (byte reset, byte dc, byte cs, byte wr, byte sr_cs) ;
  void  resetChip () ;

  void  wr_comm (byte reg) ;
  void  wr_data (byte data) ;
  void  wr_data_16 (unsigned int data) ;
  void  wr_data_16xN (unsigned int data, byte count) ;
  void  write_reg (byte reg, unsigned int val) ;
  unsigned int rd_data_16 () ;
  unsigned int read_reg (byte reg) ;
  void  gen_start_ram (byte ram_comm) ;
  void  start_ram () ;
  void  write_ram (unsigned int val) ;
  void  stop_ram () ;

 protected:
  byte pinRES ;
  byte pinCS ;
  byte pinDC ;
  byte pinWR ;
  byte pinSR_CS ;
  volatile byte * data_port ;
  volatile byte * DC_PORT ;
  byte DC_BIT ;
  volatile byte * CS_PORT ;
  byte CS_BIT ;
  volatile byte * SR_CS_PORT ;
  byte SR_CS_BIT ;
  volatile byte * WR_PORT ;
  volatile byte * WR_PIN ;
  byte WR_BIT ;
};

#endif
