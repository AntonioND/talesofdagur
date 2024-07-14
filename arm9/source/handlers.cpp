#include "Sound9.h"
#include "background.h"
#include "events.h"
#include "handlers.h"
#include "text.h"

void handleSplash()
{
    gameValues.flowerAngle += 3;

    s16 s = sinLerp(64 * (gameValues.flowerAngle & 0x1FF)) >> 4;
    s16 c = cosLerp(64 * (gameValues.flowerAngle & 0x1FF)) >> 4;

    REG_BG2PA = c;
    REG_BG2PB = -s;
    REG_BG2PC = s;
    REG_BG2PD = c;
    REG_BG2X  = (127 << 8) - (192 - 16) * (c - s);
    REG_BG2Y  = (102 << 8) - (192 - 16) * (s + c);

    gameValues.textZoom += 3;
    s32 tempAngle = sinLerp(64 * (gameValues.textZoom & 0x1FF)) >> 8;

    s = sinLerp(64 * (tempAngle & 0x1FF)) >> 4;
    c = cosLerp(64 * (tempAngle & 0x1FF)) >> 4;

    REG_BG3PA = c;
    REG_BG3PB = -s;
    REG_BG3PC = s;
    REG_BG3PD = c;
    REG_BG3X  = (128 << 8) - 128 * (c - s);
    REG_BG3Y  = (128 << 8) - 128 * (s + c);

    tempAngle = sinLerp(64 * 100) >> 8;

    s = sinLerp(64 * (tempAngle & 0x1FF)) >> 4;
    c = cosLerp(64 * (tempAngle & 0x1FF)) >> 4;

    REG_BG3PA_SUB = c;
    REG_BG3PB_SUB = -s;
    REG_BG3PC_SUB = s;
    REG_BG3PD_SUB = c;
}

void handleEffects()
{
    LEVELS levelLoad = NO_LEVEL;

    switch (gameValues.curMode) {
        case LEVELMODE_SPLASH_FADEIN:
            handleSplash();
            REG_BLDCNT = BLEND_FADE_WHITE | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG2
                         | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDY       = gameValues.fadeVal | gameValues.fadeVal << 8;
            REG_BLDCNT_SUB = BLEND_FADE_WHITE | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG2
                             | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDY_SUB = gameValues.fadeVal | gameValues.fadeVal << 8;
            SoundSendCmd(SND_CMD_SETVOLUME, 127 - (gameValues.fadeVal * 8));
            if (--gameValues.tickVal < 0) {
                gameValues.tickVal = 4;
                --gameValues.fadeVal;
            }
            if (gameValues.fadeVal < 0) {
                // Lock and loaded
                gameValues.fadeVal = 0;
                SoundSendCmd(SND_CMD_SETVOLUME, 127);
                gameValues.curMode = LEVELMODE_SPLASH;
            }
            break;
        case LEVELMODE_SPLASH:
            handleSplash();
            break;
        case LEVELMODE_SPLASH_FADEOUT:
            handleSplash();
            REG_BLDCNT = BLEND_FADE_WHITE | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG2
                         | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDY       = gameValues.fadeVal | gameValues.fadeVal << 8;
            REG_BLDCNT_SUB = BLEND_FADE_WHITE | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG2
                             | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDY_SUB = gameValues.fadeVal | gameValues.fadeVal << 8;
            SoundSendCmd(SND_CMD_SETVOLUME, 127 - (gameValues.fadeVal * 8));
            if (--gameValues.tickVal < 0) {
                gameValues.tickVal = 4;
                ++gameValues.fadeVal;
            }
            if (gameValues.fadeVal > 15) {
                gameValues.fadeVal = 15;
                gameValues.tickVal = 30;
                SoundSendCmd(SND_CMD_SETVOLUME, 127);
                gameValues.curMode = LEVELMODE_MENU_FADEIN;
            }
            break;
        case LEVELMODE_MENU_FADEIN:
            REG_BLDCNT = BLEND_FADE_WHITE | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG2
                         | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDCNT_SUB = BLEND_FADE_WHITE | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG2
                             | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDY     = gameValues.fadeVal | gameValues.fadeVal << 8;
            REG_BLDY_SUB = gameValues.fadeVal | gameValues.fadeVal << 8;
            if (--gameValues.tickVal < 0) {
                gameValues.tickVal = 5;
                --gameValues.fadeVal;
            }
            if (gameValues.fadeVal < 0) {
                gameValues.fadeVal = 0;
                gameValues.curMode = LEVELMODE_MENU_START;
            }
            break;
        case LEVELMODE_MENU_START:
            break;
        case LEVELMODE_MENU_CHOICE:
            moveSprite(textVars.cursorSprite, 11 * 8 - 4, 11 * 8 + textVars.menuSel * 8 + 4);
            break;
        case LEVELMODE_MENU_LOAD:
            moveSprite(textVars.cursorSprite, 6 * 8 - 16, 6 * 8 + textVars.menuSel * 8 * 6 + 4);
            break;
        case LEVELMODE_INTRO:
            break;
        case LEVELMODE_PLAY:
            if (curPlayer->posY + 32 > curLevel->dimY) {
                int curLinkedLevel = 0;
                while (curLevel->linkedLevels[curLinkedLevel].loadedLevel != NO_LEVEL) {
                    if (curPlayer->posX / 8 >= curLevel->linkedLevels[curLinkedLevel].startTile) {
                        if (curPlayer->posX / 8 <= curLevel->linkedLevels[curLinkedLevel].endTile) {
                            if (curLevel->linkedLevels[curLinkedLevel].wallDir == DIR_DOWN) {
                                levelLoad = curLevel->linkedLevels[curLinkedLevel].loadedLevel;
                                gameValues.newPosY = -16;
                                gameValues.newPosX =
                                    curLevel->linkedLevels[curLinkedLevel].posFix * 8;
                                gameValues.newDir = curLevel->linkedLevels[curLinkedLevel].wallDir;
                                break;
                            }
                        }
                    }
                    ++curLinkedLevel;
                }
            }
            if (curPlayer->posY + 32 < 0) {
                int curLinkedLevel = 0;
                while (curLevel->linkedLevels[curLinkedLevel].loadedLevel != NO_LEVEL) {
                    if (curPlayer->posX / 8 >= curLevel->linkedLevels[curLinkedLevel].startTile) {
                        if (curPlayer->posX / 8 <= curLevel->linkedLevels[curLinkedLevel].endTile) {
                            if (curLevel->linkedLevels[curLinkedLevel].wallDir == DIR_UP) {
                                levelLoad = curLevel->linkedLevels[curLinkedLevel].loadedLevel;
                                gameValues.newPosY =
                                    Levels[curLevel->linkedLevels[curLinkedLevel].loadedLevel].dimY
                                    - 32;
                                gameValues.newPosX =
                                    curLevel->linkedLevels[curLinkedLevel].posFix * 8;
                                gameValues.newDir = curLevel->linkedLevels[curLinkedLevel].wallDir;
                                break;
                            }
                        }
                    }
                    ++curLinkedLevel;
                }
            }
            if (curPlayer->posX < -8) {
                int curLinkedLevel = 0;
                while (curLevel->linkedLevels[curLinkedLevel].loadedLevel != NO_LEVEL) {
                    if ((curPlayer->posY + 32) / 8
                        >= curLevel->linkedLevels[curLinkedLevel].startTile) {
                        if ((curPlayer->posY + 32) / 8
                            <= curLevel->linkedLevels[curLinkedLevel].endTile) {
                            if (curLevel->linkedLevels[curLinkedLevel].wallDir == DIR_LEFT) {
                                levelLoad = curLevel->linkedLevels[curLinkedLevel].loadedLevel;
                                gameValues.newPosX =
                                    Levels[curLevel->linkedLevels[curLinkedLevel].loadedLevel].dimX
                                    - 24;
                                gameValues.newPosY =
                                    curLevel->linkedLevels[curLinkedLevel].posFix * 8;
                                gameValues.newDir = curLevel->linkedLevels[curLinkedLevel].wallDir;
                                break;
                            }
                        }
                    }
                    ++curLinkedLevel;
                }
            }
            if (curPlayer->posX + 24 > curLevel->dimX) {
                int curLinkedLevel = 0;
                while (curLevel->linkedLevels[curLinkedLevel].loadedLevel != NO_LEVEL) {
                    if ((curPlayer->posY + 32) / 8
                        >= curLevel->linkedLevels[curLinkedLevel].startTile) {
                        if ((curPlayer->posY + 32) / 8
                            <= curLevel->linkedLevels[curLinkedLevel].endTile) {
                            if (curLevel->linkedLevels[curLinkedLevel].wallDir == DIR_RIGHT) {
                                levelLoad = curLevel->linkedLevels[curLinkedLevel].loadedLevel;
                                gameValues.newPosX = -8;
                                gameValues.newPosY =
                                    curLevel->linkedLevels[curLinkedLevel].posFix * 8;
                                gameValues.newDir = curLevel->linkedLevels[curLinkedLevel].wallDir;
                                break;
                            }
                        }
                    }
                    ++curLinkedLevel;
                }
            }

            if (levelLoad != NO_LEVEL) {
                gameValues.loadedLevel = levelLoad;
                sndControl->toFade     = checkMusicFade(gameValues.loadedLevel);
                gameValues.curMode     = LEVELMODE_FADEOUT;
            }
            break;
        case LEVELMODE_INGAME_MENU:
            REG_BLDCNT = BLEND_FADE_BLACK | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG2
                         | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDY = 8 | 8 << 8;
            break;
        case LEVELMODE_FADEOUT:
            REG_BLDCNT = BLEND_FADE_BLACK | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG2
                         | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDY = gameValues.fadeVal | gameValues.fadeVal << 8;
            if (sndControl->toFade)
                SoundSendCmd(SND_CMD_SETVOLUME, 127 - (gameValues.fadeVal * 8));
            if (--gameValues.tickVal < 0) {
                gameValues.tickVal = 2;
                gameValues.fadeVal++;
            }
            if (gameValues.fadeVal > 15)
                gameValues.curMode = LEVELMODE_SWITCHLEVEL;
            break;
        case LEVELMODE_SWITCHLEVEL:
            loadLevel(gameValues.loadedLevel);
            curPlayer->posX = gameValues.newPosX;
            curPlayer->posY = gameValues.newPosY;
            setDirFigure(curPlayer, gameValues.newDir);
            textVars.pauseMode = false;
            SoundSendCmd(SND_CMD_SETVOLUME, 127);
            gameValues.curMode = LEVELMODE_FADEIN;
            break;
        case LEVELMODE_FADEIN:
            REG_BLDCNT = BLEND_FADE_BLACK | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG2
                         | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDY = gameValues.fadeVal | gameValues.fadeVal << 8;
            if (--gameValues.tickVal < 0) {
                gameValues.tickVal = 2;
                gameValues.fadeVal--;
            }
            if (gameValues.fadeVal < 0) {
                // Lock and loaded
                gameValues.fadeVal = 3;
                gameValues.curMode = LEVELMODE_PLAY;
            }
            break;
        case LEVELMODE_BATTLE_FADEIN:
            gameValues.isFade = true;
            REG_BLDCNT        = BLEND_FADE_WHITE | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG2
                         | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDY = gameValues.fadeVal | gameValues.fadeVal << 8;
            SoundSendCmd(SND_CMD_SETVOLUME, 127 - (gameValues.fadeVal * 8));
            if (--gameValues.tickVal < 0) {
                gameValues.tickVal = 2;
                gameValues.fadeVal++;
            }
            if (gameValues.fadeVal > 15) {
                gameValues.fadeHalt = 30;
                gameValues.curMode  = LEVELMODE_BATTLE_FADEPAUSE;
            }
            break;
        case LEVELMODE_BATTLE_FADEPAUSE:
            --gameValues.fadeHalt;
            if (gameValues.fadeHalt < 0)
                gameValues.curMode = LEVELMODE_BATTLE_FADEOUT;
            break;
        case LEVELMODE_BATTLE_FADEOUT:
            REG_BLDCNT = BLEND_FADE_WHITE | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG2
                         | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDY = gameValues.fadeVal | gameValues.fadeVal << 8;
            SoundSendCmd(SND_CMD_SETVOLUME, 127 - (gameValues.fadeVal * 8));
            if (--gameValues.tickVal < 0) {
                gameValues.tickVal = 2;
                gameValues.fadeVal--;
            }
            if (gameValues.fadeVal < 0) {
                // Lock and loaded
                gameValues.fadeVal  = 3;
                gameValues.curMode  = LEVELMODE_BATTLE;
                gameValues.isFade   = false;
                gameValues.isBattle = true;
            }
            break;
        case LEVELMODE_BATTLE:

            break;
        case LEVELMODE_EXITBATTLE:
            REG_BLDCNT = BLEND_FADE_BLACK | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG2
                         | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDY = gameValues.fadeVal | gameValues.fadeVal << 8;
            SoundSendCmd(SND_CMD_SETVOLUME, 127 - (gameValues.fadeVal * 8));
            if (--gameValues.tickVal < 0) {
                gameValues.tickVal = 2;
                gameValues.fadeVal++;
            }
            if (gameValues.fadeVal > 15) {
                graphicInitGame();
                gameValues.loadedLevel = (LEVELS)battleValues.lstLevel;
                gameValues.newPosX     = battleValues.posX;
                gameValues.newPosY     = battleValues.posY;
                gameValues.newDir      = battleValues.lstDir;
                gameValues.curMode     = LEVELMODE_SWITCHLEVEL;
            }
            break;
        case LEVELMODE_GAMEOVER_FADEIN:
            REG_BLDCNT = BLEND_FADE_BLACK | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG1
                         | BLEND_SRC_BG2 | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDY       = gameValues.fadeVal | gameValues.fadeVal << 8;
            REG_BLDCNT_SUB = BLEND_FADE_BLACK | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG1
                             | BLEND_SRC_BG2 | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDY_SUB = gameValues.fadeVal | gameValues.fadeVal << 8;
            SoundSendCmd(SND_CMD_SETVOLUME, 127 - (gameValues.fadeVal * 8));
            BG_PALETTE_SUB[0] = BG_PALETTE[0] = 0;
            if (--gameValues.tickVal < 0) {
                gameValues.tickVal = 2;
                gameValues.fadeVal++;
            }
            if (gameValues.fadeVal > 15) {
                gameValues.fadeVal = 15;
                gameValues.tickVal = 30;
                gameValues.curMode = LEVELMODE_GAMEOVER_FADEOUT;
            }
            break;
        case LEVELMODE_GAMEOVER_FADEOUT:
            REG_BLDCNT = BLEND_FADE_BLACK | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG1
                         | BLEND_SRC_BG2 | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDY       = gameValues.fadeVal | gameValues.fadeVal << 8;
            REG_BLDCNT_SUB = BLEND_FADE_BLACK | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG1
                             | BLEND_SRC_BG2 | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDY_SUB = gameValues.fadeVal | gameValues.fadeVal << 8;
            SoundSendCmd(SND_CMD_SETVOLUME, 127 - (gameValues.fadeVal * 8));
            if (--gameValues.tickVal < 0) {
                gameValues.tickVal = 2;
                gameValues.fadeVal--;
            }
            if (gameValues.fadeVal < 0) {
                gameValues.fadeVal = 0;
                gameValues.tickVal = 30;
                gameValues.curMode = LEVELMODE_GAMEOVER;
            }
            break;
        case LEVELMODE_GAMEOVER:
            break;
        case LEVELMODE_GAMEOVER_WHITEFADE:
            REG_BLDCNT = BLEND_FADE_WHITE | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG2
                         | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDY       = gameValues.fadeVal | gameValues.fadeVal << 8;
            REG_BLDCNT_SUB = BLEND_FADE_WHITE | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG2
                             | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
            REG_BLDY_SUB = gameValues.fadeVal | gameValues.fadeVal << 8;
            SoundSendCmd(SND_CMD_SETVOLUME, 127 - (gameValues.fadeVal * 8));
            BG_PALETTE_SUB[0] = BG_PALETTE[0] =
                RGB15(gameValues.fadeVal * 2, gameValues.fadeVal * 2, gameValues.fadeVal * 2);
            if (--gameValues.tickVal < 0) {
                gameValues.tickVal = 4;
                ++gameValues.fadeVal;
            }
            if (gameValues.fadeVal > 15) {
                // Resetting the DS.
                REG_IME = 0;
                SndStopMOD();
                for (int i = 0; i < 32 * 32; i++)
                    ((u16 *)BG_MAP_RAM_SUB(31))[i] = 0;
                BG_PALETTE_SUB[0] = BG_PALETTE[0] = 0x7FFF;
                swiSoftReset();
            }
            break;
    }
}

