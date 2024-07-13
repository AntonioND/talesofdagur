#include "main.h"
		
volatile u32 frameCounter = 0;
volatile u16 KEYS = 0;
volatile u16 HKEYS = 0;
		
	// Main entry point for the program 
int main()
{	
		// Init the DS
	powerSET(POWER_ALL_2D);
		
	irqInit();
	irqSet(IRQ_VBLANK, handleVBlank);	
	irqEnable(IRQ_VBLANK);
	irqSet(IRQ_HBLANK, handleHBlank);
	irqEnable(IRQ_HBLANK); 
		
	vramSetMainBanks(VRAM_A_MAIN_SPRITE, VRAM_B_MAIN_BG_0x06000000,
                     VRAM_C_SUB_BG_0x06200000, VRAM_D_MAIN_BG_0x06020000); 
	vramSetBankG(VRAM_G_OBJ_EXT_PALETTE);
	vramSetBankH(VRAM_H_SUB_BG);
	vramSetBankI(VRAM_I_SUB_SPRITE);
		
	initOAM();
	initLevels();
	SndInit9();
	gameValues.isFAT = FAT_InitFiles();
		
#if DEBUG
	initMode(LEVELMODE_PLAY);
	gameValues.curMode = LEVELMODE_PLAY;
#else
	initMode(LEVELMODE_SPLASH_FADEIN);
	gameValues.curMode = LEVELMODE_SPLASH_FADEIN;
#endif
		
	while(1) {
		handleKeys();
		handleGame();
		handleEffects();
			
		updateOAM();
		swiWaitForVBlank();
	}
		
	return 0;
}
