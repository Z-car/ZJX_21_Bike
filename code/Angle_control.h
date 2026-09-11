/*
 * Angle_control.h
 *
 *  Created on: 2026Äê4ÔÂ1ÈÕ
 *      Author: zjx
 */

#ifndef CODE_ANGLE_CONTROL_H_
#define CODE_ANGLE_CONTROL_H_

float LowPassFilter(float current, float last, float alpha);

float StepApproach(float target, float step_size);

float AngleErrorNormalize(float error);

float StepApproachAngleMode(float target, float real_angle, float step_size, int mode);

#endif /* CODE_ANGLE_CONTROL_H_ */
