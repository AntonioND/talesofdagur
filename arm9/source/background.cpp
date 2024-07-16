#include "Sound9.h"
#include "background.h"
#include "events.h"
#include "handlers.h"
#include "text.h"

// Data
pLevel curLevel;
gameVars gameValues;
battleVars battleValues;
Level Levels[LEVEL_SIZE];
Level battleLevel;

Map levelMaps[LEVEL_SIZE * 3] = {
    levelData(1),  levelData(2),  levelData(3),  levelData(4),  levelData(5),  levelData(6),
    levelData(7),  levelData(8),  levelData(9),  levelData(10), levelData(11), levelData(12),
    levelData(13), levelData(14), levelData(15), levelData(16), levelData(17), levelData(18),
};

u16 *levelPalettes[LEVEL_SIZE] = {
    levelPal(1),  levelPal(2),  levelPal(3),  levelPal(4),  levelPal(5),  levelPal(6),
    levelPal(7),  levelPal(8),  levelPal(9),  levelPal(10), levelPal(11), levelPal(12),
    levelPal(13), levelPal(14), levelPal(15), levelPal(16), levelPal(17), levelPal(18),
};

u8 *levelCollisions[LEVEL_SIZE] = {
    levelCollision(1),  levelCollision(2),  levelCollision(3),  levelCollision(4),
    levelCollision(5),  levelCollision(6),  levelCollision(7),  levelCollision(8),
    levelCollision(9),  levelCollision(10), levelCollision(11), levelCollision(12),
    levelCollision(13), levelCollision(14), levelCollision(15), levelCollision(16),
    levelCollision(17), levelCollision(18),
};

const u32 levelSizes[LEVEL_SIZE * 2] = {
    480,  480,  480,  320,  256, 192, 256,  192,  320,  320,  1280, 1280,
    1280, 1280, 1280, 1280, 256, 192, 256,  192,  1280, 1280, 1280, 1280,
    320,  256,  320,  320,  320, 320, 1280, 1280, 320,  320,  416,  416,
};

#if 0
const bool levelAnimation[LEVEL_SIZE] = {
    false, false, false, false, false, true, true, true,  false,
    false, false, true,  false, false, true, true, false, false,
};

Map animLevelTiles[LEVEL_SIZE * 4] = {
    NO_ANIMATION,       NO_ANIMATION,       NO_ANIMATION,      NO_ANIMATION, NO_ANIMATION,
    levelAnimation(6),  levelAnimation(7),  levelAnimation(8), NO_ANIMATION, NO_ANIMATION,
    NO_ANIMATION,       levelAnimation(12), NO_ANIMATION,      NO_ANIMATION, levelAnimation(15),
    levelAnimation(16), NO_ANIMATION,       NO_ANIMATION,
};
#endif

pLevelLink levelLinking[LEVEL_SIZE] = {
    level1Link,  level2Link,  level3Link,  level4Link,  level5Link,  level6Link,
    level7Link,  level8Link,  level9Link,  level10Link, level11Link, level12Link,
    level13Link, level14Link, level15Link, level16Link, level17Link, level18Link,
};

pDoorLink doorLinking[LEVEL_SIZE] = {
    doorLink1,  doorLink2,  doorLink3,  doorLink4,  doorLink5,  doorLink6,
    doorLink7,  doorLink8,  doorLink9,  doorLink10, doorLink11, doorLink12,
    doorLink13, doorLink14, doorLink15, doorLink16, doorLink17, doorLink18,
};

const u8 *bgMusic[LEVEL_SIZE] = {
    mscPalace_mod, mscPalace_mod, mscPalace_mod, mscPalace_mod, mscPalace_mod, mscBeach_mod,
    mscTown_mod,   mscWoods_mod,  mscBeach_mod,  mscBeach_mod,  mscWoods_mod,  mscBeach_mod,
    mscTown_mod,   mscCave_mod,   mscCave_mod,   mscWoods_mod,  mscCave_mod,   mscCave_mod,
};

const void *battleMapsPals[LEVEL_SIZE * 2] = {
    NULL,
    NULL,
    NULL,
    NULL,
    battleCastleTiles,
    battleCastlePal,
    NULL,
    NULL,
    NULL,
    NULL,
    battleBeachTiles,
    battleBeachPal,
    battleTownTiles,
    battleTownPal,
    battleWoodsTiles,
    battleWoodsPal,
    NULL,
    NULL,
    NULL,
    NULL,
    battleWoodsTiles,
    battleWoodsPal,
    battleBeachTiles,
    battleBeachPal,
    NULL,
    NULL,
    battleICaveTiles,
    battleICavePal,
    battleFCaveTiles,
    battleFCavePal,
    battleWoodsTiles,
    battleWoodsPal,
    battleECaveTiles,
    battleECavePal,
    battleECastleTiles,
    battleECastlePal,
};