void handleInGameMenu()
{
    const char *equips[6] = {
        "Weapon: ", "Head: ", "Armor: ", "Shield: ", "Access.: ", "Orb: ",
    };
    for (int i = 0; i < (int)textVars.smallMenu.size(); ++i) {
        if (textVars.curMenu == &textVars.smallMenu) {
            if (i == textVars.curOption)
                menuText(2, 16 + i, "\4%s\4", textVars.smallMenu[i].c_str());
            else
                menuText(2, 16 + i, "%s", textVars.smallMenu[i].c_str());
        } else {
            if (i != textVars.curOption)
                menuText(2, 16 + i, "\5%s\5", textVars.smallMenu[i].c_str());
            else
                menuText(2, 16 + i, "%s", textVars.smallMenu[i].c_str());
        }
    }

    int minSize = std::min((int)curPlayer->figureItemList.size(), textVars.virtualPosY + 12);
    switch (textVars.curOption) {
        case EQUIP:
            for (int i = WEAPON; i <= ORB; ++i) {
                if (curPlayer->equipedItems[i] != -1)
                    menuText(2, 2 + (i - WEAPON), "%s%s", equips[i - WEAPON],
                             itemDB[curPlayer->equipedItems[i]].itemName);
                else
                    menuText(2, 2 + (i - WEAPON), "%s\2NONE\2", equips[i - WEAPON]);
            }
        case ITEM:
            if (curPlayer->figureItemList.empty()) {
                moveSprite(textVars.cursorSprite, 12 * 8 - 16, 17 * 5);
                break;
            }
            if (textVars.menuSel > (int)curPlayer->figureItemList.size() - 1)
                textVars.menuSel = curPlayer->figureItemList.size() - 1;

            textVars.subMenus[textVars.curOption].clear();
            for (int i = 0; i < (int)curPlayer->figureItemList.size(); ++i)
                textVars.subMenus[textVars.curOption].push_back(
                    curPlayer->figureItemList[i].itemData->itemName);

            for (int i = textVars.virtualPosY; i < minSize; ++i) {
                if (textVars.curOption == ITEM) {
                    if (curPlayer->figureItemList[i].itemData->itemType == POTION) {
                        menuText(12, 11 + (i - textVars.virtualPosY), "%s",
                                 curPlayer->figureItemList[i].itemData->itemName);
                        menuText(26, 11 + (i - textVars.virtualPosY), "%2d",
                                 curPlayer->figureItemList[i].quantity);
                    } else {
                        menuText(12, 11 + (i - textVars.virtualPosY), "\5%s\5",
                                 curPlayer->figureItemList[i].itemData->itemName);
                        menuText(26, 11 + (i - textVars.virtualPosY), "\5%2d\5",
                                 curPlayer->figureItemList[i].quantity);
                    }
                } else {
                    if (curPlayer->figureItemList[i].itemData->itemType != POTION) {
                        menuText(12, 11 + (i - textVars.virtualPosY), "%s",
                                 curPlayer->figureItemList[i].itemData->itemName);
                        menuText(26, 11 + (i - textVars.virtualPosY), "%2d",
                                 curPlayer->figureItemList[i].quantity);
                    } else {
                        menuText(12, 11 + (i - textVars.virtualPosY), "\5%s\5",
                                 curPlayer->figureItemList[i].itemData->itemName);
                        menuText(26, 11 + (i - textVars.virtualPosY), "\5%2d\5",
                                 curPlayer->figureItemList[i].quantity);
                    }
                }

                if (curPlayer->figureItemList[i].equiped)
                    menuText(11, 11 + (i - textVars.virtualPosY), "\2%c\2", ('~' + 1));
            }

            if (textVars.menuSel > 11) {
                textVars.menuSel = 11;
                ++textVars.virtualPosY;
                if (textVars.virtualPosY > (int)curPlayer->figureItemList.size() - 12)
                    textVars.virtualPosY = (int)curPlayer->figureItemList.size() - 12;
            } else if (textVars.menuSel == 0) {
                if (KEYS & KEY_UP) {
                    --textVars.virtualPosY;
                    if (textVars.virtualPosY < 0)
                        textVars.virtualPosY = 0;
                }
            }
            if (!curPlayer->figureItemList[textVars.menuSel + textVars.virtualPosY].equiped) {
                textVars.writeStatics = false;
                writeStaticsWith(
                    curPlayer->figureItemList[textVars.menuSel + textVars.virtualPosY].itemData);
            } else
                textVars.writeStatics = true;

            menuText(12, 11 + textVars.menuSel, "\4%s\4",
                     curPlayer->figureItemList[textVars.menuSel + textVars.virtualPosY]
                         .itemData->itemName);
            menuText(26, 11 + textVars.menuSel, "\4%2d\4",
                     curPlayer->figureItemList[textVars.menuSel + textVars.virtualPosY].quantity);
            moveSprite(textVars.cursorSprite, 12 * 8 - 16, 17 * 5 + textVars.menuSel * 8);
            break;
        case MAGIC:
            break;
        case CONFIG:
            break;
        case SAVE:
            moveSprite(textVars.cursorSprite, 11 * 8 - 16, 16 * 5 + textVars.menuSel * 8 * 5);
            for (int i = 0; i < 3; ++i) {
                if (textVars.menuSel == i)
                    menuText(11, 10 + i * 5, "\4%s\4", gameValues.saveSlots[i].saveName);
                else
                    menuText(11, 10 + i * 5, "%s", gameValues.saveSlots[i].saveName);
            }
            break;
        case RESUME:
            break;
        case MENUS_SIZE:
            break;
        case MAIN_MENU:
            moveSprite(textVars.cursorSprite, 2 * 8 - 16, 16 * 8 + textVars.menuSel * 8);
            menuText(2, 16 + textVars.menuSel, "\4%s\4",
                     textVars.smallMenu[textVars.menuSel].c_str());
            break;
    }
}

