#include "naive_debug.h"

void _arrow_show(TopoArrow *arrow);
void _edge_show(Edge *edge);
void _path_show(Path *path);
void _bitmap_show(Bitmap *bitmap);

void path_show(Path *path) {
  _path_show(path);
}

void _path_show(Path *path) {
  printf("Path ==> \n");
  int i = 0;
  while (path != NULL) {
    printf("    [%d]: {size: %d, pass_count: %d, ", i, path->size, path->pass_count);
    printf("bitmap: ");
    _bitmap_show(path->bitmap);
    printf(", ");
    printf("path: ");
    _edge_show(path->path);
    printf("}\n");
    path = path->next;
    i ++;
  }
  printf("<== Path\n");
}

void path_list_show(Path **path_list, int size) {
  printf("Path List ===>\n");
  for (int i = 0; i < size; i ++) {
    if (path_list[i] == NULL) {
      continue;
    }
    printf("[%d]: ", i);
    _path_show(path_list[i]);
  }
  printf("<=== Path List\n");
}

void edge_array_show(Edge *edges, int size) {
  printf("Edges ===>\n");
  for (int i = 0; i < size; i ++) {
    printf("[%d]: ", i);
    EDGE_SHOW(&edges[i]);
  }
  printf("<=== Edges\n");
}

void _edge_show(Edge *edge) {
  printf("Edge => {From: %d, ", edge->from);
  _arrow_show(edge->arrow);
  printf("}");
}

void edge_show(Edge *edge) {
  _edge_show(edge);
  printf("\n");
}

void _arrow_show(TopoArrow *arrow) {
  if (arrow != NULL) {
    printf("Arrow => {target: %d, number: %d, cost: %d}", arrow->target, arrow->number, arrow->cost);
  }
}

void arrow_show(TopoArrow *arrow) {
  _arrow_show(arrow);
  printf("\n");
}

void heap_show(TrickyHeap *heap) {
  printf("Heap ===>\n");
  int index = 0;
  HeapSlot *slot = heap->slots;
  while (slot != NULL) {
    for (int i = 0; i < slot->size; i ++) {
      printf("[%d] => ", index);
      CURSOR_SHOW(&slot->fa[i]);
      index ++;
    }
    slot = slot->next;
  }
  printf("<=== Heap\n");
}

void _bitmap_show(Bitmap *bitmap) {
  printf("Bitmap => {size: %d, data_size: %d, data:", bitmap->size, bitmap->data_size);
  for (int i = 0; i < bitmap->data_size; i ++) {
    printf(" %d", bitmap->data[i]);
  }
  printf("}");
}

void bitmap_show(Bitmap *bitmap) {
  _bitmap_show(bitmap);
  printf("\n");
}

void cursor_show(RouteCursor *cursor) {
  printf("Cursor: cur_node=>%d, cost=>%d, pass_count=>%d, value=>%d, path=>{", cursor->cur_node, cursor->cost, cursor->pass_count, cursor->value);
  if (cursor->path_size > 0) {
    printf("%d", cursor->path[0]);
  }
  for (int i = 1; i < cursor->path_size; i ++) {
    printf(" ,%d", cursor->path[i]);
  }
  printf("}\n");
}

void topo_show(TopoNode *topo, int topo_size) {
  printf("%s\n", "Topo Details:");
  for (int i = 0; i <= topo_size; i ++) {
    if (topo[i].out_degree == 0) {
      continue;
    }
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
