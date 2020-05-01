/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/* HAL and Application includes */
#include <Application.h>
#include <HAL/HAL.h>
#include <HAL/Timer.h>

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
#define FIXED 1
#define CHANGES 0
#define MINSCREENTIME 500
#define BUFFER_SIZE 5 //buffer size for printf

#define SHIELDTIME 1500 //1.5 seconds for a shield

#define LEFT_THRESHOLD  1000 //1500
#define RIGHT_THRESHOLD 7000 //10000
#define UP_THRESHOLD 7000 //10000
#define DOWN_THRESHOLD 1000 //1500

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

    app.shieldtime = SWTimer_construct(1500); //minimum time to display main menu
    SWTimer_start(&app.shieldtime);

    app.playermove = SWTimer_construct(0.01); //minimum time to display main menu
    SWTimer_start(&app.playermove);

    app.newshieldtime = SWTimer_construct(5000);
    SWTimer_start(&app.newshieldtime);

    app.newdangertime = SWTimer_construct(3000);
    SWTimer_start(&app.newdangertime);

    //Game should begin with splash screen, indicated by a value if 0
    app.SCREENMODE = SPLASHSCREEN;

    //As the game does not begin with the menu screen, this flag is initialized to be 0
    app.STATIC_SCREEN_MODE = 0;

    //This corresponds to any of the three menu options and the pointer starts with the first option, hence 1
    app.menu_options = PLAYGAME;

    app.health = 3; // players start with three health points

    app.score = 0; //players start with a zero (0) score

    app.game_just_started = true;

    app.dead_danger = true; // game starts with a new danger when a danger is killed

    app.difficulty = 0; //difficulty begins at 0

    app.shield_points = 0; //players started with zero (0) shield points

    app.new_shield = true; //game must start with a shield

    app.player_x = 60; //player starts at the center of the screen

    app.player_y = 60; //player starts at the center of the screen

    app.oldpos_x = 60; //player starts at the center of the screen

    app.oldpos_y = 60; //player starts at the center of the screen

    app.sp_x = rand() % 110 + 15; //random position for the x-coordinate of a shield

    app.sp_y = rand() % 95 + 25; //random position for the y-coordinate of a shield

    app.d_x = rand() % 110 + 15; //random position for the x-coordinate of a danger

    app.d_y = rand() % 95 + 25; //random position for the y-coordinate of a danger

    app.B2pressed = false;

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
    shield_pickup(app, hal);
    shield(app, hal);
    dangers(app, hal);

    app->STATIC_SCREEN_MODE = FIXED;
}

void howtoplay_screen(Application* app, HAL* hal)
{
    bbgdisplay(app, hal);

    Graphics_drawString(&app->gfx.context, "How to Play", -1, 0, 0, false);
    Graphics_drawString(&app->gfx.context, "-------------------------------------", -1, 0, 6, false);

    Graphics_drawString(&app->gfx.context, "Avoid all the RED", -1, 5, 15, false);
    Graphics_drawString(&app->gfx.context, "dangers! Collect Blue", -1, 5, 25, false);
    Graphics_drawString(&app->gfx.context, "shield points by", -1, 5, 35, false);
    Graphics_drawString(&app->gfx.context, "running into them.", -1, 5, 45, false);
    Graphics_drawString(&app->gfx.context, "Then, spend shield", -1, 5, 55, false);
    Graphics_drawString(&app->gfx.context, "points by pressing", -1, 5, 65, false);
    Graphics_drawString(&app->gfx.context, "B2 to get a temporary", -1, 5, 75, false);
    Graphics_drawString(&app->gfx.context, "shield which destroys", -1, 5, 85, false);
    Graphics_drawString(&app->gfx.context, "RED dangers. Score by", -1, 5, 95, false);
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

    Graphics_drawString(&app->gfx.context, "Score 1: 0", -1, 30, 25, false);
    Graphics_drawString(&app->gfx.context, "Score 2: 0", -1, 30, 40, false);
    Graphics_drawString(&app->gfx.context, "Score 3: 0", -1, 30, 55, false);
    Graphics_drawString(&app->gfx.context, "Score 4: 0", -1, 30, 70, false);
    Graphics_drawString(&app->gfx.context, "Score 5: 0", -1, 30, 85, false);

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
    if((app->oldpos_x == app->player_x) && (app->oldpos_y == app->player_y))
        Graphics_fillCircle(&app->gfx.context, app->player_x, app->player_y, 3);

    if(isTiltedLeft(hal->joystick) && (app->player_x > PLAYER_X_MIN) && SWTimer_expired(&app->playermove))
    {
        app->oldpos_x = app->player_x;
        app->player_x = app->oldpos_x - 1;
        update_player_pos(app, hal);
    }
    if(isTiltedRight(hal->joystick) && (app->player_x < PLAYER_X_MAX) && SWTimer_expired(&app->playermove))
    {
        SWTimer_start(&app->playermove);
        app->oldpos_x = app->player_x;
        app->player_x = app->oldpos_x + 1;
        update_player_pos(app, hal);
    }
    if(isTiltedUp(hal->joystick) && (app->player_y > PLAYER_Y_MIN) && SWTimer_expired(&app->playermove))
    {
        SWTimer_start(&app->playermove);
        app->oldpos_y = app->player_y;
        app->player_y = app->oldpos_y - 1;
        update_player_pos(app, hal);
    }
    if(isTiltedDown(hal->joystick) && (app->player_y < PLAYER_Y_MAX) && SWTimer_expired(&app->playermove))
    {
        SWTimer_start(&app->playermove);
        app->oldpos_y = app->player_y;
        app->player_y = app->oldpos_y + 1;
        update_player_pos(app, hal);
    }
}

