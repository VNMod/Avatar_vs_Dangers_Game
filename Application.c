/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* HAL and Application includes */
#include <Application.h>
#include <HAL/HAL.h>
#include <HAL/Timer.h>

#define UP_BOOST 13000 //medium fast upwards threshold
#define UP_BETTERBOOST 15000 //fast upwards threshold
#define DOWN_BOOST 3000 //medium fast downwards threshold
#define DOWN_BETTERBOOST 1000 //fast downwards threshold
#define LEFT_BOOST 3000 //medium fast left threshold
#define LEFT_BETTERBOOST 1000 //fast upwards threshold
#define RIGHT_BOOST 13000 //medium fast right threshold
#define RIGHT_BETTERBOOST 15000 //fast upwards threshold

#define SUPERFAST 3
#define FAST 2
#define SLOW 1

#define SPLASHSCREEN_TIME 1500 //1.5s for Splash Screen
#define MINIMUM_SCREEN_TIME 500 //minimum time for which a screen is displayed (500ms or 0.5s)
#define SPLASHSCREEN 0
#define MENUSCREEN 1
#define PLAYGAME 1 //Option 1
#define HOWTOPLAY 2 //Option 2
#define HIGHSCORES 3 //Option 3
#define SCREEN1 1 //Menu
#define SCREEN2 2 //Game Screen
#define SCREEN3 3 //How to Play instructions screen
#define SCREEN4 4 //High Scores Screen
#define SCREEN5 5 //Game Over Screen
#define FIXED 1
#define CHANGES 0
#define MINSCREENTIME 500
#define SHIELD_TIME 1500
#define NEW_SHIELD_TIME 5000
#define NEW_DANGER_TIME 3000
#define BUFFER_SIZE 5 //buffer size for printf
#define NO_GAMES_PLAYED 0
#define INVULNERABILITYTIME 6000
#define MID_SCREEN 60

#define SHIELDTIME 1500 //1.5 seconds for a shield
#define DEAD 0
#define MAX_HEALTH 3
#define MIN_SCORE 0
#define MIN_SHIELD_POINTS 0
#define EASY 0
#define NOSCORE 0
#define DEFAULT_SCORE 0
#define NO_COUNT 0
#define MIN_DIST 9 //minimum distance between an icon and avatar where it remains unaffected, be it danger or shield pickup

#define PLAYER_X_MAX 120
#define PLAYER_X_MIN 10
#define PLAYER_Y_MAX 108
#define PLAYER_Y_MIN 18

extern Graphics_Image pacman4BPP_UNCOMP; //splash screen image

/**
 * The main entry point of your project. The main function should immediately
 * stop the Watchdog timer, call the Application constructor, and then
 * repeatedly call the main super-loop function. The Application constructor
 * should be responsible for initializing all hardware components as well as all
 * other finite state machines you choose to use in this project.
 *
 * THIS FUNCTION IS ALREADY COMPLETE. Unless you want to temporarily experiment
 * with some behavior of a code snippet you may have, we DO NOT RECOMMEND
 * modifying this function in any way.
 */
int main(void)
{
    // Stop Watchdog Timer - THIS SHOULD ALWAYS BE THE FIRST LINE OF YOUR MAIN
    WDT_A_holdTimer();

    // Initialize the system clock and background hardware timer, used to enable
    // software timers to time their measurements properly.
    InitSystemTiming();

    // Initialize the main Application object and HAL object
    HAL hal = HAL_construct();
    Application app = Application_construct();

    // Main super-loop! In a polling architecture, this function should call
    // your main FSM function over and over.
    while (true)
    {
        Application_loop(&app, &hal);
        HAL_refresh(&hal);
    }
}

/**
 * The main constructor for your application. This function should initialize
 * each of the FSMs which implement the application logic of your project.
 *
 * @return a completely initialized Application object
 */
