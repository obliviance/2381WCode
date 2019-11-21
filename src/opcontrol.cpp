#include "main.h"

//defines the ports for each motor
#define MOTOR1 2 //L1
#define MOTOR2 3 //L2
#define MOTOR3 11 //R1
#define MOTOR4 12 //R2

#define MOTOR5 13 //Angler
#define MOTOR6 4 //Arm
#define MOTOR7 5 //LIntake
#define MOTOR8 14 //RIntake

using namespace pros;

//defines controller
Controller master (CONTROLLER_MASTER);
Mutex mutex;

//defines the ports that are associated with each wheel
Motor left_wheels_1 (MOTOR1, 0); //L1
Motor right_wheels_1 (MOTOR3, 1); //R1
Motor left_wheels_2 (MOTOR2, 1); //L2
Motor right_wheels_2 (MOTOR4, 0); //R2

Motor intake_left (MOTOR7, 0);
Motor intake_right (MOTOR8, 1);
Motor arm (MOTOR6, 1);
Motor angler (MOTOR5, 1);

void drive(void*) {

	//loop reads the joystick controlls and powers the motors accordingly
	while(true) {
		int powerLeft = master.get_analog(ANALOG_LEFT_Y);
		int powerRight = master.get_analog(ANALOG_RIGHT_Y);

		//If there is a small difference, then program will assume person wants to go straight and will set power to motors as equal
		if((powerLeft - powerRight) >= -7 && (powerLeft - powerRight) <= 7)
			powerLeft = powerRight;

		left_wheels_1.move(powerLeft);
		left_wheels_2.move(powerLeft);
		right_wheels_1.move(powerRight);
		right_wheels_2.move(powerRight);
		delay(20);
	}
}

void intake(void*) {

	int intake_power = 100;
	//moves intakes
	while(true) {
	if(master.get_digital(DIGITAL_R2)) {
		intake_left.move(intake_power);
		intake_right.move(intake_power);
	}
	else if(master.get_digital(DIGITAL_L2)) {
		intake_left.move(-intake_power);
		intake_right.move(-intake_power);
	}
	else if(master.get_digital(DIGITAL_B)) {
		intake_left.move(intake_power/3);
		intake_right.move(intake_power/3);
		while (!master.get_digital(DIGITAL_B)) {
	    delay(20);
	  }
	}
	else {
		intake_left.move(0);
		intake_right.move(0);
	}
		delay(20);
	}
}

void arms(void*) {

		int arm_power = 100;

		while(true) {
	//moves arm
	if(master.get_digital(DIGITAL_UP)) {
		arm.move(arm_power);
	}
	else if(master.get_digital(DIGITAL_DOWN)) {
		arm.move(-arm_power);
	}
	else {
		arm.move(0);
	}
		delay(20);
	}


}

void anglerMove(void*) {

	int angler_power = 100;

	while(true) {
	//moves angler
	if(master.get_digital(DIGITAL_R1)) {
		angler.move(angler_power);
	}
	else if(master.get_digital(DIGITAL_L1)) {
		angler.move(-angler_power);
	}
	else {
		angler.move(0);
	}
	delay(20);
	}
}

void towerScore(void*) {

	if(master.get_digital(DIGITAL_A)) {

	mutex.take(TIMEOUT_MAX);

	arm.set_zero_position(0);
	arm.move_absolute(100, 100); //change values later to not damage robot!
	while (!((arm.get_position() < 105) && (arm.get_position() > 95))) {
    delay(20);
  }

	intake_left.move_relative(360, -100); //change values later to not damage robot!
	intake_right.move_relative(360, -100); //change values later to not damage robot!
	while (!((intake_left.get_position() < 365) && (intake_left.get_position() > 355))) {
    delay(20);
  }

	arm.move_absolute(0, -100); //change values later to not damage robot!
	while (!((arm.get_position() < 5) && (arm.get_position() > -5))) {
    delay(20);
  }

	mutex.give();
	}

}

void stacker(void*) {

	if(master.get_digital(DIGITAL_X)) {

	mutex.take(TIMEOUT_MAX);

	angler.set_zero_position(0);
	angler.move_absolute(100, 100); //change values later to not damage robot!
	while (!((angler.get_position() < 105) && (angler.get_position() > 95))) {
		delay(20);
	}

	left_wheels_1.move_relative(45, 50); //change values later to not damage robot!
	left_wheels_2.move_relative(45, 50); //change values later to not damage robot!
	right_wheels_1.move_relative(45, 50); //change values later to not damage robot!
	right_wheels_2.move_relative(45, 50); //change values later to not damage robot!
	while (!((left_wheels_1.get_position() < 50) && (left_wheels_1.get_position() > 40))) {
		delay(20);
	}

	mutex.give();
	}

}

void autonOne(void*){
	while(true){
		if(master.get_digital(DIGITAL_RIGHT)){
			angler.move(50);
			delay(50);
			angler.move(0);
			while(true){
				arm.set_zero_position(0);
				arm.move(80);

				while(!(arm.get_position() < 100 && arm.get_position() > 95)){
					delay(20);
				}

				arm.move(0);
				arm.move(-80);

				while(!(arm.get_position() > -1 && arm.get_position() < 1)){
					delay(20);
				}

				arm.move(0);

			}
		}
	}
}

void opcontrol() {
	Task task1 (drive, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Driving");
	Task task2 (intake, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Intake");
	Task task3 (anglerMove, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Angler");
	Task task4 (arms, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Arms");
	Task task5 (towerScore, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Tower scoring");
	Task task6 (stacker, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Cube stacking");
	Task task7 (autonOne, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Autonomous one");
}
