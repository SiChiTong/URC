## Simpler ArmControl

The command bytes are as follows:

**Command Byte**|**Action**
:-----:|:-----:
1|Setpoints from tx1 for joints JO-J5
2|Winch direction (1 = Forward, 0 = stop, -1 = reverse)
3|Arm camera tilt
4|Velocities for the arm for joints J0-J5
5|Abort setpoints

## PID & Joint Constants

Note that J0 is the base, and J1 is the big arm, etc ...

**Joint**|**J1**|**J2**|**J3**|**J4**
:-----:|:-----:|:-----:|:-----:|:-----:
Max Angle|0.0|59.0|-164.5|-161.5
Min Angle|0.0|-23.1|-64.0|7.0
Max Pot|0.0|475|730|685
Min Pot|0.0|10|185|165
Kp|0.0|0.0|0.0|0.0
Ki|0.0|0.0|0.0|0.0
Kd|0.0|0.0|0.0|0.0

## Arm Pinout

**Device**|**Pin On Arduino**
:-----:|:-----:
**Potentiometers**
J0|A0
J1|A1
J2|A2
J3|A3
J4|A4
J5|A5
**Motor Controllers**
J0|13
J1|12
J2|11
J3|10
J4|9
J5|8
**Misc**
Camera Servo|7
L298N Enable|6
L298N Forward|5
L298N Reverse|4