Application Application_construct()
{
    Application app;

    app.splashscreen = SWTimer_construct(SPLASHSCREEN_TIME);
    SWTimer_start(&app.splashscreen);

    app.howtoplayscreen = SWTimer_construct(MINSCREENTIME); //minimum time to return back to main menu from this screen
    SWTimer_start(&app.howtoplayscreen);

    app.highscoresscreen = SWTimer_construct(MINSCREENTIME); //minimum time to return back to main menu from this screen
    SWTimer_start(&app.highscoresscreen);

    app.menuscreen = SWTimer_construct(MINSCREENTIME); //minimum time to display main menu
    SWTimer_start(&app.menuscreen);

    app.gamescreen = SWTimer_construct(MINSCREENTIME); //minimum time to display main menu
    SWTimer_start(&app.gamescreen);

    app.gameoverscreen = SWTimer_construct(MINSCREENTIME); //minimum time to display main menu
    SWTimer_start(&app.gameoverscreen);

    app.shieldtime = SWTimer_construct(SHIELD_TIME); //time for which shield is displayed
    SWTimer_start(&app.shieldtime);

    app.playermove = SWTimer_construct(1); //minimum time to display player's movement
    SWTimer_start(&app.playermove);

    app.newshieldtime = SWTimer_construct(NEW_SHIELD_TIME); //a new shield point/icon is spawned every 5 seconds after the avatar's come in contact with one
    SWTimer_start(&app.newshieldtime);

    app.newdangertime = SWTimer_construct(NEW_DANGER_TIME); //a new danger is spawned every 3 seconds after the avatar's death
    SWTimer_start(&app.newdangertime);

    app.invulnerable = SWTimer_construct(6000); //player is invulnerable for 6 seconds after being hurt
    SWTimer_start(&app.invulnerable);

    //Game should begin with splash screen, indicated by a value if 0
    app.SCREENMODE = SPLASHSCREEN;

    //As the game does not begin with the menu screen, this flag is initialized to be 0
    app.STATIC_SCREEN_MODE = false;

    //This corresponds to any of the three menu options and the pointer starts with the first option, hence 1
    app.menu_options = PLAYGAME;

    app.health = 3; // players start with three health points

    app.score = NOSCORE; //players start with a zero (0) score
    app.count = NO_COUNT;

    app.scores[0] = DEFAULT_SCORE;
    app.scores[1] = DEFAULT_SCORE;
    app.scores[2] = DEFAULT_SCORE;
    app.scores[3] = DEFAULT_SCORE;
    app.scores[4] = DEFAULT_SCORE;

    app.a = 5; //a counter variable

    app.delta = SLOW; //by default the speed is slow when a joystick is tilted

    app.game_has_started = true; //it's a flag to determine if a game has started

    app.dead_danger = false; // game starts with a new danger so no danger has been killed yet

    app.new_danger = false;

    app.difficulty = EASY; //difficulty begins at 0

    app.shield_points = MIN_SHIELD_POINTS; //players started with zero (0) shield points

    app.new_shield = true; //game must start with a shield

    app.firstpickupshield = false;

    app.refreshpickupshield = false;

    app.player_x = MID_SCREEN; //player starts at the center of the screen

    app.player_y = MID_SCREEN; //player starts at the center of the screen

    app.oldpos_x = MID_SCREEN; //player starts at the center of the screen

    app.oldpos_y = MID_SCREEN; //player starts at the center of the screen

    app.sp_x = rand() % 110 + 15; //random position for the x-coordinate of a shield

    app.sp_y = rand() % 95 + 25; //random position for the y-coordinate of a shield

    app.d_x = rand() % 110 + 15; //random position for the x-coordinate of a danger

    app.d_y = rand() % 95 + 25; //random position for the y-coordinate of a danger

    app.no_of_times_played = NO_GAMES_PLAYED; //as the number of times played is 0 when the application is launched

    app.B2pressed = false;

    app.just_died = true;

    app.position_change = false; //no change in the player's position at the beginning

    //Default Graphical Construct Settings
    app.gfx = GFX_construct(GRAPHICS_COLOR_WHITE, GRAPHICS_COLOR_BLACK);

    return app;
}

void Application_loop(Application* app, HAL* hal)
{
    nonblocktest(app, hal);

    if(!SWTimer_expired(&app->splashscreen)) //play the intro screen till it expires
        splash_screen(app, hal);

    if((app->SCREENMODE == SCREEN1) && SWTimer_expired(&app->splashscreen)) //if in menu mode and splash screen time has elapsed, menu screen is displayed
        menu_screen(app, hal);

    if(app->SCREENMODE == SCREEN2)
        playgame_screen(app, hal);

    if(app->SCREENMODE == SCREEN3)
        howtoplay_screen(app, hal);

    if(app->SCREENMODE == SCREEN4)
        highscores_screen(app, hal);

    if(app->SCREENMODE == SCREEN5)
        gameoverscreen(app, hal);
}

