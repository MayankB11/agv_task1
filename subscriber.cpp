#include<iostream>
#include "ros/ros.h"
#include "geometry_msgs/Point.h"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
Mat img=imread("/home/mayank/catkin_ws/src/beginner_tutorials/src/ps1.jpg",1);

void plot_Point(const geometry_msgs::Point& p)
{
    img.at<Vec3b>(p.x,p.y)[0]=255;
    img.at<Vec3b>(p.x,p.y)[1]=0;
    img.at<Vec3b>(p.x,p.y)[2]=0;
    cout<<p.x<<" "<<p.y<<"\n";
    imshow("tested",img);
    waitKey(100);
}

int main(int argc,char **argv)
{
    imshow("test",img);
    waitKey(10);
    ros::init(argc,argv,"subscriber");
    ros::NodeHandle nh;
    ros::Subscriber sub=nh.subscribe("points",1000,plot_Point);
    ros::spin();
    //ros::loop_rate.sleep();
    return 0;
}
