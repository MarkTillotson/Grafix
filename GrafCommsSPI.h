#ifndef __GRAFCOMMSSPI_H__
#define __GRAFCOMMSSPI_H__

#include <GrafComms.h>

class GrafCommsSPI : public GrafComms
{
 public:
  GrafCommsSPI () ;

  void  begin (byte power, byte reset, byte dc, byte cs) ;

  void  resetChip () ;

  void  wr_comm (byte reg) ;
  void  wr_data (byte data) ;
  byte  rd_data () ;
  void  wr_data_16 (unsigned int data) ;
  void  wr_data_16xN (unsigned int data, byte count) ;
  unsigned int  rd_data_16 () ;

  unsigned int  read_reg (byte reg) ;
  void  write_reg (byte reg, unsigned int val) ;
  void  gen_start_ram (byte ram_comm) ;
  void  write_ram (unsigned int val) ;
  void  start_ram () ;
  void  stop_ram () ;

 protected:
  byte pinPOW ;
  byte pinRES ;
  byte pinCS ;
  byte pinDC ;
  volatile byte * DC_PORT ;
  byte DC_BIT ;
  volatile byte * CS_PORT ;
  byte CS_BIT ;

  HardSPI spi ;
};

#endif
