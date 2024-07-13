#include "text.h"
		
textVar textVars;
		
std::string itoa(int value, int b)
{
	string buf;
		
		// Pre-allocate enough space.
	buf.reserve( 35 );
		
	int quotient = value;
		// Translating number to string with base:
	do {
		buf += "0123456789"[ std::abs( quotient % 10 ) ];
		quotient /= 10;
	} while ( quotient );
		
		// Append the negative sign for base 10
	if ( value < 0 ) buf += '-';
		
	std::reverse( buf.begin(), buf.end() );
		
	return buf;
}
		
void textPutOn(bool isColor, u16* mapBase, int x, int y, const char* fmt, ...)
{
	char temp[256];
		
	va_list marker;
	va_start(marker,fmt);
	vsprintf(temp,fmt,marker);
	va_end(marker);
		
	int len = strlen(temp);
	int posX = x;
	int posY = y;
	int textPos = 0;
	int curColor = isColor ? TEXT_COLOR_NORMAL : 0;
		
	while(textPos < len) {
		if(temp[textPos] == '\n') {
			++posY;
			posX = x;
		} else if(temp[textPos] < 32) {
			if(isColor) {
				if(curColor != temp[textPos])
					curColor = temp[textPos];
				else
					curColor = TEXT_COLOR_NORMAL;
			}
		} else {	
			mapBase[posX + posY*32] = (temp[textPos]-32) | (curColor<<12);
			++posX;
		}
		++textPos;
	}
}
		
void putText(int x, int y, const char* fmt, ...)
{
	char temp[256];
		
	va_list marker;
	va_start(marker,fmt);
	vsprintf(temp,fmt,marker);
	va_end(marker);
		
	textPutOn(true, (u16*)BG_MAP_RAM_SUB(3), x, y, temp);
}

void menuText(int x, int y, const char* fmt, ...)
{
	char temp[256];
		
	va_list marker;
	va_start(marker,fmt);
	vsprintf(temp,fmt,marker);
	va_end(marker);
		
	textPutOn(true, (u16*)BG_MAP_RAM_SUB(4), x, y, temp);
}

void clearTextOn(u16* mapBase, int x, int y, int dimX, int dimY)
{
	for(int xx = x; xx < x+dimX; ++xx)
		for(int yy = y; yy < y+dimY; ++yy)
			mapBase[xx + yy*32] = 0;
}

void clearMenuText(int x, int y, int dimX, int dimY)
{
	clearTextOn((u16*)BG_MAP_RAM_SUB(4), x, y, dimX, dimY);
}

void clearText(int x, int y, int dimX, int dimY)
{
	clearTextOn((u16*)BG_MAP_RAM_SUB(3), x, y, dimX, dimY);
}

