#include <fat.h>
#include <nds.h>

#include "Sound9.h"
#include "background.h"
#include "handlers.h"
#include "main.h"

volatile u32 frameCounter = 0;
volatile u16 KEYS         = 0;
volatile u16 HKEYS        = 0;

// Main entry point for the program
int main()
{
    // Init the DS
    powerOn(POWER_ALL_2D);

    irqSet(IRQ_VBLANK, handleVBlank);
    irqSet(IRQ_HBLANK, handleHBlank);
    irqEnable(IRQ_HBLANK);

    vramSetPrimaryBanks(VRAM_A_MAIN_SPRITE, VRAM_B_MAIN_BG_0x06000000, VRAM_C_SUB_BG_0x06200000,
                        VRAM_D_MAIN_BG_0x06020000);
    vramSetBankG(VRAM_G_SPRITE_EXT_PALETTE);
    vramSetBankH(VRAM_H_SUB_BG);
    vramSetBankI(VRAM_I_SUB_SPRITE);

    initOAM();
    initLevels();
    SndInit9();
    gameValues.isFAT = fatInitDefault();

#if DEBUG
    initMode(LEVELMODE_PLAY);
    gameValues.curMode = LEVELMODE_PLAY;
#else
    initMode(LEVELMODE_SPLASH_FADEIN);
    gameValues.curMode = LEVELMODE_SPLASH_FADEIN;
#endif

    while (1) {
        handleKeys();
        handleGame();
        handleEffects();

        updateOAM();
        swiWaitForVBlank();
    }

    return 0;
}
