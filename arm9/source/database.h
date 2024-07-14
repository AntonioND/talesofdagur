#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <vector>

using namespace std;

// defines
#define DATABASE_SIZE (45)

// Types
typedef enum _Attribute_
{
    HP,
    MAX_HP,
    MP,
    MAX_MP,
    STR,
    DEF,
    AGI,
    VIT,
    INT,
    DEX,
    EXP,
    EXP_TO_NEXT,
    GOLD,

    LEVEL,
    ATTR_SIZE,

} Attr, *pAttr;

typedef s32 Attributes[ATTR_SIZE];

typedef enum _itemAttribute_
{
    WEAPON_SWORD,
    WEAPON_SPEAR,
    WEAPON_AXE,
    WEAPON_BOW,
    WEAPON_GUN,
    WEAPON_CLUB,
    WEAPON_STAFF,
    WEAPON_DAGGER,

    POTION,
    WEAPON,
    HEAD_GEAR,
    ARMOR,
    SHIELD,
    ACCESSORY,
    ORB,

    ITEM_ATTR_SIZE,

} itemAttr, *pItemAttr;

typedef struct _Item_
{
    const char *itemName;
    Attributes itemChanges;
    itemAttr itemType;
    int itemPrice;

} Item, *pItem;

typedef struct _sprItem_
{
    pItem itemData;
    bool equiped;
    int quantity;
    int itemIdx;

} sprItem, *pSprItem;

typedef vector<sprItem> itemList;

typedef struct _Monster_
{
    const char *name;
    Attributes Attr;
    u8 *sprData;
    u16 *sprPal;
    // Info
    bool isAlive;
    pSpriteEntry mnsSpr;

} Monster, *pMonster;

// Data
extern const Item itemDB[];
extern const Monster monsterDB[];
extern const Attributes playerStartAttr[ATTR_SIZE];

#endif
