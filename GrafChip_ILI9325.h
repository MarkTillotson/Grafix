#ifndef __GRAFCHIP_ILI9325_H__
#define __GRAFCHIP_ILI9325_H__

#include <GrafChip_ILI9320.h>

class GrafChip_ILI9325 : public GrafChip_ILI9320
{
 public:
  explicit GrafChip_ILI9325 (GrafComms & comms) ;

  void  begin (byte orient) ;
  void  init_regs (byte orient) ;
 protected:
  void  set_gate_scan (byte orient) ;
};

#endif
