/*
 * JoystickL.h
 *
 *  Created on: Oct 18, 2018
 *      Author: Leyla
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

struct _Joystick
{
    unsigned int vx;

    unsigned int vy;

    unsigned int vx_old;
    unsigned int vy_old;

    // The outputs of the joystick

    bool isTappedRight;
    bool isTappedLeft;
    bool isTappedUp; //important for menu
    bool isTappedDown; //important for menu


};
typedef struct _Joystick Joystick;

Joystick Joystick_construct(); //initializes Joystick

void getSampleJoystick(unsigned *X, unsigned *Y);
void refreshJoystick(Joystick *joystick_p);
bool isTiltedLeft(Joystick joystick);
bool isTiltedRight(Joystick joystick);
bool isTiltedUp(Joystick joystick);
bool isTiltedDown(Joystick joystick);

#endif /* JOYSTICK_H_ */
