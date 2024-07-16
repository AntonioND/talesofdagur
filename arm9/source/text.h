#ifndef __TEXT_H__
#define __TEXT_H__

#include <string>

#include "aButton_bin.h"
#include "background.h"
#include "main.h"
#include "Menu/mainMenuBack.h"
#include "Menu/mainMenuFront.h"
#include "Misc/battleGauge.h"
#include "Misc/playerEmblem.h"
#include "Misc/spr16Pal.h"
#include "Misc/textCursor.h"
#include "Misc/textBox.h"
#include "Misc/textFont.h"
#include "palGrad_bin.h"
#include "sprites.h"

typedef enum _menuOption_
{
    ITEM,
    MAGIC,
    EQUIP,
    CONFIG,
    SAVE,
    RESUME,

    MENUS_SIZE,
    MAIN_MENU,

} menuOption, *pMenuOption;

typedef enum _textColor_
{
    TEXT_COLOR_NORMAL = 1,
    TEXT_COLOR_RED    = 2,
    TEXT_COLOR_GREEN  = 3,
    TEXT_COLOR_YELLOW = 4,
    TEXT_COLOR_GREY   = 5,

    TEXT_COLOR_SIZE,

} textColor, *pTextColor;

typedef enum _textMode_
{
    TEXTMODE_IDLE,
    TEXTMODE_WINDOW_DIRUP,
    TEXTMODE_WINDOW_UP,
    TEXTMODE_WRITING,
    TEXTMODE_WAITINGFORINPUT,
    TEXTMODE_FIGUREMENU,
    TEXTMODE_SHOP,
    TEXTMODE_PRINTDONE,
    TEXTMODE_WINDOW_DIRDOWN,

} textMode, *pTextMode;

typedef struct _textVar_
{
    int posX, posY, virtualPosY;
    int txtPos, txtLen;
    int menuSel;
    char txtBuffer[2048];
    int scroolY;
    pSpriteEntry aButton;
    pSpriteEntry emblemFigure;
    pSpriteEntry cursorSprite, cursorSpriteMain;
    textColor curColor;
    u16 *textMap;
    textMode curMode;
    bool needInput, pauseMode, writeStatics;
    vector<string> subMenus[MENUS_SIZE];
    vector<string> smallMenu;
    vector<string> *curMenu;
    menuOption curOption, lstOption;
    int yesNoCode;

} textVar, *pTextVar;

// Data
extern textVar textVars;
extern const u16 mainMap[32 * 32];
extern const u16 menuMap[32 * 32];

// Function prototypes
std::string itoa(int value, int base);
void initText(void);
void textPrintChar(int posX, int posY, int bufferPos);
void textHandleVBlank(void);
void textHandleHBlank(void);
void textWrite(const char *selText);
void bwPalettes();
void putText(int x, int y, const char *fmt, ...);
void menuText(int x, int y, const char *fmt, ...);
void textPutOn(bool isColor, u16 *mapBase, int x, int y, const char *fmt, ...);
void clearTextOn(u16 *mapBase, int x, int y, int dimX, int dimY);
void clearMenuText(int x, int y, int dimX, int dimY);
void clearText(int x, int y, int dimX, int dimY);
void drawRectOn(u16 *workMap, int posX, int posY, int dimX, int dimY);
void drawRectTop(int posX, int posY, int dimX, int dimY);
void drawRectBottom(int posX, int posY, int dimX, int dimY);
void undrawRectOn(u16 *workMap, int posX, int posY, int dimX, int dimY);
void undrawRectTop(int posX, int posY, int dimX, int dimY);
void undrawRectBottom(int posX, int posY, int dimX, int dimY);
void writeStatics();
void writeStaticsWith(pItem item);
void printIndicatorName(int posX, int posY, const char *name, int curHp, int maxHp);

#endif