int levelMonsters[] = {
    -1, -1, -1, -1, -1, -1, -1, 2, -1, -1, 3, 4, -1, -1, -1, 8, -1, -1,
};

void initLevels()
{
    gameValues.flowerAngle = 0;
    gameValues.textZoom    = 0;
    gameValues.isInit      = false;
    for (int i = 0; i < LEVEL_SIZE; i++) {
        Levels[i].groundMap = &levelMaps[i * 3];
        Levels[i].upMap     = &levelMaps[i * 3 + 1];
        Levels[i].downMap   = &levelMaps[i * 3 + 2];
        Levels[i].colMap    = levelCollisions[i];

        Levels[i].dimX     = levelSizes[i * 2];
        Levels[i].dimY     = levelSizes[i * 2 + 1];
        Levels[i].viewX    = 0;
        Levels[i].viewY    = 0;
        Levels[i].levelPal = levelPalettes[i];

        // Levels[i].isAnimated = levelAnimation[i];
        // if(Levels[i].isAnimated) {
        //     Levels[i].animData[0] = &animLevelTiles[i*4];
        //     Levels[i].animData[1] = &animLevelTiles[i*4 + 1];
        //     Levels[i].animData[2] = &animLevelTiles[i*4 + 2];
        //     Levels[i].animData[3] = &animLevelTiles[i*4 + 3];
        // } else
        //     memset(Levels[i].animData, 0, sizeof(Map)*4);

        // Levels[i].animIndex = 0;
        Levels[i].linkedLevels = levelLinking[i];
        Levels[i].linkedDoors  = doorLinking[i];

        Levels[i].battleTiles = (u8 *)battleMapsPals[i * 2];
        Levels[i].battlePal   = (u8 *)battleMapsPals[i * 2 + 1];
        Levels[i].battleMon   = levelMonsters[i];
    }
    curLevel          = &Levels[0];
    battleLevel.dimX  = 256;
    battleLevel.dimY  = 192;
    battleLevel.viewX = battleLevel.viewY = 0;
}

// BIOS Function helper
int getSize(uint8 *source, uint16 *dest, uint32 r2)
{
    u32 size = *((u32 *)source) >> 8;
    return (size << 8) | 16;
}

// BIOS Function helper
uint8 readByte(uint8 *source)
{
    return *source++;
}

// Decompresses to VRAM using BIOS LZ77 Compression
void decompressToVRAM(const void *source, void *dest)
{
    TDecompressionStream decStream = { getSize, NULL, readByte };
    swiDecompressLZSSVram((void *)source, dest, 0, &decStream);
}

void loadLevel(LEVELS lvlIdx)
{
    updateMusic(lvlIdx);

    curLevel = &Levels[lvlIdx];

    curLevel->levelIdx = lvlIdx;

    dmaCopy(curLevel->levelPal, BG_PALETTE, 256 * 2);
    decompressToVRAM(curLevel->groundMap->tileData, (void *)BG_TILE_RAM(0));
    decompressToVRAM(curLevel->downMap->tileData, (void *)BG_TILE_RAM(5));
    decompressToVRAM(curLevel->upMap->tileData, (void *)BG_TILE_RAM(10));
    // dmaCopy(curLevel->groundMap->tileData, (void*)BG_TILE_RAM(0), curLevel->groundMap->tileSize);
    // dmaCopy(curLevel->downMap->tileData, (void*)BG_TILE_RAM(5), curLevel->downMap->tileSize);
    // dmaCopy(curLevel->upMap->tileData, (void*)BG_TILE_RAM(10), curLevel->upMap->tileSize);
}

