#include<iostream>
#include <vector>
#include <algorithm>
using namespace std;

int vexNum, edgeNum;
//点
struct Point{
	int dis = INT_MAX;
	bool visit = false;
};

//输出
void output(Point p[], int start) {
	cout << endl;
	cout << "The start point:" << start << endl;
	for (int i = 1; i <= vexNum; i++) {
		if (i == start)
			continue;
		if(p[i].dis==INT_MAX)
			cout << i << "has no way to attach" << endl;
		else
			cout << i << "'s shortest route:" << p[i].dis << endl;
	}
    return;
}

//dij算法
void Dijkstra(int g[][20], int start) {
	int vex = vexNum;	//顶点数
    Point points[20];

	for (int i = 1; i <= vex; i++) 
		points[i].dis = g[start][i];	//初始化dis，记录起点到各点的直接距离
	points[start].visit = true;		//起点访问

	for (int i = 1; i <= vex; i++) {
		int MINdis = INT_MAX;
		int MINvex = 0;
		for (int j = 1; j <= vex; j++) {
			if (!points[j].visit && points[j].dis < MINdis) {	//找出距离最近的点
				MINdis = points[j].dis;
				MINvex = j;
			}
		}
        if(MINvex == 0) {break;}
		points[MINvex].visit = true;	//距离最近的点的最短距离确定
		//printf("selected %d\r\n",MINvex);
		for (int i = 1; i <= vex; i++)	//更新dis数组
		{
            if(g[MINvex][i]<INT_MAX)
				points[i].dis = min(points[i].dis, g[MINvex][i] + points[MINvex].dis);
            //printf("point[%d].dis:%d\r\n",i,points[i].dis);
        }
       // printf("\r\n");
    }

	output(points, start);	//输出结果
}


int main() {
	cout << "Input the number of nodes & edges: ";
	cin >> vexNum >> edgeNum;
    int graph[20][20];
    for(int i=1; i<=vexNum; i++)
    {
        for(int j=1; j<=vexNum; j++)
        graph[i][j] = 123456;
    }
	cout << "Input the edges with their weights--a b w: " << endl;
	int a, b, w;
	for (int i = 0; i < edgeNum; i++) {
		cin >> a >> b >> w;
		graph[a][b] = w;
		graph[b][a] = w;        
	}
	for (int i = 1; i <= vexNum; i++) {
		graph[i][i] = 0;
	}

    for(int i=1; i<=vexNum; i++)
	    Dijkstra(graph,i);	//dij算法

	system("pause");
	return 0;
}

/*
7  12
1 2 14
1 3 15
1 4 13
2 3 8
2 5 12
3 4 6
3 5 4
3 6 3
4 6 11
5 6 5
5 7 5
6 7 2
*/
