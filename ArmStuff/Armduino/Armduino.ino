//Main Arm Code, Ryerson Rams Robotics, URC2017
#include <Arduino.h>
#include <Servo.h>
#include "ros.h"
#include <std_msgs/UInt16.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Bool.h>

#include "Motor.h"
#include "Potentiometer.h"

#include "Joint.h"
#include "Winch.h"
#include "Carousel.h"

//motor pins
#define _j1M 0
#define _j2M 1
#define _j3M 2
#define _gM 3
#define carouselRotateM 4
#define carouselCrankM 5
#define winchM 6
//direction pins
#define _j1D 7
#define _j2D 9
#define _j3D 11
#define _gD 13
#define carouselRotateDir 17
#define carouselCrankDir 19
#define winchDir 21

//Motor declaration
Motor j1M = Motor(_j1M, _j1D);
Motor j2M = Motor(_j2M, _j2D);
Motor j3M = Motor(_j3M, _j3D);
Motor j4M = Motor(_gM, _gD);
Motor carouselRotate = Motor(carouselRotateM, carouselRotateDir);
Motor carouselCrank =  Motor(carouselCrankM, carouselCrankDir);
Motor winchMotor = Motor(winchM, winchDir);

//pot pins
#define _j1Pos A0	
#define _j2Pos A1
#define _j3Pos A2
#define _j4Pos A3
//Potentiometer declaration
Potentiometer j1Pos = Potentiometer(_j1Pos);
Potentiometer j2Pos = Potentiometer(_j2Pos);
Potentiometer j3Pos = Potentiometer(_j3Pos);
Potentiometer j4Pos = Potentiometer(_j4Pos);

//Joint Declaration
Joint joint1 = Joint(j1M,j1Pos);
Joint joint2 = Joint(j2M,j2Pos);
Joint joint3 = Joint(j3M,j3Pos);
Joint joint4 = Joint(j4M,j4Pos);

//Limit Switches
#define _closeSwitch 25
#define _openSwitch 26
#define _indexSwitch 27
//Switch declaration
Switch _close = Switch(_closeSwitch);
Switch _open = Switch(_openSwitch);
Switch _index = Switch(_indexSwitch);

//Carousel declaration
Carousel carousel = Carousel(carouselRotate, carouselCrank, _close, _open, _index);
//Winch declaration
Winch winch = Winch(winchMotor);

//ros node handle
ros::NodeHandle nh;

//Ros Subscribers
void setJoint1Position(const std_msgs::Float32& cmd_msg){
	joint1.setJointPosition(cmd_msg);
}
ros::Subscriber<std_msgs::Float32> _setJoint1Position("setjoint1Position",setJoint1Position);

void setJoint2Position(const std_msgs::Float32& cmd_msg){
	joint2.setJointPosition(cmd_msg);
}
ros::Subscriber<std_msgs::Float32> _setJoint2Position("setjoint2Position",setJoint2Position);

void setJoint3Position(const std_msgs::Float32& cmd_msg){
	joint3.setJointPosition(cmd_msg);
}
ros::Subscriber<std_msgs::Float32> _setJoint3Position("setjoint3Position",setJoint3Position);

void setJoint4Position(const std_msgs::Float32& cmd_msg){
	joint4.setJointPosition(cmd_msg);
}
ros::Subscriber<std_msgs::Float32> _setJoint4Position("setJoint4Position",setJoint4Position);

void spinCarouselTo(const std_msgs::UInt16& cmd_msg){
	carousel.rotate(cmd_msg);
}
ros::Subscriber<std_msgs::UInt16> _spinCarouselTo("spinCarouselTo",spinCarouselTo);

void carouselDoor(const std_msgs::Bool& cmd_msg){
	((cmd_msg.data==true)?carousel.Open():carousel.Close());
}
ros::Subscriber<std_msgs::Bool> _carouselDoor("carouselDoor", carouselDoor);

//Ros Publishers
std_msgs::UInt16 getJoint1Position;
ros::Publisher _getJoint1Position("getJoint1Position", &getJoint1Position);

std_msgs::UInt16 getJoint2Position;
ros::Publisher _getJoint2Position("getJoint2Position", &getJoint2Position);

std_msgs::UInt16 getJoint3Position;
ros::Publisher _getJoint3Position("getJoint3Position", &getJoint3Position);

std_msgs::UInt16 getJoint4Position;
ros::Publisher _getJoint4Position("getJoint4Position", &getJoint4Position);

std_msgs::UInt16 getCarouselPosition;
ros::Publisher _getCarouselPosition("getCarouselPosition", &getCarouselPosition);

void setup(){
	nh.initNode();//initialize the node handle
	initializePublishers();
	initializeSubscribers();
}
void loop(){
	updatePublishers();
	nh.spinOnce();//required
}
void initializePublishers(){
	//publisher initialization
	nh.advertise(_getJoint1Position);
	nh.advertise(_getJoint2Position);
	nh.advertise(_getJoint3Position);
	nh.advertise(_getJoint4Position);
	nh.advertise(_getCarouselPosition);
}
void initializeSubscribers(){
	//subscriber initialization
	nh.subscribe(_setJoint1Position);
	nh.subscribe(_setJoint2Position);
	nh.subscribe(_setJoint3Position);
	nh.subscribe(_setJoint4Position);
	nh.subscribe(_spinCarouselTo);
	nh.subscribe(_carouselDoor);

}
void updatePublishers(){
	//set data
	getJoint1Position.data = joint1.getJointPosition();
	getJoint2Position.data = joint2.getJointPosition();
	getJoint3Position.data = joint3.getJointPosition();
	getJoint4Position.data = joint4.getJointPosition();
	//publish data
	_getJoint1Position.publish(&getJoint1Position);
	_getJoint2Position.publish(&getJoint2Position);
	_getJoint3Position.publish(&getJoint3Position);
	_getJoint4Position.publish(&getJoint4Position);
	_getCarouselPosition.publish(&getCarouselPosition);
}

