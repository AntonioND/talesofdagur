#include "sprites.h"
		
		//Data
SpriteEntry spritesMain[128];
SpriteEntry spritesSub[128];
pSpriteRotation spriteRotationsMain = (pSpriteRotation)spritesMain;
pSpriteRotation spriteRotationsSub = (pSpriteRotation)spritesSub;
pFigure curPlayer;
list<pFigure > activeFigures;
list<pFigure > onScreenFigures;
list<pChest > activeChests;
list<pChest > onScreenChests;
		
		// Figures
Figure alexFigure;
Figure kingFigure;
Figure demitriFigure;
Figure guardFigure[5];
Figure civilianFigure[12];
Figure doorFigure;
Chest chestFigure[21];
Figure maidFigure, bartenderFigure, traderFigure;
Figure lumberjackFigure[2];
Figure dingFigure, dongFigure, dangFigure;
Figure vardaFigure, queenFigure;
		
const u8* chestData[] = {
	chest1Figure_bin, chest2Figure_bin, chest3Figure_bin, chest4Figure_bin,
};

const u16* chestPal[] = {
	(u16*)chest1FigurePal_bin, (u16*)chest2FigurePal_bin, (u16*)chest3FigurePal_bin, (u16*)chest4FigurePal_bin,
};
		
		//Functions
	// Inits the OAM
void initOAM(void)
{
	for(int i = 0; i < 128; i++) {
		spritesMain[i].attribute[0] = ATTR0_DISABLED;
		spritesMain[i].attribute[1] = 0;
		spritesMain[i].attribute[2] = 0;
			
		spritesSub[i].attribute[0] = ATTR0_DISABLED;
		spritesSub[i].attribute[1] = 0;
		spritesSub[i].attribute[2] = 0;
	} for(int i = 0; i < 32; i++) {
		spriteRotationsMain[i].hdx = 1 << 8;
		spriteRotationsMain[i].hdy = 0;
		spriteRotationsMain[i].vdx = 0;
		spriteRotationsMain[i].vdy = 1 << 8;
			
		spriteRotationsSub[i].hdx = 1 << 8;
		spriteRotationsSub[i].hdy = 0;
		spriteRotationsSub[i].vdx = 0;
		spriteRotationsSub[i].vdy = 1 << 8; 
	}
	updateOAM();
}

	// Updates the OAM
void updateOAM(void)
{
	DC_FlushAll();
	dmaCopy(spritesMain, OAM, 		128 * sizeof(SpriteEntry));
	dmaCopy(spritesSub,  OAM_SUB, 	128 * sizeof(SpriteEntry));
}

	// Moves a Sprite
void moveSprite(pSpriteEntry sp, u16 x, u16 y)
{
	sp->attribute[1] &= 0xFE00;
	sp->attribute[1] |= (x & 0x01FF);
		
	sp->attribute[0] &= 0xFF00;
	sp->attribute[0] |= (y & 0x00FF);
} 

	// Rotates a Sprite (I Hate math (¬¬))
void rotateSprite(pSpriteRotation rotdata, u16 angle)
{
	s16 s = -sinLerp(64 * (angle & 0x1FF)) >> 4;
	s16 c =  cosLerp(64 * (angle & 0x1FF)) >> 4;
		
	rotdata->hdx =  c;
	rotdata->hdy = -s;
	rotdata->vdx =  s;
	rotdata->vdy =  c;
}

	// Zoom a Sprite (I Hate math (¬¬))
void zoomSprite(pSpriteRotation rotdata, u16 z)
{
	rotdata->hdx =  (rotdata->hdx * z) >> 8;
	rotdata->hdy =  (rotdata->hdy * z) >> 8;
	rotdata->vdx =  (rotdata->vdx * z) >> 8;
	rotdata->vdy =  (rotdata->vdy * z) >> 8;
}
	
	// Rotates THEN scale sprite (No, you can't use the two functions together)
