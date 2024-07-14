#ifndef __EVENTS_H__
#define __EVENTS_H__

#include "main.h"

// Defines
#define STORY_SIZE (9)
#define buildEvent(x, y) \
    {                    \
        x, (void *)(y)   \
    }
#define buildCutScene(x, y, z, a)          \
    {                                      \
        x, (void *)(y), (u32)(z), (u32)(a) \
    }
#define END_OF_CUTSCENE \
    {                   \
        CS_END, 0, 0, 0 \
    }

// Types
typedef bool (*boolFuncPtr)(void);
typedef void (*voidFuncPtr)(void);

typedef enum _eventType_
{
    EVENT_GETITEM,   // Player needs to recieve an item      // Param: Item index
    EVENT_GOTOLEVEL, // Player needs to be in a specific map // Param: Level index
    EVENT_CUTSCENE,  // Cutscene                             // Param: Curscene index
    EVENT_TALKTO,    // Player needs to talk to a NPC        // Param: Figure pointer
    EVENT_CUSTOM,    // Custom event                         // Param: bool Function pointer
    EVENT_BATTLE,    // Battle with monster and defet it     // Param: bool Function pointer
    EVENT_END,       // End of the game \o/                  // Param: none

    EVENT_SIZE,

} eventType, pEventType;

typedef struct _Event_
{
    eventType EventType;
    void *eventParam;

} Event, *pEvent;

typedef enum _cutSceneEvent_
{
    CS_GIVEITEM,   // Param1: Item Index Param2: quantity
    CS_LOADLEVEL,  // Param1: Level index
    CS_MOVETO,     // Param1: Figure pointer; Param2: X Param3: Y
    CS_SETPOS,     // Param1: figure pointer; Param2: X Param3: Y
    CS_FIGURETALK, // Param1: Figure pointer; Param2: script index
    CS_SETDIR,     // Param1: Figure pointer; Param2: dir
    CS_END,

    CS_SIZE,

} cutSceneEvent, *pCutSceneEvent;

typedef struct _cutSceneAction_
{
    cutSceneEvent eventType;
    void *ptrParam;
    u32 uParam1;
    u32 uParam2;

} cutSceneAction, *pCutSceneAction;

// Data
extern Event gameEvents[];
extern pCutSceneAction cutScenes[];

// Functions
bool isOverAction(pCutSceneAction curEvent);
void performCutsceneAction(pCutSceneAction curEvent);

#endif
