// game_task.c
#include "game_task.h"
#include <stdlib.h>
#include <time.h>

#define DELAY_PARAM       (5u)
#define DELAY_10_MS       (10)    /* milliseconds */

lv_obj_t* ui_PillarU[3];
lv_obj_t* ui_PillarD[3];

int flappy_y = SCREEN_HEIGHT / 2;
int velocity = 0;
int pillar_x[3];
int score = 0; // Initialize score

int flappyNose_x = 0;
int flappyNose_y = 0;

bool game_over = false; // Track game state
volatile bool startGameRequested = false;

cyhal_gpio_callback_data_t button_callback_data;
GameState gameState = GAME_START_SCREEN;

/**
 * @brief Store Pillar objects in Array
 * 
 */
void store_pillars_in_array()
{
    ui_PillarU[0] = ui_PillarU0;
    ui_PillarU[1] = ui_PillarU1;
    ui_PillarU[2] = ui_PillarU2;

    ui_PillarD[0] = ui_PillarD0;
    ui_PillarD[1] = ui_PillarD1;
    ui_PillarD[2] = ui_PillarD2;
}

/**
 * @brief To Remove Glitches.
 * 
 */
bool refresh_all_objects(void) {
    bool success = true;

    // Example: Check if object is valid (not NULL) before invalidating
    if (ui_gameScreen) lv_obj_invalidate(ui_gameScreen); else success = false;
    if (ui_Background) lv_obj_invalidate(ui_Background); else success = false;
    // if (ui_EmbeddedTea) lv_obj_invalidate(ui_EmbeddedTea); else success = false;
    if (ui_PillarD0) lv_obj_invalidate(ui_PillarD0); else success = false;
    if (ui_PillarU0) lv_obj_invalidate(ui_PillarU0); else success = false;
    if (ui_PillarD1) lv_obj_invalidate(ui_PillarD1); else success = false;
    if (ui_PillarU1) lv_obj_invalidate(ui_PillarU1); else success = false;
    if (ui_PillarD2) lv_obj_invalidate(ui_PillarD2); else success = false;
    if (ui_PillarU2) lv_obj_invalidate(ui_PillarU2); else success = false;
    // if (ui_FlappyWingUp) lv_obj_invalidate(ui_FlappyWingUp); else success = false;
    // if (ui_Flappy) lv_obj_invalidate(ui_Flappy); else success = false;
    if (ui_GameScore) lv_obj_invalidate(ui_GameScore); else success = false;
    if (ui_GameOver) lv_obj_invalidate(ui_GameOver); else success = false;
    // if (ui_FlappyGameTitle) lv_obj_invalidate(ui_FlappyGameTitle); else success = false;
    // if (ui_Brightness) lv_obj_invalidate(ui_Brightness); else success = false;

    return success;
}

/**
 * @brief Initializes Flappy Bird's position at the start of the game.
 */
void flappy_initial_position()
{
    velocity = 0; // Reset vertical velocity
    lv_obj_add_flag(ui_GameOver, LV_OBJ_FLAG_HIDDEN);    
    lv_label_set_text(ui_GameScore, "0");

    if(lv_obj_get_x(ui_Flappy) + lv_obj_get_width (ui_Flappy) <= SCREEN_WIDTH/4 )
    {
        lv_obj_set_x(ui_Flappy, SCREEN_WIDTH/4);
        lv_obj_set_x(ui_FlappyWingUp, SCREEN_WIDTH/4);
    }
    lv_obj_set_y(ui_Flappy, flappy_y);
    lv_obj_set_y(ui_FlappyWingUp, flappy_y);

    GAME_LOG("Flappy initialized at X: %d, Y: %d", SCREEN_WIDTH / 4, flappy_y);
}

/**
 * @brief Generate Pillar initial position
 * 
 */
void generate_pillars_initial_position()
{
    for (int i = 0; i < 3; i++) {
        pillar_x[i] = SCREEN_WIDTH + (i * PILLAR_GAP);
        lv_obj_set_x(ui_PillarU[i], pillar_x[i]);
        lv_obj_set_x(ui_PillarD[i], pillar_x[i]);
    }
}

/**
 * @brief Hides the game start screen and prepares for gameplay.
 */