void splash_screen(Application* app, HAL* hal)
{
    if(app->SCREENMODE == SPLASHSCREEN)
    {
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_RED);
        Graphics_Rectangle ds;
                        ds.xMin = 0;
                        ds.xMax = 128;
                        ds.yMin = 0;
                        ds.yMax = 128;
                        Graphics_drawRectangle(&app->gfx.context, &ds);
                        Graphics_fillRectangle(&app->gfx.context, &ds);
                        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
                        Graphics_setFont(&app->gfx.context, &g_sFontCm14b);
    }
    Graphics_drawImage(&app->gfx.context, &pacman4BPP_UNCOMP, 38, 0);
    Graphics_drawString(&app->gfx.context, "Avatar", -1, 41, 60, false);
    Graphics_drawString(&app->gfx.context, "Vs", -1, 51, 75, false);
    Graphics_drawString(&app->gfx.context, "Dangers", -1, 41, 90, false);
    app->SCREENMODE = SCREEN1; //after Timer Ends (in 1.5 seconds), application moves to menu screen
}

void menu_screen(Application* app, HAL* hal)
{
    bbgdisplay(app, hal);

    Graphics_drawString(&app->gfx.context, "Main Menu:", -1, 0, 0, false);
    Graphics_drawString(&app->gfx.context, "-------------------------------------", -1, 0, 6, false);

    //Displaying the Menu Screen Options

    Graphics_drawString(&app->gfx.context, "Play Game", -1, 30, 50, false);
    Graphics_drawString(&app->gfx.context, "How to Play", -1, 30, 60, false);
    Graphics_drawString(&app->gfx.context, "High Scores", -1, 30, 70, false);

    //Cycling Between The Options with a pointer (>)

    optioncycling(app, hal);

    app->STATIC_SCREEN_MODE = FIXED;

    //Selecting an Option

    if(joystickpush(hal) && SWTimer_expired(&app->menuscreen))
    {
        app->SCREENMODE = (app->menu_options + 1); //As the screen modes for the three options are 2, 3 and 4 respectively
        app->STATIC_SCREEN_MODE = CHANGES;
        SWTimer_start(&app->menuscreen);
    }
}

void playgame_screen(Application* app, HAL* hal)
{
    bbgdisplay(app, hal);

    Graphics_drawString(&app->gfx.context, "Avatar vs. Dangers", -1, 0, 0, false);
    Graphics_drawString(&app->gfx.context, "-------------------------------------", -1, 0, 6, false);

    meterdisplay(app, hal);

    player(app, hal);
    shield(app, hal);
    difficultylevels(app, hal);
    shield_pickup(app, hal);
    danger(app, hal);

    app->STATIC_SCREEN_MODE = FIXED;

    if(app->health == DEAD && app->game_has_started == false && SWTimer_expired(&app->gamescreen))
    {
        app->SCREENMODE = SCREEN5;
        app->STATIC_SCREEN_MODE = CHANGES;
    }
}

void howtoplay_screen(Application* app, HAL* hal)
{
    bbgdisplay(app, hal);

    Graphics_drawString(&app->gfx.context, "How to Play", -1, 0, 0, false);
    Graphics_drawString(&app->gfx.context, "-------------------------------------", -1, 0, 6, false);

    Graphics_drawString(&app->gfx.context, "Avoid all the RED", -1, 5, 15, false);
    Graphics_drawString(&app->gfx.context, "danger! Collect Yellow", -1, 5, 25, false);
    Graphics_drawString(&app->gfx.context, "shield points by", -1, 5, 35, false);
    Graphics_drawString(&app->gfx.context, "running into them.", -1, 5, 45, false);
    Graphics_drawString(&app->gfx.context, "Then, spend shield", -1, 5, 55, false);
    Graphics_drawString(&app->gfx.context, "points by pressing", -1, 5, 65, false);
    Graphics_drawString(&app->gfx.context, "B2 to get a temporary", -1, 5, 75, false);
    Graphics_drawString(&app->gfx.context, "shield which destroys", -1, 5, 85, false);
    Graphics_drawString(&app->gfx.context, "RED danger. Score by", -1, 5, 95, false);
    Graphics_drawString(&app->gfx.context, "destroying REDs.", -1, 5, 105, false);

    Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_YELLOW);
    Graphics_drawString(&app->gfx.context, "MAIN MENU: JS", -1, 20, 117, false);
    Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);

    app->STATIC_SCREEN_MODE = FIXED;

    if(joystickpush(hal) && SWTimer_expired(&app->howtoplayscreen))
    {
        app->SCREENMODE = SCREEN1; //BACK TO MENU SCREEN
        app->STATIC_SCREEN_MODE = CHANGES;
        SWTimer_start(&app->howtoplayscreen);
    }
}

