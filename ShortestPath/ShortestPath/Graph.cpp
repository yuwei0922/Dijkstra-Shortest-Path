#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <malloc.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>

using namespace std;

#define  M  1975  //最大边数
#define  N 100000 //无穷大
#define  TRUE 1
#define  FALSE 0
typedef int Boolean;
Boolean visited[199];  //定义visited[]数组，用于图的遍历

//建立一个结构City
struct City
{
	bool pass;  //判断路径是否经过该城市
	string country, city;  //国家名，城市名
	float latitude, longitude;  //纬度，经度
};

//建立一个结构Route
struct Route
{
	string origion_city, destination_city,  //出发地，目的地
		transport, other_information;  //交通工具，信息地址
	float time, cost;  //该段路线所花费的时间，金钱
};

//建立边
struct Arc
{
	float time;  //所花费的时间
	float cost;  //所花费的金钱
	string transport;  //交通工具
	string otherInfo;  //信息地址
};

//图的邻接矩阵
struct Graph
{
	City* city;  //指向城市的指针
	Arc arcs[199][199];  //邻接矩阵
	int vexnum, arcnum;  //定义顶点数，边数
};

//建立一个结构Way
struct Way
{
	int* path;  //路线指针，指向路径经过的城市序号
};

//在图中查找顶点
int LocatVex(const string& origin, City ct[])
{
	int i = 0;
	for (; i < 199; i++)  //依次查找输入的城市的序号
	{
		if (origin == ct[i].city)  //输入城市的名称与第i+1个城市名称相同
			return i;  //返回i值
	}
	return N;  //若没找到，返回一个无穷大的数
}

Way Joint(Way& a, Way& b, int k)  //连接两条路径
{
	Way c;
	c.path = (int*)malloc(sizeof(int) * 199);
	if (c.path == NULL)  //判断异常
	{
		cout << "error2!"; 
		exit(0);
	}
	int i = 0;
	for (; i < 199; i++)  //初始化c.path[]
	{
		c.path[i] = -1;
	}
	i = 0;
	while (a.path[i] != -1)  //找到a中的路径经过的点
	{
		c.path[i] = a.path[i];  //将a中路径经过的点存入c中
		i++;
	}
	c.path[i] = k;  //记录当前更新集合状态的城市的序号
	i++;
	int j = 0;
	while (b.path[j] != -1)  //连接b中的路径经过的点
	{
		c.path[i] = b.path[j];
		i++;
		j++;
	}
	return c;  //返回新连接好的最短路径
}

//邻接矩阵的深度优先递归算法遍历
void DFS(Graph &g, int r)
{
	int s;
	visited[r] = TRUE;  //遍历起点城市被访问，改变其visited的值为1
	cout<<g.city[r].city<<"->";  //输出遍历起点的城市名称

	for (s = 0; s < g.vexnum; s++)  //循环至遍历完所有顶点
	{
		if (g.arcs[r][s].time != 0 && g.arcs[r][s].time != N && visited[s]== FALSE)  //如果该点有路线没有访问，则对其进行访问
		{
			DFS(g, s);  //递归调用
		}
	}
}

//Dijkstra算法求最短路径
void DIJ(const Graph& g, int depart, int dest, Way way[][199], float dist[][199],float min)
{
	for (int v = 0; v < g.vexnum; ++v)  //遍历完所有顶点
	{
		g.city[v].pass = false;  //初始化pass都为false
		dist[depart][v] = g.arcs[depart][v].time;
	}
	dist[depart][depart] = 0;
	g.city[depart].pass = true;   //出发城市pass标记为为true,其最短路径为0.此顶点以后不会再用到
	for (int i = 0; i < g.vexnum; ++i)  //开始主循环，每次求得到出发城市到某个城市的最短路径，并将该顶点添加到S中
	{
		int v = 0;
		double mincost = N;
		for (int w = 0; w < g.vexnum; ++w)  //找出当前源点到其余点的最短路径
		{
			if (g.city[w].pass == false)
				if (dist[depart][w] < mincost)  //如果w城市离出发城市更近，且当前城市在集合V-S中
				{
					v = w;  //用v记录离出发城市更近的w城市的序号
					mincost = dist[depart][w];  //用mincost记录权值（这里是时间）
				}
		}
		g.city[v].pass = true;  //更新v城市的pass状态
		for (int w = 0; w < g.vexnum; ++w)  //对其余各条路径进行适当调整
		{
			if (g.city[w].pass == false && (mincost + g.arcs[v][w].time < dist[depart][w]))
			{
				dist[depart][w] = mincost + g.arcs[v][w].time;
				way[depart][w] = Joint(way[depart][v], way[v][w], v);
			}
		}
		if (g.city[dest].pass == true)  //最终目的地的pass为true，即目的地被添加到集合S中，则停止寻找最短路径
		{
			min = dist[depart][dest];  //得到所需最短时间
			cout << '\n'<<"总时长:" << min << endl;
			break;  //跳出主循环
		}
	}
}