// Updates view by player's position
void updateView()
{
    int playerPosX = curPlayer->posX;
    int playerPosY = curPlayer->posY;

    if (playerPosX > curLevel->dimX - 128 - curPlayer->dimX / 2)
        playerPosX = curLevel->dimX - 256;
    else if (playerPosX > 128 - curPlayer->dimX / 2)
        playerPosX -= 128 - curPlayer->dimX / 2;
    else
        playerPosX = 0;

    if (playerPosY > curLevel->dimY - 96 - curPlayer->dimY / 2)
        playerPosY = curLevel->dimY - 192;
    else if (playerPosY > 96 - curPlayer->dimY / 2)
        playerPosY -= 96 - curPlayer->dimY / 2;
    else
        playerPosY = 0;

    curLevel->viewX = playerPosX;
    curLevel->viewY = playerPosY;

#if 0
    if (curLevel->isAnimated) {
        if (frameCounter % 12 == 0)
            ++curLevel->animIndex;
        dmaCopy(curLevel->animData[curLevel->animIndex % 4]->tileData, (void *)BG_TILE_RAM(0),
                curLevel->animData[curLevel->animIndex % 4]->tileSize);
        backgroundUpdateView((u16 *)BG_MAP_RAM(31),
                             (u16 *)curLevel->animData[curLevel->animIndex % 4]->mapData);
    } else
#endif
    backgroundUpdateView((u16 *)BG_MAP_RAM(31), (u16 *)curLevel->groundMap->mapData);

    backgroundUpdateView((u16 *)BG_MAP_RAM(25), (u16 *)curLevel->upMap->mapData);
    backgroundUpdateView((u16 *)BG_MAP_RAM(28), (u16 *)curLevel->downMap->mapData);

    REG_BG0HOFS = curLevel->viewX % 8;
    REG_BG0VOFS = curLevel->viewY % 8;
    REG_BG2HOFS = curLevel->viewX % 8;
    REG_BG2VOFS = curLevel->viewY % 8;
    REG_BG3HOFS = curLevel->viewX % 8;
    REG_BG3VOFS = curLevel->viewY % 8;
}

void backgroundUpdateView(u16 *workMap, const u16 *targetMap)
{
    // First chunk map
    for (u32 x = 0; x < 32; x++) {
        for (u32 y = 0; y < 32; y++) {
            workMap[x + y * 32] = targetMap[(x + curLevel->viewX / 8)
                                            + (y + curLevel->viewY / 8) * (curLevel->dimX / 8)];
        }
    }
    // Second chunk map
    workMap += 32 * 31;
    for (u32 x = 32; x < 64; x++) {
        for (u32 y = 0; y < 32; y++) {
            workMap[x + y * 32] = targetMap[(x + curLevel->viewX / 8)
                                            + (y + curLevel->viewY / 8) * (curLevel->dimX / 8)];
        }
    }
}

void graphicInitMainMenu()
{
    initOAM();

    // Return the screens as they were
    lcdMainOnBottom();

    videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE | DISPLAY_SPR_ACTIVE
                 | DISPLAY_SPR_1D);
    videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG3_ACTIVE);

    REG_BG0CNT = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(31) | BG_TILE_BASE(0) | 0; // Text
    REG_BG1CNT = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(30) | BG_TILE_BASE(1) | 3; // Boxs

    REG_BG0CNT_SUB = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(5) | BG_TILE_BASE(0) | 3; // Figure HBlank
    REG_BG3CNT_SUB = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(3) | BG_TILE_BASE(2) | 1; // Text Hblank

    decompressToVRAM(textBoxData_bin, (void *)BG_TILE_RAM(1));
    dmaCopy(textBoxPal_bin, BG_PALETTE, 16);
    decompressToVRAM(textFontTiles, (void *)BG_TILE_RAM(0));
    undrawRectOn((u16 *)BG_MAP_RAM(30), 0, 0, 32, 32);
    BG_PALETTE[17] = RGB15(0, 25, 25);
    BG_PALETTE[33] = RGB15(31, 0, 0);
    BG_PALETTE[49] = RGB15(0, 20, 7);
    BG_PALETTE[65] = RGB15(28, 28, 9);
    BG_PALETTE[81] = RGB15(20, 20, 20);
    for (int i = 18; i < 256; i += 16)
        BG_PALETTE[i] = 0;
    BG_PALETTE[0] = RGB15(31, 31, 31);

    decompressToVRAM(mainMenuBackTiles, (void *)BG_TILE_RAM_SUB(0));
    decompressToVRAM(mainMenuBackMap, (void *)BG_MAP_RAM_SUB(5));
    decompressToVRAM(mainMenuFrontTiles, (void *)BG_TILE_RAM_SUB(2));
    decompressToVRAM(mainMenuFrontMap, (void *)BG_MAP_RAM_SUB(3));
    BG_PALETTE_SUB[0] = RGB15(31, 31, 31);

    dmaCopy(spr16PalPal, SPRITE_PALETTE, spr16PalPalLen);
    dmaCopy(textCursorTiles, SPRITE_GFX, textCursorTilesLen);

    textVars.cursorSprite               = &spritesMain[2];
    textVars.cursorSprite->attribute[0] = ATTR0_NORMAL | ATTR0_COLOR_16;
    textVars.cursorSprite->attribute[1] = ATTR1_SIZE_16;
    textVars.cursorSprite->attribute[2] = ATTR2_PRIORITY(0);
    hideSprite(textVars.cursorSprite, true);
}