void hide_gameStart_screen()
{
    // if (!refresh_all_objects()) {
    //     GAME_LOG("refresh_all_objects failed — some objects are NULL!");
    //     refresh_all_objects();
    // }

    GAME_LOG("Hiding game start screen.");
    lv_obj_move_background(ui_FlappyGameTitle);
    lv_obj_invalidate(ui_Background);

    // Show Gameplay (clear hidden flag for pillars)
    for (int i = 0; i < 3; i++) {
        lv_obj_clear_flag(ui_PillarU[i], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_PillarD[i], LV_OBJ_FLAG_HIDDEN);
    }
    
    lv_obj_clear_flag(ui_GameScore, LV_OBJ_FLAG_HIDDEN);

    flappy_initial_position();
    generate_pillars_initial_position();

    if (!refresh_all_objects()) {
        GAME_LOG("refresh_all_objects failed — some objects are NULL!");
        refresh_all_objects();
    }
}

/**
 * @brief Shows the game start screen and prepares for gameplay.
 */
void show_gameStart_screen()
{
    game_over = true;
    if (!refresh_all_objects()) {
        GAME_LOG("refresh_all_objects failed — some objects are NULL!");
        refresh_all_objects();
    }

    score = 0;
    GAME_LOG("Showing game start screen.");
    
    // Show Gameplay (clear hidden flag for pillars)
    for (int i = 0; i < 3; i++) {
        lv_obj_add_flag(ui_PillarU[i], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_PillarD[i], LV_OBJ_FLAG_HIDDEN);
    }
    
    lv_obj_add_flag(ui_GameScore, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(ui_FlappyGameTitle);
    lv_obj_invalidate(ui_FlappyGameTitle);

    lv_obj_set_x(ui_Flappy, 10);
    lv_obj_set_x(ui_FlappyWingUp, 10);
    
    lv_obj_set_y(ui_Flappy, SCREEN_HEIGHT / 2);
    lv_obj_set_y(ui_FlappyWingUp, SCREEN_HEIGHT / 2);

    if (!refresh_all_objects()) {
        GAME_LOG("refresh_all_objects failed — some objects are NULL!");
        refresh_all_objects();
    }
    game_over = false;
}

/**
 * @brief Updates pillar positions to simulate movement.
 */
void update_pillars() {
    for (int i = 0; i < 3; i++) {
        // Move pillar left
        pillar_x[i] -= PILLAR_SPEED;

        // Reset pillar position when it goes off-screen
        if (pillar_x[i] < -25) {
            pillar_x[i] = SCREEN_WIDTH;
        }

        // Update pillar positions
        lv_obj_set_x(ui_PillarU[i], pillar_x[i]);
        lv_obj_set_x(ui_PillarD[i], pillar_x[i]);
    }
}

/**
 * @brief Applies gravity and updates Flappy's position.
 */
void update_flappy() {
    velocity += GRAVITY;
    flappy_y += velocity;

    if (flappy_y < 0) flappy_y = 0;
    if (flappy_y > SCREEN_HEIGHT - 20) flappy_y = SCREEN_HEIGHT - 20;
    
    lv_obj_set_y(ui_Flappy, flappy_y);
    lv_obj_set_y(ui_FlappyWingUp, flappy_y);
}

/**
 * @brief Wing Position Change
 * 
 * @param timer 
 */
static void reset_wing_animation(lv_timer_t * timer) {
    lv_obj_clear_flag(ui_Flappy, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_FlappyWingUp, LV_OBJ_FLAG_HIDDEN);
}

void flappy_jump() {
    velocity = JUMP_STRENGTH;
    lv_obj_clear_flag(ui_FlappyWingUp, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Flappy, LV_OBJ_FLAG_HIDDEN);
    
    // Set a timer to revert after 100ms (adjust as needed)
    lv_timer_create(reset_wing_animation, 200, NULL);
}

/**
 * @brief Handles button press for game state transitions and jumping.
 */
void button_isr_handler(void *callback_arg, cyhal_gpio_event_t event) {
    (void)callback_arg; // Unused parameter

    if (event == CYHAL_GPIO_IRQ_FALL) 
    {
        switch (gameState)
        {
        case GAME_START_SCREEN:
            startGameRequested = true;
            break;

        case GAME_RUNNING:
            flappy_jump(); // Normal jump action
            GAME_LOG("Button pressed: Flappy jumps.");
            break;

        default:
            break;
        }
    }
}

/**
 * @brief Game Over Function
 * 
 */
void game_over_fun()
{
    game_over = true;
    if (!refresh_all_objects()) {
        GAME_LOG("refresh_all_objects failed — some objects are NULL!");
        refresh_all_objects();
    }
    
    lv_obj_clear_flag(ui_GameOver, LV_OBJ_FLAG_HIDDEN); // Show Game Over UI
    GAME_LOG("Game Over !!!");
    
    vTaskDelay(1000); // Delay to show "Game Over" message (optional)
    
    // Restart the game immediately
    score = 0;
    game_over = false;
    gameState = GAME_RUNNING;

    lv_label_set_text(ui_GameScore, "0"); // Reset score UI
    lv_obj_add_flag(ui_GameOver, LV_OBJ_FLAG_HIDDEN); // Show Game Over UI

    flappy_initial_position(); // Reset flappy position
    generate_pillars_initial_position(); // Reset pillars

    if (!refresh_all_objects()) {
        GAME_LOG("refresh_all_objects failed — some objects are NULL!");
        refresh_all_objects();
    }
    game_over = false;
}

/**
 * @brief Check Flappy Collision with Pillars
 * 
 */
void collision_check() {
    int flappyNose_x = lv_obj_get_x(ui_Flappy) + lv_obj_get_width(ui_Flappy);
    int flappyNose_y = lv_obj_get_y(ui_Flappy) + (lv_obj_get_height(ui_Flappy) / 2);
    
    static bool passedPillar[3] = {false, false, false}; // Track if Flappy passed a pillar

    for (int i = 0; i < 3; i++) {
        int pillar_x = lv_obj_get_x(ui_PillarU[i]);
        int pillarU_height = lv_obj_get_y(ui_PillarU[i]) + lv_obj_get_height(ui_PillarU[i]);
        int pillarD_y = lv_obj_get_y(ui_PillarD[i]);
        
        // **Collision Check**
        if (flappyNose_x >= pillar_x && flappyNose_x <= (pillar_x + lv_obj_get_width(ui_PillarU[i]))) {
            if (flappyNose_y <= pillarU_height || flappyNose_y >= pillarD_y) {
                game_over_fun();
                return;
            }
        }
        
        // **Score Update: If Flappy passes a pillar**
        if (flappyNose_x > (pillar_x + lv_obj_get_width(ui_PillarU[i])) && !passedPillar[i]) {
            score++; // Increment score
            passedPillar[i] = true; // Mark as passed
            
            // Convert score to string and update UI
            char scoreText[12];
            snprintf(scoreText, sizeof(scoreText), "%d", score);
            lv_label_set_text(ui_GameScore, scoreText);
        }
        
        // **Fix: Reset when pillar moves to the right again**
        if (pillar_x >= SCREEN_WIDTH - lv_obj_get_width(ui_PillarU[i])) { // When pillar resets to the right
            passedPillar[i] = false;
        }
    }
}

/**
 * @brief Initializes button input with an interrupt.
 */
void button_game_event() {
    srand(time(NULL));

    // Initialize USER_BTN2 as an input with interrupt
    GAME_LOG("Initializing button.");
    cyhal_gpio_init(CYBSP_USER_BTN2, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, true);
    button_callback_data.callback = button_isr_handler;
    cyhal_gpio_register_callback(CYBSP_USER_BTN2, &button_callback_data);
    cyhal_gpio_enable_event(CYBSP_USER_BTN2, CYHAL_GPIO_IRQ_FALL, CYHAL_ISR_PRIORITY_DEFAULT, true);
}

/**
 * @brief Main game loop, updates game state at regular intervals.
 */
void game_task(void *pvParameters) {
    GAME_LOG("Game task started.");
    ui_init();
    button_game_event();
    
    store_pillars_in_array();
    show_gameStart_screen(); // Start with the title screen

    while (1) {
        if (startGameRequested) {
            startGameRequested = false;
            hide_gameStart_screen();
            GAME_LOG("Button pressed: Starting game.");
            gameState = GAME_RUNNING;
            game_over = false;
        }
    
        if (gameState == GAME_RUNNING && !game_over) {
            collision_check();
            update_pillars();
            update_flappy();
        }
        
        lv_task_handler();
        lv_tick_inc(DELAY_PARAM);
        vTaskDelay(DELAY_PARAM);
    }
}
