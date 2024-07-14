#ifndef __SPRITES_H__
#define __SPRITES_H__

#include <list>
#include <string>

#include <nds.h>
#include <nds/arm9/trig_lut.h>

#include "database.h"

using namespace std;

// Graphics
// Figures
#include "alexFigureDown_bin.h"
#include "alexFigurePal_bin.h"
#include "alexFigure_bin.h"
#include "bartenderFigurePal_bin.h"
#include "bartenderFigure_bin.h"
#include "civilian10FigurePal_bin.h"
#include "civilian10Figure_bin.h"
#include "civilian11FigurePal_bin.h"
#include "civilian11Figure_bin.h"
#include "civilian12FigurePal_bin.h"
#include "civilian12Figure_bin.h"
#include "civilian1FigureData_bin.h"
#include "civilian1FigurePal_bin.h"
#include "civilian2FigureData_bin.h"
#include "civilian2FigurePal_bin.h"
#include "civilian3FigureData_bin.h"
#include "civilian3FigurePal_bin.h"
#include "civilian4FigureData_bin.h"
#include "civilian4FigurePal_bin.h"
#include "civilian5FigureData_bin.h"
#include "civilian5FigurePal_bin.h"
#include "civilian6FigureData_bin.h"
#include "civilian6FigurePal_bin.h"
#include "civilian7FigurePal_bin.h"
#include "civilian7Figure_bin.h"
#include "civilian8FigurePal_bin.h"
#include "civilian8Figure_bin.h"
#include "civilian9FigurePal_bin.h"
#include "civilian9Figure_bin.h"
#include "dangFigureData_bin.h"
#include "dangFigurePal_bin.h"
#include "demitriFigurePal_bin.h"
#include "demitriFigure_bin.h"
#include "dingFigurePal_bin.h"
#include "dingFigure_bin.h"
#include "dongFigurePal_bin.h"
#include "dongFigure_bin.h"
#include "guardFigurePal_bin.h"
#include "guardFigure_bin.h"
#include "kingFigureData_bin.h"
#include "kingFigurePal_bin.h"
#include "lumberjack1FigurePal_bin.h"
#include "lumberjack1Figure_bin.h"
#include "lumberjackFigurePal_bin.h"
#include "lumberjackFigure_bin.h"
#include "maidFigurePal_bin.h"
#include "maidFigure_bin.h"
#include "queenFigurePal_bin.h"
#include "queenFigure_bin.h"
#include "traderFigurePal_bin.h"
#include "traderFigure_bin.h"
#include "vardaFigurePal_bin.h"
#include "vardaFigure_bin.h"

// Weapon
#include "wpnAxe_bin.h"
#include "wpnBow_bin.h"
#include "wpnClub_bin.h"
#include "wpnDagger_bin.h"
#include "wpnGun_bin.h"
#include "wpnPal_bin.h"
#include "wpnSpear_bin.h"
#include "wpnStaff_bin.h"
#include "wpnSword_bin.h"

// Monsters
#include "aquticBattlerPal_bin.h"
#include "aquticBattler_bin.h"
#include "batBattlerPal_bin.h"
#include "batBattler_bin.h"
#include "beeBattlerPal_bin.h"
#include "beeBattler_bin.h"
#include "chickenBattlerPal_bin.h"
#include "chickenBattler_bin.h"
#include "demitriBattlerPal_bin.h"
#include "demitriBattler_bin.h"
#include "devilBattlerPal_bin.h"
#include "devilBattler_bin.h"
#include "earthBattlerPal_bin.h"
#include "earthBattler_bin.h"
#include "fireBattlerPal_bin.h"
#include "fireBattler_bin.h"
#include "ghostBattlerPal_bin.h"
#include "ghostBattler_bin.h"
#include "scropionBattlerPal_bin.h"
#include "scropionBattler_bin.h"
#include "squidBattlerPal_bin.h"
#include "squidBattler_bin.h"
#include "treeBattlerPal_bin.h"
#include "treeBattler_bin.h"
#include "vardaBattlerPal_bin.h"
#include "vardaBattler_bin.h"
#include "waterBattlerPal_bin.h"
#include "waterBattler_bin.h"
#include "zombieBattlerPal_bin.h"
#include "zombieBattler_bin.h"

// Chests
#include "chest1FigurePal_bin.h"
#include "chest1Figure_bin.h"
#include "chest2FigurePal_bin.h"
#include "chest2Figure_bin.h"
#include "chest3FigurePal_bin.h"
#include "chest3Figure_bin.h"
#include "chest4FigurePal_bin.h"
#include "chest4Figure_bin.h"

// Types
typedef u16 *Palette;
typedef u8 *spriteData;

// Figure direction
typedef enum _Direction_
{
    DIR_NONE  = -1,
    DIR_UP    = 2,
    DIR_LEFT  = 1,
    DIR_RIGHT = 3,
    DIR_DOWN  = 0,

} Direction, *pDirection;