void graphicInitSplash()
{
    // Set to MODE0 and Sprites
    videoSetMode(MODE_5_2D | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE | DISPLAY_SPR_ACTIVE
                 | DISPLAY_SPR_1D);
    videoSetModeSub(MODE_5_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG3_ACTIVE);

    // Init Graphic Mode on main core
    REG_BG2CNT = BG_BMP8_256x256 | BG_BMP_BASE(0) | 2; // Flower
    REG_BG3CNT = BG_BMP8_256x256 | BG_BMP_BASE(4) | 0; // Text
                                                       // Init sub core with text
    REG_BG0CNT_SUB = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(31) | BG_TILE_BASE(0) | 3;
    REG_BG3CNT_SUB = BG_BMP8_128x128 | BG_BMP_BASE(2);

    REG_BG3PA = 1 << 8;
    REG_BG3PB = 0;
    REG_BG3PC = 0;
    REG_BG3PD = 1 << 8;

    // Copy the text & setup the palette
    decompressToVRAM(stamp1stBitmap, (void *)BG_BMP_RAM_SUB(2));
    dmaCopy(stamp1stPal, BG_PALETTE_SUB, stamp1stPalLen);

    decompressToVRAM(textFontTiles, (void *)BG_TILE_RAM_SUB(0));
    BG_PALETTE_SUB[17] = RGB15(0, 25, 25);
    BG_PALETTE_SUB[18] = RGB15(0, 0, 0);

    textPutOn(true, (u16 *)BG_MAP_RAM_SUB(31), 7, 12, "Touch to continue");

    // and have the splashscreen on the top screen
    lcdMainOnTop();

    // Fill in the rotation values for the first time
    handleSplash();
}

void graphicInitGOver()
{
    // Set to MODE0 and Sprites
    videoSetMode(MODE_0_2D | DISPLAY_SPR_1D | DISPLAY_SPR_ACTIVE);
    videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);

    // Init Graphic Mode on sub core
    REG_BG0CNT_SUB = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(31) | BG_TILE_BASE(0) | 1;
}

void graphicInitGame()
{
    // Set to MODE0 and Sprites
    videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG2_ACTIVE | DISPLAY_BG3_ACTIVE
                 | DISPLAY_SPR_1D | DISPLAY_SPR_ACTIVE);
    videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE | DISPLAY_BG2_ACTIVE
                    | DISPLAY_BG3_ACTIVE | DISPLAY_SPR_1D | DISPLAY_SPR_ACTIVE);

    // Init Graphic Mode on main core
    REG_BG0CNT = BG_64x32 | BG_COLOR_256 | BG_MAP_BASE(28) | BG_TILE_BASE(5) | 3;  // Down
    REG_BG2CNT = BG_64x32 | BG_COLOR_256 | BG_MAP_BASE(25) | BG_TILE_BASE(10) | 0; // Up
    REG_BG3CNT = BG_64x32 | BG_COLOR_256 | BG_MAP_BASE(31) | BG_TILE_BASE(0) | 3;  // Ground

    // Init Graphic Mode on sub core
    REG_BG0CNT_SUB = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(2) | BG_TILE_BASE(0) | 1; // TextBox
    REG_BG1CNT_SUB = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(3) | BG_TILE_BASE(1) | 2; // Text lower
    REG_BG2CNT_SUB = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(4) | BG_TILE_BASE(1) | 0; // Text upper
    REG_BG3CNT_SUB = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(5) | BG_TILE_BASE(0) | 3; // Main Screen
}

void initGauge(pGauge workGauge, int posX, int posY, int gRate)
{
    workGauge->gTimer = 0;
    workGauge->gTick  = 10 - gRate;
    workGauge->posX   = posX;
    workGauge->posX   = posY;
    workGauge->gRate  = gRate;
    workGauge->isFull = false;
    workGauge->isHalt = true;

    int curSpr = 127;
    while (curSpr) {
        if (spritesSub[curSpr].attribute[0] & ATTR0_DISABLED)
            break;
        else
            --curSpr;
    }

    workGauge->indSpr     = &spritesSub[curSpr];
    curSpr                = 127 - curSpr;
    workGauge->dataOffset = (150 + curSpr * 32);

    workGauge->indSpr->attribute[0] = ATTR0_NORMAL | ATTR0_COLOR_16 | ATTR0_WIDE;
    workGauge->indSpr->attribute[1] = ATTR1_SIZE_64;
    workGauge->indSpr->attribute[2] = ATTR2_PALETTE(15) | workGauge->dataOffset;

    dmaCopy(battleGaugeTiles, SPRITE_GFX_SUB + workGauge->dataOffset * 16, 32);
    for (int i = 1; i < 7; ++i)
        dmaCopy((u8 *)battleGaugeTiles + 32, SPRITE_GFX_SUB + (workGauge->dataOffset + i) * 16, 32);
    dmaCopy((u8 *)battleGaugeTiles + 32 * 10, SPRITE_GFX_SUB + (workGauge->dataOffset + 7) * 16, 32);
    moveSprite(workGauge->indSpr, posX, posY);
}

