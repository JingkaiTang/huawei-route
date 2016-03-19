#include "data_parse.h"

#include "stdio.h"

int32_t topo_parse(TopoNode *topo, int32_t topo_size, char **lines, int32_t line_num) {
  if (topo_size <= 0 || line_num <= 0) {
    printf("%s\n", "Call topo_parse Erro for negative topo_size or line_num");
    return -1;
  }

  uint32_t index = 0;
  uint32_t max_node = 0;
  uint32_t bound = topo_size < line_num ? topo_size : line_num;

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

void topo_show(TopoNode *topo, int32_t topo_size) {
  printf("%s\n", "Topo Details:");
  for (int i = 0; i <= topo_size; i ++) {
    printf("Node[%d]: %d.", i, topo[i].out_degree);
    for (int j = 0; j < topo[i].out_degree; j ++) {
      printf(" [%d]=> %d, %d, %d.", j, topo[i].arrows[j].target, topo[i].arrows[j].number, topo[i].arrows[j].cost);
    }
    printf("\n");
  }
}
