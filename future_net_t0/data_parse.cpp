#include "data_parse.h"

#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) {
  return *(int *)a - *(int *)b;
}

int topo_parse(TopoNode *topo, int topo_size, char **lines, int line_num) {
  if (topo_size <= 0 || line_num <= 0) {
    printf("%s\n", "Call topo_parse Erro for negative topo_size or line_num");
    return -1;
  }

  int index = 0;
  int max_node = 0;
  int bound = topo_size < line_num ? topo_size : line_num;

  int tmp_path = 0;
  int tmp_src = 0;
  int tmp_dst = 0;
  int tmp_cost = 0;
  TopoNode *cur_node = NULL;
  TopoArrow *cur_arrow = NULL;
  bool conflict = false;

  while (index < bound) {
    sscanf(lines[index], "%d,%d,%d,%d", &tmp_path, &tmp_src, &tmp_dst, &tmp_cost);

    if (tmp_src >= topo_size) {
      printf("%s\n", "Too large source node number, when parsing topo");
      return -1;
    }

    conflict = false;
    cur_node = &(topo[tmp_src]);

    // parallel edge ditection
    if (cur_node->out_degree > 0) {
      for (int i = 0; i < cur_node->out_degree; i ++) {
        if ((cur_node->arrows[i]).target == tmp_dst) {
          conflict = true;
          cur_arrow = &(cur_node->arrows[i]);
          if (cur_arrow->cost > tmp_cost) {
            cur_arrow->cost = tmp_cost;
            cur_arrow->number = tmp_path;
          }
          break;
        }
      }
    } else {
      // new node
      max_node = tmp_src > max_node ? tmp_src : max_node;
    }

    // non parallel edge
    if (!conflict) {
      cur_arrow = &(cur_node->arrows[cur_node->out_degree]);
      cur_arrow->target = tmp_dst;
      cur_arrow->number = tmp_path;
      cur_arrow->cost = tmp_cost;
      cur_node->out_degree ++;
    }

    index ++;
  }

  return max_node;
}

void topo_show(TopoNode *topo, int topo_size) {
  printf("%s\n", "Topo Details:");
  for (int i = 0; i <= topo_size; i ++) {
    printf("Node[%d]: %d.", i, topo[i].out_degree);
    for (int j = 0; j < topo[i].out_degree; j ++) {
      printf(" [%d]=> %d, %d, %d.", j, topo[i].arrows[j].target, topo[i].arrows[j].number, topo[i].arrows[j].cost);
    }
    printf("\n");
  }
}

DemandSet *demand_parse(char *demand_line) {
  int tmp_start = 0;
  int tmp_end = 0;
  sscanf(demand_line, "%d,%d", &tmp_start, &tmp_end);
  printf("S,E: %d,%d\n", tmp_start, tmp_end);

  int comma_count = 0;
  int pass_index = 0;
  for (int i = 0; demand_line[i] != 0; i ++) {
    if (comma_count == 2) {
      pass_index = i;
      break;
    }
    if (demand_line[i] == ',') {
      comma_count ++;
    }
  }
  if (comma_count != 2 || pass_index < 4) {
    printf("%s\n", "Error demand input, while demand parsing");
    return NULL;
  }

  int pipe_count = 0;
  for (int i = pass_index; demand_line[i] != 0; i ++) {
    if (demand_line[i] == '|') {
      pipe_count ++;
    }
  }

  int tmp_pass_size = pipe_count + 1;
  int *tmp_pass = (int *)malloc(sizeof(int) * tmp_pass_size);

  sscanf(&demand_line[pass_index], "%d", &tmp_pass[0]);
  for (int i = 1; i < tmp_pass_size; i ++) {
    while (demand_line[pass_index] != '|') {
      pass_index ++;
    }
    pass_index ++;
    sscanf(&demand_line[pass_index], "%d", &tmp_pass[i]);
  }

  DemandSet *demand = new DemandSet();
  demand->start = tmp_start;
  demand->end = tmp_end;
  demand->pass_size = tmp_pass_size;
  demand->pass = tmp_pass;

  // sort
  qsort(demand->pass, demand->pass_size, sizeof(int), compare);

  return demand;
}

void demand_show(DemandSet *demand) {
  printf("%s\n", "Demand Details:");
  printf("Start: %d, End: %d, Pass: {", demand->start, demand->end);
  if (demand->pass_size > 0) {
    printf("%d", demand->pass[0]);
  }
  for (int i = 1; i < demand->pass_size; i ++) {
    printf(", %d", demand->pass[i]);
  }
  printf("}\n");
}