void highscores_screen(Application* app, HAL* hal)
{
    bbgdisplay(app, hal);

    Graphics_drawString(&app->gfx.context, "High Scores", -1, 0, 0, false);
    Graphics_drawString(&app->gfx.context, "-------------------------------------", -1, 0, 6, false);

    Graphics_setFont(&app->gfx.context, &g_sFontCm14b);

    Graphics_drawString(&app->gfx.context, "Score 1: ", -1, 30, 25, false);
    char buffer1[BUFFER_SIZE];
    snprintf(buffer1, BUFFER_SIZE, "%d ", app->scores[0]);
    Graphics_drawString(&app->gfx.context, (int8_t*) buffer1, -1, 80, 25, true);

    Graphics_drawString(&app->gfx.context, "Score 2: ", -1, 30, 40, false);
    char buffer2[BUFFER_SIZE];
    snprintf(buffer2, BUFFER_SIZE, "%d ", app->scores[1]);
    Graphics_drawString(&app->gfx.context, (int8_t*) buffer2, -1, 80, 40, true);

    Graphics_drawString(&app->gfx.context, "Score 3: ", -1, 30, 55, false);
    char buffer3[BUFFER_SIZE];
    snprintf(buffer3, BUFFER_SIZE, "%d ", app->scores[2]);
    Graphics_drawString(&app->gfx.context, (int8_t*) buffer3, -1, 80, 55, true);

    Graphics_drawString(&app->gfx.context, "Score 4: ", -1, 30, 70, false);
    char buffer4[BUFFER_SIZE];
    snprintf(buffer4, BUFFER_SIZE, "%d ", app->scores[3]);
    Graphics_drawString(&app->gfx.context, (int8_t*) buffer4, -1, 80, 70, true);

    Graphics_drawString(&app->gfx.context, "Score 5: ", -1, 30, 85, false);
    char buffer5[BUFFER_SIZE];
    snprintf(buffer5, BUFFER_SIZE, "%d ", app->scores[4]);
    Graphics_drawString(&app->gfx.context, (int8_t*) buffer5, -1, 80, 85, true);


    Graphics_setFont(&app->gfx.context, &g_sFontCm12b);

    Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_YELLOW);
    Graphics_drawString(&app->gfx.context, "MAIN MENU: JS", -1, 20, 115, false);
    Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);

    app->STATIC_SCREEN_MODE = FIXED;

    if(joystickpush(hal) && SWTimer_expired(&app->highscoresscreen))
    {
        app->SCREENMODE = SCREEN1; //BACK TO MENU SCREEN
        app->STATIC_SCREEN_MODE = CHANGES;
        SWTimer_start(&app->highscoresscreen);
    }
}

void scores_sort(Application* app, HAL* hal)
{
    int i = 0, j;

        app->scores[app->count] = app->score;
        app->count++;

    int temp;

    if(app->count > 5)
    {
        for (i = 0; i < app->a; i++)
        for (j = 0; j < app->a-i; j++)
            if (app->scores[j] < app->scores[j+1])
            {
                temp = app->scores[j];
                app->scores[j] = app->scores[j+1];
                app->scores[j+1] = temp;
            }
        app->a++;
    }
    else
    {

        for (i = 0; i < 4; i++)
        for (j = 0; j < 4-i; j++)
            if (app->scores[j] < app->scores[j+1])
            {
                temp = app->scores[j];
                app->scores[j] = app->scores[j+1];
                app->scores[j+1] = temp;
            }
    }

}

