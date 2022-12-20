#include <ros/ros.h>
#include <gazebo_msgs/SetModelState.h>
#include <math.h>
#include <assert.h>
#include <iostream>

#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <sensor_msgs/PointCloud2.h>

#include <tf/tf.h>
#include "tf/transform_datatypes.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "gazebo_set_states_publisher");
 
    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");
      

    gazebo_msgs::SetModelState objstate;
    double distance = 2.0;
    double height = 1.0;
    double angle = 0;
    objstate.request.model_state.model_name = "mobile_base";//"acircles_pattern_0";
    objstate.request.model_state.pose.position.x = distance*cos(angle);
    objstate.request.model_state.pose.position.y = distance*sin(angle);
    objstate.request.model_state.pose.position.z = height;
    tf::Quaternion q_init = tf::createQuaternionFromRPY(0, 35.0/180.0*M_PI, angle+M_PI);
    objstate.request.model_state.pose.orientation.w = q_init.w();
    objstate.request.model_state.pose.orientation.x = q_init.x();
    objstate.request.model_state.pose.orientation.y = q_init.y();
    objstate.request.model_state.pose.orientation.z = q_init.z();
    objstate.request.model_state.twist.linear.x = 0.0;
    objstate.request.model_state.twist.linear.y = 0.0;
    objstate.request.model_state.twist.linear.z = 0.0;
    objstate.request.model_state.twist.angular.x = 0.0;
    objstate.request.model_state.twist.angular.y = 0.0;
    objstate.request.model_state.twist.angular.z = 0.0;
    objstate.request.model_state.reference_frame = "world";
    std::cout<<"imu pose x:"<<objstate.request.model_state.pose.position.x
                <<",y:"<<objstate.request.model_state.pose.position.y
                <<",z:"<<objstate.request.model_state.pose.position.z
                <<",qw:"<<q_init.w()
                <<",qx:"<<q_init.x()
                <<",qy:"<<q_init.y()
                <<",qz:"<<q_init.z()
                <<std::endl;
    client.call(objstate);
    std::cout<<"INIT POSE ----------------------"<<std::endl;
    std::cout<<"按任意键继续 ----------------------"<<std::endl;
    double cout = 0;

    ros::Rate loop_rate(1000);
    std::cin.get();
    while(ros::ok()) {
        angle = cout/180.0*M_PI ;
        double x = distance*cos(angle);
        double y = distance*sin(angle);
        double z = height;
        double mean_x=0, mean_y=0;
        //cv::Mat view = (cv::Mat_<float>(3, 1) << mean_x-x, mean_y-y, 0);
        //double angle = atan( (mean_y-y)/(mean_x-x) );
        //if( (mean_x-x)<0 && (mean_y-y)>0 )
        //    angle = angle +  M_PI;
        //if( (mean_x-x)<0 && (mean_y-y)<0 )
        //    angle = angle -  M_PI;
        //// Eigen::AngleAxisd rotation_vector (angle, Eigen::Vector3d(0,0,1));
        //// Eigen::Matrix3d rotation_matrix = rotation_vector.toRotationMatrix();
        //// cv::Mat rotate_mat = Converter::toCvMat(rotation_matrix);
        tf::Quaternion q = tf::createQuaternionFromRPY(0, 35.0/180.0*M_PI, angle+M_PI);

        objstate.request.model_state.pose.position.x = x;
        objstate.request.model_state.pose.position.y = y;
        objstate.request.model_state.pose.position.z = z;
        objstate.request.model_state.pose.orientation.w = q.w();
        objstate.request.model_state.pose.orientation.x = q.x();
        objstate.request.model_state.pose.orientation.y = q.y();
        objstate.request.model_state.pose.orientation.z = q.z();

        std::cout<<"pose x:"<<objstate.request.model_state.pose.position.x
                <<",y:"<<objstate.request.model_state.pose.position.y
                <<",z:"<<objstate.request.model_state.pose.position.z
                <<",qw:"<<q.w()
                <<",qx:"<<q.x()
                <<",qy:"<<q.y()
                <<",qz:"<<q.z()
                <<std::endl;
        client.call(objstate);
        ROS_INFO("call service");

        cout += 0.01;
        ros::spinOnce();
        // loop_rate.sleep(); // is stuck on loop rate, reboot roscore
        //ROS_INFO("loop_rate sleep over");
    }
    ROS_INFO("end service");
    return 0;
}