void handleGame()
{
    if (gameValues.curMode != gameValues.lstMode) {
        initMode(gameValues.curMode);
        gameValues.lstMode = gameValues.curMode;
    }
    switch (gameValues.curMode) {
        case LEVELMODE_SPLASH_FADEIN:
            break;
        case LEVELMODE_SPLASH:
            break;
        case LEVELMODE_SPLASH_FADEOUT:
            break;
        case LEVELMODE_MENU_FADEIN:
            break;
        case LEVELMODE_MENU_START:
            break;
        case LEVELMODE_MENU_CHOICE:
            break;
        case LEVELMODE_MENU_LOAD:
            break;
        case LEVELMODE_INTRO:
            break;
        case LEVELMODE_FADEOUT:
        case LEVELMODE_FADEIN:
            curPlayer->spriteIndex = 0;
            writeStatics();
            updateView();
            handleFigures();
            break;
        case LEVELMODE_PLAY:
            if (curPlayer->isMotion) {
                if (frameCounter % 640 == 0) {
                    startBattle(curLevel->battleMon);
                    break;
                }
            }
            if (textVars.writeStatics)
                writeStatics();
            updateView();
            handleFigures();
            handleEvents();
            break;
        case LEVELMODE_INGAME_MENU:
            if (textVars.writeStatics)
                writeStatics();
            handleInGameMenu();
            break;
        case LEVELMODE_SWITCHLEVEL:
            break;
        case LEVELMODE_BATTLE_FADEIN:
        case LEVELMODE_BATTLE_FADEPAUSE:
            break;
        case LEVELMODE_BATTLE_FADEOUT:
            break;
        case LEVELMODE_EXITBATTLE:
            break;
        case LEVELMODE_BATTLE:
            writeStatics();
            handleBattle();
            handleFigures();
            break;
        case LEVELMODE_GAMEOVER_FADEIN:
            break;
        case LEVELMODE_GAMEOVER_FADEOUT:
        case LEVELMODE_GAMEOVER:
        case LEVELMODE_GAMEOVER_WHITEFADE:
            handleFigures();
            break;
    }
}

