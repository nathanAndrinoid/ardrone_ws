/*
* ardrone_controller:
*
* This is a basic skeleton program to programmatically control drone movement. 
*
* Author: Nathan Abderson
*
*/


#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <gazebo_msgs/SetModelState.h>
#include "gazebo_msgs/GetModelState.h"
#include "gazebo_msgs/GetPhysicsProperties.h"
#include "std_msgs/Empty.h"
#include <iostream>              /* cout cin */
#include <unistd.h>              /* usleep */
#include <cstring>

using namespace std;

// This struct will create a ardone control object
struct ArDroneController
{
   ros::Rate* r;
   ros::Publisher pub_takeoff, pub_land, pub_vel;
	ros::NodeHandle nh_;
	ros::ServiceClient modelStateCaller;
	
	geometry_msgs::Twist twist;
	geometry_msgs::Pose start_pose;
	geometry_msgs::Twist start_twist;
	gazebo_msgs::GetModelState getmodelstate;

   // Commands for straight movement
   void moveLinear(double x, double y, double z)
   {
      twist.linear.x = x;
      twist.linear.y = y;
      twist.linear.z = z;
      
      pub_vel.publish(twist);
      ros::spinOnce();
      r->sleep();
      /* 0.00001 second wait to wait for transmission to avoid collisions*/
      usleep(10); 
   }

   // Commands for twists and turns
   void moveAngular(double x, double y, double z)
   {
      twist.angular.x = x;
      twist.angular.y = y;
      twist.angular.z = z;
      
      pub_vel.publish(twist);
      ros::spinOnce();
      r->sleep();
      /* 0.00001 second wait to wait for transmission to avoid collisions*/
      usleep(10); 
   }
   
   // Command for landing. I have seen it sometimes fail to work if it is the
   // last thing called before the program terminates. 
   void land()
   {
      pub_land.publish(std_msgs::Empty());
      ros::spinOnce();
      r->sleep();
      /* 0.00001 second wait to wait for transmission to avoid collisions*/
      usleep(10); 
   }
   
   // Command for takeoff
   void takeoff()
   {
      pub_takeoff.publish(std_msgs::Empty());
      ros::spinOnce();
      r->sleep();
      /* 0.00001 second wait to wait for transmission to avoid collisions*/
      usleep(50);  
   }
   
   // this gives exampels on how to get location data in print statements
   void printState()
   {
      modelStateCaller.call(getmodelstate);
      //get just one double location value like this
      cout << "height: "<< getmodelstate.response.pose.position.z<<endl; 
      //print all state information like this
      cout << getmodelstate.response.pose<<endl; 
   }
   
   // Obect contstructor
	ArDroneController()
	{
      twist.linear.x = twist.linear.y = twist.linear.z = 0;
      twist.angular.x = twist.angular.y = twist.angular.z = 0;

      pub_takeoff       = nh_.advertise<std_msgs::Empty>("/ardrone/takeoff",1);
      pub_land          = nh_.advertise<std_msgs::Empty>("/ardrone/land",1);
      pub_vel           = nh_.advertise<geometry_msgs::Twist>("/cmd_vel",1);
      modelStateCaller  = nh_.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
      
      r = new ros::Rate(50);
   }
};


// demo of how to use the functions. 
int main(int argc, char **argv)
{
   ros::init(argc, argv, "ardroneBot");
   
   ArDroneController droneBot;
   droneBot.getmodelstate.request.model_name="quadrotor";
   
   if (!droneBot.nh_.ok())
   {
      cout << "Simulator is not running"<< endl;
      return 0;
   }
   
   // to make sure state handler is ready
   droneBot.modelStateCaller.waitForExistence();
      
   // Stores inputs from user to contol flow of program
   string dummy;
   
   cout << "Enter something to takeoff"<< endl;
   cin >> dummy;
   droneBot.printState();
   
   droneBot.takeoff();
   
   cout << "Enter something to fly up"<< endl;
   cin >> dummy;
   droneBot.printState();

   droneBot.moveLinear(0.0, 0.0, 1.0);
   
   cout << "Enter something to stop"<< endl;
   cin >> dummy;
   droneBot.printState();
   
   droneBot.moveLinear(0.0, 0.0, 0.0);
   
   cout << "Enter something to land"<< endl;
   cin >> dummy;
   droneBot.printState();

   droneBot.land();
   return 0;
}