void gameoverscreen(Application* app, HAL* hal)
{
    bbgdisplay(app, hal);
                        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
                        Graphics_setFont(&app->gfx.context, &g_sFontCm18b);
                        Graphics_drawString(&app->gfx.context, "GAME OVER", -1, 8, 90, false);
                        Graphics_drawString(&app->gfx.context, "Score: ", -1, 15, 110, false);

                        char buffer[BUFFER_SIZE];
                        snprintf(buffer, BUFFER_SIZE, "%d ", app->score);
                        Graphics_drawString(&app->gfx.context, (int8_t*) buffer, -1, 70, 110, true);

                        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_RED);
                        Graphics_drawLineV(&app->gfx.context, 40, 25, 70);
                        Graphics_drawLineV(&app->gfx.context, 39, 26, 70);
                        Graphics_drawLineV(&app->gfx.context, 90, 25, 70);
                        Graphics_drawLineV(&app->gfx.context, 91, 26, 70);
                        Graphics_drawLine(&app->gfx.context, 40, 70, 90, 70);
                        Graphics_drawLine(&app->gfx.context, 39, 70, 91, 70);

                        if(app->STATIC_SCREEN_MODE != FIXED)
                        {
                            GFX_drawHollowCircle(&app->gfx, 65, 30, 25);
                            GFX_drawHollowCircle(&app->gfx, 65, 30, 24);
                            GFX_drawHollowCircle(&app->gfx, 65, 30, 23);
                        }

                        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_RED);
                        Graphics_Rectangle box;
                        box.xMin = 41;
                        box.xMax = 89;
                        box.yMin = 26;
                        box.yMax = 69;
                        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_BLACK);
                        Graphics_drawRectangle(&app->gfx.context, &box);
                        Graphics_fillRectangle(&app->gfx.context, &box);

                        Graphics_setFont(&app->gfx.context, &g_sFontCm12b);
                        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_ORANGE);
                        Graphics_drawString(&app->gfx.context, "Press", -1, 48, 30, false);
                        Graphics_drawString(&app->gfx.context, "JS FOR", -1, 48, 40, false);
                        Graphics_drawString(&app->gfx.context, "-MENU", -1, 45, 50, false);

                        if(app->STATIC_SCREEN_MODE == CHANGES)
                        {
                            scores_sort(app, hal);
                        }

                        app->STATIC_SCREEN_MODE = FIXED;

                        if(joystickpush(hal) && SWTimer_expired(&app->gameoverscreen))
                        {
                            app->SCREENMODE = SCREEN1; //BACK TO MENU SCREEN
                            app->STATIC_SCREEN_MODE = CHANGES;
                            SWTimer_start(&app->gameoverscreen);
                        }
}


void optioncycling(Application* app, HAL* hal)
{
    if(hal->joystick.isTappedDown == true)
    {
        if(app->menu_options == HIGHSCORES) //implements a cyclic movement between the options of the menu screen
            app->menu_options = PLAYGAME;
        else
            app->menu_options++;
    }

    if(hal->joystick.isTappedUp == true)
    {
        if(app->menu_options == PLAYGAME) //implements a cyclic movement between the options of the menu screen
            app->menu_options = HIGHSCORES;
        else
            app->menu_options--;
    }

    switch(app->menu_options)
    {
    case 1:
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_DARK_ORANGE);
        Graphics_drawString(&app->gfx.context, "--", -1, 15, 60, false);
        Graphics_drawString(&app->gfx.context, "--", -1, 15, 70, false);
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
        Graphics_drawString(&app->gfx.context, "--", -1, 15, 50, false);
        break;
    case 2:
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_DARK_ORANGE);
        Graphics_drawString(&app->gfx.context, "--", -1, 15, 50, false);
        Graphics_drawString(&app->gfx.context, "--", -1, 15, 70, false);
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
        Graphics_drawString(&app->gfx.context, "--", -1, 15, 60, false);
        break;
    case 3:
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_DARK_ORANGE);
        Graphics_drawString(&app->gfx.context, "--", -1, 15, 50, false);
        Graphics_drawString(&app->gfx.context, "--", -1, 15, 60, false);
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
        Graphics_drawString(&app->gfx.context, "--", -1, 15, 70, false);
        break;
    }
}

void nonblocktest(Application* app, HAL* hal)
{
    LED_turnOff(&hal->launchpadLED1);
    if (Button_isPressed(&hal->launchpadS1))
    {
        LED_turnOn(&hal->launchpadLED1);
    }
}