void handleKeys()
{
    int deltaMoveX = 0;
    int deltaMoveY = 0;
    switch (gameValues.curMode) {
        case LEVELMODE_SPLASH_FADEIN:
            break;
        case LEVELMODE_SPLASH:
            if (KEYS & KEY_TOUCH)
                gameValues.curMode = LEVELMODE_SPLASH_FADEOUT;
            break;
        case LEVELMODE_SPLASH_FADEOUT:
            break;
        case LEVELMODE_MENU_FADEIN:
            break;
        case LEVELMODE_MENU_START:
            if (KEYS & (KEY_START | KEY_TOUCH))
                gameValues.curMode = LEVELMODE_MENU_CHOICE;
            break;
        case LEVELMODE_MENU_CHOICE:
            if (KEYS & KEY_UP)
                --textVars.menuSel;
            if (KEYS & KEY_DOWN)
                ++textVars.menuSel;

            if (textVars.menuSel < 0)
                textVars.menuSel = 0;
            if (textVars.menuSel > 1)
                textVars.menuSel = 1;

            if (KEYS & KEY_A) {
                if (textVars.menuSel) {
                    textVars.menuSel   = 0;
                    gameValues.curMode = LEVELMODE_MENU_LOAD;
                } else {
                    gameValues.usesGameSave = -1;
                    gameValues.curMode      = LEVELMODE_PLAY;
                }
            }
            break;
        case LEVELMODE_MENU_LOAD:
            if (KEYS & KEY_UP)
                --textVars.menuSel;
            if (KEYS & KEY_DOWN)
                ++textVars.menuSel;

            if (KEYS & KEY_B) {
                textVars.menuSel   = 1;
                gameValues.curMode = LEVELMODE_MENU_CHOICE;
            }

            if (KEYS & KEY_A) {
                if (gameValues.saveSlots[textVars.menuSel].isLoaded) {
                    gameValues.usesGameSave = textVars.menuSel;
                    gameValues.curMode      = LEVELMODE_PLAY;
                }
            }

            if (textVars.menuSel < 0)
                textVars.menuSel = 0;
            if (textVars.menuSel > 2)
                textVars.menuSel = 2;
            break;
        case LEVELMODE_INTRO:
            break;
        case LEVELMODE_PLAY:
            if (KEYS & KEY_A) {
                pFigure chatFigure;
                if (isPlayerChat(chatFigure)) {
                    string tempString = "\2" + chatFigure->figName + ": \2" + chatFigure->figText;
                    textWrite(tempString.c_str());
                    ++chatFigure->talkCount;
                    gameValues.lstTalkedFigure = chatFigure;
                }
            }

            if (KEYS & KEY_START) {
                if (textVars.curMode == TEXTMODE_IDLE) {
                    textVars.menuSel   = ITEM;
                    textVars.curOption = MAIN_MENU;
                    for (int i = 0; i < 32 * 32; i++)
                        ((u16 *)BG_MAP_RAM_SUB(2))[i] = 9;
                    drawRectTop(1, 23, 8, textVars.smallMenu.size() + 2);
                    hideSprite(textVars.cursorSprite, false);
                    textVars.curMenu     = &textVars.smallMenu;
                    textVars.virtualPosY = 0;
                    gameValues.curMode   = LEVELMODE_INGAME_MENU;
                    SoundSendCmd(SND_CMD_SETVOLUME, 32);
                }
            }

            if (textVars.curMode == TEXTMODE_IDLE) {
                if (gameEvents[gameValues.curEvent].EventType == EVENT_CUTSCENE)
                    break;

                curPlayer->curMove = (HKEYS & 0xF0) >> 4;
                if (HKEYS & KEY_RIGHT) {
                    setDirFigure(curPlayer, DIR_RIGHT);
                    deltaMoveX = 1;
                } else if (HKEYS & KEY_LEFT) {
                    setDirFigure(curPlayer, DIR_LEFT);
                    deltaMoveX = -1;
                }
                if (HKEYS & KEY_UP) {
                    setDirFigure(curPlayer, DIR_UP);
                    deltaMoveY = -1;
                } else if (HKEYS & KEY_DOWN) {
                    setDirFigure(curPlayer, DIR_DOWN);
                    deltaMoveY = 1;
                }
                if (HKEYS & KEY_B) {
                    deltaMoveX <<= 1;
                    deltaMoveY <<= 1;
                    curPlayer->motionInterval = 5;
                } else
                    curPlayer->motionInterval = 10;

                curPlayer->posX += deltaMoveX;
                if (isCollision(curPlayer))
                    curPlayer->posX -= deltaMoveX;
                curPlayer->posY += deltaMoveY;
                if (isCollision(curPlayer))
                    curPlayer->posY -= deltaMoveY;
            } else
                HKEYS = 0;

            curPlayer->isMotion = (HKEYS & 0xF0);
            break;
        case LEVELMODE_FADEOUT:
            break;
        case LEVELMODE_FADEIN:
            break;
        case LEVELMODE_INGAME_MENU:
            if (KEYS & KEY_A) {
                if (textVars.curMenu == &textVars.smallMenu)
                    textVars.lstOption = (menuOption)textVars.menuSel;
                handleInGameMenuChoice();
            }

            if (KEYS & KEY_B) {
                if (textVars.curMenu != &textVars.smallMenu) {
                    textVars.virtualPosY = 0;
                    textVars.curMenu     = &textVars.smallMenu;
                    for (int i = 0; i < 32 * 32; i++)
                        ((u16 *)BG_MAP_RAM_SUB(2))[i] = 9;
                    drawRectTop(1, 23, 8, textVars.smallMenu.size() + 2);
                    textVars.writeStatics = true;
                    textVars.menuSel      = textVars.lstOption;
                    textVars.curOption    = MAIN_MENU;
                } else
                    KEYS |= KEY_START; // hack :P
            }

            if (KEYS & KEY_START) {
                hideSprite(textVars.cursorSprite, true);
                for (int i = 0; i < 32 * 32; i++)
                    ((u16 *)BG_MAP_RAM_SUB(2))[i] = 9;
                drawRectTop(0, 0, 32, 7);
                textVars.virtualPosY = 19;
                REG_BLDCNT           = 0; // Remove blending
                SoundSendCmd(SND_CMD_SETVOLUME, 127);
                gameValues.curMode = LEVELMODE_PLAY;
            }

            if (KEYS & KEY_DOWN) {
                ++textVars.menuSel;
                if (textVars.menuSel > (int)textVars.curMenu->size() - 1)
                    textVars.menuSel = textVars.curMenu->size() - 1;
            } else if (KEYS & KEY_UP) {
                --textVars.menuSel;
                if (textVars.menuSel < 0)
                    textVars.menuSel = 0;
            }
            break;
        case LEVELMODE_SWITCHLEVEL:
            break;
        case LEVELMODE_BATTLE_FADEIN:
            break;
        case LEVELMODE_BATTLE_FADEPAUSE:
            break;
        case LEVELMODE_BATTLE_FADEOUT:
            break;
        case LEVELMODE_BATTLE:
            handleBattleKeys();
            break;
        case LEVELMODE_GAMEOVER_FADEIN:
            break;
        case LEVELMODE_GAMEOVER_FADEOUT:
            break;
        case LEVELMODE_EXITBATTLE:
        case LEVELMODE_GAMEOVER:
            if (KEYS & KEY_TOUCH)
                gameValues.curMode = LEVELMODE_GAMEOVER_WHITEFADE;
            break;
        case LEVELMODE_GAMEOVER_WHITEFADE:
            break;
    }
}

void handleVBlank()
{
    scanKeys();
    KEYS  = keysDown();
    HKEYS = keysHeld();

    // 2-in-1 :)
    gameValues.curCou = ++frameCounter;
    srand(frameCounter);

    if (gameValues.curCou - gameValues.lstCou > 60 * 60 - 1) {
        ++gameValues.timePlayed;
        gameValues.lstCou = gameValues.curCou;
    }

    textHandleVBlank();
}

void handleHBlank()
{
    textHandleHBlank();
    switch (gameValues.curMode) {
        case LEVELMODE_SPLASH_FADEIN:
        case LEVELMODE_SPLASH:
        case LEVELMODE_SPLASH_FADEOUT:
            break;
        case LEVELMODE_MENU_FADEIN:
        case LEVELMODE_MENU_START:
        case LEVELMODE_MENU_CHOICE:
        case LEVELMODE_MENU_LOAD:
            BG_PALETTE[1]     = ((u16 *)palGrad_bin)[DISP_Y / 4];
            BG_PALETTE_SUB[1] = RGB15(0, 5 + DISP_Y / 7, 25);
            BG_PALETTE_SUB[2] = RGB15((DISP_Y - 121) / 2, (DISP_Y - 121) / 2, (DISP_Y - 121) / 2);
            break;
        case LEVELMODE_INTRO:
            break;
        case LEVELMODE_EXITBATTLE:
        case LEVELMODE_BATTLE_FADEPAUSE:
        case LEVELMODE_BATTLE_FADEOUT:
        case LEVELMODE_BATTLE:
        case LEVELMODE_BATTLE_FADEIN:
            BG_PALETTE_SUB[0] = RGB15(0, 15 + (DISP_Y - 192) / 15, 25);
            break;
        case LEVELMODE_SWITCHLEVEL:
        case LEVELMODE_INGAME_MENU:
        case LEVELMODE_FADEIN:
        case LEVELMODE_FADEOUT:
        case LEVELMODE_PLAY:
            BG_PALETTE[0]     = RGB15(0, 15 + (DISP_Y) / 15, 25);
            BG_PALETTE_SUB[0] = RGB15(0, 15 + (DISP_Y - 192) / 15, 25);
            break;
        case LEVELMODE_GAMEOVER_FADEIN:
            break;
        case LEVELMODE_GAMEOVER_FADEOUT:
        case LEVELMODE_GAMEOVER:
        case LEVELMODE_GAMEOVER_WHITEFADE:
            BG_PALETTE_SUB[1] = RGB15(15 + (DISP_Y - 192) / 15, 5, 5);
            break;
    }
}

void handleInGameMenuChoice()
{
    if (textVars.curMenu == &textVars.smallMenu) {
        textVars.curMenu   = &textVars.subMenus[textVars.menuSel];
        textVars.curOption = (menuOption)textVars.menuSel;
        switch (textVars.menuSel) {
            case EQUIP:
                drawRectTop(1, 9, 20, 8);
            case ITEM:
                drawRectTop(10, 18, 20, 14);
                textVars.virtualPosY = 0;
                break;
            case MAGIC:
                break;
            case CONFIG:
                drawRectTop(10, 18, 20, 14);
                moveSprite(textVars.cursorSprite, 10 * 8 - 16, 18 * 5);
                break;
            case SAVE:
                drawRectTop(9, 16, 22, 5);
                drawRectTop(9, 16 + 5, 22, 5);
                drawRectTop(9, 16 + 10, 22, 5);
                moveSprite(textVars.cursorSprite, 11 * 8 - 16, 16 * 5);
                fillSaves();
                break;
            case RESUME:
                hideSprite(textVars.cursorSprite, true);
                for (int i = 0; i < 32 * 32; i++)
                    ((u16 *)BG_MAP_RAM_SUB(2))[i] = 9;
                drawRectTop(0, 0, 32, 7);
                REG_BLDCNT           = 0;
                textVars.virtualPosY = 19;
                textVars.txtPos      = 0;
                SoundSendCmd(SND_CMD_SETVOLUME, 127);
                gameValues.curMode = LEVELMODE_PLAY;
                break;
        }
        textVars.menuSel = 0;
    } else {
        switch (textVars.curOption) {
            case ITEM:
                if (!curPlayer->figureItemList.empty()) {
                    if (curPlayer->figureItemList[textVars.menuSel].itemData->itemType == POTION)
                        useItem(curPlayer,
                                curPlayer->figureItemList[textVars.menuSel + textVars.virtualPosY]
                                    .itemIdx);
                }
                break;
            case MAGIC:
                break;
            case EQUIP:
                if (!curPlayer->figureItemList.empty()) {
                    if (curPlayer->figureItemList[textVars.menuSel].itemData->itemType != POTION)
                        useItem(curPlayer,
                                curPlayer->figureItemList[textVars.menuSel + textVars.virtualPosY]
                                    .itemIdx);
                }
                break;
            case CONFIG:
                break;
            case SAVE:
                saveGame(textVars.menuSel);
                fillSaves();
                break;
            case RESUME:
            default:
                textVars.virtualPosY = 19;
                textVars.txtPos      = 0;
                break;
        }
    }
}

