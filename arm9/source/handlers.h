#ifndef __HANDLERS_H__
#define __HANDLERS_H__

#include "main.h"

	// Function prototypes
void handleEffects(void);
void handleGame(void);
void handleInGameMenu(void);
void handleInGameMenuChoice(void);
void handleKeys(void);
void handleHBlank(void);
void handleVBlank(void);
void handleSplash(void);
void handleFigures(void);
void handleInnAnswer(bool innAnswer);
void handleShopAnswer(bool shopAnswer);
void handleShop(void);
void handleEvents(void);
bool handleCutScene(int curCutscene);
bool handleEquip(pFigure Figure, itemAttr itemType, int itemIdx);
void handleBattle(void);
void handleBattleMenu(void);
void handleBattleChoice(int battleSel);
void handleBattleKeys(void);

#endif
