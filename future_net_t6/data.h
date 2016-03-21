#ifndef __DATA_H__
#define __DATA_H__

#include "bitmap.h"

struct TopoArrow {
  int target;
  int number;
  int cost;
};

struct TopoNode {
  int out_degree;
  TopoArrow arrows[8];
};

struct DemandSet {
  int start;
  int end;
  int pass_size;
  int *pass;
  Bitmap *bitmap;
};

#endif
