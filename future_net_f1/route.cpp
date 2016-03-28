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

#define ALPHA 0.9       //The importance of pheromone
#define BETA 1        //The importance of heuristic information(cost of edge)
#define RHO 0.3         //The decrease of pheromone after every iteration
#define QSUM 10.0       //The amount of pheromone
#define MULT 2          //The multiple of probability for node in set V'
//#define GAMMA 20
#define BESTCOST 60000
#define GEN 100            //Iteration times
#define MAX 101         //Random Max

void search_route(TopoNode *topo, int node_scope, DemandSet *demand) {
  LOG("DEMAND => ");
  srand(time(NULL));

  if (demand->start == demand->end) {
    LOG("Start == End\n");
    return;
  }
  
  float pheromone[node_scope][node_scope];
  for(int i=0;i<node_scope;i++){
    for(int j=0;j<node_scope;j++){
      pheromone[i][j]=0.1f;
    }
  }

  int bestCost = BESTCOST;
  Ant *bestAnt = NULL;
    
  Ant *ants[demand->pass_size];
  for(int g=0;g<GEN;g++){
    for(int i=0;i<demand->pass_size;i++){
      
      LOG("No.%d ant.\n",i);
      ants[i] =  new Ant();
      ants[i]->cur_node = demand->pass[i];
      ants[i]->start_node = demand->pass[i];
      ants[i]->bitmap = new int[node_scope]{0};
      ants[i]->path = new TopoArrow[node_scope];
      ants[i]->path_size = 0;
      ants[i]->cost = 0;

      //initial v' node
      for(int y=0;y<demand->pass_size;y++){
        ants[i]->bitmap[demand->pass[y]] = 2;
      }
      ants[i]->bitmap[demand->pass[i]] = 1;


      while(true){//for(int z=0;z<5;z++){//
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
        }
        LOG("CUR=>%d\n",ants[i]->cur_node);

        TopoNode *tn = &topo[ants[i]->cur_node];
        double p[tn->out_degree];
        int reach[tn->out_degree];
        int num;
        double sum = 0.0;
        int count = 0;
        int lastnode = -1;
        for(int j=0;j<tn->out_degree;j++){
          LOG("TARGET=%d ",tn->arrows[j].target);
          int target = tn->arrows[j].target;
          if(target == ants[i]->start_node){
            lastnode = j;
          }
          int exist = ants[i]->bitmap[target];
          if(exist == 1){
            p[j]=0.0;
            count++;
            LOG(" p=0.0\n");
            continue;
          }
          //LOG("%f ",pow(1.0/tn->arrows[j].cost, BETA));
            p[j] = pow(pheromone[ants[i]->cur_node][tn->arrows[j].target], ALPHA)
                *pow(1.0/tn->arrows[j].cost, BETA);
          
            LOG(" p=%f\n",p[j]);
          sum += p[j];
        }
        //no reachable node
        if(count == tn->out_degree){
          ants[i]->flag = 0;
          if(lastnode>0){
            int res=1;
            for(int y=0;y<demand->pass_size;y++){
              if(ants[i]->bitmap[demand->pass[y]] == 2){
                res=0;
                break;
              }
            }
            if(res==0){
              break;
            }
            //printf("find..\n");
            ants[i]->flag = 1;
            ants[i]->path[ants[i]->path_size] =tn->arrows[lastnode];
            ants[i]->path_size ++;
            ants[i]->cost += tn->arrows[lastnode].cost;
            if(bestCost>ants[i]->cost){
              bestCost = ants[i]->cost;
              bestAnt = ants[i];
              LOG("end.......bestCost=%d\n",bestCost);
            }
          }
          //printf("%d.\n",i);
          //print_path(ants[i]);

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
          LOG("TARGET=%d,EXIST=%d\n",tn->arrows[j].target,ants[i]->bitmap[tn->arrows[j].target]==0);
          if(ransum >= random&&ants[i]->bitmap[tn->arrows[j].target]!= 1){
            selectNum = j;
            break;
          }
        }
        int selectNode = tn->arrows[selectNum].target;

        LOG("SELECT=%d\n",selectNode);

        if(selectNode == demand->pass[i]){
          LOG("Find!!!\n");
          //if(bestCost>ants[i]->cost){
            bestCost = ants[i]->cost;
            bestAnt = ants[i];
            //print_path(ants[i]);
            LOG("path_size=%d\n",ants[i]->path_size);
            for(int x=0;i<ants[x]->path_size;x++){
              LOG("%d ",ants[x]->path[x].number);
            }
            LOG("end.......\n");
          //}
          break;
        }
        if(ants[i]->bitmap[selectNode] != 1){
          ants[i]->bitmap[selectNode] = 1;
          ants[i]->cur_node = selectNode;
          ants[i]->path[ants[i]->path_size] =tn->arrows[selectNum];
          ants[i]->path_size ++;
          ants[i]->cost += tn->arrows[selectNum].cost;
          LOG("CUR=>%d\n",ants[i]->cur_node);
        }
      }
    }
    LOG("Iteration\n");
    //update pheromone
    for(int i=0;i<demand->pass_size;i++){
      for(int m=0;m<node_scope;m++){
        for(int n=0;n<node_scope;n++){
          pheromone[m][n] *= (1-RHO);
          int from = ants[i]->start_node;
          int to;
          int cost = ants[i]->cost;
          if(ants[i]->flag == 0){
            for(int p=0;p<ants[i]->path_size;p++){
              to = ants[i]->path[p].target;
              pheromone[from][to] += QSUM/ants[i]->path[p].cost;
              from = to;
            }
          }else{
            for(int p=0;p<ants[i]->path_size;p++){
              to = ants[i]->path[p].target;
              pheromone[from][to] += 2.0*QSUM/ants[i]->path[p].cost;
              from = to;
            }
          }

        }
      }
    }
    //release ants
    //delete ants;
  }
  print_path(bestAnt);

}

void print_path(Ant *ant) {
  if(NULL == ant){
    LOG("null point\n");
    return;
  }
  int pathtemp[ant->path_size];
  int flag=-1;
  /*

  for(int i=0;i<ant->path_size;i++){
    printf("%d ",ant->path[i].number);
  }
  printf("\n");
  printf("%d ",ant->start_node);
  for(int i=0;i<ant->path_size;i++){
    printf("%d ",ant->path[i].target);
  }
  printf("\n");
  */

  for(int i=0;i<ant->path_size;i++){
    if(ant->path[i].cost == 0){
      flag=i;
      continue;
    }
    if(flag>-1){
      //printf("%d ",ant->path[i].number);
      record_result(ant->path[i].number);
    }
    else{
      pathtemp[i]=ant->path[i].number;
    }
  }
  if(flag>0){
    for(int i=0;i<flag;i++){
      //printf("%d ",ant->path[i].number);
      record_result(ant->path[i].number);
    }
  }

}
