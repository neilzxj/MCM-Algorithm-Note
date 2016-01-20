//  main.cpp
//  TSPcode
//  Created by 葛鑫 on 1/17/16.
//  Copyright © 2016 tesp. All rights reserved.
//
#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <random>
#include <time.h>
#include <stdlib.h>
#include <ctime>
#define MAXN 10000               //搜索区域大小
using namespace std;

//遗传算法答案构造
typedef struct{
    vector<int> road;double fitness;int iter;
}AnsForm;

//全局变量
double CityDistance[MAXN][MAXN],MutateP=0.0;
int NumofCity=0,NumofGeneration=0,NumofCrossOver=0,iterTime=0,cnt=1,low,high;
AnsForm Genration[MAXN];
vector<AnsForm> ans;
clock_t Initstart,Initend,Iterstart,Iterend;

//随机浮点数产生器：均匀分布
double fRand(double fMin, double fMax){
    uniform_real_distribution<double> dist(fMin,fMax);  //(min, max)
    mt19937 rng;
    rng.seed(random_device{}());
    return dist(rng);
}

//随机整数产生器：均匀分布
int iRand(int iMin,int iMax){
    static default_random_engine re {};
    using Dist = uniform_int_distribution<int>;
    static Dist uid {};
    return uid(re, Dist::param_type{iMin,iMax});
}

//获得n个城市间的距离(1.0, 1000.0)
void GetDistance(void){
    pair<int, int> city[MAXN];
    for(int i=0;i<NumofCity;i++){
        city[i].first=iRand(low, high);
        city[i].second=iRand(low, high);
    }
    for(int i=0;i<NumofCity;i++)
        for(int j=i;j<NumofCity;j++){
            int x1=city[i].first,x2=city[j].second;
            int y1=city[i].second,y2=city[j].second;
            double d=(double)abs((x2*x2-x1*x1)+(y2*y2-y1*y1));
            if(j!=i) {CityDistance[i][j]=sqrt(d);CityDistance[j][i]=CityDistance[i][j];}
            else CityDistance[i][j]=0.0;
        }
}

//打印城市间距离。
void PrintDistance(void){
    for(int i=0;i<NumofCity;i++){
        for(int j=0;j<NumofCity;j++)
            printf("%4.3f  ",CityDistance[i][j]);
        cout<<endl;
    }
}

//初始化种群:获得NoG个随机个体
void InitGeneration(void){
    AnsForm Standard;
    for(int i=0;i<NumofGeneration;i++){
        for(int j=1;j<=NumofCity;j++)
            Standard.road.push_back(j);             //构造一个标准个体
        int iterTime=iRand(10, 50);                 //随机交换iterTime次 [10,100]
        for(int iter=1;iter<=iterTime;iter++){
            int A=iRand(0, NumofCity-1),B=iRand(0, NumofCity-1);
            swap(Standard.road[A],Standard.road[B]);//随机交换标准个体中的位置A与位置B的值
        }                                           //A与B也是随机整数
        for(int j=0;j<NumofCity;j++)
            Genration[i]=Standard;                  //得到NoG个随机个体
    }
}

//打印种群中的答案信息，适应度。
void PrintGeneration(){
    for(int i=0;i<NumofGeneration;i++){
        for(int j=0;j<NumofCity;j++)
            cout<<Genration[i].road[j]<<" ";
        cout<<Genration[i].fitness<<endl;
    }
}

//计算适应度
void CalFitness(){
    for(int i=0;i<NumofGeneration;i++){
        double fitness=CityDistance[Genration[i].road[0]-1][Genration[i].road[NumofCity-1]-1];
        for(int j=1;j<NumofCity;j++)
            fitness+=CityDistance[Genration[i].road[j-1]-1][Genration[i].road[j]-1];
        Genration[i].fitness=fitness;
    }
}

//比较函数,种群按适应度从小到大排序
int FitnessCMP(AnsForm a,AnsForm b){return a.fitness<b.fitness;}

//对种群适应度靠后的70%个体进行CrossOver,cnt是CrossOver次数
void CrossOver(int cnt){
    while(cnt--){
        //确定进行CrossOver的2个个体
        int PersoneA=iRand(0, NumofGeneration-1);
        int PersoneB=iRand(0, NumofGeneration-1);
        int len=iRand(0, NumofCity/2);//交换片段长度为随机数,最短为0,最长为城市数的一半
        int begin=iRand(0, NumofCity-1);//交换起点
        for(int i=0;i<len;i++){
            int pos=(begin+i)%(NumofCity),find=0;
            int replace=Genration[PersoneB].road[pos];
            while(find<NumofCity && Genration[PersoneA].road[find]!=replace) find++;
            swap(Genration[PersoneA].road[pos], Genration[PersoneA].road[find]);
        }
    }
}

//变异函数
void Mutation(double p){
    int NumofMutation=(int)(p*NumofGeneration);
    while(NumofMutation--){
        int Mutate=iRand(0, NumofGeneration-1);
        int x=iRand(0, NumofCity-1),y=iRand(0, NumofCity-1);
        swap(Genration[Mutate].road[x], Genration[Mutate].road[y]);
    }
}

int FindMin(){
    int flag=0;
    for(int i=0;i<NumofGeneration;i++)
        if(Genration[i].fitness<Genration[flag].fitness)
            flag=i;
    return flag;
}
void PrintInfo(int index){
    for(int i=0;i<NumofCity && i<10;i++)
        printf("%d ",Genration[index].road[i]);
    if(NumofCity>10) printf("......");
    printf("距离: %.3f 迭代次数:第 %d 次\n",Genration[index].fitness,cnt);
}

int main() {
    freopen("/Users/gexin/Desktop/in.txt", "r", stdin);
    freopen("/Users/gexin/Desktop/out.txt","w", stdout);
    
    cout<<"请输入城市个数,种群数量,以及最大迭代次数(以空格分开)"<<endl;cin>>NumofCity>>NumofGeneration>>iterTime;
    cout<<"请输入地图范围(x,x->y,y),x:_ y:_"<<endl;               cin>>low>>high;
    cout<<"请输入每次迭代的CrossOver次数"<<endl;                   cin>>NumofCrossOver;
    cout<<"请输入种群变异概率[0.0, 1.0]"<<endl;                   cin>>MutateP;
    
    cout<<"城市数量:"<<NumofCity<<endl<<"种群数量:"<<NumofGeneration<<endl
    <<"迭代次数:"<<iterTime<<endl<<"CrossOver次数:"
    <<NumofCrossOver<<endl<<"种群中个体变异概率:"<<MutateP<<endl;
    
    GetDistance();
    InitGeneration();
    CalFitness();
    sort(Genration,Genration+NumofGeneration,FitnessCMP);
    double MinDistance=Genration[NumofGeneration-1].fitness;
    
    Iterstart=clock();
    while(cnt<=iterTime){
        int index=FindMin();
        Genration[index].iter=cnt;
        if(ans.empty() || Genration[index].fitness<MinDistance){
            ans.push_back(Genration[index]);
            MinDistance=Genration[index].fitness;
            PrintInfo(index);
        }
        CrossOver(NumofCrossOver);
        Mutation(MutateP);
        CalFitness();
        cnt++;
    }
    //sort(ans.begin(), ans.end(), FitnessCMP);
    Iterend=clock();
    cout<<"----------------------------------------"<<endl;
    printf("遗传算法运行时间: %.3fs\n",(double)(Iterend-Initstart)/CLOCKS_PER_SEC);
    return 0;
}

