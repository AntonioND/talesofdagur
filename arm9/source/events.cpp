// SPDX-License-Identifier: WTFPL
//
// SPDX-FileContributor: LiraNuna, 2006

#include "background.h"
#include "events.h"
#include "main.h"
#include "text.h"

#define CS_loadLevel(level, posx, posy, dir) \
    buildCutScene(CS_LOADLEVEL, level, posx, (posy << 2) | dir)

// clang-format off
const char *gameScript[] = {
    "\2Alex\2: Have you called me, my lord?\6"
    "\2King\2: Aye, Alex. Our kingdom is in great trouble...\6"
    "\2King\2: The holy stone has been stolen... You are the only one who can get it back - "
    "The only one suitable for a mission so difficult as this one.\6"
    "If the holy stone won't come back to us soon... we are doomed!... This stone is the only "
    "thing that stands between us and death... The only power we have against our enemies!\6"
    "What say you? Will you do it for your kingdom? Are you ready to sacrifice your life for "
    "it?\6"

    "\2Alex\2: Aye, my lord. If I have to die for my kingdom, so be it!\6"

    "\2King\2: I knew that you won't ever ""disappoint me...\6"
    "Demitri, the Royal councler claims he has seen the incodent. He is still in shock...\6"
    "He is at the top tower of the palace, trying to find out some more information about our "
    "enemy...\6"
    "And with that, my brave boy, I leave you to go to your way. Good luck, son, and be careful!",

    // ---

    "\2Demitri\2: Hello Alex... I've been... Expecting you...",

    // ---

    "\2Alex\2: ???\6"
    "\2Demitri\2: I saw what happened yesterday... It was Varda - an evil witch. Varda had been "
    "trying to take our stone for some time now.\6"
    "She never succeeded thanks to our beloved queen, Victoia, who always knew how to use the "
    "stone's power wisely.\6"
    "But now, that the queen is gone... we do not have anyone to protect us from Varda's evil. "
    "I believe Varda will try to use the stone's power to take over Dagur.\6"
    "But the thing demands a lot of knowledge, so I suppose you still have the time to stop her. "
    "Varda's castle is at the north-west dark forest.\6"
    "All we have is this scroll... \"Defet the three Elements - Fire, Water and Earth to obtain "
    "the key to to the forbidden castle...\"\6"
    "Now go! and save the kingdom!",

    // ---

    "\2Ding\2: Who dares enter Ding's cave?",

    // ---

    "\2Alex\2: I'm here for the piece of the key\6"

    "\2Ding\2: Hah! I would like to see that! Prepare yourself to die, you fool!",

    // ---

    "\2Ding\2: Noooooooooooooo!\nI must... not... lose!...\6"
    "You got a piece of the key",

    // ---

    "\2Dang\2: Who is it that dares to bother Dang in his sleep?...",

    // ---

    "\2Alex\2: I'm here for the piece of the key\6"
    "\2Dang\2: Hahaha! Do you believe you are my match? Say your prayers, you fool!",

    // ---

    "\2Dang\2: Arghhhhh! Impossible!... I can't die!...\6"
    "You got a piece of the key",

    // ---

    "\2Dong\2: Who is it that dares to bother Dang in his meal time?...",

    // ---

    "\2Alex\2: I'm here for the piece of the key\6"
    "\2Dong\2: Hahahaha! Do you really think you can beat me?! you fool!",

    // ---

    "\2Dong\2: Noooooooooooooo! I must not lose!...\6"
    "You got a piece of the key",
    "\2Varda\2: WHAT!!! How have you succeeded to collect all of the pieces of the key, "
    "you little verment?!\6"
    "...Never mind! Soon... I WILL RULE THE WORLD!\6"
    "Muahahahhaaaa!",

    // ---

    "\2Varda\2: ARRRGH! Noooooooooooooooooooooo!\6"
    "You have received the holy stone",
    "\2King\2: You have returned us the holy stone! I'm very proud of you, son.\6"
    "Unfortunately... My beloved victoria is still missing... Didn't Demitri mention anything "
    "about who took her?...",

    // ---

    "\2Demitri\2: I can't believe that lousy soldier has defeated my master, Varda!\6"
    "\2Alex\2: ???\6"
    "\2Demitri\2: !!!...\6"
    "Bwhahaha I was the one who helped Varda steal the stone, I WAS THE ONE who kidnapped queen "
    "Victoria! It was me all along! Me, Me, ME!!!",

    // ---

    "\2Demitri\2: AAAAAAAAAAARGHHHH!!!",

    // ---

    "\2Alex\2: My Queen!\6"
    "\2Queen\2: Thank you for saving me, brave warrior!\6"
    "\2Queen\2: I must place the stone back in it's place!",

    // ---

    "\2King\2: Come forward, Son.",

    // ---

    "\2King\2: I here by pronounce you as a knight of Dagur. You are now a hero of this kingdom! "
    "I look at you, and you make me proud!",
};

