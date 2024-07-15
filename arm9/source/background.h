#ifndef __BACKGROUNDS_H__
#define __BACKGROUNDS_H__

#include <nds.h>

#include "background.h"
#include "sprites.h"

// Backgrounds
#include "battleBacks/battleBeach.h"
#include "battleBacks/battleCastle.h"
#include "battleBacks/battleECastle.h"
#include "battleBacks/battleECave.h"
#include "battleBacks/battleFCave.h"
#include "battleBacks/battleICave.h"
#include "battleBacks/battleTown.h"
#include "battleBacks/battleWoods.h"
#include "gameOverMap_bin.h"
#include "gameOver_bin.h"
#include "Levels/level1Col.h"
#include "Levels/level1Down.h"
#include "Levels/level1Ground.h"
#include "Levels/level1Pal.h"
#include "Levels/level1Up.h"
#include "Levels/level2Col.h"
#include "Levels/level2Down.h"
#include "Levels/level2Ground.h"
#include "Levels/level2Pal.h"
#include "Levels/level2Up.h"
#include "Levels/level3Col.h"
#include "Levels/level3Down.h"
#include "Levels/level3Ground.h"
#include "Levels/level3Pal.h"
#include "Levels/level3Up.h"
#include "Levels/level4Col.h"
#include "Levels/level4Down.h"
#include "Levels/level4Ground.h"
#include "Levels/level4Pal.h"
#include "Levels/level4Up.h"
#include "Levels/level5Col.h"
#include "Levels/level5Down.h"
#include "Levels/level5Ground.h"
#include "Levels/level5Pal.h"
#include "Levels/level5Up.h"
#include "Levels/level6Col.h"
#include "Levels/level6Down.h"
#include "Levels/level6Ground.h"
#include "Levels/level6Pal.h"
#include "Levels/level6Up.h"
#include "Levels/level7Col.h"
#include "Levels/level7Down.h"
#include "Levels/level7Ground.h"
#include "Levels/level7Pal.h"
#include "Levels/level7Up.h"
#include "Levels/level8Col.h"
#include "Levels/level8Down.h"
#include "Levels/level8Ground.h"
#include "Levels/level8Pal.h"
#include "Levels/level8Up.h"
#include "Levels/level9Col.h"
#include "Levels/level9Down.h"
#include "Levels/level9Ground.h"
#include "Levels/level9Pal.h"
#include "Levels/level9Up.h"
#include "Levels/level10Col.h"
#include "Levels/level10Down.h"
#include "Levels/level10Ground.h"
#include "Levels/level10Pal.h"
#include "Levels/level10Up.h"
#include "Levels/level11Col.h"
#include "Levels/level11Down.h"
#include "Levels/level11Ground.h"
#include "Levels/level11Pal.h"
#include "Levels/level11Up.h"
#include "Levels/level12Col.h"
#include "Levels/level12Down.h"
#include "Levels/level12Ground.h"
#include "Levels/level12Pal.h"
#include "Levels/level12Up.h"
#include "Levels/level13Col.h"
#include "Levels/level13Down.h"
#include "Levels/level13Ground.h"
#include "Levels/level13Pal.h"
#include "Levels/level13Up.h"
#include "Levels/level14Col.h"
#include "Levels/level14Down.h"
#include "Levels/level14Ground.h"
#include "Levels/level14Pal.h"
#include "Levels/level14Up.h"
#include "Levels/level15Col.h"
#include "Levels/level15Down.h"
#include "Levels/level15Ground.h"
#include "Levels/level15Pal.h"
#include "Levels/level15Up.h"
#include "Levels/level16Col.h"
#include "Levels/level16Down.h"
#include "Levels/level16Ground.h"
#include "Levels/level16Pal.h"
#include "Levels/level16Up.h"
#include "Levels/level17Col.h"
#include "Levels/level17Down.h"
#include "Levels/level17Ground.h"
#include "Levels/level17Pal.h"
#include "Levels/level17Up.h"
#include "Levels/level18Col.h"
#include "Levels/level18Down.h"
#include "Levels/level18Ground.h"
#include "Levels/level18Pal.h"
#include "Levels/level18Up.h"
#include "Splash/splLayer1.h"
#include "Splash/splLayer2.h"
#include "Splash/splLayer3.h"
#include "Splash/splPal.h"
#include "Splash/splSprPal.h"
#include "Splash/stamp1st.h"

