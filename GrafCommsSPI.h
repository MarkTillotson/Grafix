#ifndef __GRAFCOMMSSPI_H__
#define __GRAFCOMMSSPI_H__

#include <GrafComms.h>

class GrafCommsSPI : public GrafComms
{
 public:
  GrafCommsSPI () ;

  void  begin (byte power, byte reset, byte dc, byte cs) ;

  void  wr_comm (byte reg) ;
  void  wr_data (byte data) ;
  byte  rd_data () ;
  void  gen_start_ram (byte ram_comm) ;
  void  write_ram (unsigned int val) ;
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
