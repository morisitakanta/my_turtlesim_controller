#include "my_turtlesim_controller/my_turtlesim_controller.h"
MyTurtlesimController::MyTurtlesimController():private_nh("~")
{
    private_nh.param("hz",hz,{10});

    sub_pose=nh.subscribe("/turtle1/pose",100,&MyTurtlesimController::pose_callback,this);

    pub_cmd_vel=nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",1);

    /*turn_flag=false;
    direction=1;
    axis=0;
    passed_point=0;
    pre_theta=0;
    */
    move_length=0;
    pre_x=current_pose.x;
    pre_y=current_pose.y;
    pre_theta=current_pose.theta;
    integrated_theta=current_pose.theta;
    turn_flag=false;
    turn_over=false;
}

void MyTurtlesimController::pose_callback(const turtlesim::Pose::ConstPtr &msg)
{
    current_pose=*msg;
}
void MyTurtlesimController::go_straight()
{
    ROS_INFO_STREAM(current_pose);
    geometry_msgs::Twist cmd_vel;
    cmd_vel.linear.x=1;
    cmd_vel.angular.z=0;

////foward////
/*    if(0<((1-axis)*(current_pose.x-turn_point[(passed_point+1)%4][0])+axis*(current_pose.y-turn_point[(passed_point+1)%4][1]))*direction){
        turn_flag=true;
    }

    while(turn_flag && ros::ok()){
        cmd_vel.linear.x=0;
        cmd_vel.angular.z=M_PI/2/2;

        std::cout<<current_pose.theta<<std::endl;

        if(turn_point[(passed_point+1)%4][2]<0){
            while(current_pose.theta>0 && ros::ok()){
                pub_cmd_vel.publish(cmd_vel);
                ros::spinOnce();
            }
        }
        if(current_pose.theta>=turn_point[(passed_point+1)%4][2]){

            passed_point=(passed_point+1)%4;
            axis=!axis;
            if(axis==0) direction*=-1;
            turn_flag=false;
        }
    }
*/

////triangle////
    move_length+=sqrt(pow(current_pose.x-pre_x,2)+pow(current_pose.y-pre_y,2));
    pre_x=current_pose.x;
    pre_y=current_pose.y;

    if(move_length>length && turn_flag==false){
        turn_flag=true;
        target_theta=integrated_theta+2*M_PI/N;
    }

    if(turn_flag){
        if(target_theta>M_PI){
            target_theta-=2*M_PI;
            integrated_theta-=2*M_PI;
            pre_theta-=2*M_PI;
            turn_over=true;
        }

        if(target_theta>integrated_theta){
            cmd_vel.linear.x=0;
            cmd_vel.angular.z=omega;
            if(current_pose.theta>0) integrated_theta+=current_pose.theta-turn_over*2*M_PI-pre_theta;
            else integrated_theta+=current_pose.theta-pre_theta;
            pre_theta=integrated_theta;
        }else{
            turn_over=false;
            cmd_vel.linear.x=1;
            cmd_vel.angular.z=0;
            turn_flag=false;
            move_length=0;
        }
    }

    pub_cmd_vel.publish(cmd_vel);

}

void MyTurtlesimController::process()
{
    ros::Rate loop_rate(hz);
    while(ros::ok())
    {
        go_straight();

        ros::spinOnce();
        loop_rate.sleep();
    }
}

int main(int argc,char** argv)
{
    ros::init(argc,argv,"my_turtlesim_controller");
    MyTurtlesimController my_turtlesim_controller;
    my_turtlesim_controller.process();
    return 0;
}
