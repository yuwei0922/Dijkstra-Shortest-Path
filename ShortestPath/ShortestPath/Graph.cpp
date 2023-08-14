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

#define  M  1975  //������
#define  N 100000 //�����
#define  TRUE 1
#define  FALSE 0
typedef int Boolean;
Boolean visited[199];  //����visited[]���飬����ͼ�ı���

//����һ���ṹCity
struct City
{
	bool pass;  //�ж�·���Ƿ񾭹��ó���
	string country, city;  //��������������
	float latitude, longitude;  //γ�ȣ�����
};

//����һ���ṹRoute
struct Route
{
	string origion_city, destination_city,  //�����أ�Ŀ�ĵ�
		transport, other_information;  //��ͨ���ߣ���Ϣ��ַ
	float time, cost;  //�ö�·�������ѵ�ʱ�䣬��Ǯ
};

//������
struct Arc
{
	float time;  //�����ѵ�ʱ��
	float cost;  //�����ѵĽ�Ǯ
	string transport;  //��ͨ����
	string otherInfo;  //��Ϣ��ַ
};

//ͼ���ڽӾ���
struct Graph
{
	City* city;  //ָ����е�ָ��
	Arc arcs[199][199];  //�ڽӾ���
	int vexnum, arcnum;  //���嶥����������
};

//����һ���ṹWay
struct Way
{
	int* path;  //·��ָ�룬ָ��·�������ĳ������
};

//��ͼ�в��Ҷ���
int LocatVex(const string& origin, City ct[])
{
	int i = 0;
	for (; i < 199; i++)  //���β�������ĳ��е����
	{
		if (origin == ct[i].city)  //������е��������i+1������������ͬ
			return i;  //����iֵ
	}
	return N;  //��û�ҵ�������һ����������
}

Way Joint(Way& a, Way& b, int k)  //��������·��
{
	Way c;
	c.path = (int*)malloc(sizeof(int) * 199);
	if (c.path == NULL)  //�ж��쳣
	{
		cout << "error2!"; 
		exit(0);
	}
	int i = 0;
	for (; i < 199; i++)  //��ʼ��c.path[]
	{
		c.path[i] = -1;
	}
	i = 0;
	while (a.path[i] != -1)  //�ҵ�a�е�·�������ĵ�
	{
		c.path[i] = a.path[i];  //��a��·�������ĵ����c��
		i++;
	}
	c.path[i] = k;  //��¼��ǰ���¼���״̬�ĳ��е����
	i++;
	int j = 0;
	while (b.path[j] != -1)  //����b�е�·�������ĵ�
	{
		c.path[i] = b.path[j];
		i++;
		j++;
	}
	return c;  //���������Ӻõ����·��
}

//�ڽӾ����������ȵݹ��㷨����
void DFS(Graph &g, int r)
{
	int s;
	visited[r] = TRUE;  //���������б����ʣ��ı���visited��ֵΪ1
	cout<<g.city[r].city<<"->";  //����������ĳ�������

	for (s = 0; s < g.vexnum; s++)  //ѭ�������������ж���
	{
		if (g.arcs[r][s].time != 0 && g.arcs[r][s].time != N && visited[s]== FALSE)  //����õ���·��û�з��ʣ��������з���
		{
			DFS(g, s);  //�ݹ����
		}
	}
}