// Defines
#define LEVEL_GND_TILES_ANIM(n, m)      (u8 *)level##n##Ground##m##_bin
#define LEVEL_GND_TILES_ANIM_SIZE(n, m) level##n##Ground##m##_bin_size
#define LEVEL_GND_MAP_ANIM(n, m)        (u8 *)level##n##Ground##m##Map_bin
#define LEVEL_GND_MAP_ANIM_SIZE(n, m)   level##n##Ground##m##Map_bin_size
#define LEVEL_GND_TILES(n)              (u8 *)level##n##GroundTiles
#define LEVEL_GND_TILES_SIZE(n)         level##n##GroundTilesLen
#define LEVEL_GND_MAP(n)                (u8 *)level##n##GroundMap
#define LEVEL_GND_MAP_SIZE(n)           level##n##GroundMapLen
#define LEVEL_UP_TILES(n)               (u8 *)level##n##UpTiles
#define LEVEL_UP_TILES_SIZE(n)          level##n##UpTilesLen
#define LEVEL_UP_MAP(n)                 (u8 *)level##n##UpMap
#define LEVEL_UP_MAP_SIZE(n)            level##n##UpMapLen
#define LEVEL_DOWN_TILES(n)             (u8 *)level##n##DownTiles
#define LEVEL_DOWN_TILES_SIZE(n)        level##n##DownTilesLen
#define LEVEL_DOWN_MAP(n)               (u8 *)level##n##DownMap
#define LEVEL_DOWN_MAP_SIZE(n)          level##n##DownMapLen
#define levelPal(n)                     (u16 *)level##n##PalPal
#define levelCollision(n)               (u8 *)level##n##ColMap

// clang-format off
#define levelData(n) \
    { LEVEL_GND_TILES(n), LEVEL_GND_MAP(n), LEVEL_GND_TILES_SIZE(n), LEVEL_GND_MAP_SIZE(n) }, \
    { LEVEL_UP_TILES(n), LEVEL_UP_MAP(n), LEVEL_UP_TILES_SIZE(n), LEVEL_UP_MAP_SIZE(n) }, \
    { LEVEL_DOWN_TILES(n), LEVEL_DOWN_MAP(n), LEVEL_DOWN_TILES_SIZE(n), LEVEL_DOWN_MAP_SIZE(n) }

#if 0
#define NO_ANIMATION \
    {}, {}, {}, {}

#define levelAnimation(n)                                                   \
        levelMaps[(n - 1) * 3],                                             \
        { LEVEL_GND_TILES_ANIM(n, 2), LEVEL_GND_MAP_ANIM(n, 2),             \
          LEVEL_GND_TILES_ANIM_SIZE(n, 2), LEVEL_GND_MAP_ANIM_SIZE(n, 2) }, \
        { LEVEL_GND_TILES_ANIM(n, 3), LEVEL_GND_MAP_ANIM(n, 3),             \
          LEVEL_GND_TILES_ANIM_SIZE(n, 3), LEVEL_GND_MAP_ANIM_SIZE(n, 3) }, \
        { LEVEL_GND_TILES_ANIM(n, 4), LEVEL_GND_MAP_ANIM(n, 4),             \
          LEVEL_GND_TILES_ANIM_SIZE(n, 4), LEVEL_GND_MAP_ANIM_SIZE(n, 4) }
#endif
// clang-format on

// Types
typedef u8 *mapTiles;
typedef u8 *animMapTiles;
typedef u8 *mapMap;
typedef u16 *mapPalette;

