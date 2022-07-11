#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

ros::ServiceClient client;
void  drive_robot(float lin_x, float ang_z)
{
    ROS_INFO_STREAM("moving the Robot to Target");
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    if(!client.call(srv))
        ROS_ERROR("Failed to call service");

}

void  process_image_callback(const sensor_msgs::Image img) {
    bool white_ball = false;
    int white_pixels = 255;
    int index = 0;
    int y= 0;

    // Looping Through each pixel to check if there is a white one
    int step = 0;
    for ( y = 0; y < img.height && white_ball == false; y++)
    {
        for (step =0; step < img.step && white_ball == false; ++step)
        {
            index = (y * img.step) + step;
            if (img.data[index] == white_pixels)
            {
                white_ball = true;
            }
        }
    }
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    if(white_ball){
        int column = step/3;
        int img_section = img.width/3; // left , middle , right

        // the ball lies on the left side , turn left!

        if (column < img_section){
            drive_robot(0.1 , 0.1);
        }

        //  the ball lies on the middle section, move straight towards the ball
         else if (column < 2 * img_section && column >= img_section){
            drive_robot(0.5 , 0);
        }
        // the ball lies on the right side , turn right!
         else if (column >= 2 * img_section ){
            drive_robot(0.1 , -0.1);
        }
    }

    // No white ball, so stop
    else{
        drive_robot(0 , 0);
    }
}


int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}