void handleFigures()
{
    int curLnkLvl = 0;
    onScreenFigures.clear();
    for (list<pFigure>::iterator i = activeFigures.begin(); i != activeFigures.end(); ++i) {
        pFigure curFigure = *i;

        if (curLevel != &battleLevel) {
            while (curLevel->linkedDoors[curLnkLvl].loadedLevel != NO_LEVEL) {
                if (isCollisionDoor(curFigure, &curLevel->linkedDoors[curLnkLvl])) {
                    gameValues.loadedLevel = curLevel->linkedDoors[curLnkLvl].loadedLevel;
                    sndControl->toFade     = checkMusicFade(gameValues.loadedLevel);
                    gameValues.newPosX     = curLevel->linkedDoors[curLnkLvl].posFixX * 8;
                    gameValues.newPosY     = curLevel->linkedDoors[curLnkLvl].posFixY * 8;
                    gameValues.newDir      = curLevel->linkedDoors[curLnkLvl].loadedDir;
                    gameValues.curMode     = LEVELMODE_FADEOUT;
                }
                ++curLnkLvl;
            }
        }
        if (curFigure != curPlayer) {
            if (&Levels[curFigure->levelShow] == curLevel) {
                if (curFigure == gameValues.lstTalkedFigure && textVars.curMode != TEXTMODE_IDLE) {
                    curFigure->isMotion = false;
                } else
                    curFigure->isMotion = advanceFigure(curFigure);
            } else {
                disableFigure(curFigure);
                continue;
            }
        } else
            advanceFigure(curFigure);

        setPosFigure(curFigure, curFigure->posX, curFigure->posY);

        if (curFigure->isActive) {
            onScreenFigures.push_back(curFigure);
            enableFigure(curFigure);
        } else
            disableFigure(curFigure);
    }

    int chestCounter = 0;
    onScreenChests.clear();
    for (list<pChest>::iterator i = activeChests.begin(); i != activeChests.end(); ++i) {
        pChest curChest = *i;

        if (&Levels[curChest->cFigure.levelShow] != curLevel) {
            disableFigure(&curChest->cFigure);
            continue;
        }
        setPosFigure(&curChest->cFigure, curChest->cFigure.posX, curChest->cFigure.posY);

        if (isCollisionChest(curPlayer, curChest)) {
            if (KEYS & KEY_A) {
                if (!curChest->isOpen) {
                    receiveItem(curChest->itemIdx, curChest->quantity);
                    curChest->cFigure.isMotion = true;
                    curChest->isOpen           = true;
                }
            }
        }
        if (curChest->cFigure.spriteIndex == curChest->cFigure.animSize) {
            curChest->cFigure.spriteIndex = 0;
            curChest->cFigure.allData += (32 * 48 / 2) * 2;
            curChest->cFigure.isMotion = false;
        }
        if (curChest->cFigure.isActive) {
            onScreenFigures.push_back(&curChest->cFigure);
            onScreenChests.push_back(curChest);
            enableFigure(&curChest->cFigure);
            ++chestCounter;
        } else
            disableFigure(&curChest->cFigure);
    }

    for (list<pFigure>::iterator i = onScreenFigures.begin(); i != onScreenFigures.end(); ++i) {
        pFigure curFigure = *i;

        updateFigure(curFigure);
        if (curFigure->posY + curFigure->dimY / 2 > curPlayer->posY + curPlayer->dimY / 2) {
            setSpritePriority(curFigure->upperSprite, 1);
            setSpritePriority(curFigure->lowerSprite, 2);
        } else {
            setSpritePriority(curFigure->upperSprite, 2);
            setSpritePriority(curFigure->lowerSprite, 3);
        }

        if (curFigure->isMotion) {
            if (frameCounter - curFigure->lastFrame > curFigure->motionInterval) {
                ++curFigure->spriteIndex;
                curFigure->lastFrame = frameCounter;
            }
        } else
            curFigure->spriteIndex = 0;
    }
    // Remove all chests
    while (chestCounter--)
        onScreenFigures.pop_back();
}

void handleInnAnswer(bool innAnswer)
{
    memset(textVars.textMap, 0, 32 * 32 * 2);

    switch (innAnswer) {
        case true:
            if (curPlayer->Attr[GOLD] < 75) {
                textVars.curMode = TEXTMODE_IDLE;
                textWrite("Sorry, But you don't have enough Gold, Please come back later.");
            } else {
                textVars.curMode = TEXTMODE_IDLE;
                textWrite("Sleep tight!");
                textVars.needInput = false;
                textVars.pauseMode = true;

                // Load the Inn!
                gameValues.loadedLevel = LEVEL10;
                gameValues.newPosX     = 96;
                gameValues.newPosY     = 48;
                gameValues.newDir      = DIR_DOWN;
                gameValues.curMode     = LEVELMODE_FADEOUT;

                // Heal and take the cash $_$
                curPlayer->Attr[GOLD] -= 75;
                curPlayer->Attr[HP] = curPlayer->Attr[MAX_HP];
                curPlayer->Attr[MP] = curPlayer->Attr[MAX_MP];
            }
            break;
        case false:
            textVars.curMode = TEXTMODE_IDLE;
            textWrite("Come back later!");
            break;
    }
}

