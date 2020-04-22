/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

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

    app.howtoplayscreen = SWTimer_construct(500); //minimum time to return back to main menu from this screen
    SWTimer_start(&app.howtoplayscreen);

    app.highscoresscreen = SWTimer_construct(500); //minimum time to return back to main menu from this screen
    SWTimer_start(&app.highscoresscreen);

    app.menuscreen = SWTimer_construct(500); //minimum time to display main menu
    SWTimer_start(&app.menuscreen);

    //Game should begin with splash screen, indicated by a value if 0
    app.SCREENMODE = SPLASHSCREEN;

    //As the game does not begin with the menu screen, this flag is initialized to be 0
    app.STATIC_SCREEN_MODE = 0;

    //This corresponds to any of the three menu options and the pointer starts with the first option, hence 1
    app.menu_options = PLAYGAME;

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
    Graphics_drawString(&app->gfx.context, "Avatar", -1, 41, 40, false);
    Graphics_drawString(&app->gfx.context, "Vs", -1, 51, 55, false);
    Graphics_drawString(&app->gfx.context, "Dangers", -1, 41, 70, false);
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

bool joystickpush(HAL* hal)
{
    return (Button_isPressed(&hal->boosterpackJS));
}




