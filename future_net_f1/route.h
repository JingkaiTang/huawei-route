#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "data.h"

void search_route(TopoNode *topo, int node_scope, DemandSet *demand);

void print_path(Ant *ant);

#endif
