#ifndef __DATA_PARSE_H__
#define __DATA_PARSE_H__

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
};

int topo_parse(TopoNode *topo, int topo_size, char **lines, int line_num);
void topo_show(TopoNode *topo, int topo_size);

DemandSet *demand_parse(char *demand_line);
void demand_show(DemandSet *demand);

#endif
