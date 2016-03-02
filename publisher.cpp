//#include <stdio.h>
struct point
{
    long int x;
    long int y;
};
//int flag=1;
#include<stack>
#include "ros/ros.h"
#include<iostream>
#include <opencv2/opencv.hpp>
#include "geometry_msgs/Point.h"
using namespace std;
using namespace cv;
Mat img=imread("/home/mayank/catkin_ws/src/beginner_tutorials/src/ps1.jpg",1);
Mat visited(img.rows,img.cols,CV_8UC1);
Mat visited1(img.rows,img.cols,CV_8UC1);
long rx=0,ry=0,gx=0,gy=0,cr=0,cg=0,i,j;


long double v[200][200];


bool isred(int x,int y)
{
    if(img.at<Vec3b>(x,y)[0]<127&&img.at<Vec3b>(x,y)[1]<127&&img.at<Vec3b>(x,y)[2]>127)
        return true;
    else
        return false;
}
bool isgreen(int x,int y)
{
    if(img.at<Vec3b>(x,y)[0]<127&&img.at<Vec3b>(x,y)[1]>127&&img.at<Vec3b>(x,y)[2]<127)
        return true;
    else
        return false;
}
void dfs(point p)
{
stack<point> s;
s.push(p);

do
{
point A = s.top();
s.pop();
visited.at<uchar>(A.x, A.y) = 255;
if(isred(A.x,A.y))
{
    cr++;
    rx+=A.x;
    ry+=A.y;
}
else if(isgreen(A.x,A.y))
{
    cg++;
    gx+=A.x;
    gy+=A.y;
}

for (int k = -1;k < 2;k++)
{
for (int l = -1;l < 2;l++)
{

if (A.x + k>=0&&A.x+k<img.rows&&A.y + l>=0&&A.y+l<img.cols)
{
if (visited.at<uchar>(A.x + k, A.y + l) == 0&&(isred(A.x + k, A.y + l)||isgreen(A.x + k, A.y + l)))
{
point p1 = { A.x + k,A.y + l };
s.push(p1);
}
else
{
    visited.at<uchar>(A.x + k, A.y + l) == 255;
}
}
}
}
}
while (!s.empty());
}
stack<point> plott;

void dij(point p)
{
    int i=p.x,j=p.y;
    for(int k=-1;k<2;k++)
    {
        for(int l=-1;l<2;l++)
        {
            if(!(k==0&&l==0)&&i+k>=0&&i+k<img.rows&&j+l>=0&&j+l<img.cols)
            {
                if(v[i+k][j+l]!=20000)
                {
                    if(v[i+k][j+l]>v[i][j]+1)
                    {
                     v[i+k][j+l]=v[i][j]+1;
                    }
                }
            }
        }
    }
}

int main(int argc,char **argv)
{
     ros::init(argc,argv,"publisher");
     ros::NodeHandle nh;

     ros::Publisher pub=nh.advertise<geometry_msgs::Point>("points",2000);
     ros::Rate loop_rate(10);
    //imshow("test",visited);
    for(i=0;i<200;i++)
    {
        for(j=0;j<200;j++)
        {
            v[i][j]=20000;
        }
    }

for (i = 0;i < img.rows;i++)
{
for (j = 0;j < img.cols;j++)
{
if(!(img.at<Vec3b>(i,j)[0]>127&&img.at<Vec3b>(i,j)[1]>127&&img.at<Vec3b>(i,j)[2]>127))
 v[i][j]=80000;
if (visited.at<uchar>(i, j) == 0&&(isred(i,j)||isgreen(i,j)))
{
    point t;
    t.x=i;
    t.y=j;
    dfs(t);
}
}
}
// centroid calculation
rx=rx/cr;
ry=ry/cr;
gx=gx/cg;
gy=gy/cg;
cout<<"Red x : "<<rx<<" y : "<<ry<<"\nGreen x : "<<gx<<" y : "<<gy<<"\n";



    v[rx][ry]=1;
    for(i=0;i<img.rows;i++)
    {
        for(j=0;j<img.cols;j++)
        {
            point t;
            t.x=i;
            t.y=j;
            dij(t);
        }
    }
        // for subscriber
    long mini=v[gx][gy];
    i=gx;
    j=gy;
    cout<<mini;
    int flag=1;
    while(!(i==rx&&j==ry))
    {
        flag=1;
        geometry_msgs::Point p;
        p.x=i;
        p.y=j;
       pub.publish(p);
       ros::spinOnce();
       loop_rate.sleep();
       cout<<i<<" "<<j<<"\n";
       long double mini=v[i][j];
        for(int k=-1;k><=1&&flag!=0;k++)
        {
            for(int l=-1;l<=1&&flag!=0;l++)
            {
                if(i+k>=0&&i+k<img.rows&&j+l>=0&&j+l<img.cols)
                if(v[i+k][j+l]==v[i][j]-1)
                {
                    i=i+k;
                    j=j+l;
                    flag=0;
                }
            }
        }

    }
    return 0;
}
