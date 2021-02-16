#ifndef MY_TURTLESIM_CONTROLLER_H
#define MY_TURTLESIM_CONTROLLER_H

#include <ros/ros.h>
#include <vector>
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"


class MyTurtlesimController
{
public:
    MyTurtlesimController();
    void process();

private:
    void pose_callback(const turtlesim::Pose::ConstPtr &);
    void go_straight();

    int hz;
    //int turn_flag;
    //float turn_point[4][3]={{5.0,5.0,0.0},{8.0,5.0,M_PI/2},{8.0,8.0,M_PI-0.05},{5.0,8.0,-1*M_PI/2}};
    //int direction;
    //int axis;
    //int passed_point;
    //float pre_theta;

    int N=3;
    float length=3.0;
    float omega=M_PI/2.0/8.0;
    float move_length;
    float pre_x;
    float pre_y;
    float pre_theta;
    float integrated_theta;
    float target_theta;
    int turn_flag;
    int turn_over;


    ros::Publisher pub_cmd_vel;
    ros::Subscriber sub_pose;
    ros::NodeHandle nh;
    ros::NodeHandle private_nh;
    turtlesim::Pose current_pose;
};
#endif