void updateGauge(pGauge workGauge)
{
    if (workGauge->isHalt | workGauge->isFull)
        return;

    if (workGauge->gTimer > 9 * 6) {
        workGauge->isFull = true;
        dmaCopy((u8 *)battleGaugeTiles + 9 * 32, SPRITE_GFX_SUB + (workGauge->dataOffset + 6) * 16, 32);
        return;
    }

    int devisor = workGauge->gTimer / 9;
    int modulo  = workGauge->gTimer % 9;
    int inc     = (modulo == 8) ? 2 : 1; // Need to add by 2 for animation holdup

    if (11 > workGauge->gRate) { // Wait for ticks
        if (workGauge->gTick-- < 0) {
            workGauge->gTick = 10 - workGauge->gRate;
            workGauge->gTimer += inc;
        }
    } else // Jump ticks
        workGauge->gTimer += (inc - 1) + workGauge->gRate - 10;

    dmaCopy(battleGaugeTiles, SPRITE_GFX_SUB + workGauge->dataOffset * 16, 32);
    dmaCopy((u8 *)battleGaugeTiles + (modulo + 1) * 32,
            SPRITE_GFX_SUB + ((workGauge->dataOffset + 1) + devisor) * 16, 32);
    if (devisor)
        dmaCopy((u8 *)battleGaugeTiles + 9 * 32,
                SPRITE_GFX_SUB + ((workGauge->dataOffset + 1) + devisor - 1) * 16, 32);
    dmaCopy((u8 *)battleGaugeTiles + 32 * 10, SPRITE_GFX_SUB + (workGauge->dataOffset + 7) * 16, 32);
}

void resetGuage(pGauge workGauge)
{
    workGauge->gTimer = 0;
    workGauge->gTick  = 10 - workGauge->gRate;
    workGauge->isFull = false;
    for (int i = 1; i < 7; ++i)
        dmaCopy((u8 *)battleGaugeTiles + 32, SPRITE_GFX_SUB + (workGauge->dataOffset + i) * 16, 32);
}

void startBattle(int mnsIdx)
{
    if (curLevel->battleTiles == NULL || curLevel->battleMon == -1)
        return;

    gameValues.curMode = LEVELMODE_BATTLE_FADEIN;
    memcpy(&battleValues.curMonster, &monsterDB[mnsIdx], sizeof(Monster));
    battleValues.curMonster.mnsSpr               = &spritesMain[127];
    battleValues.curMonster.mnsSpr->attribute[0] = ATTR0_ROTSCALE | ATTR0_COLOR_16;
    battleValues.curMonster.mnsSpr->attribute[1] = ATTR1_SIZE_64;
    battleValues.curMonster.mnsSpr->attribute[2] = ATTR2_PRIORITY(2) | ATTR2_PALETTE(1) | 128;
    hideRotScaleSprite(battleValues.curMonster.mnsSpr, true);
    rotateSprite(&spriteRotationsMain[0], 0);
    battleValues.isBoss = false;
}

void graphicInitBattle()
{
    // Set to MODE0 and Sprites
    videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE | DISPLAY_SPR_1D
                 | DISPLAY_SPR_ACTIVE);
    // Init BG0
    REG_BG0CNT  = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(31) | BG_TILE_BASE(0) | 3; // Battle Image
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;

    REG_BG1CNT = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(25) | BG_TILE_BASE(6); // Text
    decompressToVRAM(textFontTiles, (void *)BG_TILE_RAM(6));
    BG_PALETTE[17] = RGB15(0, 20, 7);
    BG_PALETTE[18] = RGB15(0, 0, 0);
    BG_PALETTE[33] = RGB15(31, 31, 31);
    BG_PALETTE[34] = RGB15(0, 0, 0);
    memset((void *)BG_MAP_RAM(25), 0, 32 * 32 * 2);

    // Set up a sprite for the main display
    dmaCopy(spr16PalPal, SPRITE_PALETTE + 16 * 15, spr16PalPalLen);
    dmaCopy(textCursorTiles, SPRITE_GFX + 1000 * 16, textCursorTilesLen);
    hideSprite(textVars.cursorSpriteMain, true);

    // Hide all figures
    for (list<pFigure>::iterator i = activeFigures.begin(); i != activeFigures.end(); ++i) {
        if ((*i) != curPlayer) {
            (*i)->isEnabled = true;
            disableFigure(*i);
        }
    }

    // Draw the new rects and load up the window
    drawRectTop(0, 0, 16, 7);
    drawRectTop(16, 0, 16, 7);
    textVars.curMode = TEXTMODE_WINDOW_DIRUP;
}

