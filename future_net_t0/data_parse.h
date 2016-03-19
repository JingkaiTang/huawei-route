#ifndef __DATA_PARSE_H__
#define __DATA_PARSE_H__

#include "stdint.h"

struct TopoArrow {
  uint32_t target;
  uint32_t number;
  uint32_t cost;
};

struct TopoNode {
  uint32_t out_degree;
  TopoArrow arrows[8];
};

struct DemandSet {
  uint32_t start;
  uint32_t end;
  uint32_t pass_size;
  uint32_t *pass;
};

int32_t topo_parse(TopoNode *topo, int32_t topo_size, char **lines, int32_t line_num);
void topo_show(TopoNode *topo, int32_t topo_size);

DemandSet *demand_parse(char *demand_line);

#endif
