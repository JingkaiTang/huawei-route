#include "route.h"
#include "lib_record.h"
#include "data.h"
#include "route_cursor.h"
#include "naive_debug.h"

#include <stdio.h>
#include <queue>

using namespace std;

#define ALPHA 1
#define BETA 10

void search_route(TopoNode *topo, int node_scope, DemandSet *demand) {
  LOG("DEMAND => ");
  BITMAP_SHOW(demand->bitmap);

  if (demand->start == demand->end) {
    LOG("Start == End\n");
    return;
  }

  priority_queue<RouteCursor *, vector<RouteCursor *>, rc_greater> explorer;
  LOG("New Explorer: size=>%d\n", explorer.size());

  // start:
  RouteCursor *start = new RouteCursor();
  start->cur_node = demand->start;
  start->path = new vector<int>();
  start->path->push_back(demand->start);
  start->cost = 0;
  start->pass_count = 0;
  start->value = 0;
  start->bitmap = new Bitmap(node_scope);
  start->bitmap->set(demand->start);
  CURSOR_SHOW(start);
  BITMAP_SHOW(start->bitmap);

  explorer.push(start);
  LOG("Push to Explorer: size=>%d\n", explorer.size());

  RouteCursor *result = NULL;
  TopoNode *cur_node = NULL;
  TopoArrow *cur_arrow = NULL;
  while (!explorer.empty()) {
    LOG("Begin to extend a new node:\n");
    RouteCursor *cursor = explorer.top();
    explorer.pop();
    CURSOR_SHOW(cursor);

    cur_node = &topo[cursor->cur_node];
    for (int i = 0; i < cur_node->out_degree; i ++) {
      LOG("Extend node %d:\n", i);
      cur_arrow = &(cur_node->arrows[i]);

      // if early end node
      if (cur_arrow->target == demand->end && cursor->pass_count < demand->pass_size) {
        LOG("TOO EARLY TO PASS END NODE!!!\n");
        continue;
      }

      // if already added, conflict
      if (cursor->bitmap->test(cur_arrow->target)) {
          LOG("Node %d conflict!\n", i);
          continue;
      }

      RouteCursor *new_cursor = new RouteCursor();
      new_cursor->cur_node = cur_arrow->target;
      new_cursor->cost = cursor->cost + cur_arrow->cost;
      new_cursor->pass_count = cursor->pass_count;
      if (demand->bitmap->test(cur_arrow->target)) {
          new_cursor->pass_count ++;
      }
      new_cursor->path = new vector<int>(*(cursor->path));
      new_cursor->path->push_back(cur_arrow->target);
      new_cursor->bitmap = new Bitmap(cursor->bitmap);
      new_cursor->bitmap->set(cur_arrow->target);
      new_cursor->value = (ALPHA * new_cursor->cost + BETA * demand->pass_size) / (new_cursor->pass_count + 1);
      CURSOR_SHOW(new_cursor);
      BITMAP_SHOW(new_cursor->bitmap);

      // test if find
      if (new_cursor->pass_count == demand->pass_size && new_cursor->cur_node == demand->end) {
        LOG("PATH FIND!!!\n");
        result = new_cursor;
        goto RESULT;
      }
      LOG("PATH NOT FIND!!!\n");

      explorer.push(new_cursor);
      LOG("Push to Explorer: size=>%d\n", explorer.size());
    }

    LOG("Delete old cursor ...\n");
    delete cursor->path;
    delete cursor->bitmap;
    delete cursor;
  }

  RESULT:
  if (result != NULL && !result->path->empty()) {
    CURSOR_SHOW(result);
    vector<int>::iterator it = result->path->begin();
    int pre_node = *it;
    int cur_node = 0;
    for (it ++; it != result->path->end(); it ++) {
      cur_node = *it;
      for (int j = 0; j < topo[pre_node].out_degree; j ++) {
        if (topo[pre_node].arrows[j].target == cur_node) {
          record_result(topo[pre_node].arrows[j].number);
        }
      }
      pre_node = cur_node;
    }
  }
}
