#include <ros/ros.h>
#include <ar_track_alvar_msgs/AlvarMarkers.h>
#include <tf/tf.h>
#include <tf/transform_datatypes.h>
#include "geometry_msgs/Twist.h"
#include <geometry_msgs/PoseStamped.h>
#include <actionlib_msgs/GoalStatusArray.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <cmath>

ros::Publisher pub;
ros::Publisher pub_goal;
ros::Publisher pub_status;
geometry_msgs::Twist velocity;


int count_new_start=0;
int status_number=0;
int count=0;
double init_pos_x=0;
double init_pos_y=0;
double init_pos_z=0;
double init_ori_x=0;
double init_ori_y=0;
double init_ori_z=0;
double init_ori_w=1;

double curr_ori_z=0;
double curr_ori_w=1;
int check_start=0;

int count_cb=0;
int count_end=0;
int check=0;
int count_docking=0;
int start=0;

void callback3(const geometry_msgs::PoseWithCovarianceStamped PWCS){
        geometry_msgs::PoseStamped goal;
    if (!count){
        init_pos_x = PWCS.pose.pose.position.x;
        init_pos_y = PWCS.pose.pose.position.y;
        init_pos_z = PWCS.pose.pose.position.z;
        init_ori_x = PWCS.pose.pose.orientation.x;
        init_ori_y = PWCS.pose.pose.orientation.y;
        init_ori_z = PWCS.pose.pose.orientation.z;
        init_ori_w = PWCS.pose.pose.orientation.w;
/*
        goal.header.frame_id="map";
        goal.pose.position.x = init_pos_x;
        goal.pose.position.y = init_pos_y;
        goal.pose.position.z = init_pos_z;
        goal.pose.orientation.x=init_ori_x;
        goal.pose.orientation.y=init_ori_y;
        goal.pose.orientation.z=init_ori_z;
        goal.pose.orientation.w =init_ori_w;
        pub_goal.publish(goal);
        */
        ROS_INFO("x_pos : %1.2f", init_pos_x);
        ROS_INFO("y_pos : %1.2f", init_pos_y);
        ROS_INFO("z_pos : %1.2f", init_pos_z);
        ROS_INFO("x_ori : %1.2f", init_ori_x);
        ROS_INFO("y_ori : %1.2f", init_ori_y);
        ROS_INFO("z_ori : %1.2f", init_ori_z);
        ROS_INFO("w_ori : %1.2f", init_ori_w);
        count = 1;
    }
    else if (count_end==2 && count_docking ==1){
        init_pos_x = PWCS.pose.pose.position.x;
        init_pos_y = PWCS.pose.pose.position.y;
        init_pos_z = PWCS.pose.pose.position.z;
        init_ori_x = PWCS.pose.pose.orientation.x;
        init_ori_y = PWCS.pose.pose.orientation.y;
        init_ori_z = PWCS.pose.pose.orientation.z;
        init_ori_w = PWCS.pose.pose.orientation.w;
        goal.header.frame_id="map";
        goal.pose.position.x = init_pos_x;
        goal.pose.position.y = init_pos_y;
        goal.pose.position.z = init_pos_z;
        goal.pose.orientation.x=init_ori_x;
        goal.pose.orientation.y=init_ori_y;
        goal.pose.orientation.z=init_ori_z;
        goal.pose.orientation.w =init_ori_w;
        pub_goal.publish(goal);
        ROS_INFO("x_pos : %1.2f", init_pos_x);
        ROS_INFO("y_pos : %1.2f", init_pos_y);
        ROS_INFO("z_pos : %1.2f", init_pos_z);
        ROS_INFO("x_ori : %1.2f", init_ori_x);
        ROS_INFO("y_ori : %1.2f", init_ori_y);
        ROS_INFO("z_ori : %1.2f", init_ori_z);
        ROS_INFO("w_ori : %1.2f", init_ori_w);
        count_end=0;
        count_docking=0;
    }

}



void callback2(const actionlib_msgs::GoalStatusArray status1){
    geometry_msgs::PoseStamped goal;
    if (start && count_end!=2){
      if (status1.status_list.size()){
        if (start && count_end!=2 && check_start==0){
            ROS_INFO("Start!");
            check_start=1;

          }
           status_number = status1.status_list[0].status;
        }

        if (status_number==1){
          ROS_INFO("Running...");
          count_cb=0;
          }
    else if (status_number==3 && count_end!=2 && init_pos_x != goal.pose.position.x  ){
        count_cb+=1;
        ROS_INFO("count_cb: %d" ,count_cb);


     if (count_end==1 && count_cb>=20){
         if (count_cb==40){
             count_cb=0;
             count_end+=1;
         }
     }
     else if (count_end==0 && init_pos_x != goal.pose.position.x){
        if (count_cb==40){

         goal.header.frame_id="map";
         goal.pose.position.x = init_pos_x;
         goal.pose.position.y = init_pos_y;
         goal.pose.position.z = init_pos_z;
         goal.pose.orientation.x=init_ori_x;
         goal.pose.orientation.y=init_ori_y;
         goal.pose.orientation.z=init_ori_z;
         goal.pose.orientation.w =init_ori_w;
         pub_goal.publish(goal);
         //pub.publish(velocity);
         count_cb=0;
         count_end+=1;
         ROS_INFO("Arrived");
         ROS_INFO("count_end: %d", count_end);
     }

    }
}   


}
}



void printPose(const ar_track_alvar_msgs::AlvarMarkers req)
{
    double x, y, z;


    //ROS_INFO("printPose");
    //ROS_INFO("req.markers.empty(): %d", req.markers.empty());
    if (!req.markers.empty() && start==0){
        start=1;
        ROS_INFO("detected");
        ROS_INFO("Ready");
    }


    if (count_end==2){

        count_cb=0;
        ROS_INFO("end");
        ROS_INFO("count_end: %d", count_end);

        if (!req.markers.empty() && count_docking==0) {
              ROS_INFO("Docking start");
              count_docking+=1;
        }
    if (!req.markers.empty()) {

          x = req.markers[0].pose.pose.position.x;
          y = req.markers[0].pose.pose.position.y;
          z = req.markers[0].pose.pose.position.z;


          ROS_INFO("x, y, z = %1.2f  %1.2f  %1.2f", x, y, z);

          if (z>0.3){
              velocity.linear.x = 0.04;
              pub.publish(velocity);
          }
          else {
              velocity.linear.x = 0;
              pub.publish(velocity);
              //count_end=0;
              start=0;
              count=0;
              //count_end=0;
              check_start=0;
              //count_docking=0;
              ROS_INFO("Docking ended");
          }

        }


}
}



int main(int argc, char **argv)
{
    ros::init(argc, argv, "pose");

    ros::NodeHandle nh;
    pub_goal = nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 10);
    pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    pub_status = nh.advertise<actionlib_msgs::GoalStatusArray>("/move_base/status", 10);


    ros::Subscriber init_pose_sub = nh.subscribe("/amcl_pose", 100, callback3);

    ros::Subscriber status_sub = nh.subscribe("/move_base/status", 100, callback2);
    ros::Subscriber pose_sub = nh.subscribe("ar_pose_marker", 1000, printPose);

    ros::spin();

    return 0;

}