void rotScaleSprite(pSpriteRotation rotdata, u16 angle, u16 z)
{
	s16 s = -sinLerp(64 * (angle & 0x1FF)) >> 4;
	s16 c =  cosLerp(64 * (angle & 0x1FF)) >> 4;
		
	rotdata->hdx = ( c * z ) >> 8;
	rotdata->hdy = (-s * z ) >> 8;
	rotdata->vdx = ( s * z ) >> 8;
	rotdata->vdy = ( c * z ) >> 8;
}

void hideSprite(pSpriteEntry sp, bool hide)
{
	if(hide)
		sp->attribute[0] |= ATTR0_DISABLED;
	else
		sp->attribute[0] &= ~ATTR0_DISABLED;
}

void hideRotScaleSprite(pSpriteEntry sp, bool hide)
{
	sp->attribute[0] &= ~ATTR0_ROTSCALE_DOUBLE;
	if(hide)
		sp->attribute[0] |= ATTR0_DISABLED;
	else
		sp->attribute[0] |= ATTR0_ROTSCALE;
}

void setSpritePriority(pSpriteEntry sp, int pri)
{
	sp->attribute[2] &= ~ATTR2_PRIORITY(3);
	sp->attribute[2] |= ATTR2_PRIORITY(pri);
}

void initFigure(const char* selName, pFigure theFigure, spriteData allData, Palette palData)
{
	int palOffset = 0;
	while(SPRITE_PALETTE[palOffset << 4] != 0x000)
		++palOffset;
		
	if(palOffset > 15)
		palOffset = 15;
		
	theFigure->figName = selName;
	theFigure->spriteOffset = palOffset * 2;
	theFigure->palOffset = palOffset;
		
	theFigure->dimX = 32;
	theFigure->dimY = 48;
	theFigure->animSize = 4;
		
	theFigure->upperSprite = &spritesMain[theFigure->spriteOffset];
	theFigure->lowerSprite = &spritesMain[theFigure->spriteOffset + 1];
	int dataOffset = 4*6*palOffset;
	theFigure->dataOffset = dataOffset * 16;
		
	theFigure->upperSprite->attribute[0] = ATTR0_NORMAL | ATTR0_COLOR_16;
	theFigure->upperSprite->attribute[1] = ATTR1_SIZE_32;
	theFigure->upperSprite->attribute[2] = ATTR2_PALETTE(palOffset) | dataOffset;
		
	theFigure->lowerSprite->attribute[0] = ATTR0_WIDE | ATTR0_COLOR_16;
	theFigure->lowerSprite->attribute[1] = ATTR1_SIZE_32;
	theFigure->lowerSprite->attribute[2] = ATTR2_PALETTE(palOffset) | (dataOffset+16);
		
	theFigure->allData = allData;
	theFigure->palData = palData;
		
	theFigure->posX = 0;
	theFigure->posY = 0;
	theFigure->spriteIndex = 0;
	theFigure->talkCount = 0;
		
	theFigure->curDir = theFigure->lstDir = DIR_DOWN;
	theFigure->curMove = MOVE_NOT;
	theFigure->spriteDirIndex = 0;
	theFigure->lastFrame = frameCounter;
	theFigure->isMotion = false;
	theFigure->isEnabled = true;
	theFigure->movQue.clear();
	theFigure->motionInterval = 10;
	theFigure->figureItemList.clear();
	for(int i=0; i<ITEM_ATTR_SIZE; ++i) 
		theFigure->equipedItems[i] = -1;
		
	dmaCopy(theFigure->palData, SPRITE_PALETTE + theFigure->palOffset*16, 32);
		
	updateFigure(theFigure);
	activeFigures.push_back(theFigure);
}

void disableFigure(pFigure theFigure)
{
	if(!theFigure->isEnabled)
		return;
		
	SPRITE_PALETTE[theFigure->palOffset << 4] = 0x0000;
	hideSprite(theFigure->lowerSprite, true);
	hideSprite(theFigure->upperSprite, true);
	theFigure->isEnabled = false;
}

