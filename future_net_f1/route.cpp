#include "route.h"
#include "lib_record.h"
#include "data.h"
#include "route_cursor.h"
#include "naive_debug.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h> 
#include <stdlib.h>

using namespace std;

#define ALPHA 0.4       //The importance of pheromone
#define BETA 0.6        //The importance of heuristic information(cost of edge)
#define RHO 0.3         //The decrease of pheromone after every iteration
//#define GAMMA 20
#define BESTCOST 60000
#define GEN 3           //Iteration times
#define MAX 101 

void search_route(TopoNode *topo, int node_scope, DemandSet *demand) {
  LOG("DEMAND => ");
  srand(time(NULL));

  if (demand->start == demand->end) {
    LOG("Start == End\n");
    return;
  }
  
  float pheromone[node_scope][node_scope];
  for(int i=0;i<node_scope;i++)
    for(int j=0;j<node_scope;j++)
      pheromone[i][j]=0.1f;
  int bestCost = BESTCOST;
  
  RouteCursor *bestRoute = NULL;
  
  Ant *ants[demand->pass_size];
  for(int g=0;g<GEN;g++){
    for(int i=0;i<demand->pass_size;i++){
      
      ants[i] =  new Ant();
      //ants[i]->path.push(&topo[demand->pass[i]]);
      //ants[i]->path_size ++;
      ants[i]->cur_node = demand->pass[i];
      ants[i]->bitmap = new int[node_scope]{0};

      ants[i]->bitmap[demand->pass[i]] = 1;
      ants[i]->path = new TopoArrow[node_scope];
      ants[i]->path_size = 0;
      ants[i]->cost = 0;
      LOG("CUR=>%d\n",ants[i]->cur_node);

      while(true){//for(int nn=0;nn<5;nn++){
        if(ants[i]->cur_node == demand->end){
          if(ants[i]->bitmap[demand->start] == 1){
            break;
          }
          TopoArrow *ta = new TopoArrow();
          ta->cost = 0;
          ta->number = -1;
          ta->target = demand->start;
          ants[i]->path[ants[i]->path_size]=*ta;
          ants[i]->cur_node = demand->start;
          ants[i]->bitmap[demand->start] = 1;
          ants[i]->path_size ++;
          continue;
        }

        TopoNode *tn = &topo[demand->pass[i]];
        double p[tn->out_degree];
        int reach[tn->out_degree];
        int num;
        double sum = 0.0;
        int count = 0;
        for(int j=0;j<tn->out_degree;j++){
          LOG("target=%d,%d\n",tn->arrows[j].target,ants[i]->bitmap[tn->arrows[j].target]);
          if(ants[i]->bitmap[tn->arrows[j].target]==1){
            p[j]=0.0;
            count++;
            continue;
          }
          p[j] = pow(pheromone[ants[i]->cur_node][tn->arrows[j].target], ALPHA)
                *pow(1/tn->arrows[j].cost, BETA);
          sum += p[j];
        }
        //no reachable node
        if(count == tn->out_degree){
          break;
        }
        for(int j=0;j<tn->out_degree;j++){
          p[j] /= sum;
          LOG("P[%d]=%f\n",j,p[j]);
        }

        int random_i = rand()%MAX;
        double random = (double)random_i/(MAX-1.0);
        double ransum = 0.0;
        int selectNum = 0;
        for(int j=0;j<tn->out_degree;j++){
          ransum +=p[j];
          if(ransum >= random||ants[i]->bitmap[tn->arrows[j].target]==0){
            selectNum = j;
            break;
          }
        }
        int selectNode = tn->arrows[selectNum].target;


        if(selectNode == demand->pass[i]){
          //find
          LOG("Find!!!\n");
          break;
        }
        if(ants[i]->bitmap[selectNode]==0){
          ants[i]->bitmap[selectNode] = 1;
          ants[i]->cur_node = selectNode;
          ants[i]->path[ants[i]->path_size] =tn->arrows[selectNum];
          ants[i]->path_size ++;
          LOG("2CUR=>%d\n",ants[i]->cur_node);

        }
      }

      LOG("BREAK\n");
      //update pheromone
      for(int m=0;m<node_scope;m++){
        for(int n=0;n<node_scope;n++){
          pheromone[m][n] *= (1-RHO);
          for(int p=0;p<ants[i]->path_size;p++){
            
          }
        }
      }

      //reinitial ants

    }
  }
  



  
}
