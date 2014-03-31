package com.example.alfred_bluetooth_controller;

public class Protocol {
	
	// MODES
	public static byte[] MODE_MOVE = new byte[]{(byte) 0x00};
	public static byte[] MODE_STOP = new byte[]{(byte) 0x01};
	public static byte[] MODE_MOTOR_CONTROL = new byte[]{(byte) 0x02};
	// Followed by : MOTOR_NUM, SPEED
	
	public static byte[] MotorMovement(int motor, int speed){
		return new byte[]{ MODE_MOTOR_CONTROL[0] , (byte) motor, (byte) speed};
	}
}