void initMode(gameMode selMode)
{
    int blank = 0;
    switch (selMode) {
        case LEVELMODE_SPLASH_FADEIN:
            dmaCopy(splPalPal, BG_PALETTE, splPalPalLen);
            dmaCopy(splSprPalPal, SPRITE_PALETTE, splSprPalPalLen);
            BG_PALETTE_SUB[0] = BG_PALETTE[0] = 0x7FFF;
            swiCopy(&blank, BG_GFX, 256 * 256 | COPY_MODE_FILL);
            decompressToVRAM(splLayer1Tiles, SPRITE_GFX);
            decompressToVRAM(splLayer2Bitmap, (void *)BG_BMP_RAM(0));
            decompressToVRAM(splLayer3Bitmap, (void *)BG_BMP_RAM(4));
            for (int i = 0; i < 4 * 3; ++i) {
                spritesMain[i].attribute[0] = ATTR0_NORMAL | ATTR0_COLOR_16 | (i / 4) * 64;
                spritesMain[i].attribute[1] = ATTR1_SIZE_64 | (i % 4) * 64;
                spritesMain[i].attribute[2] = ATTR2_PRIORITY(3) | (i * 64);
            }
            gameValues.tickVal = 30;
            gameValues.fadeVal = 15;
            graphicInitSplash();
            SndPlayMOD(mscSplash_mod);
            break;
        case LEVELMODE_SPLASH:

            break;
        case LEVELMODE_SPLASH_FADEOUT:
            break;
        case LEVELMODE_MENU_FADEIN:
            SndPlayMOD(mscMenu_mod);
            graphicInitMainMenu();
            swiCopy(&blank, (void *)BG_MAP_RAM(31), 32 * 32 | COPY_MODE_FILL);
            textPutOn(true, (u16 *)BG_MAP_RAM(31), 11, 11, "Press Start");
            gameValues.isInit = false;
            break;
        case LEVELMODE_MENU_START:
            break;
        case LEVELMODE_MENU_CHOICE:
            hideSprite(textVars.cursorSprite, false);
            undrawRectOn((u16 *)BG_MAP_RAM(30), 0, 0, 32, 32);
            clearTextOn((u16 *)BG_MAP_RAM(31), 0, 0, 32, 32);
            textPutOn(true, (u16 *)BG_MAP_RAM(31), 11, 11, "  New Game ");
            textPutOn(true, (u16 *)BG_MAP_RAM(31), 11, 12, "  Continue ");
            break;
        case LEVELMODE_MENU_LOAD:
            undrawRectOn((u16 *)BG_MAP_RAM(30), 0, 0, 32, 32);
            clearTextOn((u16 *)BG_MAP_RAM(31), 0, 0, 32, 32);
            drawRectOn((u16 *)BG_MAP_RAM(30), 5, 4, 22, 5);
            drawRectOn((u16 *)BG_MAP_RAM(30), 5, 4 + 6, 22, 5);
            drawRectOn((u16 *)BG_MAP_RAM(30), 5, 4 + 12, 22, 5);
            fillSaves();
            for (int i = 0; i < 3; ++i)
                textPutOn(true, (u16 *)BG_MAP_RAM(31), 6, 6 + i * 6,
                          gameValues.saveSlots[i].saveName);
            break;
        case LEVELMODE_INTRO:
            break;
        case LEVELMODE_PLAY:
            if (!gameValues.isInit) {
                initOAM();
                graphicInitGame();
                initText();
                initFigures();
                gameValues.curEvent   = 0;
                gameValues.timePlayed = 0;
                gameValues.lstCou = gameValues.curCou = 0;
                if (gameValues.usesGameSave != -1) {
                    loadSave(&gameValues.saveSlots[gameValues.usesGameSave]);
                    SndPlayMOD(bgMusic[gameValues.saveSlots[gameValues.usesGameSave].curLevel]);
                    gameValues.usesGameSave = -1;
                } else {
                    loadLevel(LEVEL1);
                    SndPlayMOD(bgMusic[0]);

                    curPlayer->posX = 192;
                    curPlayer->posY = 132;
                }
                gameValues.isInit = true;
            }
            break;
        case LEVELMODE_INGAME_MENU:
            break;
        case LEVELMODE_FADEOUT:
            break;
        case LEVELMODE_SWITCHLEVEL:
            break;
        case LEVELMODE_FADEIN:
            break;
        case LEVELMODE_BATTLE_FADEIN:
            // Saves the data to know where to return
            textVars.menuSel      = 0;
            battleValues.posX     = curPlayer->posX;
            battleValues.posY     = curPlayer->posY;
            battleValues.lstDir   = curPlayer->curDir;
            battleValues.lstLevel = curLevel->levelIdx;
            battleValues.curMode  = BATTLE_GAUGESFILLS;
            break;
        case LEVELMODE_BATTLE_FADEOUT:
            if (battleValues.isBoss)
                SndPlayMOD(mscBoss_mod);
            else
                SndPlayMOD(mscBattle_mod);
            dmaCopy(battleValues.curMonster.sprData, SPRITE_GFX + 128 * 16, 64 * 64 / 2);
            dmaCopy(battleValues.curMonster.sprPal, SPRITE_PALETTE + 16, 32);
            moveSprite(battleValues.curMonster.mnsSpr, 16, (192 - 64) / 2);
            hideSprite(battleValues.playerG.indSpr, true);
            hideRotScaleSprite(battleValues.curMonster.mnsSpr, false);
            break;
        case LEVELMODE_BATTLE_FADEPAUSE:
            graphicInitBattle();
            decompressToVRAM(curLevel->battleTiles, (void *)BG_TILE_RAM(0));
            dmaCopy(curLevel->battlePal, BG_PALETTE, 256 * 2);
            for (int i = 0; i < 32 * 32; ++i)
                ((u16 *)BG_MAP_RAM(31))[i] = i;
            curLevel = &battleLevel; // NOT loadLevel!
            initGauge(&battleValues.playerG, 188, 148 + 8, curPlayer->Attr[AGI]);
            initGauge(&battleValues.mnsGuage, 8 * 8 - 4, 148 + 8,
                      battleValues.curMonster.Attr[AGI]);
            hideSprite(battleValues.playerG.indSpr, true);
            hideSprite(battleValues.mnsGuage.indSpr, true);
            // Set the player's figure new info
            curPlayer->isMotion       = true;
            curPlayer->motionInterval = 12;
            enableFigure(curPlayer);
            setPosFigure(curPlayer, 256 - 32 - 16, (192 - 48) / 2 + 32);
            setDirFigure(curPlayer, DIR_LEFT);
            updateFigure(curPlayer);
            break;
        case LEVELMODE_BATTLE:
            hideSprite(battleValues.playerG.indSpr, false);
            hideSprite(battleValues.mnsGuage.indSpr, false);
            battleValues.playerG.isHalt  = false;
            battleValues.mnsGuage.isHalt = false;
            battleValues.curMode         = BATTLE_GAUGESFILLS;
            break;
        case LEVELMODE_EXITBATTLE:
            clearMenuText(0, 0, 32, 32);
            break;
        case LEVELMODE_GAMEOVER_FADEIN:
            break;
        case LEVELMODE_GAMEOVER_FADEOUT:
            graphicInitGOver();
            hideRotScaleSprite(battleValues.curMonster.mnsSpr, true);
            textVars.scroolY = 52 * 2;
            BG_PALETTE[0] = 0;

            curPlayer->allData = (u8 *)alexFigureDownTiles;
            curPlayer->palData = (u16 *)alexFigureDownPal;

            setPosFigure(curPlayer, (256 - 32) / 2, (192 - 48) / 2);
            updateFigure(curPlayer);

            decompressToVRAM(gameOverTiles, (void *)BG_TILE_RAM_SUB(0));
            decompressToVRAM(gameOverMap, (void *)BG_MAP_RAM_SUB(31));
            dmaCopy(gameOverPal, BG_PALETTE_SUB, gameOverPalLen);

            curPlayer->isMotion    = false;
            curPlayer->spriteIndex = 0;
            setDirFigure(curPlayer, DIR_DOWN);
            updateFigure(curPlayer);
            SndPlayMOD(mscGOver_mod);
            break;
        case LEVELMODE_GAMEOVER:
            break;
        case LEVELMODE_GAMEOVER_WHITEFADE:
            break;
    }
}