void handleShop()
{
#define OPTION_MAIN 0
#define OPTION_BUY  1
#define OPTION_SELL 2
#define OPTION_EXIT 3

    static int curOption   = OPTION_MAIN;
    static int virtualPosY = 0;

    hideSprite(textVars.cursorSprite, false);

    drawRectTop(11, 8, 21, 3);
    drawRectTop(11, 11, 21, 14);

    switch (curOption) {
        case OPTION_MAIN:
            textVars.writeStatics = true;
            menuText(15, 9, "Buy");
            menuText(20, 9, "Sell");
            menuText(25, 9, "Exit");
            if (KEYS & KEY_RIGHT) {
                ++textVars.menuSel;
                if (textVars.menuSel > 2)
                    textVars.menuSel = 2;
            }
            if (KEYS & KEY_LEFT) {
                --textVars.menuSel;
                if (textVars.menuSel < 0)
                    textVars.menuSel = 0;
            }
            moveSprite(textVars.cursorSprite, 13 * 8 + textVars.menuSel * 5 * 8, 9 * 8);
            if (KEYS & KEY_A) {
                switch (textVars.menuSel) {
                    case 0: // Buy mode
                        textVars.menuSel = virtualPosY = 0;
                        curOption                      = OPTION_BUY;
                        break;
                    case 1: // Sell mode
                        textVars.menuSel = virtualPosY = 0;
                        curOption                      = OPTION_SELL;
                        break;
                    case 2: // Exit
                        hideSprite(textVars.cursorSprite, true);
                        textVars.curMode = TEXTMODE_WRITING;
                        textVars.txtLen  = 34;
                        strcpy(textVars.txtBuffer, "Nice doin' business with ya, mate!");
                        textVars.txtPos      = 0;
                        textVars.posX        = 0;
                        textVars.virtualPosY = textVars.posY = 19;
                        for (int i = 0; i < 32 * 32; i++)
                            ((u16 *)BG_MAP_RAM_SUB(2))[i] = 9;
                        textVars.scroolY = 0;
                        memset(textVars.textMap, 0, 32 * 32 * 2);
                        drawRectTop(0, 0, 32, 7);
                        textVars.writeStatics = true;
                        textVars.virtualPosY  = 19;
                        // for next time the shop will be shown
                        curOption = OPTION_MAIN;
                        break;
                }
            }
            break;
        case OPTION_BUY:
            textVars.writeStatics = false;
            menuText(15, 9, "Buy");
            menuText(20, 9, "\5Sell\5");
            menuText(25, 9, "\5Exit\5");
            moveSprite(textVars.cursorSprite, 10 * 8, 12 * 8 + textVars.menuSel * 8);
            for (int i = virtualPosY; i < 12 + virtualPosY; ++i) {
                if (i == textVars.menuSel + virtualPosY) {
                    menuText(12, 12 + (i - virtualPosY), "\4%-12s\4", itemDB[i].itemName);
                    menuText(27, 12 + (i - virtualPosY), "\4%4d\4", itemDB[i].itemPrice);
                } else {
                    menuText(12, 12 + (i - virtualPosY), "%-12s", itemDB[i].itemName);
                    menuText(27, 12 + (i - virtualPosY), "%4d", itemDB[i].itemPrice);
                }
            }

            if (KEYS & KEY_DOWN) {
                for (int i = 0; i < 32 * 6; ++i)
                    ((u16 *)BG_MAP_RAM_SUB(2))[i] = 9;
                memset((void *)BG_MAP_RAM_SUB(4), 0, 32 * 12);
                ++textVars.menuSel;
                if (textVars.menuSel > 11) {
                    textVars.menuSel = 11;
                    ++virtualPosY;
                    if (virtualPosY > DATABASE_SIZE - 12)
                        virtualPosY = DATABASE_SIZE - 12;
                }
            }
            if (KEYS & KEY_UP) {
                for (int i = 0; i < 32 * 6; ++i)
                    ((u16 *)BG_MAP_RAM_SUB(2))[i] = 9;
                memset((void *)BG_MAP_RAM_SUB(4), 0, 32 * 12);
                --textVars.menuSel;
                if (textVars.menuSel < 0) {
                    textVars.menuSel = 0;
                    --virtualPosY;
                    if (virtualPosY < 0)
                        virtualPosY = 0;
                }
            }

            writeStaticsWith((pItem)&itemDB[textVars.menuSel + virtualPosY]);

            if (KEYS & KEY_A) {
                drawRectTop(3, 3, 24, 3);
                if (curPlayer->Attr[GOLD] < itemDB[textVars.menuSel + virtualPosY].itemPrice)
                    menuText(5, 4, "\2  Not enough Gold!  \2");
                else {
                    addItem(curPlayer, textVars.menuSel + virtualPosY, 1);
                    curPlayer->Attr[GOLD] -= itemDB[textVars.menuSel + virtualPosY].itemPrice;
                    menuText(5, 4, "\3Successfully Bought!\3");
                }
            }
            break;
        case OPTION_SELL:
            textVars.writeStatics = true;
            menuText(15, 9, "\5Buy\5");
            menuText(20, 9, "Sell");
            menuText(25, 9, "\5Exit\5");
            moveSprite(textVars.cursorSprite, 10 * 8, 12 * 8 + textVars.menuSel * 8);
            int minSize = std::min(12 + virtualPosY, (int)curPlayer->figureItemList.size());
            if (minSize < 1) {
                moveSprite(textVars.cursorSprite, 10 * 8, 12 * 8);
                memset((void *)(BG_MAP_RAM_SUB(4) + 32 * 10 * 2), 0, 32 * 20 * 2);
                break;
            }
            memset((void *)(BG_MAP_RAM_SUB(4) + 32 * 10 * 2), 0, 32 * 20 * 2);
            for (int i = virtualPosY; i < minSize; ++i) {
                if (i == textVars.menuSel + virtualPosY) {
                    menuText(12, 12 + (i - virtualPosY), "\4%-12s\4",
                             curPlayer->figureItemList[i].itemData->itemName);
                    menuText(27, 12 + (i - virtualPosY), "\4%4d\4",
                             curPlayer->figureItemList[i].itemData->itemPrice / 2);
                } else {
                    menuText(12, 12 + (i - virtualPosY), "%-12s",
                             curPlayer->figureItemList[i].itemData->itemName);
                    menuText(27, 12 + (i - virtualPosY), "%4d",
                             curPlayer->figureItemList[i].itemData->itemPrice / 2);
                }
            }

            if (KEYS & KEY_DOWN) {
                for (int i = 0; i < 32 * 6; ++i)
                    ((u16 *)BG_MAP_RAM_SUB(2))[i] = 9;
                memset((void *)BG_MAP_RAM_SUB(4), 0, 32 * 12);
                ++textVars.menuSel;
            }
            if (KEYS & KEY_RIGHT) {
                for (int i = 0; i < 32 * 6; ++i)
                    ((u16 *)BG_MAP_RAM_SUB(2))[i] = 9;
                memset((void *)BG_MAP_RAM_SUB(4), 0, 32 * 12);
                textVars.menuSel += 6;
            }
            if (KEYS & KEY_UP) {
                for (int i = 0; i < 32 * 6; ++i)
                    ((u16 *)BG_MAP_RAM_SUB(2))[i] = 9;
                memset((void *)BG_MAP_RAM_SUB(4), 0, 32 * 12);
                --textVars.menuSel;
            }
            if (KEYS & KEY_LEFT) {
                for (int i = 0; i < 32 * 6; ++i)
                    ((u16 *)BG_MAP_RAM_SUB(2))[i] = 9;
                memset((void *)BG_MAP_RAM_SUB(4), 0, 32 * 12);
                textVars.menuSel -= 6;
            }

            if (textVars.menuSel < 0) {
                textVars.menuSel = 0;
                --virtualPosY;
                if (virtualPosY < 0)
                    virtualPosY = 0;
            } else if (textVars.menuSel > minSize - 1) {
                textVars.menuSel = minSize - 1;
                if (curPlayer->figureItemList.size() > 12) {
                    ++virtualPosY;
                    if (virtualPosY > (int)curPlayer->figureItemList.size() - 12)
                        virtualPosY = curPlayer->figureItemList.size() - 12;
                }
            }

            if (KEYS & KEY_A) {
                drawRectTop(3, 3, 24, 3);
                if (!curPlayer->figureItemList[textVars.menuSel + virtualPosY].equiped) {
                    removeItem(curPlayer,
                               curPlayer->figureItemList[textVars.menuSel + virtualPosY].itemIdx,
                               1);
                    curPlayer->Attr[GOLD] +=
                        curPlayer->figureItemList[textVars.menuSel + virtualPosY]
                            .itemData->itemPrice
                        / 2;
                    menuText(6, 4, "\3Successfully Sold!\3");
                } else {
                    if (curPlayer->figureItemList[textVars.menuSel + virtualPosY].quantity > 1) {
                        removeItem(
                            curPlayer,
                            curPlayer->figureItemList[textVars.menuSel + virtualPosY].itemIdx, 1);
                        curPlayer->Attr[GOLD] +=
                            curPlayer->figureItemList[textVars.menuSel + virtualPosY]
                                .itemData->itemPrice
                            / 2;
                        menuText(6, 4, "\3Successfully Sold!\3");
                    } else
                        menuText(6, 4, "\2 Item is Equiped! \2");
                }
            }
            break;
    }

    if (KEYS & KEY_B) {
        for (int i = 0; i < 32 * 6; ++i)
            ((u16 *)BG_MAP_RAM_SUB(2))[i] = 9;
        memset((void *)BG_MAP_RAM_SUB(4), 0, 32 * 32 * 2);
        if (curOption != OPTION_MAIN) {
            textVars.menuSel = curOption - 1;
            curOption        = OPTION_MAIN;
            menuText(15, 9, "Buy");
            menuText(20, 9, "Sell");
            menuText(25, 9, "Exit");
        } else
            textVars.menuSel = 2;
    }
}

void handleEvents()
{
    pEvent curEvent = &gameEvents[gameValues.curEvent];

    switch (curEvent->EventType) {
        case EVENT_GETITEM:
            for (int i = 0; i < (int)curPlayer->figureItemList.size(); ++i) {
                if (curPlayer->figureItemList[i].itemData == curEvent->eventParam)
                    ++gameValues.curEvent;
            }
            break;
        case EVENT_GOTOLEVEL:
            if (curLevel->levelIdx == (int)curEvent->eventParam)
                ++gameValues.curEvent;
            break;
        case EVENT_CUTSCENE:
            if (handleCutScene((int)curEvent->eventParam))
                ++gameValues.curEvent;
            break;
        case EVENT_TALKTO:
            if (textVars.curMode == TEXTMODE_IDLE) {
                if (gameValues.lstTalkedFigure == curEvent->eventParam)
                    ++gameValues.curEvent;
            }
            break;
        case EVENT_CUSTOM:
            if (((boolFuncPtr)curEvent->eventParam)())
                ++gameValues.curEvent;
            break;
        case EVENT_BATTLE:
            memcpy(&battleValues.curMonster, &monsterDB[(int)curEvent->eventParam],
                   sizeof(Monster));
            battleValues.curMonster.mnsSpr               = &spritesMain[127];
            battleValues.curMonster.mnsSpr->attribute[0] = ATTR0_ROTSCALE | ATTR0_COLOR_16;
            battleValues.curMonster.mnsSpr->attribute[1] = ATTR1_SIZE_64;
            battleValues.curMonster.mnsSpr->attribute[2] =
                ATTR2_PRIORITY(2) | ATTR2_PALETTE(1) | 128;
            hideRotScaleSprite(battleValues.curMonster.mnsSpr, true);
            rotateSprite(&spriteRotationsMain[0], 0);
            battleValues.isBoss = true;
            gameValues.curMode  = LEVELMODE_BATTLE_FADEIN;
            ++gameValues.curEvent;
            break;
        case EVENT_END:
            REG_IME = 0;
            swiSoftReset();
        default:
            break;
    }
}

void handleShopAnswer(bool shopAnswer)
{
    memset(textVars.textMap, 0, 32 * 32 * 2);

    switch (shopAnswer) {
        case true:
            for (int i = 0; i < 32 * 32; i++)
                ((u16 *)BG_MAP_RAM_SUB(2))[i] = 9;
            textVars.scroolY = 113;
            textVars.menuSel = 0;
            textVars.curMode = TEXTMODE_SHOP;
            break;
        case false:
            textVars.txtLen = 29;
            strcpy(textVars.txtBuffer, "Come back later, will ya lad?");
            textVars.curMode = TEXTMODE_WRITING;
            // Init vars
            textVars.posX        = 0;
            textVars.virtualPosY = textVars.posY = 19;
            textVars.txtPos                      = 0;
            break;
    }
}

bool handleCutScene(int curCutscene)
{
    pCutSceneAction curEvent = &cutScenes[curCutscene][gameValues.curCutsceneIdx];

    if (gameValues.curCutsceneIdx == 0) {
        performCutsceneAction(curEvent);
        ++gameValues.curCutsceneIdx;
    } else {
        if (isOverAction(&cutScenes[curCutscene][gameValues.curCutsceneIdx - 1])) {
            performCutsceneAction(curEvent);
            ++gameValues.curCutsceneIdx;
        }
    }
    if (curEvent->eventType == CS_END) {
        if (isOverAction(&cutScenes[curCutscene][gameValues.curCutsceneIdx - 1])) {
            gameValues.curCutsceneIdx = 0;
            return true;
        }
    }

    return false;
}

bool handleEquip(pFigure Figure, itemAttr itemType, int itemIdx)
{
    // There's something in here...
    if (Figure->equipedItems[itemType] != -1) {
        // In any case, removing the old attributes
        for (int i = 0; i < ATTR_SIZE; ++i)
            Figure->Attr[i] -= itemDB[Figure->equipedItems[itemType]].itemChanges[i];

        // Remove item
        if (Figure->equipedItems[itemType] == itemIdx) {
            setItemEquiped(Figure, Figure->equipedItems[itemType], false);
            Figure->equipedItems[itemType] = -1;
            return false;
        } else { // Replace item
            setItemEquiped(Figure, Figure->equipedItems[itemType], false);
            Figure->equipedItems[itemType] = itemIdx;
            setItemEquiped(Figure, Figure->equipedItems[itemType], true);
            return true;
        }
        // Item is blank. Let's fill it
    } else {
        Figure->equipedItems[itemType] = itemIdx;
        setItemEquiped(Figure, Figure->equipedItems[itemType], true);

        return true;
    }
}

