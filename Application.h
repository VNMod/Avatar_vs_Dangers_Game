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

    int health;
    int score;
    int scores[5];

    int count;

    int shield_points;
    int difficulty; //ranges from 0-3

    float player_x; //x coordinate for player
    float player_y; //y coordinate for player

    bool B2pressed; //flag to check if boosterpack button S2 has been pressed
    bool position_change; //flag to check if the player's position has changed
    bool game_has_started;

    float delta;

    int no_of_times_played;

    float distance_player_danger;

    bool dead_danger; //when a danger dies, a new danger is true
    bool new_danger; //flag for when a new danger must respawn
    bool new_shield; //flag for when a new shield can appear

    float distance_shield_player;

    float oldpos_x;
    float oldpos_y;

    int sp_x; //x coordinate for shield pickup
    int sp_y; //y coordinate for shield pickup

    int d_x; //x coordinate for dangers
    int d_y; //y coordinate for dangers

    SWTimer splashscreen; //to display splash screen only for a given time
    SWTimer howtoplayscreen; //minimum time for which HOW TO PLAY screen is displayed
    SWTimer highscoresscreen; //minimum time for which GAME SCORES screen is displayed
    SWTimer menuscreen; //minimum time for which the MAIN MENU screen is displayed
    SWTimer gameoverscreen; //minimum time for which the GAME OVER screen is displayed
    SWTimer gamescreen; //minimum time for which the game screen is displayed
    SWTimer shieldtime; //time for which shield is effective
    SWTimer playermove;
    SWTimer newshieldtime;
    SWTimer newdangertime;
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

// Called to display the game over screen
void gameoverscreen(Application* app, HAL* hal);

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

// Called to display player and player related functionalities
void player(Application* app, HAL* hal);

// Called to display and update the values of the player's H, S, P, D meters
void meterdisplay(Application* app, HAL* hal);

// Called to display the static texts in game screen that remain unchanged
void staticobjects(Application* app, HAL* hal);

// Called to display the playable area
void game_window(Application* app, HAL* hal);

// Called to update the player's position corresponding to the changes in the game
void update_player_pos(Application* app, HAL* hal);

// Called to display the shield pick ups at random positions
void shield_pickup(Application* app, HAL* hal);

// Called to display the shield pick ups at random positions
void dangers(Application* app, HAL* hal);

// Called to enable shield
void shield(Application* app, HAL* hal);

// Called to update the player's shield's position corresponding to the changes in the game
void update_shield_pos(Application* app, HAL* hal);

// Called to use the shield to kill a danger
void kill_danger(Application* app, HAL* hal);

// Called to refresh the game stats when a new game is started
void restart_game_stats(Application* app, HAL* hal);

// Called to perform a bubble sort to sort the scores in descending order
void scores_sort(Application* app, HAL* hal);

#endif /* APPLICATION_H_ */