void enableFigure(pFigure theFigure)
{
	if(theFigure->isEnabled)
		return;
		
	int palOffset = 0;
	while(SPRITE_PALETTE[palOffset << 4] != 0x000)
		++palOffset;
		
		// Sorry sprite, no room for you :(
	if(palOffset > 15)
		return;
		
	theFigure->spriteOffset = palOffset * 2;
	theFigure->palOffset = palOffset;
	dmaCopy(theFigure->palData, SPRITE_PALETTE + theFigure->palOffset*16, 32);
		
	theFigure->upperSprite = &spritesMain[theFigure->spriteOffset];
	theFigure->lowerSprite = &spritesMain[theFigure->spriteOffset + 1];
	int dataOffset = 4*6*palOffset;
	theFigure->dataOffset = dataOffset * 16;
		
	theFigure->upperSprite->attribute[0] = ATTR0_NORMAL | ATTR0_COLOR_16;
	theFigure->upperSprite->attribute[1] = ATTR1_SIZE_32;
	theFigure->upperSprite->attribute[2] = ATTR2_PALETTE(palOffset) | dataOffset;
		
	theFigure->lowerSprite->attribute[0] = ATTR0_WIDE | ATTR0_COLOR_16;
	theFigure->lowerSprite->attribute[1] = ATTR1_SIZE_32;
	theFigure->lowerSprite->attribute[2] = ATTR2_PALETTE(palOffset) | (dataOffset+16);
		
	theFigure->isEnabled = true;
	updateFigure(theFigure);
}

void setDirFigure(pFigure Figure, Direction Dir)
{
	Figure->curDir = Dir;
	Figure->lstDir = Dir;
}

void updateSpriteData(pFigure theFigure)
{
	if(!theFigure->isEnabled)
		return;
		
	const u8* sourceImage = theFigure->allData + ((32*48/2)*theFigure->animSize*theFigure->spriteDirIndex) + (32*48*(theFigure->spriteIndex % theFigure->animSize))/2;
	dmaCopy(sourceImage, SPRITE_GFX + theFigure->dataOffset, 32*48/2);
}

void setPosFigure(pFigure theFigure, int posX, int posY)
{
	theFigure->posX = posX;
	theFigure->posY = posY;
		
	theFigure->virtualPosX = posX - curLevel->viewX;
	theFigure->virtualPosY = posY - curLevel->viewY;
		
	theFigure->isActive = true;
		// Out of sight
	if(theFigure->virtualPosX + theFigure->dimX < 0)
		theFigure->isActive = false;
		
	if(theFigure->virtualPosY + theFigure->dimY < 0)
		theFigure->isActive = false;
		
	if(theFigure->virtualPosX > 256)
		theFigure->isActive = false;
		
	if(theFigure->virtualPosY > 192)
		theFigure->isActive = false;
		
	if(!theFigure->isActive) {
		theFigure->virtualPosX = 256;
		theFigure->virtualPosY = 256;
	}
		
	moveSprite(theFigure->upperSprite, theFigure->virtualPosX, theFigure->virtualPosY);
	moveSprite(theFigure->lowerSprite, theFigure->virtualPosX, theFigure->virtualPosY+32);
		
	updateSpriteData(theFigure);
}

void updateFigure(pFigure theFigure)
{
	setPosFigure(theFigure, theFigure->posX, theFigure->posY);
	if(theFigure->curDir == DIR_RIGHT) {
		theFigure->lowerSprite->attribute[1] |= ATTR1_FLIP_X;
		theFigure->upperSprite->attribute[1] |= ATTR1_FLIP_X;
		theFigure->spriteDirIndex = DIR_LEFT;
	} else {
		theFigure->lowerSprite->attribute[1] &= ~ATTR1_FLIP_X;
		theFigure->upperSprite->attribute[1] &= ~ATTR1_FLIP_X;
		theFigure->spriteDirIndex = theFigure->curDir;
	}
	updateSpriteData(theFigure);
}

