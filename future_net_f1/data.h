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

struct TopoEdge {
  int cost;
  int number;
  int to;
};

struct Ant {
  int path_size;
  int start_node;
  int cur_node;
  int *bitmap;
  int cost;
  int flag;
  TopoArrow* path;
};
#endif