// Event script
Event gameEvents[] = {
#if !DEBUG
    buildEvent(EVENT_CUTSCENE, 0),
    buildEvent(EVENT_GOTOLEVEL, LEVEL3),
    buildEvent(EVENT_CUTSCENE, 1),
    buildEvent(EVENT_GOTOLEVEL, LEVEL15),
    buildEvent(EVENT_CUTSCENE, 2),
    buildEvent(EVENT_BATTLE, 10),
    buildEvent(EVENT_CUTSCENE, 3),
    buildEvent(EVENT_GOTOLEVEL, LEVEL14),
    buildEvent(EVENT_CUTSCENE, 4),
    buildEvent(EVENT_BATTLE, 11),
    buildEvent(EVENT_CUTSCENE, 5),
    buildEvent(EVENT_GOTOLEVEL, LEVEL17),
    buildEvent(EVENT_CUTSCENE, 6),
    buildEvent(EVENT_BATTLE, 12),
    buildEvent(EVENT_CUTSCENE, 7),
    buildEvent(EVENT_TALKTO, &doorFigure),
    buildEvent(EVENT_CUTSCENE, 8),
    buildEvent(EVENT_BATTLE, 13),
    buildEvent(EVENT_CUTSCENE, 9),
    buildEvent(EVENT_GOTOLEVEL, LEVEL3),
    buildEvent(EVENT_CUTSCENE, 10),
    buildEvent(EVENT_BATTLE, 14),
    buildEvent(EVENT_CUTSCENE, 11),
    buildEvent(EVENT_END, 0),
#endif
};
// clang-format on

cutSceneAction cutScene0[] = {
    buildCutScene(CS_FIGURETALK, 0, 0, 0),
    END_OF_CUTSCENE,
};

// clang-format off
cutSceneAction cutScene1[] = {
    buildCutScene(CS_MOVETO, &alexFigure, 32, 90),
    buildCutScene(CS_FIGURETALK, 0, 1, 0),
    buildCutScene(CS_SETDIR, &demitriFigure, DIR_DOWN, 0),
    buildCutScene(CS_FIGURETALK, 0, 2, 0),
    buildCutScene(CS_SETDIR, &demitriFigure, DIR_UP, 0),
    END_OF_CUTSCENE,
};
// clang-format on

cutSceneAction cutScene2[] = {
    buildCutScene(CS_FIGURETALK, 0, 3, 0),
    buildCutScene(CS_MOVETO, &alexFigure, 32, 152),
    buildCutScene(CS_FIGURETALK, 0, 4, 0),
    END_OF_CUTSCENE,
};

cutSceneAction cutScene3[] = {
    buildCutScene(CS_FIGURETALK, 0, 5, 0),
    END_OF_CUTSCENE,
};

cutSceneAction cutScene4[] = {
    buildCutScene(CS_FIGURETALK, 0, 6, 0),
    buildCutScene(CS_MOVETO, &alexFigure, 210, 115),
    buildCutScene(CS_FIGURETALK, 0, 7, 0),
    END_OF_CUTSCENE,
};

cutSceneAction cutScene5[] = {
    buildCutScene(CS_FIGURETALK, 0, 8, 0),
    END_OF_CUTSCENE,
};

cutSceneAction cutScene6[] = {
    buildCutScene(CS_FIGURETALK, 0, 9, 0),
    buildCutScene(CS_MOVETO, &alexFigure, 100, 127),
    buildCutScene(CS_FIGURETALK, 0, 10, 0),
    END_OF_CUTSCENE,
};

cutSceneAction cutScene7[] = {
    buildCutScene(CS_FIGURETALK, 0, 11, 0),
    END_OF_CUTSCENE,
};

cutSceneAction cutScene8[] = {
    CS_loadLevel(LEVEL18, 192, 380, DIR_UP),
    buildCutScene(CS_MOVETO, &alexFigure, 192, 152),
    buildCutScene(CS_SETDIR, &vardaFigure, DIR_DOWN, 0),
    buildCutScene(CS_FIGURETALK, 0, 12, 0),
    END_OF_CUTSCENE,
};

