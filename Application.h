/*
 * Application.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <HAL/HAL.h>
#include <HAL/Timer.h>
#include <HAL/Graphics.h>

struct _Application
{
    // Put your application members and FSM state variables here!
    // =========================================================================
    GFX gfx;  //gfx stands for graphics!!!

    unsigned int frameIndex;
    unsigned int frameOffset;

    int SCREENMODE; //Screen Mode
    bool STATIC_SCREEN_MODE; //Static Screen Mode flag
    int menu_options; //Menu Options (1-3)

    SWTimer splashscreen; //to display splash screen only for a given time
    SWTimer howtoplayscreen; //minimum time for which HOW TO PLAY screen is displayed
    SWTimer highscoresscreen; //minimum time for which GAME SCORES screen is displayed
    SWTimer menuscreen; //minimum time for which the MAIN MENU screen is displayed
};
typedef struct _Application Application;

// Called only a single time - inside of main(), where the application is constructed
Application Application_construct();

// Called once per super-loop of the main application.
void Application_loop(Application* app, HAL* hal);

// Called to display the intro/splash screen for the game
void splash_screen(Application* app, HAL* hal);

// Called to display the play game screen and play the game
void playgame_screen(Application* app, HAL* hal);

// Called to display the details for how to play
void howtoplay_screen(Application* app, HAL* hal);

// Called to display the highest game scores on the screen
void highscores_screen(Application* app, HAL* hal);

// Called to invoke a non-blocking test
void nonblocktest(Application* app, HAL* hal);

// Called to cycle between options
void optioncycling(Application* app, HAL* hal);

// Called to display menu screen and its options
void menu_screen(Application* app, HAL* hal);

// Called to return the joystick pushbutton response
bool joystickpush(HAL* hal);

// Called to display the backgrounds of screens 2, 3 and 4
void bbgdisplay(Application* app, HAL* hal);

#endif /* APPLICATION_H_ */
