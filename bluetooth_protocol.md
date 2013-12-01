(Very early draft)

ARDUINO
=========================

The arduino operates in two states, moving and dispensing.

Regardless of state, the arduino sends a packet every Xms to the android device, the packet looks like...

    "[Motor State],[Dispensing Sensor State]\n"

...where [Motor State] and [Dispensing Sensor State] are integers representing the
motion of the motors and the reading of the dispensing sensor (a placeholder for now).

Motor State integers correspond to the following states...

0   =  ROTATE_RIGHT 
1   =  ROTATE_LEFT 
2   =  ROTATE_STOP 
3   =  RIGHT 
4   =  LEFT 
5   =  FORWARD 
6   =  BACK 
7   =  STOP 

So if the motor was going forward and the dispensing sensor read 117, the arduino should send...

    "5,117\n"


ANDROID
=========================

The android device controls the arduino's state (dispensing or moving)

The device sends a message whenever it wants the arduino to change state.

It sends "0\n" for the moving state, and "1\n" for the dispensing state.

The arduino only cares about the last message sent.