bool advanceFigure(pFigure tFigure)
{
	if(tFigure->movQue.empty())
		return false; 
		
	moveData movData = tFigure->movQue.front();
		
	int deltax, deltay;
	int d;
	// int dinc1, dinc2;
	int xinc1, xinc2;
	int yinc1, yinc2;
	deltax = abs(movData.destX - tFigure->posX);
	deltay = abs(movData.destY - tFigure->posY);
	if(deltax >= deltay) {
		d = (2 * deltay) - deltax;
		//dinc1 = deltay << 1;
		//dinc2 = (deltay - deltax) << 1;
		xinc1 = xinc2 = yinc2 = 1;
		yinc1 = 0;
	} else {
		d = (2 * deltax) - deltay;
		//dinc1 = deltax << 1;
		//dinc2 = (deltax - deltay) << 1;
		xinc1 = 0;
		xinc2 = yinc1 = yinc2 = 1;
	}
	if(tFigure->posX > movData.destX) {
		xinc1 = -xinc1;
		xinc2 = -xinc2;
	}
	if(tFigure->posY > movData.destY) {
		yinc1 = -yinc1;
		yinc2 = -yinc2;
	}
	
	if(deltax > deltay) {
		if(tFigure->posX > movData.destX)
			setDirFigure(tFigure, DIR_LEFT);
		else if(tFigure->posX < movData.destX)
			setDirFigure(tFigure, DIR_RIGHT);
	} else {
		if(tFigure->posY > movData.destY)
			setDirFigure(tFigure, DIR_UP);
		else if(tFigure->posY < movData.destY)
			setDirFigure(tFigure, DIR_DOWN);
	}
	
	if(d < 0) {
		tFigure->posX += xinc1;
		if(isCollision(tFigure))
			tFigure->posX -= xinc1;
			
		tFigure->posY += yinc1;
		if(isCollision(tFigure))
			tFigure->posY -= yinc1;
	} else {
		tFigure->posX += xinc2;
		if(isCollision(tFigure))
			tFigure->posX -= xinc2;
			
		tFigure->posY += yinc2;
		if(isCollision(tFigure))
			tFigure->posY -= yinc2;
	}
			// No more moves left
	if((movData.destY == tFigure->posY) && (movData.destX == tFigure->posX)) {
		if(!movData.isRepeat) {
			tFigure->movQue.pop_front();
			tFigure->curDir = tFigure->lstDir;
		} else {
			movData.destX = movData.startX;
			movData.destY = movData.startY;
			movData.startX = tFigure->posX;
			movData.startY = tFigure->posY;
			tFigure->movQue.pop_front();
			tFigure->movQue.push_front(movData);
		}
	}
		
	return true;
}

void moveFigure(pFigure theFigure, int posX, int posY, bool isRepeat)
{
	moveData tempData;
	tempData.destX = posX;
	tempData.destY = posY;
	tempData.startX = theFigure->posX;
	tempData.startY = theFigure->posY;
	tempData.isRepeat = isRepeat;
	theFigure->movQue.push_back(tempData);
}

bool isCollision(pFigure Figure)
{
	for(list<pFigure>::iterator i=onScreenFigures.begin(); i!=onScreenFigures.end(); ++i) {
		pFigure curFigure = *i;
			
		if(curFigure != Figure)
			if(isCollisionFigures(Figure, curFigure))
				return true;
	}
		// Checking for chests collision
	for(list<pChest>::iterator i=onScreenChests.begin(); i!=onScreenChests.end(); ++i) {
		pFigure curFigure = &((*i)->cFigure);
			
		if( isCollisionFigures(Figure, curFigure) )
			return true;
	}
		// No collision?! Check with the BG!
	return isCollisionBackground(Figure);
}

	// Checks collision between two figures
bool isCollisionFigures(pFigure Figure1, pFigure Figure2)
{
	int startX1 = Figure1->posX + 10;
	int startY1 = Figure1->posY + 32;
	int endX1 = startX1 + Figure1->dimX - 10;
	int endY1 = startY1 + Figure1->dimY - 32;
		
	int startX2 = Figure2->posX + 10;
	int startY2 = Figure2->posY + 32;
	int endX2 = startX2 + Figure2->dimX - 10;
	int endY2 = startY2 + Figure2->dimY - 32;
		
	if(startX1 > endX2)
		return false;
		
	if(endX1 < startX2)
		return false;
		
	if(startY1 > endY2)
		return false;
		
	if(endY1 < startY2)
		return false;
		
	return true;
}

