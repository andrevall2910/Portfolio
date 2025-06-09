#ifndef INC_TRAPEZOID_H_
#define INC_TRAPEZOID_H_

#include "vector.h"
#include "motors.h"

// 1. CONSTANTS

// Trapezoidal velocity profile
static const double ACCELERATION_DISTANCE = 60; //[mm]
static const double TARGET_DISTANCE_THRESHOLD = 1; // [mm]


// 2. FUNCTION DEFINITIONS

vec2_f32_t Trapezoidal(float _targetDistance, vec2_f32_t _distanceTraveled, uint8_t _turn) {
	vec2_f32_t velocitySetpoint;

	float turnMul;
	if (_turn)
		turnMul = -1.0;
	else
		turnMul = 1;

	if (_targetDistance < 0) {
		_targetDistance = -1*_targetDistance;

		double averageDistance = 0.5*(_distanceTraveled.left + turnMul*_distanceTraveled.right);
		double accelerationDistance = ACCELERATION_DISTANCE;
		if (0.5*_targetDistance < accelerationDistance) // If the acceleration+deceleration distance is greater than target distance
			accelerationDistance = 0.5*_targetDistance;

		if (averageDistance <= _targetDistance+TARGET_DISTANCE_THRESHOLD && averageDistance >= _targetDistance-TARGET_DISTANCE_THRESHOLD) {
			velocitySetpoint.left = 0;
			velocitySetpoint.right = 0;
		} else if (_targetDistance - averageDistance <= accelerationDistance && _targetDistance - averageDistance > 0) { // Deceleration phase
			double velocity = (_targetDistance - averageDistance)/ACCELERATION_DISTANCE*MAX_SPEED;
			if (velocity < MIN_SPEED)
				velocity = MIN_SPEED;
			velocitySetpoint.left = velocity;
			velocitySetpoint.right = velocity*turnMul;
		} else if(averageDistance <= accelerationDistance) { // Acceleration phase
			double velocity = averageDistance/ACCELERATION_DISTANCE*MAX_SPEED;
			if (velocity < MIN_SPEED)
				velocity = MIN_SPEED;
			velocitySetpoint.left = velocity;
			velocitySetpoint.right = velocity*turnMul;
		} else if (averageDistance > _targetDistance+TARGET_DISTANCE_THRESHOLD) { // Past target distance
			velocitySetpoint.left = -MIN_SPEED;
			velocitySetpoint.right = -MIN_SPEED*turnMul;
		} else if(averageDistance > accelerationDistance && averageDistance < _targetDistance) {
			velocitySetpoint.left = MAX_SPEED;
			velocitySetpoint.right = MAX_SPEED;
		}
	} else {
		double averageDistance = 0.5*(turnMul*_distanceTraveled.left + _distanceTraveled.right);
		double accelerationDistance = ACCELERATION_DISTANCE;
		if (0.5*_targetDistance < accelerationDistance)
			accelerationDistance = 0.5*_targetDistance;

		if (averageDistance <= _targetDistance+TARGET_DISTANCE_THRESHOLD && averageDistance >= _targetDistance-TARGET_DISTANCE_THRESHOLD) {
			velocitySetpoint.left = 0;
			velocitySetpoint.right = 0;
		} else if (_targetDistance - averageDistance <= accelerationDistance && _targetDistance - averageDistance > 0) {
			double velocity = (_targetDistance - averageDistance)/ACCELERATION_DISTANCE*MAX_SPEED;
			if (velocity < MIN_SPEED)
				velocity = MIN_SPEED;
			velocitySetpoint.left = velocity * turnMul;
			velocitySetpoint.right = velocity;
		} else if(averageDistance <= accelerationDistance) {
			double velocity = averageDistance/ACCELERATION_DISTANCE*MAX_SPEED;
			if (velocity < MIN_SPEED)
				velocity = MIN_SPEED;
			velocitySetpoint.left = velocity * turnMul;
			velocitySetpoint.right = velocity;
		} else if (averageDistance > _targetDistance+TARGET_DISTANCE_THRESHOLD) {
			velocitySetpoint.left = -MIN_SPEED * turnMul;
			velocitySetpoint.right = -MIN_SPEED;
		} else if(averageDistance > accelerationDistance && averageDistance < _targetDistance) {
			velocitySetpoint.left = MAX_SPEED;
			velocitySetpoint.right = MAX_SPEED;
		}
	}

//	double averageDistance = 0.5*(turnMul*_distanceTraveled.left + _distanceTraveled.right);
//	double accelerationDistance = ACCELERATION_DISTANCE;
//	if (0.5*_targetDistance < accelerationDistance)
//		accelerationDistance = 0.5*_targetDistance;
//
//	if (averageDistance <= _targetDistance+TARGET_DISTANCE_THRESHOLD && averageDistance >= _targetDistance-TARGET_DISTANCE_THRESHOLD) {
//		velocitySetpoint.left = 0;
//		velocitySetpoint.right = 0;
//	} else if (_targetDistance - averageDistance <= accelerationDistance && _targetDistance - averageDistance > 0) { // Deceleration phase
//		double velocity = (_targetDistance - averageDistance)/ACCELERATION_DISTANCE*MAX_SPEED;
//		if (velocity < MIN_SPEED)
//			velocity = MIN_SPEED;
//		velocitySetpoint.left = velocity * turnMul;
//		velocitySetpoint.right = velocity;
//	} else if(averageDistance <= accelerationDistance) { // Acceleration phase
//		double velocity = averageDistance/ACCELERATION_DISTANCE*MAX_SPEED;
//		if (velocity < MIN_SPEED)
//			velocity = MIN_SPEED;
//		velocitySetpoint.left = velocity * turnMul;
//		velocitySetpoint.right = velocity;
//	} else if (averageDistance > _targetDistance+TARGET_DISTANCE_THRESHOLD) { // Passed target
//		velocitySetpoint.left = -MIN_SPEED * turnMul;
//		velocitySetpoint.right = -MIN_SPEED;
//	} else if(averageDistance > accelerationDistance && averageDistance < _targetDistance) { // Constant velocity
//		velocitySetpoint.left = MAX_SPEED;
//		velocitySetpoint.right = MAX_SPEED;
//	}


	return velocitySetpoint;
}

#endif /* INC_TRAPEZOID_H_ */