cutSceneAction cutScene9[] = {
    buildCutScene(CS_FIGURETALK, 0, 13, 0),
    CS_loadLevel(LEVEL1, 192, 132, DIR_UP),
    buildCutScene(CS_FIGURETALK, 0, 14, 0),
    END_OF_CUTSCENE,
};

cutSceneAction cutScene10[] = {
    buildCutScene(CS_MOVETO, &alexFigure, 32, 90),
    buildCutScene(CS_SETDIR, &demitriFigure, DIR_DOWN, 0),
    buildCutScene(CS_FIGURETALK, 0, 15, 0),
    END_OF_CUTSCENE,
};

cutSceneAction cutScene11[] = {
    buildCutScene(CS_FIGURETALK, 0, 16, 0),
    buildCutScene(CS_SETPOS, &queenFigure, 160, 96),
    CS_loadLevel(LEVEL1, 192, 152, DIR_UP),
    buildCutScene(CS_FIGURETALK, 0, 17, 0),
    buildCutScene(CS_MOVETO, &queenFigure, 32, 96),
    buildCutScene(CS_FIGURETALK, 0, 18, 0),
    buildCutScene(CS_MOVETO, &alexFigure, 192, 130),
    buildCutScene(CS_FIGURETALK, 0, 19, 0),
    //	CS_loadLevel(LEVEL1, 192, 130, DIR_UP),
    END_OF_CUTSCENE,
};

pCutSceneAction cutScenes[] = {
    cutScene0, cutScene1, cutScene2, cutScene3, cutScene4,  cutScene5,
    cutScene6, cutScene7, cutScene8, cutScene9, cutScene10, cutScene11,
};

// Functions
bool isOverAction(pCutSceneAction curEvent)
{
    switch (curEvent->eventType) {
        case CS_GIVEITEM:
            if (textVars.curMode == TEXTMODE_IDLE) {
                for (int i = 0; i < (int)curPlayer->figureItemList.size(); ++i) {
                    if (curPlayer->figureItemList[i].itemIdx == (int)curEvent->uParam1)
                        return true;
                }
            }
            break;
        case CS_LOADLEVEL:
            if (gameValues.curMode == LEVELMODE_PLAY)
                return true;
            break;
        case CS_MOVETO:
            if (((pFigure)curEvent->ptrParam)->posX == (int)curEvent->uParam1
                && ((pFigure)curEvent->ptrParam)->posY == (int)curEvent->uParam2)
                return true;
            break;
        case CS_FIGURETALK:
            if (textVars.curMode == TEXTMODE_IDLE)
                return true;
            break;
        case CS_SETDIR:
            return true;
            break;
        case CS_SETPOS:
            return true;
            break;
        case CS_END:
            textVars.curColor = TEXT_COLOR_NORMAL;
            return true;
            break;
        default:
            return true;
    }
    return false;
}

void performCutsceneAction(pCutSceneAction curEvent)
{
    string tempString;
    switch (curEvent->eventType) {
        case CS_GIVEITEM:
            receiveItem(curEvent->uParam1, curEvent->uParam2);
            break;
        case CS_LOADLEVEL:
            gameValues.loadedLevel = (LEVELS)(int)curEvent->ptrParam;
            gameValues.newPosX     = curEvent->uParam1;
            gameValues.newPosY     = curEvent->uParam2 >> 2;
            gameValues.newDir      = (Direction)(curEvent->uParam2 & 3);
            gameValues.curMode     = LEVELMODE_FADEOUT;
            break;
        case CS_MOVETO:
            moveFigure((pFigure)curEvent->ptrParam, curEvent->uParam1, curEvent->uParam2, false);
            ((pFigure)curEvent->ptrParam)->isMotion = true;
            break;
        case CS_FIGURETALK:
            tempString      = gameScript[curEvent->uParam1];
            textVars.txtLen = strlen(tempString.c_str());
            strcpy(textVars.txtBuffer, tempString.c_str());
            textVars.curMode = TEXTMODE_WINDOW_DIRUP;
            textVars.posX = textVars.txtPos = 0;
            textVars.virtualPosY = textVars.posY = 19;
            break;
        case CS_SETDIR:
            ((pFigure)curEvent->ptrParam)->curDir = (Direction)curEvent->uParam1;
            ((pFigure)curEvent->ptrParam)->lstDir = (Direction)curEvent->uParam1;
            break;
        case CS_SETPOS:
            ((pFigure)curEvent->ptrParam)->posX = curEvent->uParam1;
            ((pFigure)curEvent->ptrParam)->posY = curEvent->uParam1;
            break;
        default:
            break;
    }
}