u32 getColTile(int posX, int posY)
{
	return curLevel->colMap[(posX / 8) + (posY / 8) * curLevel->dimX/8];	
}

bool isCollisionBackground(pFigure Figure)
{
	int startX = Figure->posX + 5;
	int startY = Figure->posY + 32;
	int endX = Figure->posX + Figure->dimX - 5;
	int endY = Figure->posY + Figure->dimY;
		
	if(startX > curLevel->dimX)
		startX = curLevel->dimX;
	if(startY > curLevel->dimY)
		startY = curLevel->dimY;
	if(endX > curLevel->dimX) {
		startX = Figure->posX - 8;
		endX = curLevel->dimX;
	}
	if(endY > curLevel->dimY) {
		startY = Figure->posY - 8;
		endY = curLevel->dimY;
	}
		
	for(int x = startX; x < endX; x += 8) {
		for(int y = startY; y < endY; y += 7) {
			if ( !getColTile(x, y) )
				return true;
		}
	}
	return false;
}
	
	// Middle move 
bool isCollisionDoor(pFigure Figure, pDoorLink doorLink)
{
	int startX1 = Figure->posX;
	int startY1 = Figure->posY + 32;
	int endX1 = startX1 + Figure->dimX;
	int endY1 = startY1 + Figure->dimY - 32;
		
	int startX2 = doorLink->startTileX * 8;
	int startY2 = doorLink->startTileY * 8;
	int endX2 = doorLink->endTileX * 8;
	int endY2 = doorLink->endTileY * 8;
		
	if(startX1 > endX2)
		return false;
		
	if(endX1 < startX2)
		return false;
		
	if(startY1 > endY2)
		return false;
		
	if(endY1 < startY2)
		return false;
		
	return true;
}
	
	// Collision check between chests
bool isCollisionChest(pFigure tFigure, pChest tChest)
{
	Figure tempFigure = *tFigure;
	switch(tChest->cFigure.curDir) {
		case DIR_UP:
				if(tempFigure.curDir != DIR_DOWN)
					return false;
				tempFigure.posY += 5;
			break;
		case DIR_DOWN:
				if(tempFigure.curDir != DIR_UP)
					return false;
				tempFigure.posY -= 5;
			break;
		case DIR_LEFT:
				if(tempFigure.curDir != DIR_RIGHT)
					return false;
				tempFigure.posX += 5;
			break;
		case DIR_RIGHT:
				if(tempFigure.curDir != DIR_LEFT)
					return false;
				tempFigure.posX -= 5;
			break;
		default:
			return false;
	}
	if(isCollisionFigures(&tempFigure, &tChest->cFigure))
		return true;
		
	return false;
}
	
	// Warning! UGLY code ahead!
	// Can someone optimize it? :(
bool isPlayerChat(pFigure &retFigure)
{
	Figure tempFigure;
	for(list<pFigure>::iterator i=onScreenFigures.begin(); i!=onScreenFigures.end(); ++i) {
		pFigure curFigure = *i;
			if(curFigure == curPlayer)
				continue;
				
		tempFigure = *curPlayer;
		tempFigure.posX += 5;
		if(isCollisionFigures(&tempFigure, curFigure) && curPlayer->curDir == DIR_RIGHT) {
			curFigure->curDir = DIR_LEFT;
			retFigure = curFigure;
			return true;
		}
		tempFigure.posX -= 10;
		if(isCollisionFigures(&tempFigure, curFigure) && curPlayer->curDir == DIR_LEFT) {
			curFigure->curDir = DIR_RIGHT;
			retFigure = curFigure;
			return true;
		}
		tempFigure.posX += 5;
		tempFigure.posY += 5;
		if(isCollisionFigures(&tempFigure, curFigure) && curPlayer->curDir == DIR_DOWN) {
			curFigure->curDir = DIR_UP;
			retFigure = curFigure;
			return true;
		}
		tempFigure.posY -= 10;
		if(isCollisionFigures(&tempFigure, curFigure) && curPlayer->curDir == DIR_UP) {
			curFigure->curDir = DIR_DOWN;
			retFigure = curFigure;
			return true;
		}
	}
	return false;
}

