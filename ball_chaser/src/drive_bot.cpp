#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"
ros::Publisher motor_command_publisher;


// callback handle_drive_request function executes whenever a drive_bot service is requested
bool handle_drive_request(ball_chaser::DriveToTarget::Request & req,
                          ball_chaser::DriveToTarget::Response & res ) {
    ROS_INFO("DriveToTargetRequest received - Linear- Velocity-x:%1.2f, angular velocity:%1.2f", (float) req.linear_x, (float) req.angular_z);


    geometry_msgs::Twist motor_command;
    motor_command.linear.x = req.linear_x;
    motor_command.angular.z  = req.angular_z;

    motor_command_publisher.publish(motor_command);

    //ros::Duration(0.5).sleep();

    // return a response
    res.msg_feedback =
            "velocity x-linear set to:" + std::to_string(motor_command.linear.x ) + " velocity angular-z set to: " + std::to_string(motor_command.angular.z );
    ROS_INFO_STREAM(res.msg_feedback);
    return true;
}


int main(int argc, char** argv)
{
    // Initialize a ROS node
    ros::init(argc, argv, "drive_bot");

    // Create a ROS NodeHandle object
    ros::NodeHandle n;

    // Inform ROS master that we will be publishing a message of type geometry_msgs::Twist on the robot actuation topic with a publishing queue size of 10
    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    // TODO: Define a drive /ball_chaser/command_robot service with a handle_drive_request callback function
    ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);

    // TODO: Delete the loop, move the code to the inside of the callback function and make the necessary changes to publish the requested velocities instead of constant values

    // TODO: Handle ROS communication events
    ros::spin();

    return 0;
}



