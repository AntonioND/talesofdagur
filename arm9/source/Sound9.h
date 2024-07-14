// ----- Change log -----
// 05/08/05 - Created
// ----------------------

#ifndef SOUND9_H
#define SOUND9_H

#include <nds.h>

#include "SoundCommon.h"
#include "background.h"
#include "main.h"
#include "sprites.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "mscBattle_mod.h"
#include "mscBeach_mod.h"
#include "mscBoss_mod.h"
#include "mscCave_mod.h"
#include "mscGOver_mod.h"
#include "mscMenu_mod.h"
#include "mscPalace_mod.h"
#include "mscSplash_mod.h"
#include "mscTown_mod.h"
#include "mscWin_mod.h"
#include "mscWoods_mod.h"

// ----- Functions -----

extern void SndInit9();
extern void SndSetMemPool(void *memPool, u32 memPoolSize);
extern void SndPlayMOD(const void *modFile);
extern void SndStopMOD();
extern void SndPauseMOD(BOOL bPaused);
extern void SndSetMODCallback(MOD_CALLBACK callback);
extern void SoundSendCmd(SND_COMMAND_TYPE cmdType, u32 param32);
extern bool checkMusicFade(LEVELS lvlIdx);
extern void updateMusic(LEVELS lvlIdx);

// ----------

#ifdef __cplusplus
}
#endif

#endif // SOUND9_H
