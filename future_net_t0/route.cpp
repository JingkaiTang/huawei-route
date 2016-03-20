#include "route.h"
#include "lib_record.h"
#include "data.h"
#include "route_cursor.h"
#include "naive_debug.h"

#include <stdio.h>
#include <queue>

using namespace std;

#define ALPHA 1
#define BETA 100

void search_route(TopoNode *topo, DemandSet *demand) {
  if (demand->start == demand->end) {
    LOG("Start == End\n");
    return;
  }

  priority_queue<RouteCursor, vector<RouteCursor>, greater<RouteCursor> > explorer;
  LOG("New Explorer: size=>%d\n", explorer.size());

  // start:
  RouteCursor start;
  start.cur_node = demand->start;
  start.path.push_back(demand->start);
  start.cost = 0;
  start.pass_count = 0;
  start.value = 0;

  CURSOR_SHOW(&start);

  explorer.push(start);
  LOG("Push to Explorer: size=>%d\n", explorer.size());

  RouteCursor *result = NULL;
  RouteCursor *cursor = NULL;
  TopoNode *cur_node = NULL;
  TopoArrow *cur_arrow = NULL;
  bool conflict = false;
  while (!explorer.empty()) {
    LOG("Begin to extend a new node:\n");
    RouteCursor tmp_cursor = explorer.top();
    explorer.pop();

    cursor = &tmp_cursor;
    CURSOR_SHOW(cursor);

    cur_node = &topo[cursor->cur_node];
    for (int i = 0; i < cur_node->out_degree; i ++) {
      LOG("Extend node %d:\n", i);
      cur_arrow = &(cur_node->arrows[i]);
      conflict = false;

      for (vector<int>::iterator it = cursor->path.begin(); it != cursor->path.end(); it ++) {
        if (*it == cur_arrow->target) {
          LOG("Node %d conflict!\n", i);
          conflict = true;
        }
      }

      if (conflict) {
        continue;
      }

      RouteCursor new_cursor;
      new_cursor.cur_node = cur_arrow->target;
      new_cursor.cost = cursor->cost + cur_arrow->cost;
      new_cursor.pass_count = cursor->pass_count;
      for (int j = 0; j < demand->pass_size; j ++) {
        if (demand->pass[i] == cur_arrow->target) {
          new_cursor.pass_count ++;
          break;
        }
      }
      for (vector<int>::iterator it = cursor->path.begin(); it != cursor->path.end(); it ++) {
        new_cursor.path.push_back(*it);
      }
      new_cursor.path.push_back(cur_arrow->target);
      new_cursor.value = (ALPHA * new_cursor.cost + BETA * demand->pass_size) / (new_cursor.pass_count + 1);
      CURSOR_SHOW(&new_cursor);

      // test if find
      if (new_cursor.pass_count == demand->pass_size && new_cursor.cur_node == demand->end) {
        LOG("PATH FIND!!!\n");
        result = &new_cursor;
        break;
      }

      explorer.push(new_cursor);
      LOG("Push to Explorer: size=>%d\n", explorer.size());
    }
  }

  if (result != NULL && !result->path.empty()) {
    int pre_node = result->path[0];
    int cur_node = 0;
    for (int i = 1; i < result->path.size(); i ++) {
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