//Dijkstra�㷨�����·��
void DIJ(const Graph& g, int depart, int dest, Way way[][199], float dist[][199],float min)
{
	for (int v = 0; v < g.vexnum; ++v)  //���������ж���
	{
		g.city[v].pass = false;  //��ʼ��pass��Ϊfalse
		dist[depart][v] = g.arcs[depart][v].time;
	}
	dist[depart][depart] = 0;
	g.city[depart].pass = true;   //��������pass���ΪΪtrue,�����·��Ϊ0.�˶����Ժ󲻻����õ�
	for (int i = 0; i < g.vexnum; ++i)  //��ʼ��ѭ����ÿ����õ��������е�ĳ�����е����·���������ö�����ӵ�S��
	{
		int v = 0;
		double mincost = N;
		for (int w = 0; w < g.vexnum; ++w)  //�ҳ���ǰԴ�㵽���������·��
		{
			if (g.city[w].pass == false)
				if (dist[depart][w] < mincost)  //���w������������и������ҵ�ǰ�����ڼ���V-S��
				{
					v = w;  //��v��¼��������и�����w���е����
					mincost = dist[depart][w];  //��mincost��¼Ȩֵ��������ʱ�䣩
				}
		}
		g.city[v].pass = true;  //����v���е�pass״̬
		for (int w = 0; w < g.vexnum; ++w)  //���������·�������ʵ�����
		{
			if (g.city[w].pass == false && (mincost + g.arcs[v][w].time < dist[depart][w]))
			{
				dist[depart][w] = mincost + g.arcs[v][w].time;
				way[depart][w] = Joint(way[depart][v], way[v][w], v);
			}
		}
		if (g.city[dest].pass == true)  //����Ŀ�ĵص�passΪtrue����Ŀ�ĵر���ӵ�����S�У���ֹͣѰ�����·��
		{
			min = dist[depart][dest];  //�õ��������ʱ��
			cout << '\n'<<"��ʱ��:" << min << endl;
			break;  //������ѭ��
		}
	}
}

