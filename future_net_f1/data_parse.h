#ifndef __DATA_PARSE_H__
#define __DATA_PARSE_H__

#include "data.h"

int topo_parse(TopoNode *topo, int topo_size, char **lines, int line_num);

DemandSet *demand_parse(char *demand_line);

#endif
