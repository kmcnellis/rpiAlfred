
#define MODE_MOVE 0
#define MODE_STOP 1
#define MODE_MOTOR_CONTROL 2

void motor_check_serial();

byte motor_commands[100];
int unread_commands = 0;

bool motor_update(){
	motor_check_serial();
	return unread_commands != 0;
}

byte motor_read(){
	byte result = motor_commands[0];
	for (int i = 1;i < unread_commands;i++){
		motor_commands[i-1] = motor_commands[i];
	}
	unread_commands --;
	return result;
}



void motor_check_serial(){
    if (Serial1.available()){

        byte c = Serial1.read();

        if (c == MODE_MOVE || c == MODE_STOP){
            motor_commands[unread_commands] = c;
           	unread_commands ++;
        }else if (c == MODE_MOTOR_CONTROL){
            while (!Serial1.available()){
            }
            byte c2 = Serial1.read();
            while (!Serial1.available()){
            }
            byte c3 = Serial1.read();

            motor_commands[unread_commands] = c;
            motor_commands[unread_commands+1] = c2;
            motor_commands[unread_commands+2] = c3;

            unread_commands += 3;

        }
        
    }
}