void writeSRAM(u8 *src, u32 offset, u32 size)
{
    for (u32 i = offset; i < offset + size; ++i)
        SRAM[i] = src[i - offset];
}

void readSRAM(u8 *dst, u32 offset, u32 size)
{
    for (u32 i = offset; i < offset + size; ++i)
        dst[i - offset] = SRAM[i];
}

bool saveGame(int saveSlot)
{
    saveData gameData;
    gameData.posX       = curPlayer->posX;
    gameData.posY       = curPlayer->posY;
    gameData.curLevel   = curLevel->levelIdx;
    gameData.curEvent   = gameValues.curEvent;
    gameData.timePlayed = gameValues.timePlayed;

    memcpy(gameData.playerStats, curPlayer->Attr, sizeof(Attributes));
    memset(gameData.playerItems, 0, DATABASE_SIZE * 4);
    for (int i = 0; i < (int)curPlayer->figureItemList.size(); ++i)
        gameData.playerItems[curPlayer->figureItemList[i].itemIdx] =
            curPlayer->figureItemList[i].quantity;

    memset(gameData.equipedItems, 0, sizeof(gameData.equipedItems));
    memcpy(gameData.equipedItems, curPlayer->equipedItems, sizeof(curPlayer->equipedItems));

    for (int i = 0; i < 21; ++i)
        gameData.workingChests[i] = chestFigure[i].isOpen;

    sprintf(gameData.saveName, "%s \2Lvl%ld\2 \3%d%%%%\3 \4%02lu:%02lu\4",
            curPlayer->figName.c_str(), curPlayer->Attr[LEVEL],
            (((gameValues.curEvent - 1) / 2) * 99) / STORY_SIZE, gameValues.timePlayed / 60,
            gameValues.timePlayed % 60);

    memcpy(&gameValues.saveSlots[saveSlot], &gameData, sizeof(saveData));
    gameData.isLoaded = true;

    if (gameValues.isFAT) {
        string fileName    = "ToD" + itoa(saveSlot, 10) + ".sav";
        FILE *outputBuffer = fopen(fileName.c_str(), "w");
        if (!outputBuffer)
            return false;

        if (!fwrite(&gameData, sizeof(saveData), 1, outputBuffer)) {
            fclose(outputBuffer);
            return false;
        }

        if (fclose(outputBuffer))
            return false;

        // Check if file exists
        outputBuffer = fopen(fileName.c_str(), "r");
        if (outputBuffer == NULL)
            return false;

        fclose(outputBuffer);
        return true;
    } else {
        writeSRAM((u8 *)&gameData, sizeof(saveData) * saveSlot, sizeof(saveData));
        return true;
    }
}

