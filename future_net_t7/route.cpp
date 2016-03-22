#include "route.h"
#include "lib_record.h"
#include "data.h"
#include "route_cursor.h"
#include "naive_debug.h"
#include "tricky_heap.h"

#include <stdio.h>
#include <string.h>

using namespace std;

#define ALPHA 8
#define BETA 4

void search_route(TopoNode *topo, int node_scope, DemandSet *demand) {
  LOG("DEMAND => ");
  BITMAP_SHOW(demand->bitmap);

  if (demand->start == demand->end) {
    LOG("Start == End\n");
    return;
  }

  TrickyHeap explorer;
  LOG("New Explorer: size=>%d\n", explorer.size);
  HEAP_SHOW(&explorer);

  // start:
  RouteCursor &start = explorer.get();
  start.cur_node = demand->start;
  start.path_size = 1;
  start.path = new int[1]();
  start.path[0] = demand->start;
  start.cost = 0;
  start.pass_count = 0;
  start.value = 0;
  start.bitmap = new Bitmap(node_scope);
  start.bitmap->set(demand->start);
  CURSOR_SHOW(&start);
  BITMAP_SHOW(start.bitmap);

  explorer.balance();
  LOG("Push to Explorer: size=>%d\n", explorer.size);
  HEAP_SHOW(&explorer);

  RouteCursor *result = NULL;
  TopoNode *cur_node = NULL;
  TopoArrow *cur_arrow = NULL;
  while (explorer.size > 0) {
    LOG("Begin to extend a new node:\n");
    RouteCursor &cursor = explorer.pop();
    LOG("Pop from Explorer: size=>%d\n", explorer.size);
    HEAP_SHOW(&explorer);
    CURSOR_SHOW(&cursor);

    // test if find
    if (cursor.pass_count == demand->pass_size && cursor.cur_node == demand->end) {
      LOG("PATH FIND!!!\n");
      result = &cursor;
      goto RESULT;
    }
    LOG("PATH NOT FIND!!!\n");

    cur_node = &topo[cursor.cur_node];
    for (int i = 0; i < cur_node->out_degree; i ++) {
      LOG("Extend node %d:\n", i);
      cur_arrow = &(cur_node->arrows[i]);

      // if early end node
      if (cur_arrow->target == demand->end && cursor.pass_count < demand->pass_size) {
        LOG("TOO EARLY TO PASS END NODE!!!\n");
        continue;
      }

      // if already added, conflict
      if (cursor.bitmap->test(cur_arrow->target)) {
          LOG("Node %d conflict!\n", i);
          continue;
      }

      RouteCursor &new_cursor = explorer.get();
      new_cursor.cur_node = cur_arrow->target;
      new_cursor.cost = cursor.cost + cur_arrow->cost;
      new_cursor.pass_count = cursor.pass_count;
      if (demand->bitmap->test(cur_arrow->target)) {
          new_cursor.pass_count ++;
      }
      new_cursor.path_size = cursor.path_size + 1;
      new_cursor.path = new int[new_cursor.path_size];
      memcpy(new_cursor.path, cursor.path, sizeof(int)*cursor.path_size);
      new_cursor.path[cursor.path_size] = cur_arrow->target;
      new_cursor.bitmap = new Bitmap(cursor.bitmap);
      new_cursor.bitmap->set(cur_arrow->target);
      new_cursor.value = ((new_cursor.cost << ALPHA) + (demand->pass_size << BETA)) / (new_cursor.pass_count + 1);
      CURSOR_SHOW(&new_cursor);
      BITMAP_SHOW(new_cursor.bitmap);

      explorer.balance();
      LOG("Push to Explorer: size=>%d\n", explorer.size);
      HEAP_SHOW(&explorer);
    }

    LOG("Delete old cursor ...\n");
    int *tmp_path = cursor.path;
    cursor.path = NULL;
    delete[] tmp_path;
    Bitmap *tmp_bitmap = cursor.bitmap;
    cursor.bitmap = NULL;
    delete tmp_bitmap;
  }

  RESULT:
  if (result != NULL && result->path_size > 1) {
    CURSOR_SHOW(result);
    int pre_node = result->path[0];
    int cur_node = 0;
    for (int i = 1; i < result->path_size; i ++) {
      cur_node = result->path[i];
      for (int j = 0; j < topo[pre_node].out_degree; j ++) {
        if (topo[pre_node].arrows[j].target == cur_node) {
          record_result(topo[pre_node].arrows[j].number);
        }
      }
      pre_node = cur_node;
    }
  }
}
