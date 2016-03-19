#ifndef __DATA_PARSE_H__
#define __DATA_PARSE_H__

#include "data.h"

int topo_parse(TopoNode *topo, int topo_size, char **lines, int line_num);
void topo_show(TopoNode *topo, int topo_size);

DemandSet *demand_parse(char *demand_line);
void demand_show(DemandSet *demand);

#endif