void initText()
{
		// Those things were so small, it was stupid to compress...
	dmaCopy(textBoxPal_bin, BG_PALETTE_SUB, 16);
	dmaCopy(aButton_bin, SPRITE_GFX_SUB, aButton_bin_size);
	dmaCopy(emmlemPal_bin,  SPRITE_PALETTE_SUB, emmlemPal_bin_size);
	dmaCopy(spr16Pal_bin,  SPRITE_PALETTE_SUB + 16*15, spr16Pal_bin_size);
	dmaCopy(textCursor_bin, SPRITE_GFX_SUB + 16*16/2 + 64*64/2, textCursor_bin_size);
		// Decompressing...
	decompressToVRAM(textBoxData_bin, (void*)BG_TILE_RAM_SUB(0));
	decompressToVRAM(textFont_bin, (void*)BG_TILE_RAM_SUB(1));
	decompressToVRAM(playerEmblem_bin, SPRITE_GFX_SUB + 16*16/2);
		// Text colors
	BG_PALETTE_SUB[17] = RGB15(31,31,31);
	BG_PALETTE_SUB[33] = RGB15(31, 0, 0);
	BG_PALETTE_SUB[49] = RGB15(0, 20, 7);
	BG_PALETTE_SUB[65] = RGB15(28, 28, 9);
	BG_PALETTE_SUB[81] = RGB15(20, 20, 20);
		
	for(int i=0; i<32*32; i++) {
		((u16*)BG_MAP_RAM_SUB(2))[i] = 9;
		((u16*)BG_MAP_RAM_SUB(5))[i] = 9;
	}
		// Filling boring vars
	textVars.curColor = TEXT_COLOR_NORMAL;
	textVars.curMode = TEXTMODE_IDLE;
	textVars.txtPos = textVars.posX = textVars.posY = 0;
	textVars.textMap = (u16 *)BG_MAP_RAM_SUB(4);
	memset((u16*)BG_MAP_RAM_SUB(3), 0, 32*32*2);
	memset((u16*)BG_MAP_RAM_SUB(4), 0, 32*32*2);
	textVars.scroolY = 52;
	textVars.txtLen = 0;
	textVars.txtPos = 0;
	textVars.posX = 0;
	textVars.posY = 19;
	textVars.virtualPosY = 19;
	textVars.needInput = true; // Default
	textVars.pauseMode = false;
	textVars.writeStatics = true;
	textVars.menuSel = 0;
		
	textVars.aButton = &spritesSub[0];
	textVars.emblemFigure = &spritesSub[1];
	textVars.cursorSprite = &spritesSub[2];
	textVars.aButton->attribute[0] = ATTR0_NORMAL | ATTR0_COLOR_16;
	textVars.aButton->attribute[1] = ATTR1_SIZE_16;
	textVars.aButton->attribute[2] = ATTR2_PALETTE(15) | ATTR2_PRIORITY(1);
	textVars.cursorSprite->attribute[0] = ATTR0_NORMAL | ATTR0_COLOR_16;
	textVars.cursorSprite->attribute[1] = ATTR1_SIZE_16;
	textVars.cursorSprite->attribute[2] = ATTR2_PALETTE(15) | 136;
	
	textVars.cursorSpriteMain = &spritesMain[50];
	textVars.cursorSpriteMain->attribute[0] = ATTR0_NORMAL | ATTR0_COLOR_16;
	textVars.cursorSpriteMain->attribute[1] = ATTR1_SIZE_16;
	textVars.cursorSpriteMain->attribute[2] = ATTR2_PALETTE(15) | 1000;
		
	textVars.emblemFigure->attribute[0] = ATTR0_NORMAL | ATTR0_COLOR_256 | 8;
	textVars.emblemFigure->attribute[1] = ATTR1_SIZE_64 | 8+2;
	textVars.emblemFigure->attribute[2] = 8 | ATTR2_PRIORITY(2);
		
	hideSprite(textVars.aButton, true);
	hideSprite(textVars.cursorSprite, true);
	hideSprite(textVars.cursorSpriteMain, true);
		
	SUB_BG1_X0 = 256-5;
	SUB_BG1_Y0 = 256-5;
	SUB_BG2_X0 = 2;
	SUB_BG2_Y0 = 5;
	SUB_BG3_Y0 = 256-2;
		
		// Menus
	textVars.curMenu = &textVars.smallMenu;
	textVars.smallMenu.clear();
	textVars.smallMenu.push_back("Item");
	textVars.smallMenu.push_back("Magic");
	textVars.smallMenu.push_back("Equip");
	textVars.smallMenu.push_back("Config");
	textVars.smallMenu.push_back("Save");
	textVars.smallMenu.push_back("Resume");
		
	drawRectTop(0, 0, 32, 7);
	drawRectBottom(0, 0, 12, 15);
	drawRectBottom(21, 0, 9, 9);
}

