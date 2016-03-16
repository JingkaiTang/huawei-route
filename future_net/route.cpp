
#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRINT printf

//节点
typedef struct node_t
{
	int value;	//编号
	int n_child;//子节点数
	int level;  //层数
	int parent; //父节点
	int cost;   //到父节点的花费
	int num;    //到父节点的边号
	struct node_t** child;
} NODE;

//边权值
int (*p)[600];
p=(int (*)[600])malloc(360000*sizeof(int)); 
//序号
int (*n)[600];
n=(int (*)[600])malloc(360000*sizeof(int));

//源、目标点，点集，点状态集
int s =0,d=0,v_num=0;
int state[50],pass[50];
//cost初始值
int cost = 60000;


//分割demand
int get_demand(int s,int d,int pass[50],char *demand);
//判断点集是否全部命中
int all_hit(int v_num);
//命中点v
void hit(int v);
//取消命中点v
void dishit(int v); 

//你要完成的功能总入口
void search_route(int *topo[5000], int edge_num, char *demand)
{
	//初始化边序号/边权值矩阵
	for(int i=0;i<edge_num;i++){
		int x = topo[i][1], y=topo[i][2], v = topo[i][3];
		n[x][y]=topo[i][0];
		if(p[x][y]>v){
			p[x][y]=v;
		}
	}

	v_num = get_demand(s,d,pass,demand);

	/*
	for(int i=0;i<20;i++){
		for(int j=0;j<20;j++){
			PRINT("%2d ",p[i][j]);
		}
		PRINT(" \n");
	}
	*/


    unsigned short result[] = {2, 6, 3};//示例中的一个解

    for (int i = 0; i < 3; i++)
        record_result(result[i]);
}

//分割demand
int get_demand(int s,int d,int pass[50],char *demand)
{
	s = atoi(strtok(demand,","));
	d = atoi(strtok(NULL, ","));
	//PRINT("s=%d d=%d\n",s,d);
	int i=0;
	char* tmp = strtok(strtok(NULL, ","), "|");
    while( tmp != NULL )
	{
		pass[i++]=atoi(tmp);
		//PRINT("%d\n", pass[i] );
		//i++;
		tmp = strtok( NULL, "|");
	}
	return i;
}

int all_hit(int v_num)
{
	int ret=1;
	for(int i=0;i<v_num;i++){
		ret &= state[i];
	}
	return ret;
}

void hit(int v){
	for(int i=0;i<v_num;i++){
		if(v==pass[i]){
			state[i]=1;
			break;
		}
	}
}

void dishit(int v){
	for(int i=0;i<v_num;i++){
		if(v==pass[i]){
			state[i]=0;
			break;
		}
	}
}