void bbgdisplay(Application* app, HAL* hal)
{
    //So that the static features of the screen are permanent while the rest are dynamically updated
    if(app->STATIC_SCREEN_MODE == CHANGES)
    {
        if(app->SCREENMODE == SCREEN1)
            SWTimer_start(&app->menuscreen);
        else if(app->SCREENMODE == SCREEN4)
            SWTimer_start(&app->highscoresscreen);
        else if(app->SCREENMODE == SCREEN3)
            SWTimer_start(&app->howtoplayscreen);
        else if(app->SCREENMODE == SCREEN5)
            SWTimer_start(&app->gameoverscreen);
        else if(app->SCREENMODE == SCREEN2)
        {
            restart_game_stats(app, hal);
            SWTimer_start(&app->gamescreen);
        }

        if(app->SCREENMODE == SCREEN1)
            Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_DARK_ORANGE);
        else
            Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_BLACK);

        Graphics_Rectangle ds;
                        ds.xMin = 0;
                        ds.xMax = 128;
                        ds.yMin = 0;
                        ds.yMax = 128;
                        Graphics_drawRectangle(&app->gfx.context, &ds);
                        Graphics_fillRectangle(&app->gfx.context, &ds);
                        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
                        Graphics_setFont(&app->gfx.context, &g_sFontCm12b);
    }
}

void restart_game_stats(Application* app, HAL* hal)
{
    SWTimer_start(&app->gamescreen);
    app->no_of_times_played++;
    app->game_has_started = true;
    app->just_died = true;
    app->dead_danger = false;
    app->health = MAX_HEALTH;
    app->oldpos_x = MID_SCREEN;
    app->oldpos_y = MID_SCREEN;
    app->player_x = MID_SCREEN;
    app->player_y = MID_SCREEN;
    app->score = MIN_SCORE;
    app->shield_points = MIN_SHIELD_POINTS;
    app->firstpickupshield = false;
    app->refreshpickupshield = false;
    app->new_danger = false;
}

void meterdisplay(Application* app, HAL* hal)
{
    staticobjects(app, hal);

    Graphics_setFont(&app->gfx.context, &g_sFontCmsc12);
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "%02d ", app->health);
    Graphics_drawString(&app->gfx.context, (int8_t*) buffer, -1, 13, 114, true);
    snprintf(buffer, BUFFER_SIZE, "%02d ", app->score);
    Graphics_drawString(&app->gfx.context, (int8_t*) buffer, -1, 43, 114, true);
    snprintf(buffer, BUFFER_SIZE, "%02d ", app->shield_points);
    Graphics_drawString(&app->gfx.context, (int8_t*) buffer, -1, 73, 114, true);
    snprintf(buffer, BUFFER_SIZE, "%02d ", app->difficulty);
    Graphics_drawString(&app->gfx.context, (int8_t*) buffer, -1, 103, 114, true);
    Graphics_setFont(&app->gfx.context, &g_sFontCm12b);
}

void player(Application* app, HAL* hal)
{
    if(app->B2pressed == true)
    {
        kill_danger(app, hal); //kills danger if within the shield's field of influence
    }

    if((app->oldpos_x == app->player_x) && (app->oldpos_y == app->player_y) || app->game_has_started == true)
    {
        hurt_checker(app, hal);
        Graphics_fillCircle(&app->gfx.context, app->player_x, app->player_y, 3);
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
        if(app->B2pressed == true)
        {
            kill_danger(app, hal); //kills danger if within the shield's field of influence
        }
    }

    if(isTiltedLeft(hal->joystick) && (app->player_x > PLAYER_X_MIN) && SWTimer_expired(&app->playermove))
    {
        if(hal->joystick.vx < LEFT_BETTERBOOST)
        {
            app->delta = SUPERFAST;
        }
        else if(hal->joystick.vx < LEFT_BOOST)
            app->delta = FAST;
        else
            app->delta = SLOW;
        app->oldpos_x = app->player_x;
        app->oldpos_y = app->player_y;
        app->player_x = app->oldpos_x - app->delta;
        update_player_pos(app, hal);
    }
    if(isTiltedRight(hal->joystick) && (app->player_x < PLAYER_X_MAX) && SWTimer_expired(&app->playermove))
    {
        if(hal->joystick.vx > RIGHT_BETTERBOOST)
        {
            app->delta = SUPERFAST;
        }
        else if(hal->joystick.vx > RIGHT_BOOST)
            app->delta = FAST;
        else
            app->delta = SLOW;
        SWTimer_start(&app->playermove);
        app->oldpos_x = app->player_x;
        app->oldpos_y = app->player_y;
        app->player_x = app->oldpos_x + app->delta;
        update_player_pos(app, hal);
    }
    if(isTiltedUp(hal->joystick) && (app->player_y > PLAYER_Y_MIN) && SWTimer_expired(&app->playermove))
    {
        if(hal->joystick.vy > UP_BETTERBOOST)
        {
            app->delta = SUPERFAST;
        }
        else if(hal->joystick.vy > UP_BOOST)
            app->delta = FAST;
        else
            app->delta = SLOW;
        SWTimer_start(&app->playermove);
        app->oldpos_y = app->player_y;
        app->oldpos_x = app->player_x;
        app->player_y = app->oldpos_y - app->delta;
        update_player_pos(app, hal);
    }
    if(isTiltedDown(hal->joystick) && (app->player_y < PLAYER_Y_MAX) && SWTimer_expired(&app->playermove))
    {
        if(hal->joystick.vy < DOWN_BETTERBOOST)
        {
            app->delta = SUPERFAST;
        }
        else if(hal->joystick.vy < DOWN_BOOST)
            app->delta = FAST;
        else
            app->delta = SLOW;
        SWTimer_start(&app->playermove);
        app->oldpos_y = app->player_y;
        app->oldpos_x = app->player_x;
        app->player_y = app->oldpos_y + app->delta;
        update_player_pos(app, hal);
    }

}

