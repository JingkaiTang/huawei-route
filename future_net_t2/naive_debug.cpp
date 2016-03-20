#include "naive_debug.h"

#include <vector>

void bitmap_show(Bitmap *bitmap) {
  printf("Bitmap => {size: %d, data_size: %d, data:", bitmap->size, bitmap->data_size);
  for (int i = 0; i < bitmap->data_size; i ++) {
    printf(" %d", bitmap->data[i]);
  }
  printf("}\n");
}

void cursor_show(RouteCursor *cursor) {
  printf("Cursor: cur_node=>%d, cost=>%d, pass_count=>%d, value=>%d, path=>{", cursor->cur_node, cursor->cost, cursor->pass_count, cursor->value);
  vector<int>::iterator it = cursor->path->begin();
  if (cursor->path->size() > 0) {
    printf("%d", *it);
  }
  for (it ++; it != cursor->path->end(); it ++) {
    printf(" ,%d", *it);
  }
  printf("}\n");
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