void textHandleVBlank()
{
	switch(gameValues.curMode) {
		case LEVELMODE_SPLASH_FADEIN:
		case LEVELMODE_SPLASH:
		case LEVELMODE_SPLASH_FADEOUT:
		case LEVELMODE_MENU_FADEIN:
		case LEVELMODE_MENU_START:
		case LEVELMODE_MENU_CHOICE:
		case LEVELMODE_MENU_LOAD:
			SUB_BG0_Y0 = 256-10;
				return;
		case LEVELMODE_INTRO:
		case LEVELMODE_PLAY:
			break;
		case LEVELMODE_INGAME_MENU:
				textVars.scroolY = 50;
				textVars.curMode = TEXTMODE_PRINTDONE;
			break;
		case LEVELMODE_FADEOUT:
		case LEVELMODE_SWITCHLEVEL:
		case LEVELMODE_FADEIN:
		case LEVELMODE_BATTLE_FADEIN:
		case LEVELMODE_BATTLE_FADEOUT:
		case LEVELMODE_BATTLE_FADEPAUSE:
		case LEVELMODE_BATTLE:
			break;
		case LEVELMODE_GAMEOVER_FADEOUT:
		case LEVELMODE_GAMEOVER:
		case LEVELMODE_GAMEOVER_FADEIN:
		case LEVELMODE_GAMEOVER_WHITEFADE:
			textVars.scroolY = 52*2;
		case LEVELMODE_EXITBATTLE:
				break;
			break;
	}
		
	const int spriteX = 256-16+4, spriteY = 192-16+4;
	SUB_BG0_Y0 = 256-(192-52)-textVars.scroolY;
		
	string tempStr;
		
	switch(textVars.curMode) {
		case TEXTMODE_IDLE:
			break;
		case TEXTMODE_WINDOW_UP:
			break;
		case TEXTMODE_WINDOW_DIRUP:
				textVars.scroolY -= 2;
				if(textVars.scroolY < 0) {
					textVars.scroolY = 0;
					if(textVars.txtLen)
						textVars.curMode = TEXTMODE_WRITING;
					else
						textVars.curMode = TEXTMODE_WINDOW_UP;
				}
			break;
		case TEXTMODE_WRITING:
				hideSprite(textVars.aButton, true);
				if( textVars.txtLen > 0 ) {
						// Handle chars other then text
					if(textVars.txtBuffer[textVars.txtPos] < 32) {
						if(textVars.txtBuffer[textVars.txtPos] < TEXT_COLOR_SIZE) {
							if(textVars.curColor == textVars.txtBuffer[textVars.txtPos])
								textVars.curColor = TEXT_COLOR_NORMAL;
							else
								textVars.curColor = (textColor)textVars.txtBuffer[textVars.txtPos];
						} else if(textVars.txtBuffer[textVars.txtPos] == '\n') { 
							textVars.posX = 0;
							++textVars.posY;
							++textVars.virtualPosY;
						} else if(textVars.txtBuffer[textVars.txtPos] == '\6') { // New window
							textVars.curMode = TEXTMODE_WAITINGFORINPUT;
						} else if(textVars.txtBuffer[textVars.txtPos] > '\12') { // Yes/no Questions
							textVars.menuSel = 1;
							textVars.curMode = TEXTMODE_FIGUREMENU;
							textVars.yesNoCode = textVars.txtBuffer[textVars.txtPos] + 2;
						} 
					} else {
						if(textVars.txtBuffer[textVars.txtPos] == ' ') { 
								// We can't just write space at the start, can't we...
							if(textVars.posX == 0) {
								++textVars.txtPos;
								break;
							}
							int tempPos = textVars.txtPos+1;
							int sizeCou = 0;
							while(tempPos < (int)strlen(textVars.txtBuffer)) {
								if(textVars.txtBuffer[tempPos] == ' ')
									break;
								if(textVars.txtBuffer[tempPos] == '\n')
									break;
								++tempPos;
									
								if(textVars.txtBuffer[tempPos] >= 32)
									++sizeCou;
							}
							if(textVars.posX + sizeCou > 28) {
								textVars.posX = -1;
								++textVars.posY;
								++textVars.virtualPosY;
							}
						}
					}
					if(textVars.posY > 23)
						textVars.curMode = TEXTMODE_WAITINGFORINPUT;
					else {
						if(textVars.txtBuffer[textVars.txtPos] >= 32) {
							textPrintChar(++textVars.posX, textVars.posY, textVars.txtPos++);
						} else
							++textVars.txtPos;
						--textVars.txtLen;
					}
				} else {
					if(!textVars.pauseMode) {
						if(textVars.needInput)
								textVars.curMode = TEXTMODE_WAITINGFORINPUT;
						else {
							textVars.curMode = TEXTMODE_PRINTDONE;
							textVars.needInput = true; // default
						}
					}
				}
			break;
		case TEXTMODE_WAITINGFORINPUT:
				hideSprite(textVars.aButton, false);
				moveSprite(textVars.aButton, spriteX, spriteY);
				if(KEYS & KEY_A) {
					moveSprite(textVars.aButton, spriteX+1, spriteY+1);
					if(textVars.txtLen) {
						memset(textVars.textMap, 0, 32*32*2);
						textVars.posX = 0;
						textVars.posY = 19;
						textVars.curMode = TEXTMODE_WRITING;
					} else
						textVars.curMode = TEXTMODE_PRINTDONE;
				}
			break;
		case TEXTMODE_FIGUREMENU:
				hideSprite(textVars.aButton, true);
				hideSprite(textVars.cursorSprite, false);
					
				if(KEYS & KEY_LEFT)
					textVars.menuSel = 1;
				else if(KEYS & KEY_RIGHT)
					textVars.menuSel = 0;
					
				moveSprite(textVars.cursorSprite, 5*8 + 15*(textVars.menuSel^1)*8, 22*8);
				menuText( 7,22, "Yes");
				menuText(22,22, "No");
					
					// It's hard coded, no REAL script engine yet...
				if(KEYS & KEY_A) {
					hideSprite(textVars.cursorSprite, true);
					switch(textVars.yesNoCode) {
						case 13:	// Inn
								handleInnAnswer(textVars.menuSel);
							break;
						case 14:	// Shop
								handleShopAnswer(textVars.menuSel);
							break;
					}
				}
			break;
		case TEXTMODE_SHOP:
				handleShop();
			break;
		case TEXTMODE_PRINTDONE:
				hideSprite(textVars.aButton, true);
				memset(textVars.textMap, 0, 32*32*2);
				textVars.curMode = TEXTMODE_WINDOW_DIRDOWN;
			break;
		case TEXTMODE_WINDOW_DIRDOWN:
				textVars.scroolY += 2;
				if(textVars.scroolY > 52) {
					textVars.scroolY = 52;
					textVars.txtPos = 0;
					textVars.posX = 0;
					textVars.posY = 19;
					textVars.virtualPosY = 19;
					textVars.curMode = TEXTMODE_IDLE;
					textVars.curColor = TEXT_COLOR_NORMAL;
						
					for(list<pFigure>::iterator i=activeFigures.begin(); i!=activeFigures.end(); ++i) {
						pFigure curFigure = *i;
							
						curFigure->curDir = curFigure->lstDir;
					}
				}
			break;
	}
}

	// Mainly top screen HBlank effects