void hurt_checker(Application* app, HAL* hal) //BONUS FEATURE
{
    if(app->health < 3)
    {
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_PURPLE);
    }
    else
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
}

void update_player_pos(Application* app, HAL* hal)
{
    //erases previous player position and draws player in new position
    Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_BLACK);
    Graphics_fillCircle(&app->gfx.context, app->oldpos_x, app->oldpos_y, 5);
    hurt_checker(app, hal);
    Graphics_fillCircle(&app->gfx.context, app->player_x, app->player_y, 3);
    Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
    if(app->B2pressed == true)
    {
        kill_danger(app, hal); //kills danger if within the shield's field of influence
        update_shield_pos(app, hal);
    }
}

void shield_pickup(Application* app, HAL* hal)
{
    app->distance_shield_player = sqrt(pow(app->player_x - app->sp_x, 2) + pow(app->player_y - app->sp_y, 2));

    if(app->distance_shield_player < MIN_DIST && app->new_shield == false)
    {
        app->shield_points++;
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_BLACK);
        Graphics_fillCircle(&app->gfx.context, app->sp_x, app->sp_y, 5);
        SWTimer_start(&app->newshieldtime);
        app->new_shield = true;
    }

    if(app->game_has_started == true && app->firstpickupshield == false) //when the game starts, a shield is provided
    {
        app->firstpickupshield = true;
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_YELLOW);
        Graphics_fillCircle(&app->gfx.context, app->sp_x, app->sp_y, 3);
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
        app->new_shield = false;
    }

    if(SWTimer_expired(&app->newshieldtime) && app->new_shield == true)
    {
        app->sp_x = rand() % 112 + 10; //x - position of shield pick up in the range of 10 to 112
        app->sp_y = rand() % 90 + 20; //y - position of shield pick up in the range of 20 to 9
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_YELLOW);
        Graphics_fillCircle(&app->gfx.context, app->sp_x, app->sp_y, 3);
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
        SWTimer_start(&app->newshieldtime);
        app->new_shield = false;
    }

    if(app->refreshpickupshield == true && app->new_shield == false) //if shield runs over pickup it should be unaffected/its state should be refreshed
    {
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_YELLOW);
        Graphics_fillCircle(&app->gfx.context, app->sp_x, app->sp_y, 3);
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
        app->refreshpickupshield = false;
    }
}

