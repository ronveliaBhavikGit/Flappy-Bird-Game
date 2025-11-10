// game_task.h
#ifndef GAME_TASK_H
#define GAME_TASK_H

#include <stdio.h>
#include "lvgl.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "cyhal.h"
#include "cybsp.h"
#include "../squareline/ui.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT (240 - 12)
#define PILLAR_SPEED 1
#define GRAVITY 1
#define JUMP_STRENGTH -6
#define PILLAR_GAP 120

#define GAME_LOG_ENABLED 1

#if GAME_LOG_ENABLED
    #define GAME_LOG(format, ...) printf("[Game State] " format "\r\n", ##__VA_ARGS__)
#endif

typedef enum {
    GAME_START_SCREEN,
    GAME_RUNNING
} GameState;

void flappy_jump();
void game_task(void *pvParameters);

void show_gameStart_screen();

#endif // GAME_TASK_H