void textHandleHBlank()
{	
	switch(textVars.curMode) {
		case TEXTMODE_IDLE:
				if(gameValues.curMode > LEVELMODE_INTRO)
					BG_PALETTE_SUB[1] = ((u16*)palGrad_bin)[DISP_Y/4];
			break;
		case TEXTMODE_WINDOW_UP:
		case TEXTMODE_WINDOW_DIRUP:
		case TEXTMODE_WRITING:
		case TEXTMODE_WAITINGFORINPUT:
		case TEXTMODE_PRINTDONE:
		case TEXTMODE_WINDOW_DIRDOWN:	
		case TEXTMODE_FIGUREMENU:
		case TEXTMODE_SHOP:
				BG_PALETTE_SUB[1] = ((u16*)palGrad_bin)[DISP_Y/4];
				if(DISP_Y > 192-(52-textVars.scroolY))
					BG_PALETTE_SUB[1] = ((u16*)palGrad_bin)[DISP_Y-(192-52)-textVars.scroolY];
			break;
	}
}

	// No idea why this can't be static inline :/
void textPrintChar(int posX, int posY, int bufferPos)
{
	textVars.textMap[posX + posY*32] = (textVars.txtBuffer[bufferPos] - 32) | (textVars.curColor << 12);
}

void textWrite(const char* selText)
{
	if(textVars.curMode == TEXTMODE_IDLE) {
		textVars.txtLen = strlen(selText);
		strcpy(textVars.txtBuffer, selText);
		textVars.curMode = TEXTMODE_WINDOW_DIRUP;
			// Init vars
		textVars.posX = 0;
		textVars.virtualPosY = textVars.posY = 19;
		textVars.txtPos = 0;
	}
}

void undrawRectOn(u16* workMap, int posX, int posY, int dimX, int dimY)
{
	for(int x=posX; x<posX+dimX; ++x)
		for(int y=posY; y<posY+dimY; ++y)
			workMap[x + y*32] = 9;
}

