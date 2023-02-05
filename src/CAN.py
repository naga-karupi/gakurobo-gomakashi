#!/usr/bin/env python3

from copy import copy
import rospy
from my_msgs.msg import can_msg
import can
import os

try:
	os.system('sudo ip link set can0 type can bitrate 500000')
	os.system('sudo ifconfig can0 up')
	can0 = can.interface.Bus(channel = 'can0', bustype = 'socketcan_ctypes')
except OSError:
	rospy.loginfo('error OS')
	exit()


def callback(msg):
	rospy.loginfo(msg)
	can_tx_message = can.Message(arbitration_id=msg.id, data=msg.data, extended_id=False)
	can0.send(can_tx_message)
	
def param_init():
	rospy.init_node('CAN', anonymous=True)

def main():
	param_init()
	sub = rospy.Subscriber('can_tx', can_msg, callback)

	rate = rospy.Rate(500) #control rate is 500Hz

	while not rospy.is_shutdown():		
		rate.sleep()

try:
	main()
except rospy.ROSInterruptException: pass