void main()
{
	//读取文件数据
	FILE* fp=NULL;
	fp = fopen("D:\\学习\\大一下yyw\\数据结构与算法\\实习\\cities.csv", "r");
	if (fp == NULL)  //判断异常
	{
		printf("error1!");
		exit(0);
	}
	char ch;  //控制循环
	float f;
	int i = 0, j;
	City ct[199];

	while (!feof(fp))  //读至文件末尾的条件
	{
		ch = fgetc(fp);
		for (; ch != ','; ch = fgetc(fp))  //以‘,’为界，逐个字符读国家名并存入
		{
			ct[i].country += ch;
		}
		ch = fgetc(fp);
		for (; ch != ','; ch = fgetc(fp))  //以‘,’为界，逐个字符读城市名并存入
		{
			ct[i].city += ch;
		}
		fscanf(fp, "%f,", &f);  //读维度
		ct[i].latitude = f;
		fscanf(fp, "%f\n", &f);  //读经度
		ct[i].longitude = f;
		i++;  //为读取下一个city信息做准备
	}
	fclose(fp);  //关闭文件

	FILE* fp1 = NULL;
	fp1 = fopen("D:\\学习\\大一下yyw\\数据结构与算法\\实习\\routes.csv", "r");
	if (fp1 == NULL)  //判断异常
	{
		cout << "error2";
		exit(0);
	}

	i = 0;
	Route route[M];

	ch = fgetc(fp1);
	while (!feof(fp1))  //读至文件结尾的条件
	{
		while (ch != ',')
		{
			route[i].origion_city += ch;  //以‘,’为界，逐个字符读取出发城市名并存入
			ch = fgetc(fp1);
		}
		ch = fgetc(fp1);
		while (ch != ',')
		{
			route[i].destination_city += ch;  //以‘,’为界，逐个字符读取到达城市名并存入
			ch = fgetc(fp1);
		}
		ch = fgetc(fp1);
		while (ch != ',')
		{
			route[i].transport += ch;  //以‘,’为界，逐个字符读取交通工具并存入
			ch = fgetc(fp1);
		}

		fscanf(fp1, "%f,", &route[i].time);  //读取所需时间
		fscanf(fp1, "%f,", &route[i].cost);  //读取所需花费

		ch = fgetc(fp1);
		while (ch != '\n')
		{
			route[i].other_information += ch;  //以‘\n’为界，读取其他信息
			ch = fgetc(fp1);
		}
		ch = fgetc(fp1);
		i++;   //为读取下一个route信息做准备
	}
	fclose(fp1);  //关闭文件

	//初始化邻接矩阵
	Graph g;
	g.arcnum = 199 * 199;  //矩阵的边数
	g.vexnum = 199;  //矩阵的顶点数
	g.city = ct;  
	for (i = 0; i < 199; i++)  //循环遍历所有的边
		for (j = 0; j < 199; j++) 
		{
			if (i == j)  //出发地和目的地相同的边赋初值0
			{
				g.arcs[i][j].cost = 0;
				g.arcs[i][j].time = 0;
			}
			else  //其余边赋初值无穷大
			{
				g.arcs[i][j].cost = N;
				g.arcs[i][j].time = N;
			}
		}
	//建立图的邻接矩阵
	for (i = 0; i < M; i++)
	{
		int ori , des;
		ori = LocatVex(route[i].origion_city, ct);  //找到出发地城市的序号
		des = LocatVex(route[i].destination_city, ct);  //找到目的地城市的序号
		g.arcs[ori][des].cost = route[i].cost;  //将路线上的金钱权值存入矩阵中
		g.arcs[ori][des].time = route[i].time;  //将路线上的时间权值存入矩阵中
		g.arcs[ori][des].transport = route[i].transport;  //将路线的交通工具存入矩阵中
		g.arcs[ori][des].otherInfo = route[i].other_information;  //将路线的信息地址存入矩阵中
	}

	int m, n = 0;
	string start;
	do {
		cout << "深度优先遍历:" << endl << "请输入遍历起点:";
		cin >> start;
		for (m = 0; m < g.vexnum; m++)
		{
			if (start == g.city[m].city)  //找到遍历起点城市
			{
				n = 1;
				break;
			}
		}
	} while (n=0);  //没有找到遍历起点城市则一直循环

	DFS(g, m);  //进行深度优先遍历
	cout << "END" << endl;

	Way way[199][199];
	int k;
	for (i = 0; i < 199; i++)  //初始化way中的path
		for (j = 0; j < 199; j++)
		{
			way[i][j].path = (int*)malloc(sizeof(int) * 199);
			for (k = 0; k < 199; k++)
				way[i][j].path[k] = -1;
		}
	float dist[199][199];

	int found = 0;
	char a[30], b[30];
	string city1;
	string city2;
	int depart, dest;
	float min=N;  //初始化min为无穷大

	do {
		cout << "\n请输入您的出发地:" << endl;
		cin >> a;
		city1 = a;

		cout << "\n请输入您的目的地:" << endl;
		cin >> b;
		city2 = b;

		depart = LocatVex(city1, ct);
		dest = LocatVex(city2, ct);
		if (depart == N)
		{
			cout << "\n没有找到出发地\n";
			found = 1;
		}
		if (dest == N)
		{
			cout << "\n没有找到目的地\n";
			found = 1;
		}
	} while (found);

	int t1 = clock();  //记录开始的时间
	DIJ(g, depart, dest, way, dist, min);

	cout << "\n时间最短路径:" << city1;

	FILE* fp3=NULL;
	fp3 = fopen("D:\\学习\\大一下yyw\\数据结构与算法\\实习\\graph.htm", "w");
	if (fp3 == NULL)
	{
		cout << "error3!";
		exit(0);
	}
	//最短路径可视化
	std::string s = "<!DOCTYPE html><html><head>\
<style type='text/css'>body, html{width: 100%;height: 100%;margin:0;font-family:'微软雅黑';}#allmap{height:100%;width:100%;}#r-result{width:100%;}</style>\
<script type='text/javascript' src='http://api.map.baidu.com/api?v=2.0&ak=nSxiPohfziUaCuONe4ViUP2N'></script>";

	fprintf(fp3, "%s<title>Shortest path from %s to %s</title></head><body>\
<div id='allmap'></div></div></body></html><script type='text/javascript'>\
var map = new BMap.Map('allmap');\
var point = new BMap.Point(0, 0);\
map.centerAndZoom(point, 2);\
map.enableScrollWheelZoom(true);", s.c_str(), a, b);

	i = 0; j = 0;
	int bycity = depart, bycity2;
	
	while (bycity != dest)
	{
		fprintf(fp3, "var marker%d = new BMap.Marker(new BMap.Point(%.4f, %.4f));\
map.addOverlay(marker%d);\n\
var infoWindow%d = new BMap.InfoWindow(\"<p style = 'font-size:14px;'>country: %s<br/>city: %s</p>\");\
marker%d.addEventListener(\"click\", function(){\
this.openInfoWindow(infoWindow%d);}); ", j, ct[bycity].longitude, ct[bycity].latitude, j, j, ct[bycity].country.c_str(), ct[bycity].city.c_str(), j, j);

		j++;
		bycity2 = bycity;
		if (way[depart][dest].path[i] != -1)
			bycity = way[depart][dest].path[i++];
		else bycity = dest;

		fprintf(fp3, "var marker%d = new BMap.Marker(new BMap.Point(%.4f, %.4f));\
map.addOverlay(marker%d);\n\
var infoWindow%d = new BMap.InfoWindow(\"<p style = 'font-size:14px;'>country: %s<br/>city: %s</p>\");\
marker%d.addEventListener(\"click\", function(){\
this.openInfoWindow(infoWindow%d);}); ", j, ct[bycity].longitude, ct[bycity].latitude, j, j, ct[bycity].country.c_str(), ct[bycity].city.c_str(), j, j);


		fprintf(fp3, "var contentString0%d = '%s, %s --> %s, %s (%s - %.0f hours - $%.0f - %s)';\
var path%d = new BMap.Polyline([new BMap.Point(%.4f, %.4f),new BMap.Point(%.4f, %.4f)], {strokeColor:'#18a45b', strokeWeight:8, strokeOpacity:0.8});\
map.addOverlay(path%d);\
path%d.addEventListener(\"click\", function(){\
alert(contentString0%d);});", j, ct[bycity2].city.c_str(), ct[bycity2].country.c_str(), ct[bycity].city.c_str(), ct[bycity].country.c_str(), g.arcs[bycity2][bycity].transport.c_str(), g.arcs[bycity2][bycity].time, g.arcs[bycity2][bycity].cost, g.arcs[bycity2][bycity].otherInfo.c_str(), j, ct[bycity2].longitude, ct[bycity2].latitude, ct[bycity].longitude, ct[bycity].latitude, j, j, j);

		cout << "->" << ct[bycity].city;
	}
	cout << endl;
	fprintf(fp3, "</script>\n");

	fclose(fp3);  //关闭graph.htm文件

	int t2 = clock();  //记录结束的时间
	cout << "\n算法运行时间为：" << t2 - t1 << " ms" << endl;  //计算算法运行时间
}