bool loadSavedGame(int saveSlot, pSaveData gameData)
{
    if (gameValues.isFAT) {
        string fileName    = "ToD" + itoa(saveSlot, 10) + ".sav";
        FILE *outputBuffer = fopen(fileName.c_str(), "r");
        if (outputBuffer == NULL)
            return false;

        if (fread(gameData, sizeof(saveData), 1, outputBuffer) != 1) {
            fclose(outputBuffer);
            return false;
        }

        if (fclose(outputBuffer))
            return false;

        if (gameData->isLoaded)
            return true;

        return false;
    } else {
        gameData->isLoaded = false;
        readSRAM((u8 *)gameData, sizeof(saveData) * saveSlot, sizeof(saveData));
        return gameData->isLoaded;
    }
}

void loadSave(pSaveData gameData)
{
    curPlayer->posX = gameData->posX;
    curPlayer->posY = gameData->posY;
    loadLevel(gameData->curLevel);
    gameValues.curEvent   = gameData->curEvent;
    gameValues.timePlayed = gameData->timePlayed;

    curPlayer->figureItemList.clear(); // duh
    for (int i = 0; i < DATABASE_SIZE; ++i)
        if (gameData->playerItems[i])
            addItem(curPlayer, i, gameData->playerItems[i]);

    memcpy(curPlayer->Attr, gameData->playerStats, sizeof(Attributes));
    memset(curPlayer->equipedItems, -1, sizeof(curPlayer->equipedItems));
    for (int i = 0; i < ITEM_ATTR_SIZE; ++i) {
        if (gameData->equipedItems[i] != -1) {
            curPlayer->equipedItems[i] = gameData->equipedItems[i];
            setItemEquiped(curPlayer, gameData->equipedItems[i], true);
        }
    }

    for (int i = 0; i < 21; ++i) {
        if (chestFigure[i].isOpen ^ gameData->workingChests[i]) {
            chestFigure[i].isOpen = gameData->workingChests[i];
            if (gameData->workingChests[i])
                chestFigure[i].cFigure.allData += (32 * 48 / 2) * 2;
            else
                chestFigure[i].cFigure.allData -= (32 * 48 / 2) * 2;
        }
    }
}

void fillSaves()
{
    textVars.subMenus[SAVE].clear();
    for (int curSave = 0; curSave < 3; ++curSave) {
        textVars.subMenus[SAVE].push_back("");
        if (!loadSavedGame(curSave, &gameValues.saveSlots[curSave]))
            strcpy(gameValues.saveSlots[curSave].saveName, "Empty Slot");
    }
}
