#ifndef MAIN_H
#define MAIN_H

#include "global.h"

/** File signature */
#define SIGNATURE 'MCZ3'
/** Maximum message speed. */
#define MAX_MSG_SPEED 3
/** Number of save slots */
#define NUM_SAVE_SLOTS 3
/** Maximum brightness. */
#define MAX_BRIGHTNESS 3

/** Supported game languages. */
typedef enum {
    LANGUAGE_JP,
    LANGUAGE_EN,
    LANGUAGE_FR,
    LANGUAGE_DE,
    LANGUAGE_ES,
    LANGUAGE_IT,
    NUM_LANGUAGES,
} Language;

#ifdef ENGLISH
#define GAME_LANGUAGE LANGUAGE_EN
#else
#define GAME_LANGUAGE LANGUAGE_JP
#endif

/** Program tasks. */
typedef enum {
    TASK_TITLE,       /**< Title task. This is the first task to be entered. */
    TASK_FILE_SELECT, /**< File selection task. */
    TASK_GAME,        /**< Gameplay task. Overworld, menus, cutscenes are all contained here. */
    TASK_GAMEOVER,    /**< Gameover task. */
    TASK_STAFFROLL,   /**< Staffroll task. Only accessible through the script played during the game ending. */
    TASK_DEBUG,       /**< Debug task. Inaccessible in normal gameplay. */
} Task;

/** System sleep status. */
typedef enum {
    DEFAULT,
    SLEEP,
} SleepStatus;

/**
 * Main system structure.
 */
typedef struct {
    vu8 interruptFlag;
    u8 sleepStatus;
    u8 task;     /**< Current #Task. */
    u8 state;    /**< State of the current #Task. */
    u8 substate; /**< Substate of the current #Task. */
    u8 field_0x5;
    u8 muteAudio; /**< Mute audio. */
    u8 field_0x7;
    u8 pauseFrames;   /**< Number of frames to pause. */
    u8 pauseCount;    /**< Number of pauses to make. */
    u8 pauseInterval; /**< Number of frames to play between each pause. */
    u8 pad;
    union SplitHWord ticks; /**< Current time. */
} Main;

/**
 * HUD structure.
 */
typedef struct {
    /*0x00*/ u8 nextToLoad;
    /*0x01*/ u8 _1;
    /*0x02*/ u8 lastState;
    /*0x03*/ u8 field_0x3;
    /*0x04*/ u8 state;
    /*0x05*/ u8 field_0x5;
    /*0x06*/ u8 field_0x6;
    /*0x07*/ u8 pauseFadeIn;
    /*0x08*/ u16 isLoading;
    /*0x0A*/ u16 fadeInTime;
    /*0x0C*/ u8 fillerC[0x3A8];
} UI;
static_assert(sizeof(UI) == 0x3b4);

extern Main gMain;       /**< Main instance. */
extern UI gUnk_02032EC0; /**< UI instance. */

/**
 * Program entry point.
 */
void AgbMain(void);

/**
 * Begin a new task.
 *
 * @param task #Task to begin.
 */
void SetTask(u32 task);

/**
 * Initialize the DMA system.
 */
void InitDMA(void);

/**
 * Soft reset the system.
 */
void DoSoftReset(void);

/**
 * Put the system into sleep mode.
 */
void SetSleepMode(void);

extern void sub_0805622C(void* a1, u32 a2, u32 a3);
extern void sub_08056208(void);
extern void ResetPalettes(void);
extern void VBlankIntrWait();
extern void VBlankInterruptWait(void);
extern void DisableInterruptsAndDMA(void);
extern void EnableVBlankIntr(void);
extern void sub_08056250(void);
extern void sub_08056208(void);

/** @name Task entrypoints */
///@{
/** Task entrypoint. */
extern void TitleTask(void);
extern void FileSelectTask(void);
extern void GameTask(void);
extern void GameOverTask(void);
extern void StaffrollTask(void);
extern void DebugTask(void);

#ifdef DEMO_USA
extern void DemoTask(void);
#endif
/// @}

extern u8 gUnk_03003DE4;
extern u16 gPaletteBuffer[];

#endif
