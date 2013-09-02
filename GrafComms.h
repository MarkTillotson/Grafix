#ifndef __GRAFCOMMS_H__
#define __GRAFCOMMS_H__

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class GrafComms
{
 public:
  GrafComms () ;

  virtual void  resetChip () = 0 ;

  virtual void  wr_comm (byte comm) = 0 ;
  virtual void  wr_data (byte data) = 0 ;
  virtual void  wr_data_16 (unsigned int data) = 0 ;
  virtual void  wr_data_16xN (unsigned int data, byte count) = 0 ;
  virtual unsigned int  rd_data_16 () = 0 ;

  virtual unsigned int  read_reg (byte reg) = 0 ;
  virtual void  write_reg (byte reg, unsigned int val) = 0 ;

  virtual void  gen_start_ram (byte ram_comm) = 0 ;
  virtual void  write_ram (unsigned int val) = 0 ;
  virtual void  start_ram () = 0 ;
  virtual void  stop_ram () = 0 ;
};

#endif