void handleBattleChoice(int battleSel)
{
    switch (battleSel) {
        case 0: // Attack
            hideSprite(textVars.cursorSprite, true);
            hideSprite(textVars.cursorSpriteMain, false);
            moveSprite(textVars.cursorSpriteMain, 8, (192 - 64) / 2 + 32);
            battleValues.curMode = BATTLE_PLAYERCHOICE_MONSTER;
            break;
        case 1: // Magic

            break;
        case 2: // Item
            textVars.menuSel = 0;
            battleValues.battleItems.clear();
            for (itemList::iterator i = curPlayer->figureItemList.begin();
                 i != curPlayer->figureItemList.end(); ++i) {
                if ((*i).itemData->itemType == POTION)
                    battleValues.battleItems.push_back((*i));
            }
            if (battleValues.battleItems.empty())
                return;
            drawRectTop(3, 18, 13, 2 + battleValues.battleItems.size());
            for (u32 i = 0; i < battleValues.battleItems.size(); ++i)
                menuText(4, 5 + i, "%s", battleValues.battleItems[i].itemData->itemName);
            battleValues.curMode = BATTLE_CHOOSEITEM;
            break;
        case 3: // Flee
            if (battleValues.isBoss)
                break;

            gameValues.fadeVal = 0;
            for (int i = 0; i < 256; i += 16, SPRITE_PALETTE[i] = 0)
                ;
            memset(textVars.textMap, 0, 32 * 32 * 2);
            hideSprite(textVars.cursorSprite, true);
            hideSprite(textVars.cursorSpriteMain, true);
            curPlayer->Attr[GOLD] -= battleValues.curMonster.Attr[GOLD] / 2;
            curPlayer->Attr[EXP] -= battleValues.curMonster.Attr[EXP] / 2;
            if (curPlayer->Attr[GOLD] < 0)
                curPlayer->Attr[GOLD] = 0;
            if (curPlayer->Attr[EXP] < 0)
                curPlayer->Attr[EXP] = 0;
            hideRotScaleSprite(battleValues.curMonster.mnsSpr, true);
            hideSprite(battleValues.mnsGuage.indSpr, true);
            hideSprite(battleValues.playerG.indSpr, true);
            undrawRectTop(0, 0, 32, 32);
            clearMenuText(0, 0, 32, 10);
            drawRectTop(0, 0, 32, 7);
            textVars.curMode   = TEXTMODE_WINDOW_DIRDOWN;
            gameValues.curMode = LEVELMODE_EXITBATTLE;
            break;
    }
}

void handleBattleKeys()
{
    switch (battleValues.curMode) {
        case BATTLE_GAUGESFILLS:
            break;
        case BATTLE_PLAYERGAUGEFULL:
            break;
        case BATTLE_MONSTERGAUGEFULL:
            break;
        case BATTLE_PLAYERCHOICE:
            if (KEYS & KEY_DOWN) {
                ++textVars.menuSel;
                if (textVars.menuSel > 3)
                    textVars.menuSel = 3;
            } else if (KEYS & KEY_UP) {
                --textVars.menuSel;
                if (textVars.menuSel < 0)
                    textVars.menuSel = 0;
            }
            if (KEYS & KEY_A)
                handleBattleChoice(textVars.menuSel);
            break;
        case BATTLE_PLAYERCHOICE_MONSTER:
            if (KEYS & KEY_B) {
                hideSprite(textVars.cursorSprite, false);
                hideSprite(textVars.cursorSpriteMain, true);
                battleValues.curMode = BATTLE_PLAYERGAUGEFULL;
            }
            if (KEYS & KEY_A) {
                hideSprite(textVars.cursorSpriteMain, true);
                if (curPlayer->equipedItems[WEAPON] != -1) {
                    initWeapon(&battleValues.activeWeapon,
                               itemDB[curPlayer->equipedItems[WEAPON]].itemType);
                    moveSprite(battleValues.activeWeapon.wpnSpr, 208 - 24, 108);
                }
                curPlayer->isMotion  = false;
                battleValues.curMode = BATTLE_PLAYERACTION;
            }
            break;
        case BATTLE_PLAYERACTION:
            break;
        case BATTLE_MONSTERACTION:
            break;
        case BATTLE_SHOWRESULT_PLAYER:
            break;
        case BATTLE_SHOWRESULT_MONSTER:
            break;
        case BATTLE_ELIMINATEMONSTER:
            break;
        case BATTLE_VICTORY:
            if (KEYS & KEY_A) {
                if (battleValues.deltaLevel) {
                    textVars.menuSel = 0;
                    hideSprite(textVars.cursorSprite, false);
                    battleValues.curMode = BATTLE_UPGRADESTATS;
                } else {
                    undrawRectTop(1, 18, 30, 10);
                    gameValues.fadeVal = 8;
                    for (int i = 0; i < 256; i += 16, SPRITE_PALETTE[i] = 0)
                        ;
                    memset(textVars.textMap, 0, 32 * 32 * 2);
                    gameValues.curMode = LEVELMODE_EXITBATTLE;
                }
            }
            break;
        case BATTLE_UPGRADESTATS:
            if (KEYS & KEY_DOWN)
                ++textVars.menuSel;
            if (KEYS & KEY_UP)
                --textVars.menuSel;

            if (KEYS & KEY_A) {
                ++curPlayer->Attr[textVars.menuSel + 4];
                --battleValues.deltaLevel;
                if (!battleValues.deltaLevel) {
                    undrawRectTop(1, 18, 30, 10);
                    gameValues.fadeVal = 8;
                    for (int i = 0; i < 256; i += 16, SPRITE_PALETTE[i] = 0)
                        ;
                    memset(textVars.textMap, 0, 32 * 32 * 2);
                    hideSprite(textVars.cursorSprite, true);
                    gameValues.curMode = LEVELMODE_EXITBATTLE;
                }
            }
            break;
        case BATTLE_CHOOSEITEM:
            if (KEYS & KEY_DOWN)
                ++textVars.menuSel;
            if (KEYS & KEY_UP)
                --textVars.menuSel;

            if (KEYS & KEY_A) {
                useItem(curPlayer, battleValues.battleItems[textVars.menuSel].itemIdx);
                battleValues.hpDec =
                    battleValues.battleItems[textVars.menuSel].itemData->itemChanges[HP];
                battleValues.animIdx  = 0;
                battleValues.animTick = 3;
                undrawRectTop(3, 18, 13, 2 + battleValues.battleItems.size());
                clearMenuText(0, 0, 32, 32);
                battleValues.curMode = BATTLE_ITEMEFFECT;
            }
            if (KEYS & KEY_B) {
                undrawRectTop(3, 18, 13, 2 + battleValues.battleItems.size());
                clearMenuText(0, 0, 32, 32);
                textVars.menuSel     = 2;
                battleValues.curMode = BATTLE_PLAYERCHOICE;
            }
            break;
        case BATTLE_ITEMEFFECT:
            break;
    }
}

void handleBattleMenu()
{
    const char *battleMenu[] = {
        "Attack",
        "\5Magic\5",
        "Item",
        "Flee",
    };
    if (battleValues.isBoss)
        battleMenu[3] = "\5Flee\5";
    if (curPlayer->figureItemList.empty())
        battleMenu[2] = "\5Item\5";

    moveSprite(textVars.cursorSprite, 19 * 8 - 16, (12 + textVars.menuSel) * 8);
    for (int i = 0; i < 4; ++i) {
        if (textVars.menuSel == i)
            menuText(19, 12 + i, "\4%s\4", battleMenu[i]);
        else
            menuText(19, 12 + i, "%s", battleMenu[i]);
    }
}

void handleBattleResults()
{
    if (curPlayer->Attr[HP] < 1)
        gameValues.curMode = LEVELMODE_GAMEOVER_FADEIN;

    if (battleValues.curMonster.Attr[HP] < 1) {
        // Re-use those vars!
        gameValues.flowerAngle = 0;
        gameValues.textZoom    = 256;
        battleValues.curMode   = BATTLE_ELIMINATEMONSTER;
    }
}