typedef enum _gameMode_
{
    LEVELMODE_SPLASH_FADEIN,
    LEVELMODE_SPLASH,
    LEVELMODE_SPLASH_FADEOUT,
    LEVELMODE_MENU_FADEIN,
    LEVELMODE_MENU_START,
    LEVELMODE_MENU_CHOICE,
    LEVELMODE_MENU_LOAD,
    LEVELMODE_INTRO,
    LEVELMODE_PLAY,
    LEVELMODE_INGAME_MENU,
    LEVELMODE_FADEOUT,
    LEVELMODE_SWITCHLEVEL,
    LEVELMODE_FADEIN,
    LEVELMODE_BATTLE_FADEIN,
    LEVELMODE_BATTLE_FADEOUT,
    LEVELMODE_BATTLE_FADEPAUSE,
    LEVELMODE_BATTLE,
    LEVELMODE_EXITBATTLE,
    LEVELMODE_GAMEOVER_FADEIN,
    LEVELMODE_GAMEOVER_FADEOUT,
    LEVELMODE_GAMEOVER,
    LEVELMODE_GAMEOVER_WHITEFADE,

} gameMode, *pGameMode;

typedef enum _battleMode_
{
    BATTLE_GAUGESFILLS,
    BATTLE_PLAYERGAUGEFULL,
    BATTLE_MONSTERGAUGEFULL,
    BATTLE_PLAYERCHOICE,
    BATTLE_PLAYERCHOICE_MONSTER,
    BATTLE_PLAYERACTION,
    BATTLE_MONSTERACTION,
    BATTLE_SHOWRESULT_PLAYER,
    BATTLE_SHOWRESULT_MONSTER,
    BATTLE_ELIMINATEMONSTER,
    BATTLE_VICTORY,
    BATTLE_UPGRADESTATS,
    BATTLE_CHOOSEITEM,
    BATTLE_ITEMEFFECT,

} battleMode, *pBattleMode;

// Map
typedef struct _Map_
{
    mapTiles tileData;
    mapMap mapData;
    u32 tileSize;
    u32 mapSize;

} Map, *pMap;

typedef enum _LEVELS_
{
    LEVEL1,
    LEVEL2,
    LEVEL3,
    LEVEL4,
    LEVEL5,
    LEVEL6,
    LEVEL7,
    LEVEL8,
    LEVEL9,
    LEVEL10,
    LEVEL11,
    LEVEL12,
    LEVEL13,
    LEVEL14,
    LEVEL15,
    LEVEL16,
    LEVEL17,
    LEVEL18,

    LEVEL_SIZE,
    NO_LEVEL,

} LEVELS;

typedef struct _levelLink_
{
    int startTile;
    int endTile;
    int posFix;
    LEVELS loadedLevel;
    Direction wallDir;

} levelLink, *pLevelLink;

typedef struct _doorLink_
{
    int startTileX, endTileX;
    int startTileY, endTileY;
    int posFixX, posFixY;
    Direction loadedDir;
    LEVELS loadedLevel;

} doorLink, *pDoorLink;

typedef struct _saveData_
{
    char saveName[256];
    int posX, posY;
    LEVELS curLevel;
    Attributes playerStats;
    int curEvent;
    int playerItems[DATABASE_SIZE];
    int equipedItems[ITEM_ATTR_SIZE];
    bool workingChests[25];
    u32 timePlayed;
    bool isLoaded;

} PACKED saveData, *pSaveData;

typedef struct _gameVars_
{
    gameMode curMode, lstMode;
    int fadeVal, tickVal, fadeHalt;
    bool isFade, isBattle, isInit;
    LEVELS loadedLevel;
    s32 flowerAngle, textZoom;
    int newPosX, newPosY;
    Direction newDir;
    int curEvent, curCutsceneIdx, eventSaveParam;
    pFigure lstTalkedFigure;
    saveData saveSlots[3];
    u32 timePlayed, lstCou, curCou;
    s32 usesGameSave;
    bool isFAT;

} gameVars, *pGameVars;

typedef struct _Gauge_
{
    int gTimer, gTick, gRate;
    int posX, posY;
    pSpriteEntry indSpr;
    int dataOffset;
    bool isFull, isHalt;

} Gauge, *pGauge;

typedef struct _battleVars_
{
    int posX, posY;
    Direction lstDir;
    int lstLevel;

    bool isSet, isBoss;
    battleMode curMode;
    pSpriteEntry cursorSprite;
    Gauge playerG;
    Weapon activeWeapon;
    int animIdx, animTick, hpDec;
    int deltaLevel;
    Monster curMonster;
    Gauge mnsGuage;
    itemList battleItems;

} battleVars, *pBattleVars;

