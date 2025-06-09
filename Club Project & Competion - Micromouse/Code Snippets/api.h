#ifndef INC_API_H_
#define INC_API_H_

#include "stm32f4xx_hal.h"

#include <stdio.h>

#include "sensors.h"
#include "pid.h"
#include "trapezoid.h"
#include "elapsed.h"
#include "vector.h"
#include "wallFollow.h"
#include "eventLoop.h"


static const double KP_VELOCITY = 0.008;
static const double KI_VELOCITY = 0.008;
static const double KD_VELOCITY = 0.1;

//static const double KP_VELOCITY = 0.004;
//static const double KI_VELOCITY = 0.01;
//static const double KD_VELOCITY = 0.1;

static vec2_f32_t prevError = {0, 0};
static vec2_f32_t integral = {0, 0};
static vec2_f32_t adjust = {0, 0};

uint32_t prevTicks = 0;
vec2_f32_t setpoint = {0, 0};

enum MANEUVERS {FORWARD, LEFT, RIGHT};

static int maneuverCount = 0;
static int maneuverQueue[256];

void ResetControlLoops() {
	setpoint.left = 0;
	setpoint.right = 0;

	prevError.left = 0;
	prevError.right = 0;

	integral.left = 0;
	integral.right = 0;

	adjust.left = 0;
	adjust.right = 0;

	prevTicks = GetTicks();
	ResetOdometer();
}

void API_turnRight() {
	if (mode == FAST_MODE) {
		maneuverQueue[maneuverCount++] = RIGHT;
	} else {

		ResetControlLoops();

		do {
//			if (mode == IDLE) {
//				SetMotorSpeedLeft(0.0);
//				SetMotorSpeedRight(0.0);
//				break;
//			}
			vec2_f32_t o = GetOdometer();
			setpoint = Trapezoidal(-90*MM_PER_DEGREE*0.9889, o, 1);
			uint32_t ticks = GetTicks();
			float dt = ElapsedTime(prevTicks, ticks);
			prevError = PID(GetVelocity(), dt, setpoint, prevError, &integral, KP_VELOCITY, KI_VELOCITY, KD_VELOCITY, adjust);
			prevTicks = ticks;
		} while (setpoint.left != 0 || setpoint.right != 0);

		ResetControlLoops();

	}
}

void API_turnLeft() {
	if (mode == FAST_MODE) {
		maneuverQueue[maneuverCount++] = LEFT;
	} else {

		ResetControlLoops();

		do {
//			if (mode == IDLE) {
//				SetMotorSpeedLeft(0.0);
//				SetMotorSpeedRight(0.0);
//				break;
//			}
			setpoint = Trapezoidal(90*MM_PER_DEGREE*0.9889, GetOdometer(), 1);
			uint32_t ticks = GetTicks();
			float dt = ElapsedTime(prevTicks, ticks);
			prevError = PID(GetVelocity(), dt, setpoint, prevError, &integral, KP_VELOCITY, KI_VELOCITY, KD_VELOCITY, adjust);
			prevTicks = ticks;
		} while (setpoint.left != 0 || setpoint.right != 0);

		ResetControlLoops();

	}
}

void API_Stop() {
	ResetControlLoops();
	SetMotorSpeedLeft(0.0);
	SetMotorSpeedRight(0.0);
}

void API_moveForward() {

	if (mode == FAST_MODE) {
		maneuverQueue[maneuverCount++] = FORWARD;
	} else {

		ResetControlLoops();

		do {
//			if (mode == IDLE) {
//				SetMotorSpeedLeft(0.0);
//				SetMotorSpeedRight(0.0);
//				break;
//			}
			setpoint = Trapezoidal(180, GetOdometer(), 0);
			adjust = WallFollow(GetVelocity());
			uint32_t ticks = GetTicks();
			float dt = ElapsedTime(prevTicks, ticks);
			prevError = PID(GetVelocity(), dt, setpoint, prevError, &integral, KP_VELOCITY, KI_VELOCITY, KD_VELOCITY, adjust);
			prevTicks = ticks;
		} while (setpoint.left != 0 || setpoint.right != 0);

		ResetControlLoops();

	}
}

void API_moveForward_N(int _n) {

	if (mode == FAST_MODE) {
		maneuverQueue[maneuverCount++] = FORWARD;
	} else {

		ResetControlLoops();

		do {
//			if (mode == IDLE) {
//				SetMotorSpeedLeft(0.0);
//				SetMotorSpeedRight(0.0);
//				break;
//			}
			setpoint = Trapezoidal(180.0*_n, GetOdometer(), 0);
			adjust = WallFollow(GetVelocity());
			uint32_t ticks = GetTicks();
			float dt = ElapsedTime(prevTicks, ticks);
			prevError = PID(GetVelocity(), dt, setpoint, prevError, &integral, KP_VELOCITY, KI_VELOCITY, KD_VELOCITY, adjust);
			prevTicks = ticks;
		} while (setpoint.left != 0 || setpoint.right != 0);

		ResetControlLoops();

	}
}

void API_moveForward_Test() {

	ResetControlLoops();

	do {
		setpoint = Trapezoidal(300, GetOdometer(), 0);
		uint32_t ticks = GetTicks();
		float dt = ElapsedTime(prevTicks, ticks);
		prevError = PID(GetVelocity(), dt, setpoint, prevError, &integral, KP_VELOCITY, KI_VELOCITY, KD_VELOCITY, (vec2_f32_t){0, 0});
		prevTicks = ticks;
	} while (setpoint.left != 0 || setpoint.right != 0);

	vec2_f32_t o = GetOdometer();
	//printf("Hello world\n")
	printf("Odometer: %.2f, %.2f\n", o.left, o.right);

	ResetControlLoops();
}

int API_wallFront() {
	return GetIRDistances().middle < 100;
}

int API_wallRight() {
	return GetIRDistances().right < 100;
}

int API_wallLeft() {
	return GetIRDistances().left < 100;
}

void API_Queue() {
	for (int i = 0; i < maneuverCount; i++) {
		if (maneuverQueue[i] == FORWARD) {
			int numForward = 1;
			int j;
			for (j = 1; j < 16; j++) {
				if ((i+1) >= maneuverCount)
					break;
				else if (maneuverQueue[i+j] != FORWARD)
					numForward = j;
			}
			i += j - 1;
			API_moveForward_N(numForward);
		} else if (maneuverQueue[i] == LEFT) {
			API_turnLeft();
		} else if (maneuverQueue[i] == FORWARD) {
			API_turnRight();
		}
	}
	API_Stop();
}

#endif /* INC_API_H_ */