void loadNPCs()
{
	for(list<pFigure>::iterator i=activeFigures.begin(); i!=activeFigures.end(); ++i)
		if(*i != curPlayer)
			disableFigure(*i);
			
	activeFigures.clear();
		
	int npcIndex = 0;
	while(gameNPCs[npcIndex].Figure != NULL) {
		initFigure(gameNPCs[npcIndex].npcName, gameNPCs[npcIndex].Figure, gameNPCs[npcIndex].npcData, gameNPCs[npcIndex].npcPalette);
			
		setDirFigure(gameNPCs[npcIndex].Figure, gameNPCs[npcIndex].faceDir);
		gameNPCs[npcIndex].Figure->levelShow = gameNPCs[npcIndex].linkedLevel;
		gameNPCs[npcIndex].Figure->figText = gameNPCs[npcIndex].npcText;
			
		setPosFigure(gameNPCs[npcIndex].Figure, gameNPCs[npcIndex].posX, gameNPCs[npcIndex].posY);
		if(gameNPCs[npcIndex].moveX | gameNPCs[npcIndex].moveY)
			moveFigure(gameNPCs[npcIndex].Figure, gameNPCs[npcIndex].posX + gameNPCs[npcIndex].moveX, gameNPCs[npcIndex].posY + gameNPCs[npcIndex].moveY, true);
			
		++npcIndex;
	}
		// Update the changes
	handleFigures();
}

void loadChests()
{
	for(list<pChest>::iterator i=activeChests.begin(); i!=activeChests.end(); ++i)
		disableFigure(&(*i)->cFigure);
		
	activeChests.clear();
		
	int curChest = 0;
	while(loadedChests[curChest].chestDir != DIR_NONE) {
		initChest(&chestFigure[curChest], loadedChests[curChest].gfxId, loadedChests[curChest].itemIdx, loadedChests[curChest].quantity);
		setPosFigure(&chestFigure[curChest].cFigure, loadedChests[curChest].posX, loadedChests[curChest].posY);
		chestFigure[curChest].cFigure.levelShow = (LEVELS)loadedChests[curChest].loadedLevel;
		setDirFigure(&chestFigure[curChest].cFigure, loadedChests[curChest].chestDir);
		++curChest;
	}
		// Update the changes
	handleFigures();
}

void initFigures()
{
		// Resets palette indicators
	for(int i=0; i < 256; i += 16)
		SPRITE_PALETTE[i] = 0;
			
		// The main player
	initFigure("Alex", &alexFigure, (u8*)alexFigure_bin, (u16*)alexFigurePal_bin);
		
	curPlayer = &alexFigure;
	setDirFigure(curPlayer, DIR_UP);
	memcpy(curPlayer->Attr, playerStartAttr, sizeof(Attributes));
	setSpritePriority(curPlayer->upperSprite, 2);
	setSpritePriority(curPlayer->lowerSprite, 2);
		
	loadNPCs();
	loadChests();
		
		// Specials
	activeFigures.push_front(curPlayer);
	bartenderFigure.dimY = 64 + 14;
	traderFigure.dimY = 64 + 14;
	doorFigure.dimX = 32*3;
} 

void addItem(pFigure Figure, int itemIdx, int quan)
{
	sprItem tempItem;
	tempItem.itemData = (const pItem)&itemDB[itemIdx];
	tempItem.itemIdx = itemIdx;
	tempItem.quantity = quan;
	tempItem.equiped = false;
	if(Figure->figureItemList.empty()) {
		Figure->figureItemList.push_back(tempItem);
		return;
	} else {
		for(u32 i=0; i < Figure->figureItemList.size(); ++i) {
			if(tempItem.itemIdx == Figure->figureItemList[i].itemIdx) {
				Figure->figureItemList[i].quantity += tempItem.quantity;
				if(Figure->figureItemList[i].quantity > 99)
					Figure->figureItemList[i].quantity = 99;
				return;
			}
		}
	}
	Figure->figureItemList.push_back(tempItem);
}

