#include "GrafChip.h"

GrafChip::GrafChip (GrafComms & comms) : _comms (comms)
{}

GrafComms & GrafChip::get_comms () { return _comms ; }

boolean GrafChip::has_fast_fill () { return false ; }

void  GrafChip::fast_fill (int x0, int y0, int x1, int y1, int foreground)
{}