void main()
{
	//��ȡ�ļ�����
	FILE* fp=NULL;
	fp = fopen("D:\\ѧϰ\\��һ��yyw\\���ݽṹ���㷨\\ʵϰ\\cities.csv", "r");
	if (fp == NULL)  //�ж��쳣
	{
		printf("error1!");
		exit(0);
	}
	char ch;  //����ѭ��
	float f;
	int i = 0, j;
	City ct[199];

	while (!feof(fp))  //�����ļ�ĩβ������
	{
		ch = fgetc(fp);
		for (; ch != ','; ch = fgetc(fp))  //�ԡ�,��Ϊ�磬����ַ���������������
		{
			ct[i].country += ch;
		}
		ch = fgetc(fp);
		for (; ch != ','; ch = fgetc(fp))  //�ԡ�,��Ϊ�磬����ַ���������������
		{
			ct[i].city += ch;
		}
		fscanf(fp, "%f,", &f);  //��ά��
		ct[i].latitude = f;
		fscanf(fp, "%f\n", &f);  //������
		ct[i].longitude = f;
		i++;  //Ϊ��ȡ��һ��city��Ϣ��׼��
	}
	fclose(fp);  //�ر��ļ�

	FILE* fp1 = NULL;
	fp1 = fopen("D:\\ѧϰ\\��һ��yyw\\���ݽṹ���㷨\\ʵϰ\\routes.csv", "r");
	if (fp1 == NULL)  //�ж��쳣
	{
		cout << "error2";
		exit(0);
	}

	i = 0;
	Route route[M];

	ch = fgetc(fp1);
	while (!feof(fp1))  //�����ļ���β������
	{
		while (ch != ',')
		{
			route[i].origion_city += ch;  //�ԡ�,��Ϊ�磬����ַ���ȡ����������������
			ch = fgetc(fp1);
		}
		ch = fgetc(fp1);
		while (ch != ',')
		{
			route[i].destination_city += ch;  //�ԡ�,��Ϊ�磬����ַ���ȡ���������������
			ch = fgetc(fp1);
		}
		ch = fgetc(fp1);
		while (ch != ',')
		{
			route[i].transport += ch;  //�ԡ�,��Ϊ�磬����ַ���ȡ��ͨ���߲�����
			ch = fgetc(fp1);
		}

		fscanf(fp1, "%f,", &route[i].time);  //��ȡ����ʱ��
		fscanf(fp1, "%f,", &route[i].cost);  //��ȡ���軨��

		ch = fgetc(fp1);
		while (ch != '\n')
		{
			route[i].other_information += ch;  //�ԡ�\n��Ϊ�磬��ȡ������Ϣ
			ch = fgetc(fp1);
		}
		ch = fgetc(fp1);
		i++;   //Ϊ��ȡ��һ��route��Ϣ��׼��
	}
	fclose(fp1);  //�ر��ļ�

	//��ʼ���ڽӾ���
	Graph g;
	g.arcnum = 199 * 199;  //����ı���
	g.vexnum = 199;  //����Ķ�����
	g.city = ct;  
	for (i = 0; i < 199; i++)  //ѭ���������еı�
		for (j = 0; j < 199; j++) 
		{
			if (i == j)  //�����غ�Ŀ�ĵ���ͬ�ı߸���ֵ0
			{
				g.arcs[i][j].cost = 0;
				g.arcs[i][j].time = 0;
			}
			else  //����߸���ֵ�����
			{
				g.arcs[i][j].cost = N;
				g.arcs[i][j].time = N;
			}
		}
	//����ͼ���ڽӾ���
	for (i = 0; i < M; i++)
	{
		int ori , des;
		ori = LocatVex(route[i].origion_city, ct);  //�ҵ������س��е����
		des = LocatVex(route[i].destination_city, ct);  //�ҵ�Ŀ�ĵس��е����
		g.arcs[ori][des].cost = route[i].cost;  //��·���ϵĽ�ǮȨֵ���������
		g.arcs[ori][des].time = route[i].time;  //��·���ϵ�ʱ��Ȩֵ���������
		g.arcs[ori][des].transport = route[i].transport;  //��·�ߵĽ�ͨ���ߴ��������
		g.arcs[ori][des].otherInfo = route[i].other_information;  //��·�ߵ���Ϣ��ַ���������
	}

	int m, n = 0;
	string start;
	do {
		cout << "������ȱ���:" << endl << "������������:";
		cin >> start;
		for (m = 0; m < g.vexnum; m++)
		{
			if (start == g.city[m].city)  //�ҵ�����������
			{
				n = 1;
				break;
			}
		}
	} while (n=0);  //û���ҵ�������������һֱѭ��

	DFS(g, m);  //����������ȱ���
	cout << "END" << endl;

	Way way[199][199];
	int k;
	for (i = 0; i < 199; i++)  //��ʼ��way�е�path
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
	float min=N;  //��ʼ��minΪ�����

	do {
		cout << "\n���������ĳ�����:" << endl;
		cin >> a;
		city1 = a;

		cout << "\n����������Ŀ�ĵ�:" << endl;
		cin >> b;
		city2 = b;

		depart = LocatVex(city1, ct);
		dest = LocatVex(city2, ct);
		if (depart == N)
		{
			cout << "\nû���ҵ�������\n";
			found = 1;
		}
		if (dest == N)
		{
			cout << "\nû���ҵ�Ŀ�ĵ�\n";
			found = 1;
		}
	} while (found);

	int t1 = clock();  //��¼��ʼ��ʱ��
	DIJ(g, depart, dest, way, dist, min);

	cout << "\nʱ�����·��:" << city1;

	FILE* fp3=NULL;
	fp3 = fopen("D:\\ѧϰ\\��һ��yyw\\���ݽṹ���㷨\\ʵϰ\\graph.htm", "w");
	if (fp3 == NULL)
	{
		cout << "error3!";
		exit(0);
	}
	//���·�����ӻ�
	std::string s = "<!DOCTYPE html><html><head>\
<style type='text/css'>body, html{width: 100%;height: 100%;margin:0;font-family:'΢���ź�';}#allmap{height:100%;width:100%;}#r-result{width:100%;}</style>\
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

	fclose(fp3);  //�ر�graph.htm�ļ�

	int t2 = clock();  //��¼������ʱ��
	cout << "\n�㷨����ʱ��Ϊ��" << t2 - t1 << " ms" << endl;  //�����㷨����ʱ��
}