// Level
typedef struct _Level_
{
    int dimX, dimY;
    int viewX, viewY;
    Map *upMap;
    Map *downMap;
    Map *groundMap;
    u8 *colMap;
    mapPalette levelPal;
    // bool isAnimated;
    // Map* animData[4];
    // int animIndex;
    pLevelLink linkedLevels;
    pDoorLink linkedDoors;
    LEVELS levelIdx;

    // Battle vars
    u8 *battleTiles, *battlePal;
    int battleMon;

} Level, *pLevel;

typedef struct _NPC_
{
    pFigure Figure;
    int posX, posY;
    int moveX, moveY;
    LEVELS linkedLevel;
    Direction faceDir;
    u8 *npcData;
    u16 *npcPalette;
    const char *npcName;
    const char *npcText;

} NPC, *pNPC;

// Protoypes
void initGame(void);
void updateView(void);
void backgroundUpdateView(u16 *workMap, const u16 *targetMap);
void initLevels(void);
void initMode(gameMode selMode);
void loadLevel(LEVELS lvlIdx);
void graphicInitGame(void);
void graphicInitSplash(void);
void graphicInitMainMenu(void);
void graphicInitBattle(void);
bool isCollisionDoor(pFigure Figure, pDoorLink doorLink);
void startBattle(int mnsIdx);

void initGauge(pGauge workGauge, int posX, int posY, int gRate);
void updateGauge(pGauge workGauge);
void resetGuage(pGauge workGauge);

// Data
extern pLevel curLevel;
extern gameVars gameValues;
extern battleVars battleValues;
extern Map levelMaps[LEVEL_SIZE * 3];
extern Level Levels[LEVEL_SIZE];
extern Level battleLevel;
extern const u32 levelSizes[LEVEL_SIZE * 2];
extern const int levelStartPos[LEVEL_SIZE * 2];
extern u16 *levelPalettes[LEVEL_SIZE];
extern u8 *levelCollisions[LEVEL_SIZE];
extern const bool levelAnimation[LEVEL_SIZE];
extern Map animLevelTiles[LEVEL_SIZE * 4];
extern pLevelLink levelLinking[LEVEL_SIZE];
extern pDoorLink doorLinking[LEVEL_SIZE];
extern const NPC gameNPCs[];
extern const u8 *bgMusic[LEVEL_SIZE];
extern const void *battleMapsPals[LEVEL_SIZE * 2];

// Compression code
void decompressToVRAM(const void *source, void *dest);

// Save/Load features
bool loadSavedGame(int saveSlot, pSaveData gameData);
void loadSave(pSaveData gameData);
bool saveGame(int saveSlot);
void fillSaves();

// Level linkers
extern levelLink level1Link[];
extern levelLink level2Link[];
extern levelLink level3Link[];
extern levelLink level4Link[];
extern levelLink level5Link[];
extern levelLink level6Link[];
extern levelLink level7Link[];
extern levelLink level8Link[];
extern levelLink level9Link[];
extern levelLink level10Link[];
extern levelLink level11Link[];
extern levelLink level12Link[];
extern levelLink level13Link[];
extern levelLink level14Link[];
extern levelLink level15Link[];
extern levelLink level16Link[];
extern levelLink level17Link[];
extern levelLink level18Link[];

extern doorLink doorLink1[];
extern doorLink doorLink2[];
extern doorLink doorLink3[];
extern doorLink doorLink4[];
extern doorLink doorLink5[];
extern doorLink doorLink6[];
extern doorLink doorLink7[];
extern doorLink doorLink8[];
extern doorLink doorLink9[];
extern doorLink doorLink10[];
extern doorLink doorLink11[];
extern doorLink doorLink12[];
extern doorLink doorLink13[];
extern doorLink doorLink14[];
extern doorLink doorLink15[];
extern doorLink doorLink16[];
extern doorLink doorLink17[];
extern doorLink doorLink18[];

#endif // __BACKGROUNDS_H__