void drawRectOn(u16* workMap, int posX, int posY, int dimX, int dimY)
{
	--dimX; --dimY;
	workMap[posX + posY*32] = 0;
	workMap[(posX+dimX) + posY*32] = 2;
	workMap[posX + (posY+dimY)*32] = 6;
	workMap[(posX+dimX) + (posY+dimY)*32] = 8;
	for(int i=1; i<dimX; i++) {
		workMap[(posX+i) + posY*32] = 1;
		workMap[(posX+i) + (posY+dimY)*32] = 7;
	} for(int i=1; i<dimY; i++) {
		workMap[posX + (posY+i)*32] = 3;
		workMap[(posX+dimX) + (posY+i)*32] = 5;
	}
		
	for(int y=posY+1; y<posY+dimY; y++)
		for(int x=posX+1; x<posX+dimX; x++)
			workMap[x + y*32] = 4;
}
	
	// Same for those ones...
void drawRectTop(int posX, int posY, int dimX, int dimY)
{
	drawRectOn((u16*)BG_MAP_RAM_SUB(2), posX, posY, dimX, dimY);
}

void drawRectBottom(int posX, int posY, int dimX, int dimY)
{
	drawRectOn((u16*)BG_MAP_RAM_SUB(5), posX, posY, dimX, dimY);
}

void undrawRectTop(int posX, int posY, int dimX, int dimY)
{
	undrawRectOn((u16*)BG_MAP_RAM_SUB(2), posX, posY, dimX, dimY);
}

void undrawRectBottom(int posX, int posY, int dimX, int dimY)
{
	undrawRectOn((u16*)BG_MAP_RAM_SUB(5), posX, posY, dimX, dimY);
}

void writeStaticsWith(pItem item)
{
	const char* attrs[] = {
		"STR", "DEF", "AGI", "VIT", "INT", "DEX"
	};
		
	putText(1, 9, "\2Alex Lv%d \2", curPlayer->Attr[LEVEL]);
	putText(1, 10, "HP : %3d", curPlayer->Attr[HP]);
	putText(1, 11, "MP : %3d", curPlayer->Attr[MP]);
	putText(1, 12, "\4%6d G\4", curPlayer->Attr[GOLD]);
		
	Figure tempFigure = *curPlayer;
	itemAttr ItemAttr;
	switch(item->itemType) {
		case WEAPON_SWORD:
		case WEAPON_SPEAR:
		case WEAPON_AXE:
		case WEAPON_BOW:
		case WEAPON_GUN:
		case WEAPON_CLUB:
		case WEAPON_STAFF:
				ItemAttr = WEAPON;
			break;
		default:
				ItemAttr = item->itemType;
			break;
	} 
		
	for(int i=STR; i < DEX+1; ++i) {
		int before = tempFigure.Attr[i];
		if(curPlayer->equipedItems[ItemAttr] != -1)
			tempFigure.Attr[i] -= itemDB[curPlayer->equipedItems[ItemAttr]].itemChanges[i];
		int deltaAttr = before - tempFigure.Attr[i];
			
		if(deltaAttr > item->itemChanges[i])
			putText(22, 1 + (i-STR), "%s:\2%2d\2 ", attrs[i-STR], tempFigure.Attr[i] + item->itemChanges[i]);
		else if(deltaAttr < item->itemChanges[i])
			putText(22, 1 + (i-STR), "%s:\3%2d\3 ", attrs[i-STR], tempFigure.Attr[i] + item->itemChanges[i]);
		else
			putText(22, 1 + (i-STR), "%s:%2d ", attrs[i-STR], tempFigure.Attr[i] + item->itemChanges[i]);
	}
}

void writeStatics()
{
		// Fake writing with a potion which doesn't add anything.
		// as called by cReDiAr - //hack :)
	writeStaticsWith((pItem)&itemDB[0]);
}

	// For battle engine
void printIndicatorName(int posX, int posY, const char* name, int curHp, int maxHp)
{
	int third = maxHp / 3;
	if(curHp < third)
		menuText(posX, posY, "\2%s\2", name);
	else if(curHp < third*2)
		menuText(posX, posY, "\4%s\4", name);
	else
		menuText(posX, posY, "\3%s\3", name);
}