void removeItem(pFigure Figure, int itemIdx, int quan)
{
	if(Figure->figureItemList.empty()) {
			// Nothing to remove
		return;
	} else {
		vector<sprItem >::iterator itemIter = Figure->figureItemList.begin();
		for(u32 i=0; i < Figure->figureItemList.size(); ++i) {
			if(Figure->figureItemList[i].itemIdx == itemIdx) {
				Figure->figureItemList[i].quantity -= quan;
				if(Figure->figureItemList[i].quantity < 1)
					Figure->figureItemList.erase(itemIter);
				break;
			}
			++itemIter;
		}
	}
}

void receiveItem(int itemIdx, int quan)
{
	string finalMessage = "You have received ";
	if(quan > 1) {
		finalMessage += "\3";
		finalMessage += itemDB[itemIdx].itemName;
		finalMessage += "\3 x" + itoa(quan, 10) + "!";
	} else {
			// Names always starts with a capital letters.
		if(itemDB[itemIdx].itemName[0] == 'E' || itemDB[itemIdx].itemName[0] == 'A')
			finalMessage += "an \3";
		else
			finalMessage += "a \3";
		finalMessage += itemDB[itemIdx].itemName;
		finalMessage += "\3!";
	}
	textVars.txtLen = strlen(finalMessage.c_str());
	strcpy(textVars.txtBuffer, finalMessage.c_str());
	textVars.curMode = TEXTMODE_WINDOW_DIRUP;
	textVars.posX = textVars.txtPos = 0;
	textVars.virtualPosY = textVars.posY = 19;
	addItem(curPlayer, itemIdx, quan);
}

void useItem(pFigure Figure, int itemIdx)
{
	Item curItem = itemDB[itemIdx];
		
	switch(curItem.itemType) {
		case WEAPON_SWORD:
		case WEAPON_SPEAR:
		case WEAPON_AXE:
		case WEAPON_BOW:
		case WEAPON_GUN:
		case WEAPON_CLUB:
		case WEAPON_STAFF:	
		case WEAPON_DAGGER:	
				if(!handleEquip(Figure, WEAPON, itemIdx))
					return;
			break;
		case POTION:
				if(curItem.itemChanges[HP] || curItem.itemChanges[MAX_HP])
					if(Figure->Attr[HP] ==  Figure->Attr[MAX_HP])
						return;
						
				if(curItem.itemChanges[MP] || curItem.itemChanges[MAX_MP])
					if(Figure->Attr[MP] ==  Figure->Attr[MAX_MP])
						return;
						
				removeItem(Figure, itemIdx, 1);
			break;
		case ACCESSORY:
		case HEAD_GEAR:
		case ARMOR:
		case SHIELD:
		case ORB:
				if(!handleEquip(Figure, curItem.itemType, itemIdx))
					return;
			break;
				
		default: 
			return;
	}
	if(curItem.itemChanges[MAX_HP])
		Figure->Attr[HP] = Figure->Attr[MAX_HP];
		
	if(curItem.itemChanges[MAX_MP])
		Figure->Attr[MP] = Figure->Attr[MAX_MP];
		
	curItem.itemChanges[MAX_HP] = 0;
	curItem.itemChanges[MAX_MP] = 0;
	for(int i=0; i<ATTR_SIZE; ++i)
		Figure->Attr[i] += curItem.itemChanges[i];
		
		// Special cases where an att is limited
	if(Figure->Attr[HP] > Figure->Attr[MAX_HP])
		Figure->Attr[HP] = Figure->Attr[MAX_HP];
		
	if(Figure->Attr[MP] > Figure->Attr[MAX_MP])
		Figure->Attr[MP] = Figure->Attr[MAX_MP];
}

