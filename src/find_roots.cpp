#include "ros/ros.h"
#include "ros_lab_1/Numbers_roots.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Float32.h"
#include <vector>
#include <string>

ros::ServiceServer service;
ros::Publisher numbers_publisher;
ros::Subscriber roots_subscriber;

bool calc_roots(ros_lab_1::Numbers_roots::Request  &req,
         ros_lab_1::Numbers_roots::Response &res)
{
  if((req.b*req.b - 4*req.a*req.c) > 0)
  {
    double root1;
    double root2;
    root1 = ( -1*req.b + sqrt(req.b*req.b - 4*req.a*req.c) ) / (2 * req.a);
    root2 = ( -1*req.b - sqrt(req.b*req.b - 4*req.a*req.c) ) / (2 * req.a);
    res.roots.push_back(root1);
    res.roots.push_back(root2);
  }
  if ((req.b*req.b - 4*req.a*req.c) == 0)
  {
    float root;
    root = ( -1*req.b )/(2 * req.a);
    res.roots.push_back(root);
   } 
  std_msgs::Float32MultiArray result;
  result.data = res.roots;
  numbers_publisher.publish(result);
  return true;
}

void roots_callback(const std_msgs::Float32MultiArray::ConstPtr& msg)
{
  int elements = 0;
  for (std::vector<float>::const_iterator it = msg-> data.begin();
  it != msg-> data.end(); ++it)
  elements++;
  if (elements == 0)
  {
    ROS_INFO("no roots");
    return;
  }
  if (elements == 1)
  {
    ROS_INFO("x = %f",msg-> data[0]);
    return;
   }
  
    ROS_INFO("x1 = %f,x2 = %f",
    msg-> data[0],
    msg-> data[1]);
    return;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "find_roots");
  ros::NodeHandle n;

  service = n.advertiseService("calculate", calc_roots);
  numbers_publisher = n.advertise<std_msgs::Float32MultiArray>("result", 10);
  roots_subscriber = n.subscribe("result", 10, roots_callback);

  ROS_INFO("Ready to calculate equation.");
  ros::spin();

  return 0;
}

