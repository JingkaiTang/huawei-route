
#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRINT printf
#define BOUND 6000 		//数组上界
#define MAXCOST 60000	//初始化最大cost

//节点
typedef struct node_t
{
	int value;	//点编号
	int n_child;//子节点数
	//int flag;  //flag
	int parent; //父节点，-1表示根节点
	int cost;   //到父节点的花费
	int num;    //到父节点的边号
	struct node_t** child;
} NODE;

//边权值
int (*p)[BOUND];
//序号
int (*n)[BOUND];

//节点数组
NODE *node[BOUND];	

//源、目标点，V'点个数，V点个数，点集V'，点集V'状态
int source =0,dest=0;
int v_num=0,max=0;
int state[50],pass[50],check[BOUND];
//cost初始值
int cost = MAXCOST;

//分割demand
int get_demand(char *demand);
//判断点集是否全部命中
int all_hit(int v_num);
//命中V'中点v
void hit(int v);
//取消命中V'中点v
void dishit(int v);
//创建节点
node_t * create(int value,int parent);
//搜索子节点
int search(int value,int ret[]);
//找到点集V中的最大值
void maxf(int x,int y);
//释放内存，没写
void freef();


//你要完成的功能总入口
void search_route(int *topo[5000], int edge_num, char *demand)
{

	p=(int (*)[BOUND])malloc(BOUND*BOUND*sizeof(int)); 
	n=(int (*)[BOUND])malloc(BOUND*BOUND*sizeof(int));

	//初始化边序号/边权值矩阵
	for(int i=0;i<edge_num;i++){
		int x = topo[i][1], y=topo[i][2], v = topo[i][3];
		maxf(x,y);
		n[x][y]=topo[i][0];
		if(p[x][y]==0||p[x][y]>v){
			p[x][y]=v;
		}
	}

	v_num = get_demand(demand);
	//创建根节点
	node[2]=create(2,-1);

	/*
	if(node[2]==NULL){
		PRINT("1");
	}
	else{
		PRINT("v=%d p=%d",node[2]->value,node[2]->parent);
	}
	//node[source]=create()


	PRINT("max=%d\n",max);
	int ret[max];
	int j = search(2,ret);
	for(int i=0;i<j;i++){
		PRINT("%d ",ret[i]);
	}

	PRINT("%d\n",v_num);
	
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
    freef();
}

//分割demand
int get_demand(char *demand)
{
	source = atoi(strtok(demand,","));
	dest = atoi(strtok(NULL, ","));
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

node_t * create(int value,int parent){
	node_t *node = (node_t *)malloc(sizeof(node_t));
	node->value = value;
	node->parent = parent;


}

int search(int value,int ret[]){
	int j=0;
	for(int i=0;i<max+1;i++){
		if(p[value][i]>0){
			ret[j]=i;
			j++;
		}
	}
	return j;
}

void maxf(int x,int y){
	if(x>y){
		if(x>max) max=x;
	}else{
		if(y>max) max=y;
	}
}

void freef(){
	
}