void initChest(pChest Chest, int gfxId, int itemIdx, int quantity)
{
	initFigure("Chest", &Chest->cFigure, (u8*)chestData[gfxId], (u16*)chestPal[gfxId]);
	activeFigures.pop_back(); // remove the Figure we just threw in
	Chest->cFigure.isMotion = false;
	Chest->cFigure.animSize = 3;
	Chest->itemIdx = itemIdx;
	Chest->quantity = quantity;
	Chest->isOpen = false;
	Chest->isAnimtion = false;
	activeChests.push_back(Chest);
}

void initWeapon(pWeapon cWeapon, itemAttr wpnAttr)
{
	cWeapon->posX = cWeapon->posY = 0;
	cWeapon->animIdx = 0;
	cWeapon->isActive = true;
		
	int sprOffset = 7;
	cWeapon->wpnSpr = &spritesMain[sprOffset * 2];
	cWeapon->wpnSpr->attribute[0] = ATTR0_NORMAL | ATTR0_COLOR_16;
	cWeapon->wpnSpr->attribute[1] = ATTR1_SIZE_32;
	cWeapon->wpnSpr->attribute[2] = ATTR2_PALETTE(sprOffset) | sprOffset*16;
		
	switch(wpnAttr) {
		case WEAPON_SWORD:
				cWeapon->dataPtr = (u8*)wpnSword_bin;
			break;
		case WEAPON_SPEAR:	
				cWeapon->dataPtr = (u8*)wpnSpear_bin;
			break;
		case WEAPON_AXE:
				cWeapon->dataPtr = (u8*)wpnAxe_bin;
			break;
		case WEAPON_BOW:
				cWeapon->dataPtr = (u8*)wpnBow_bin;
			break;
		case WEAPON_GUN:
				cWeapon->dataPtr = (u8*)wpnGun_bin;
			break;
		case WEAPON_CLUB:
				cWeapon->dataPtr = (u8*)wpnClub_bin;
			break;
		case WEAPON_STAFF:
				cWeapon->dataPtr = (u8*)wpnStaff_bin;
			break;
		case WEAPON_DAGGER:
				cWeapon->dataPtr = (u8*)wpnDagger_bin;
			break;
		default: 
			break;
	}
	dmaCopy(cWeapon->dataPtr, SPRITE_GFX + sprOffset*16*16, 32*32/2);
	dmaCopy(wpnPal_bin, SPRITE_PALETTE + sprOffset*16, 32);
}

void upadteWeapon(pWeapon cWeapon)
{
	if(!cWeapon->isActive)
		return;
		
	if(frameCounter%8 == 0) {
		++cWeapon->animIdx;
		
		int dataOffset = cWeapon->wpnSpr->attribute[2] & 0x3FF;
		dmaCopy(cWeapon->dataPtr + (cWeapon->animIdx%3)*512, SPRITE_GFX + dataOffset*16, 32*32/2);
		
		if(cWeapon->animIdx > 8) {
			hideSprite(cWeapon->wpnSpr, true);
			cWeapon->isActive = false;
		}
	}
}

int updateFigureStates(pFigure Figure)
{
	int retVal = 0;
	while(Figure->Attr[EXP] > Figure->Attr[EXP_TO_NEXT]) {
		Figure->Attr[EXP_TO_NEXT] += (Figure->Attr[EXP_TO_NEXT] * 5) >> 1;
			
		Figure->Attr[HP] += (Figure->Attr[HP] + Figure->Attr[VIT]) >> 1;
		Figure->Attr[MAX_HP] += (Figure->Attr[MAX_HP] + Figure->Attr[VIT]) >> 1;
		Figure->Attr[MP] += (Figure->Attr[MP] + Figure->Attr[INT]) >> 1;
		Figure->Attr[MAX_MP] += (Figure->Attr[MAX_MP] + Figure->Attr[INT]) >> 1;
		++Figure->Attr[LEVEL];
		++retVal;
	}
	return retVal;
}

void setItemEquiped(pFigure curFigure, int itemIdx, bool isEquiped)
{
	for(int i=0; i<(int)curFigure->figureItemList.size(); ++i) {
		if(curFigure->figureItemList[i].itemIdx == itemIdx)
			curFigure->figureItemList[i].equiped = isEquiped;
	}
}
