#!/usr/bin/python
import time
import rospy
import serial
import struct
import numpy as np

from sensor_msgs.msg import Joy

class JoyArmSerial:
    PI = 3.14159265359

    def __init__(self,r = 0,i = 0):
        self.serialDev = serial.Serial()
        self.serialDev.port = "/dev/ttyACM0" # SET SERIAL DEVICE
        self.serialDev.open()

        self.velocities = {
            'wrist_roll': 0,
            'wrist_pitch': 0,
            'small_arm': 0,
            'big_arm': 0,
            'base_yaw': 0,
            'grip': 0,
            'winch': 0
        }
        self.positions = {
            'camera': 0
        }

    def write_serial(self):
        MOTOR_VEL_OFFSET = -0.1 # not sure why this is needed, but the motors respond to this value as zero
        for key in ['wrist_roll', 'wrist_pitch', 'small_arm', 'big_arm', 'base_yaw']:
            self.velocities[key] + MOTOR_VEL_OFFSET

        rospy.loginfo('velocities:%s\n' % self.velocities)
        rospy.loginfo('positions:%s\n' % self.positions)

      # Execute arm position
        encoded_position = struct.pack("<ffffffff",
                                        self.velocities['wrist_roll'],
                                        self.velocities['wrist_pitch'],
                                        self.velocities['small_arm'],
                                        self.velocities['big_arm'],
                                        self.velocities['base_yaw'],
                                        self.velocities['grip'],
                                        self.velocities['winch'],
                                        self.positions['camera'],
                                        )
        self.serialDev.write(encoded_position)

    def joy_callback(self, data):
        self.velocities = {
            'wrist_roll': 0,
            'wrist_pitch': 0,
            'small_arm': 0,
            'big_arm': 0,
            'base_yaw': 0,
            'grip': 0,
            'winch': 0,
            'camera': 0
        }

        # button 12 = wrist clockwise
        # button 11 = wrist counter-clockwise
        if data.buttons[8]:
            self.velocities['wrist_roll'] = 0.2
        if data.buttons[10]:
            self.velocities['wrist_roll'] = -0.2

        # +big stick twist = base clockwise
        # -big stick twist = base counter-clockwise
        self.velocities['base_yaw'] = data.axes[2] / 5.0

        # +big stick forward = big arm up
        # -big stick back = big arm down
        self.velocities['big_arm'] = data.axes[1] / 5.0

        # trigger = close gripper
        # thumb button = open gripper
        if data.buttons[0]:
            self.velocities['grip'] = 1
        if data.buttons[1]:
            self.velocities['grip'] = -1

        # thumb stick forward = small arm down
        # thumb stick back = small arm up
        self.velocities['small_arm'] = data.axes[5] / 5.0

        # paddle forward = arm camera up
        # paddle back = arm camera down
        self.positions['camera'] = np.interp(data.axes[3],[-1,1],[0,160]) # this value is angular postion in degrees

        # button 6 = wrist down
        # button 4 = wrist up
        if data.buttons[4]:
            self.velocities['wrist_pitch'] = 0.2
        if data.buttons[2]:
            self.velocities['wrist_pitch'] = -0.2

        # button 7 & 8 = winch
        if data.buttons[6]:
            self.velocities['winch'] = 1
        if data.buttons[7]:
            self.velocities['winch'] = -1
        if data.buttons[6] and data.buttons[7]:
            self.velocities['winch'] = 1337 # special flag to free-wheel

        # MOVE ARM
        self.write_serial()


controller = JoyArmSerial()
rospy.init_node("joystick_teleoperation_arm")
sub = rospy.Subscriber("/joy_arm", Joy, controller.joy_callback)
rospy.spin()
