#include <ros/ros.h>
#include <std_msgs/UInt8MultiArray.h>
#include <std_msgs/Float32.h>
#include <std_msgs/UInt8.h>
#include <my_msgs/can_msg.h>
#include <sensor_msgs/Joy.h>

struct Input {
	struct _axis {
		float left;
		float right;
	} axis;

	struct _button {
		bool left;
		bool right;
	} button;
};


Input in;
bool is_rx = false;
void ctrler_callback(const sensor_msgs::Joy joy_input) {
	in.axis.left = joy_input.axes.at(1);
	in.axis.right = joy_input.axes.at(2);

	in.button.left = joy_input.buttons.at(4);
	in.button.right = joy_input.buttons.at(5);
	is_rx = true;

	
}

int main(int argc, char**argv) {
	ros::init(argc, argv, "joy_process");
	ros::NodeHandle nh;
	ros::Publisher can_tx = nh.advertise<my_msgs::can_msg>("can_tx", 10);
	ros::Subscriber ctrler = nh.subscribe("joy", 10, ctrler_callback);

	my_msgs::can_msg msg;
	msg.id = 0x01;
	msg.data.resize(8);
	while(ros::ok()){
		if(is_rx){
			msg.data.at(0) = (in.axis.left + 1.) * 127;
			msg.data.at(1) = (in.axis.right + 1) * 127;
			msg.data.at(2) = in.button.left;
			msg.data.at(3) = in.button.right;

			ROS_INFO("%d, %d, %d, %d", msg.data.at(0), msg.data.at(1), msg.data.at(2), msg.data.at(3));

			can_tx.publish(msg);

			is_rx = false;
		}
		ros::spinOnce();
	}

}