void danger(Application* app, HAL* hal)
{
    app->distance_player_danger = sqrt(pow(app->player_x - app->d_x, 2) + pow(app->player_y - app->d_y, 2)); //distance_player_danger between player and danger (within shield)

    if(app->distance_player_danger < MIN_DIST && app->new_danger == false) //danger respawns after player is hurt through contact after a given time
    {
        app->new_danger = true;
        if(app->just_died == true) //with BONUS FEATURE: INVULNERABILITY
        {
            app->health--;
            Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_BLACK);
            Graphics_fillCircle(&app->gfx.context, app->d_x, app->d_y, 5);
            SWTimer_start(&app->newdangertime);
            SWTimer_start(&app->invulnerable);
            app->just_died = false;
        }
        else if(SWTimer_expired(&app->invulnerable)) //with BONUS FEATURE: INVULNERABILITY
        {
            app->health--;
            Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_BLACK);
            Graphics_fillCircle(&app->gfx.context, app->d_x, app->d_y, 5);
            SWTimer_start(&app->newdangertime);
            SWTimer_start(&app->invulnerable);
        }
        else //with BONUS FEATURE: INVULNERABILITY
        {
            Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_BLACK);
            Graphics_fillCircle(&app->gfx.context, app->d_x, app->d_y, 5);
            SWTimer_start(&app->newdangertime);
        }
    }
    else if(app->dead_danger == true) //danger respawns after shield kills a danger
    {
        app->dead_danger = false;
        app->new_danger = true;
        app->score++;
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_BLACK);
        Graphics_fillCircle(&app->gfx.context, app->d_x, app->d_y, 5);
        SWTimer_start(&app->newdangertime);
    }

    if(app->game_has_started == true) //when the game starts, a danger appears
    {
        if(app->firstpickupshield == true)
            app->game_has_started = false;
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_RED);
        Graphics_fillCircle(&app->gfx.context, app->d_x, app->d_y, 3);
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
    }

    if(SWTimer_expired(&app->newdangertime) && app->new_danger == true)
    {
        app->d_x = rand() % 112 + 10; //x - position of the danger up in the range of 10 to 112
        app->d_y = rand() % 90 + 20; //y - position of the danger up in the range of 20 to 90
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_RED);
        Graphics_fillCircle(&app->gfx.context, app->d_x, app->d_y, 3);
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
        app->new_danger = false;
    }

}

void kill_danger(Application* app, HAL* hal)
{
    app->distance_player_danger = sqrt(pow(app->player_x - app->d_x, 2) + pow(app->player_y - app->d_y, 2)); //distance_player_danger between player and danger (within shield)
    if(app->distance_player_danger <= 20 && app->new_danger == false) //if red danger is within the shield's field, danger is killed
    {
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_BLACK);
        Graphics_fillCircle(&app->gfx.context, app->d_x, app->d_y, 5);
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
        app->dead_danger = true;
    }
}

void shield(Application* app, HAL* hal)
{
    if(Button_isTapped(&hal->boosterpackS2) && app->shield_points > 0)
    {
        //starts shield when Boosterpack button S2/B2 is tapped
        SWTimer_start(&app->shieldtime);
        app->B2pressed = true;
        app->shield_points--;
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_GREEN);
        Graphics_drawCircle(&app->gfx.context, app->player_x, app->player_y, 20);
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
    }
    if(SWTimer_expired(&app->shieldtime) && app->B2pressed == true)
    {
        //erases player's shield at the current position and switches off the shield
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_BLACK);
        Graphics_drawCircle(&app->gfx.context, app->player_x, app->player_y, 19);
        Graphics_drawCircle(&app->gfx.context, app->player_x, app->player_y, 20);
        Graphics_drawCircle(&app->gfx.context, app->player_x, app->player_y, 21);
        Graphics_drawCircle(&app->gfx.context, app->player_x, app->player_y, 22);
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
        app->B2pressed = false;
    }
}

void update_shield_pos(Application* app, HAL* hal)
{

    app->refreshpickupshield = true;

    //erases player's shield at previous position and prints new one
    Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_BLACK);
    Graphics_drawCircle(&app->gfx.context, app->oldpos_x, app->oldpos_y, 20);

    Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_GREEN);
    Graphics_drawCircle(&app->gfx.context, app->player_x, app->player_y, 20);
    Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
}

void staticobjects(Application* app, HAL* hal)
{
    Graphics_drawString(&app->gfx.context, "H:", -1, 0, 115, false);
    Graphics_drawString(&app->gfx.context, "S:", -1, 30, 115, false);
    Graphics_drawString(&app->gfx.context, "P:", -1, 60, 115, false);
    Graphics_drawString(&app->gfx.context, "D:", -1, 90, 115, false);
    game_window(app, hal);
}

void game_window(Application* app, HAL* hal)
{
    //GAMING SCREEN
            Graphics_Rectangle R; //coordinates for displaying space solely for Tamagotchi movement
                R.xMin = 3;
                R.xMax = 124;
                R.yMin = 14;
                R.yMax = 113;
            Graphics_drawRectangle(&app->gfx.context, &R);
}

bool joystickpush(HAL* hal)
{
    return (Button_isPressed(&hal->boosterpackJS));
}

void difficultylevels(Application* app, HAL* hal)
{
    if(Button_isTapped(&hal->boosterpackS1))
    {
        if(app->difficulty < 3)
            app->difficulty++;
        else
            app->difficulty = 0;
    }
}