void update_player_pos(Application* app, HAL* hal)
{
    //erases previous player position and draws player in new position
    Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_BLACK);
    Graphics_fillCircle(&app->gfx.context, app->oldpos_x, app->oldpos_y, 5);
    Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
    Graphics_fillCircle(&app->gfx.context, app->player_x, app->player_y, 3);
    if(app->B2pressed == true)
    {
        update_shield_pos(app, hal);
        kill_danger(app, hal);
    }
}

void shield_pickup(Application* app, HAL* hal)
{
    app->distance_shield_player = sqrt(pow(app->player_x - app->sp_x, 2) + pow(app->player_y - app->sp_y, 2));

    if(app->distance_shield_player < 6)
    {
        app->shield_points++;
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_BLACK);
        Graphics_fillCircle(&app->gfx.context, app->sp_x, app->sp_y, 5);
        app->sp_x = rand() % 112 + 10; //x - position of shield pick up in the range of 15 to 115
        app->sp_y = rand() % 90 + 20; //y - position of shield pick up in the range of 25 to 95
        app->new_shield = true;
    }

    if(app->game_just_started == true) //when the game starts, a shield is provided
    {
        app->game_just_started = false;
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_YELLOW);
        Graphics_fillCircle(&app->gfx.context, app->sp_x, app->sp_y, 3);
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
        SWTimer_start(&app->newshieldtime);
    }

    if(SWTimer_expired(&app->newshieldtime) && app->new_shield == true)
    {
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_YELLOW);
        Graphics_fillCircle(&app->gfx.context, app->sp_x, app->sp_y, 3);
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
        SWTimer_start(&app->newshieldtime);
    }
}

void dangers(Application* app, HAL* hal)
{
    app->distance_player_danger = sqrt(pow(app->player_x - app->d_x, 2) + pow(app->player_y - app->d_y, 2)); //distance_player_danger between player and danger (within shield)
    if(app->distance_player_danger < 6) //danger respawns after player is hurt through contact
    {
        app->health--;
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_BLACK);
        Graphics_fillCircle(&app->gfx.context, app->d_x, app->d_y, 5);
        app->d_x = rand() % 112 + 10; //x - position of the danger up in the range of 15 to 115
        app->d_y = rand() % 90 + 20; //y - position of the danger up in the range of 25 to 95
    }
    else if(app->dead_danger == true) //danger respawns after shield kills a danger
    {
        app->dead_danger = false;
        app->score++;
        Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_BLACK);
        Graphics_fillCircle(&app->gfx.context, app->d_x, app->d_y, 5);
        app->d_x = rand() % 112 + 10; //x - position of the danger up in the range of 15 to 115
        app->d_y = rand() % 90 + 20; //y - position of the danger up in the range of 25 to 95
    }

    Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_RED);
    Graphics_fillCircle(&app->gfx.context, app->d_x, app->d_y, 3);
    Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_WHITE);
}

void kill_danger(Application* app, HAL* hal)
{
    app->distance_player_danger = sqrt(pow(app->player_x - app->d_x, 2) + pow(app->player_y - app->d_y, 2)); //distance_player_danger between player and danger (within shield)
    if(app->distance_player_danger < 20) //if red danger is within the shield's field, danger is killed
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
    //erases player's shield at previous position and prints new one
    Graphics_setForegroundColor(&app->gfx.context, GRAPHICS_COLOR_BLACK);
    Graphics_drawCircle(&app->gfx.context, app->oldpos_x, app->oldpos_y, 20);
    Graphics_drawCircle(&app->gfx.context, app->oldpos_x, app->oldpos_y, 21);
    Graphics_drawCircle(&app->gfx.context, app->oldpos_x, app->oldpos_y, 22);
    Graphics_drawCircle(&app->gfx.context, app->oldpos_x, app->oldpos_y, 19);
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
