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
#include "Figures/alexFigureDown.h"
#include "Figures/alexFigure.h"
#include "Figures/bartenderFigure.h"
#include "Figures/civilian10Figure.h"
#include "Figures/civilian11Figure.h"
#include "Figures/civilian12Figure.h"
#include "Figures/civilian1Figure.h"
#include "Figures/civilian2Figure.h"
#include "Figures/civilian3Figure.h"
#include "Figures/civilian4Figure.h"
#include "Figures/civilian5Figure.h"
#include "Figures/civilian6Figure.h"
#include "Figures/civilian7Figure.h"
#include "Figures/civilian8Figure.h"
#include "Figures/civilian9Figure.h"
#include "Figures/dangFigure.h"
#include "Figures/demitriFigure.h"
#include "Figures/dingFigure.h"
#include "Figures/dongFigure.h"
#include "Figures/guardFigure.h"
#include "Figures/kingFigure.h"
#include "Figures/lumberjack1Figure.h"
#include "Figures/lumberjackFigure.h"
#include "Figures/maidFigure.h"
#include "Figures/queenFigure.h"
#include "Figures/traderFigure.h"
#include "Figures/vardaFigure.h"

// Weapon
#include "Weapons/wpnAxe.h"
#include "Weapons/wpnBow.h"
#include "Weapons/wpnClub.h"
#include "Weapons/wpnDagger.h"
#include "Weapons/wpnGun.h"
#include "Weapons/wpnSpear.h"
#include "Weapons/wpnStaff.h"
#include "Weapons/wpnSword.h"

// Monsters
#include "Monsters/aquticBattler.h"
#include "Monsters/batBattler.h"
#include "Monsters/beeBattler.h"
#include "Monsters/chickenBattler.h"
#include "Monsters/demitriBattler.h"
#include "Monsters/devilBattler.h"
#include "Monsters/earthBattler.h"
#include "Monsters/fireBattler.h"
#include "Monsters/ghostBattler.h"
#include "Monsters/scropionBattler.h"
#include "Monsters/squidBattler.h"
#include "Monsters/treeBattler.h"
#include "Monsters/vardaBattler.h"
#include "Monsters/waterBattler.h"
#include "Monsters/zombieBattler.h"

// Chests
#include "Figures/chest1Figure.h"
#include "Figures/chest2Figure.h"
#include "Figures/chest3Figure.h"
#include "Figures/chest4Figure.h"

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
    u16 *palPtr;
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