void handleBattle()
{
    switch (battleValues.curMode) {
        case BATTLE_GAUGESFILLS:
            REG_BG1VOFS        = 0;
            battleValues.isSet = false;
            printIndicatorName(17, 19, curPlayer->figName.c_str(), curPlayer->Attr[HP],
                               curPlayer->Attr[MAX_HP]);
            printIndicatorName(1, 19, battleValues.curMonster.name,
                               battleValues.curMonster.Attr[HP],
                               battleValues.curMonster.Attr[MAX_HP]);
            handleBattleResults();
            updateGauge(&battleValues.playerG);
            updateGauge(&battleValues.mnsGuage);

            if (battleValues.playerG.isFull) {
                if (curPlayer->Attr[HP] > 1) {
                    drawRectTop(18, 25, 8, 6);
                    hideSprite(textVars.cursorSprite, false);
                    moveSprite(textVars.cursorSprite, 19 * 8 - 16, 12 * 8);
                    battleValues.curMode = BATTLE_PLAYERGAUGEFULL;
                }
            } else if (battleValues.mnsGuage.isFull) {
                if (battleValues.curMonster.Attr[HP] > 1) {
                    battleValues.curMode  = BATTLE_MONSTERACTION;
                    battleValues.animTick = 3;
                    curPlayer->isMotion   = false;
                    battleValues.isSet    = false;
                }
            }
            break;
        case BATTLE_PLAYERGAUGEFULL:
            // Init?
            battleValues.curMode = BATTLE_PLAYERCHOICE;
            break;
        case BATTLE_MONSTERGAUGEFULL:
            break;
        case BATTLE_PLAYERCHOICE:
            printIndicatorName(17, 19, curPlayer->figName.c_str(), curPlayer->Attr[HP],
                               curPlayer->Attr[MAX_HP]);
            printIndicatorName(1, 19, battleValues.curMonster.name,
                               battleValues.curMonster.Attr[HP],
                               battleValues.curMonster.Attr[MAX_HP]);
            handleBattleMenu();
            break;
        case BATTLE_PLAYERCHOICE_MONSTER:
            break;
        case BATTLE_PLAYERACTION:
            undrawRectTop(18, 25, 8, 6);
            clearMenuText(19, 12, 8, 5);
            if (curPlayer->equipedItems[WEAPON]) {
                upadteWeapon(&battleValues.activeWeapon);
                if (!battleValues.activeWeapon.isActive) {
                    battleValues.animTick = 3;
                    curPlayer->isMotion   = true;
                    battleValues.curMode  = BATTLE_SHOWRESULT_PLAYER;
                    resetGuage(&battleValues.playerG);
                }
            } else {
                curPlayer->isMotion = true;
                resetGuage(&battleValues.playerG);
                battleValues.animTick = 3;
                battleValues.isSet    = false;
                battleValues.curMode  = BATTLE_SHOWRESULT_PLAYER;
            }
            break;
        case BATTLE_SHOWRESULT_PLAYER:
            if (!battleValues.isSet) {
                battleValues.hpDec = curPlayer->Attr[STR] * 10;
                battleValues.hpDec += rand() % (curPlayer->Attr[DEX] + curPlayer->Attr[AGI]);
                battleValues.hpDec -= battleValues.curMonster.Attr[DEF] * 5;
                battleValues.hpDec = (battleValues.hpDec < 1) ? 1 : battleValues.hpDec;
                textPutOn(false, (u16 *)BG_MAP_RAM(25), 8, 8, "\2%d\2", battleValues.hpDec);
                battleValues.isSet = true;
            }
            if (!battleValues.animTick--) {
                REG_BG1VOFS = battleValues.animIdx++;
                if (battleValues.animIdx > 8) {
                    REG_BG1VOFS = battleValues.animIdx = 0;
                    clearTextOn((u16 *)BG_MAP_RAM(25), 8, 8, 10, 1);
                    battleValues.curMonster.Attr[HP] -= battleValues.hpDec;
                    battleValues.isSet   = false;
                    battleValues.curMode = BATTLE_GAUGESFILLS;
                }
                if (battleValues.animIdx & 1) {
                    int sprPal = battleValues.curMonster.mnsSpr->attribute[2] >> 12;
                    int redCol = RGB15(31, 0, 0);
                    swiCopy(&redCol, SPRITE_PALETTE + sprPal * 16, 16 | COPY_MODE_FILL);
                } else {
                    int sprPal = battleValues.curMonster.mnsSpr->attribute[2] >> 12;
                    swiCopy(battleValues.curMonster.sprPal, SPRITE_PALETTE + sprPal * 16, 16);
                }
                battleValues.animTick = (15 - battleValues.animIdx) / 3;
            }
            break;
        case BATTLE_SHOWRESULT_MONSTER:
            textPutOn(false, (u16 *)BG_MAP_RAM(25), 26, 14, "\2%d\2", battleValues.hpDec);
            if (!battleValues.animTick--) {
                REG_BG1VOFS = battleValues.animIdx++;
                if (battleValues.animIdx > 8) {
                    battleValues.animIdx = 0;
                    curPlayer->Attr[HP] -= battleValues.hpDec;
                    resetGuage(&battleValues.mnsGuage);
                    clearTextOn((u16 *)BG_MAP_RAM(25), 26, 14, 10, 1);
                    curPlayer->isMotion  = true;
                    battleValues.curMode = BATTLE_GAUGESFILLS;
                }
                if (battleValues.animIdx & 1) {
                    int Col = RGB15(31, 0, 0);
                    swiCopy(&Col, SPRITE_PALETTE + curPlayer->palOffset * 16, 16 | COPY_MODE_FILL);
                } else
                    swiCopy(curPlayer->palData, SPRITE_PALETTE + curPlayer->palOffset * 16, 16);
                battleValues.animTick = (15 - battleValues.animIdx) / 3;
            }
            break;
        case BATTLE_MONSTERACTION:
            if (!battleValues.isSet) {
                battleValues.hpDec = battleValues.curMonster.Attr[STR] * 10;
                battleValues.hpDec +=
                    rand()
                    % (battleValues.curMonster.Attr[DEX] + battleValues.curMonster.Attr[AGI]);
                battleValues.hpDec -= curPlayer->Attr[DEF] * 5;
                battleValues.hpDec = (battleValues.hpDec < 1) ? 1 : battleValues.hpDec;
                battleValues.isSet = true;
            }
            if (!battleValues.animTick--) {
                ++battleValues.animIdx;
                if (battleValues.animIdx > 8) {
                    battleValues.animIdx  = 0;
                    battleValues.animTick = 3;
                    battleValues.curMode  = BATTLE_SHOWRESULT_MONSTER;
                }

                if (battleValues.animIdx & 1) {
                    int sprPal = battleValues.curMonster.mnsSpr->attribute[2] >> 12;
                    int Col    = RGB15(31, 31, 31);
                    swiCopy(&Col, SPRITE_PALETTE + sprPal * 16, 16 | COPY_MODE_FILL);
                } else {
                    int sprPal = battleValues.curMonster.mnsSpr->attribute[2] >> 12;
                    swiCopy(battleValues.curMonster.sprPal, SPRITE_PALETTE + sprPal * 16, 16);
                }
                battleValues.animTick = 3;
            }
            break;
        case BATTLE_ELIMINATEMONSTER:
            rotScaleSprite(&spriteRotationsMain[0], gameValues.flowerAngle += 16,
                           gameValues.textZoom += 64);
            SoundSendCmd(SND_CMD_SETVOLUME, 127 - (gameValues.flowerAngle >> 2));
            if (gameValues.textZoom > 256 * 10) {
                SndPlayMOD(mscWin_mod);
                curPlayer->isMotion = false;
                hideRotScaleSprite(battleValues.curMonster.mnsSpr, true);
                battleValues.curMode = BATTLE_VICTORY;
                clearMenuText(0, 19, 32, 8);
                drawRectTop(0, 0, 32, 7);
                hideSprite(battleValues.mnsGuage.indSpr, true);
                hideSprite(battleValues.playerG.indSpr, true);
                SoundSendCmd(SND_CMD_SETVOLUME, 127);
                REG_BLDCNT = BLEND_FADE_BLACK | BLEND_SRC_SPRITE | BLEND_SRC_BG0 | BLEND_SRC_BG2
                             | BLEND_SRC_BG3 | BLEND_DST_BACKDROP;
                REG_BLDY           = 8 | 8 << 8; // 50/50 Blend with black
                battleValues.isSet = false;
                textVars.curMode   = TEXTMODE_WINDOW_DIRDOWN;
            }
            break;
        case BATTLE_VICTORY:
            if (textVars.curMode == TEXTMODE_IDLE) {
                if (!battleValues.isSet) {
                    drawRectTop(1, 18, 30, 10);
                    menuText(12, 12, "EXP : %d", curPlayer->Attr[EXP]);
                    menuText(8, 13, "EXP Gained : %d", battleValues.curMonster.Attr[EXP]);

                    curPlayer->Attr[EXP] += battleValues.curMonster.Attr[EXP];
                    curPlayer->Attr[GOLD] += battleValues.curMonster.Attr[GOLD];
                    battleValues.deltaLevel = updateFigureStates(curPlayer);

                    menuText(5, 14, "EXP To next level : %d",
                             curPlayer->Attr[EXP_TO_NEXT] - curPlayer->Attr[EXP]);
                    if (battleValues.deltaLevel)
                        menuText(7, 16, "\3New Level Achieved!\3");

                    menuText(8, 18, "Gold Gained : %d", battleValues.curMonster.Attr[GOLD]);
                    battleValues.isSet = true;
                }
            }
            break;
        case BATTLE_UPGRADESTATS:
            if (textVars.menuSel < 0)
                textVars.menuSel = 5;
            if (textVars.menuSel > 5)
                textVars.menuSel = 0;
            moveSprite(textVars.cursorSprite, 21 * 8, 16 + 8 * textVars.menuSel);
            break;
        case BATTLE_CHOOSEITEM:
            if (textVars.menuSel < 0)
                textVars.menuSel = 0;
            if (textVars.menuSel > (int)battleValues.battleItems.size() - 1)
                textVars.menuSel = battleValues.battleItems.size() - 1;
            moveSprite(textVars.cursorSprite, 16, 5 * 8 + 8 * textVars.menuSel);
            break;
        case BATTLE_ITEMEFFECT:
            resetGuage(&battleValues.playerG);
            hideSprite(textVars.cursorSprite, true);
            undrawRectTop(3, 18, 13, 2 + battleValues.battleItems.size());
            clearMenuText(0, 0, 32, 32);
            undrawRectTop(18, 25, 8, 6);
            battleValues.curMode = BATTLE_GAUGESFILLS;
            break;
    }
}