enum figMoveDir
{
    MOVE_NOT   = 0x0,
    MOVE_RIGHT = 0x1,
    MOVE_LEFT  = 0x2,
    MOVE_UP    = 0x4,
    MOVE_DOWN  = 0x8,
};

typedef struct _moveData_
{
    int destX, destY;
    int startX, startY;
    bool isRepeat;

} moveData, *pMoveData;

typedef list<moveData> moveQueue;

typedef struct _Figure_
{
    // Usefull data
    s32 posX, posY, dimX, dimY;
    s32 virtualPosX, virtualPosY;
    u32 spriteOffset, palOffset, dataOffset, spriteIndex, spriteDirIndex;
    u32 lastFrame, motionInterval, animSize;
    bool isMotion, isActive, isEnabled;
    int curMove, levelShow;
    pSpriteEntry upperSprite, lowerSprite;
    spriteData allData;
    Palette palData;
    Direction curDir, lstDir;

    // Move Queue
    moveQueue movQue;

    // Talking attributes
    string figName, figText;
    u32 talkCount;

    // Battle attributes
    Attributes Attr;
    itemList figureItemList;

    // Equips
    int equipedItems[ITEM_ATTR_SIZE];

} Figure, *pFigure;

typedef struct _Chest_
{
    Figure cFigure; // Virtual position, handling, talking...
    int itemIdx;
    int quantity;
    bool isOpen, isAnimtion;
} Chest, *pChest;

typedef struct _chestLoadingData_
{
    int posX, posY;
    Direction chestDir;
    int loadedLevel;
    int gfxId, itemIdx, quantity;

} chestLoadingData, *pChestLoadingData;

typedef struct _Weapon_
{
    pSpriteEntry wpnSpr;
    int posX, posY;
    u8 *dataPtr;
    int animIdx;
    bool isActive;

} Weapon, *pWeapon;

// Data
extern SpriteEntry spritesMain[128];
extern SpriteEntry spritesSub[128];
extern pSpriteRotation spriteRotationsMain;
extern pSpriteRotation spriteRotationsSub;
extern list<pFigure> activeFigures;
extern list<pFigure> onScreenFigures;
extern list<pChest> activeChests;
extern list<pChest> onScreenChests;
extern pFigure curPlayer;
extern Figure alexFigure;
extern Figure kingFigure;
extern Figure demitriFigure;
extern Figure guardFigure[5];
extern Figure civilianFigure[12];
extern Chest chestFigure[21];
extern Figure maidFigure, bartenderFigure, traderFigure;
extern Figure lumberjackFigure[2];
extern Figure dingFigure, dangFigure, dongFigure;
extern Figure vardaFigure, queenFigure;
extern Figure doorFigure;
extern const chestLoadingData loadedChests[];

// Proto-types
// Sprite Handling
void initOAM(void);
void updateOAM(void);
void moveSprite(SpriteEntry *sp, u16 x, u16 y);
void rotateSprite(pSpriteRotation rotdata, u16 angle);
void zoomSprite(pSpriteRotation rotdata, u16 z);
void rotScaleSprite(pSpriteRotation rotdata, u16 angle, u16 z);
void hideSprite(pSpriteEntry sp, bool hide);
void hideRotScaleSprite(pSpriteEntry sp, bool hide);
void setSpritePriority(pSpriteEntry sp, int pri);

// Figure handling
void initFigure(const char *selName, pFigure theFigure, spriteData allData, Palette palData);
void updateSpriteData(pFigure theFigure);
void setPosFigure(pFigure theFigure, int posX, int posY);
void updateFigure(pFigure theFigure);
void setCurPlayer(pFigure tFigure);
bool advanceFigure(pFigure tFigure);
void moveFigure(pFigure theFigure, int posX, int posY, bool isRepeat);
bool isPlayerChat(pFigure &retFigure);
void disableFigure(pFigure theFigure);
void enableFigure(pFigure theFigure);
void setDirFigure(pFigure Figure, Direction Dir);
int updateFigureStates(pFigure Figure);

// Loaders
void loadNPCs(void);
void loadChests(void);
void initFigures(void);

// Weapons
void initWeapon(pWeapon cWeapon, itemAttr wpnAttr);
void upadteWeapon(pWeapon cWeapon);

// Item handling
void addItem(pFigure Figure, int itemIdx, int quan);
void removeItem(pFigure Figure, int itemIdx, int quan);
void receiveItem(int itemIdx, int quan);
void useItem(pFigure Figure, int itemIdx);
void setItemEquiped(pFigure curFigure, int itemIdx, bool isEquiped);
void initChest(pChest Chest, int gfxId, int itemIdx, int quantity);

// Collision code
bool isCollision(pFigure Figure);
bool isCollisionFigures(pFigure Figure1, pFigure Figure2);
bool isCollisionBackground(pFigure Figure);
bool isCollisionChest(pFigure tFigure, pChest tChest);

#endif
