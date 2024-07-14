#include <list>
#include <vector>

#include "background.h"
#include "sprites.h"

// NPC list
#define END_OF_NPC \
    {              \
        NULL       \
    }

#define END_OF_CHEST   \
    {                  \
        0, 0, DIR_NONE \
    }

// Var, posX, posY, moveX, moveY, Level, Direction,
// Data, Palette, text

// clang-format off
const NPC gameNPCs[] = {
    {
        &kingFigure, 192, 104, 0, 0, LEVEL1, DIR_DOWN,
        (u8 *)kingFigureData_bin, (u16 *)kingFigurePal_bin, "King",
        "Please, \3Alex\3! The kingdom of Dagur is depending on you!\6"
        "Find my love and bring her back to the palace!"
    },

    {
        &civilianFigure[0], 388, 566, 256, 0, LEVEL7, DIR_RIGHT,
        (u8 *)civilian1FigureData_bin, (u16 *)civilian1FigurePal_bin, "Boy",
        "My mother said not to talk to strangers. I think she is worried because "
        "\3queen Victoria\3 has been kidnapped."
    },

    {
        &civilianFigure[1], 625, 765, 192, 0, LEVEL6, DIR_LEFT,
        (u8 *)civilian2FigureData_bin, (u16 *)civilian2FigurePal_bin, "Girl",
        "My dad says that \3queen Victoria\3 has been kidnapped. I don't want to "
        "be kidnapped when I become a queen!"
    },

    {
        &civilianFigure[2], 194, 492, 0, 0, LEVEL6, DIR_DOWN,
        (u8 *)civilian3FigureData_bin, (u16 *)civilian3FigurePal_bin, "Woman",
        "Business is not going so well since \3queen Victoria\3 had been kidnapped... "
        "I wonder how long the situation is going to stay as it is..."
    },

    {
        &civilianFigure[3], 498, 8, 0, 212, LEVEL7, DIR_DOWN,
        (u8 *)civilian4FigureData_bin, (u16 *)civilian4FigurePal_bin, "Woman",
        "Nice day today, isn't it? Too bad \3queen Victoria\3 has disappeared on such a "
        "fine season..."
    },

    {
        &civilianFigure[4], 748, 824, 0, 0, LEVEL7, DIR_UP,
        (u8 *)civilian5FigureData_bin, (u16 *)civilian5FigurePal_bin, "Man",
        "Oh lord...\nI wonder what will happen to our kingdom...\6What are we supposed to "
        "do without the holy stone?!..."
    },

    {
        &civilianFigure[5], 958, 178, 0, 0, LEVEL6, DIR_DOWN,
        (u8 *)civilian6FigureData_bin, (u16 *)civilian6FigurePal_bin, "Fisherman",
        "Fishing loot is low today. Do you think it has any connection to the fact that the "
        "Holy Stone was stolen?\6"
        "The kingdom won't survive without it for too long..."
    },

    {
        &civilianFigure[6], 1030, 600, -200, 0, LEVEL7,  DIR_LEFT,
        (u8 *)civilian7Figure_bin, (u16 *)civilian7FigurePal_bin, "Boy",
        "Hi! You can buy some good stuff in the \3shop\3!"
    },

    {
        &civilianFigure[7], 210, 20, 0, 110, LEVEL7, DIR_DOWN,
        (u8 *)civilian8Figure_bin, (u16 *)civilian8FigurePal_bin, "Young Lady",
        "A lot of the kingdom's troops went off looking for the queen. My husband was one of "
        "them but he has never returned...\6"
        "I'm so worried I'm walking here for hours each day!"
    },

    {
        &civilianFigure[8], 770, 720, 0, 0, LEVEL6, DIR_DOWN,
        (u8 *)civilian9Figure_bin, (u16 *)civilian9FigurePal_bin, "Man",
        "Look at my li'l princess playin'! I keep tellin' her, when she'll grow up, she'll be "
        "the queen of Dagur!"
    },

    {
        &civilianFigure[9], 155, 590, 0, 0, LEVEL6, DIR_LEFT,
        (u8 *)civilian10Figure_bin, (u16 *)civilian10FigurePal_bin, "Young Lady",
        "Hello there! We are in the fishing buisness. My husband is the \3Fisherman\3. He "
        "brings some good fish! Then i clean them up, and my mother sells them!\6"
        "Still I'm worried... Me and my husband have no children and there will be no-one to "
        "go on with the legacy of fishing..."
    },

    {
        &civilianFigure[10], 400, 650, 0, 0, LEVEL12, DIR_DOWN,
        (u8 *)civilian11Figure_bin, (u16 *)civilian11FigurePal_bin, "Woman",
        "I like to rest at the beach of Mar Seogath region when I'm tired...\6"
        "But something is very odd with this area's climate. The sun is shining, yet I feel a "
        "strong sensation of... cold!"
    },

    {
        &civilianFigure[11], 755, 1160, 0, 0, LEVEL11, DIR_RIGHT,
        (u8 *)civilian12Figure_bin, (u16 *)civilian12FigurePal_bin, "Lumberjack Donni's Son",
        "I'm helping my father cut trees!\6"
        "When I grow to be a strong man as he is, I will fight the monsters in the forest and "
        "kill them!"
    },

    {
        &lumberjackFigure[0], 710, 1180, 0, 0, LEVEL11, DIR_DOWN,
        (u8 *)lumberjackFigure_bin, (u16 *)lumberjackFigurePal_bin, "Lumberjack Donni",
        "Hey there, mate! There are a lot of dangerous creatures in the woods of Mar Serdan. "
        "Be careful!\6"
        "By the way, have I mentioned my brother \3Remi\3 works at the other side of the woods? "
        "you should meet him, he is an adorable person!"
    },

    {
        &lumberjackFigure[1], 411, 817, 0, 0, LEVEL8, DIR_LEFT,
        (u8 *)lumberjack1Figure_bin, (u16 *)lumberjack1FigurePal_bin, "Lumberjack Remi",
        "Hello! Me and my brother \3Donni\3 sell some tree materials to the shop in Dagur.\6"
        "My brother works at the other side of the woods, you should go talk to him, he is a "
        "very nice person.\6"
        "Usually he takes his son to help him. He's son is very diligent. I wish I had a son "
        "that would help me, too!"
    },

    {
        &bartenderFigure, 64, 72, 0, 0, LEVEL9, DIR_DOWN,
        (u8 *)bartenderFigure_bin, (u16 *)bartenderFigurePal_bin, "Teller",
        "Welcome to our modest inn!\6"
        "Would you like you rest? The fee is 75 Gold per night!\13"
    },

    {
        &traderFigure, 208, 72, 0, 0, LEVEL13, DIR_DOWN,
        (u8 *)traderFigure_bin, (u16 *)traderFigurePal_bin, "Salesman",
        "Aye there, mate! Care to see me goods?\14"
    },

    {
        &maidFigure, 226, 32, 0, 0, LEVEL9, DIR_DOWN,
        (u8 *)maidFigure_bin, (u16 *)maidFigurePal_bin, "Maid",
        "The room is ready for visitors, Sir."
    },

    {
        &guardFigure[0], 836, 64, 0, 0, LEVEL7, DIR_DOWN,
        (u8 *)guardFigure_bin, (u16 *)guardFigurePal_bin, "Guard",
        "\3The king\3 is still not over the  disappearance of \3queen Victoria\3. Our troops "
        "have been trying to find her for over 7 days now... but none of us has succeeded..."
    },

    {
        &guardFigure[1], 289, 102, 0, 0, LEVEL5, DIR_LEFT,
        (u8 *)guardFigure_bin, (u16 *)guardFigurePal_bin, "Guard",
        "ZzZzzzZZzzzZz...\6Huh? Was I sleeping? Please don't tell \3The king\3! I don't want "
        "to depress him even more then he is currently..."
    },

    {
        &guardFigure[2], 28, 113, 0, 0, LEVEL2, DIR_RIGHT,
        (u8 *)guardFigure_bin, (u16 *)guardFigurePal_bin, "Guard",
        "It's a great disaster that the holy stone has been stolen... It is the only power "
        "that protects us from evil. Without it, our city is bound to oblivion..."
    },

    {
        &guardFigure[3], 379, 26, 0, 0, LEVEL1, DIR_DOWN,
        (u8 *)guardFigure_bin, (u16 *)guardFigurePal_bin, "Guard",
        "All of us are worried sick about \3queen Victoria\3\6"
        "I wonder if we are ever going to see her again..."
    },

    {
        &demitriFigure, 114, 32 + 8, 0, 0, LEVEL3, DIR_UP,
        (u8 *)demitriFigure_bin, (u16 *)demitriFigurePal_bin, "Dimitri",
        "Mmm... I don't understand why everyone is interested in the queen...\6"
        "Don't they have anything else to do exept looking for her?! It's obvious that she won't "
        "ever come back!\6"
        "In my opinion it's just a waste of the troops' strength!"
    },

    {
        &dingFigure, 95, 80, 0, 0, LEVEL17, DIR_RIGHT,
        (u8 *)dingFigure_bin, (u16 *)dingFigurePal_bin, "Ding",
        "Get out of here!"
    },

    {
        &dangFigure, 160, 64, 0, 0, LEVEL14, DIR_DOWN,
        (u8 *)dangFigureData_bin, (u16 *)dangFigurePal_bin, "Dang",
        "Get out of here!"
    },

    {
        &dongFigure, 124, 75, 0, 0, LEVEL15, DIR_DOWN,
        (u8 *)dongFigure_bin, (u16 *)dongFigurePal_bin, "Dong",
        "Get out of here!"
    },

    {
        &vardaFigure, 192, 109, 0, 0, LEVEL18, DIR_UP,
        (u8 *)vardaFigure_bin, (u16 *)vardaFigurePal_bin, "Varda",
        "Nooo...! My plans...!"
    },

    {
        &doorFigure, 900, 64 - 16, 0, 0, LEVEL16, DIR_DOWN,
        NULL, NULL, "Gate",
        "Combine the three Elements - Fire Water and Earth..."
    },

    {
        &queenFigure, 1024, 1024, 0, 0, LEVEL1, DIR_DOWN,
        (u8 *)queenFigure_bin, (u16 *)queenFigurePal_bin, "Queen",
        ""
    },

    END_OF_NPC,
};
// clang-format on

// posX, posY, direction, level, type of box, item, quantity
const chestLoadingData loadedChests[] = {
    // Palace
    {    3, 128, DIR_RIGHT,  LEVEL1, 0,  0, 3 },
    {    3, 165, DIR_RIGHT,  LEVEL1, 0,  7, 1 },
    {  307,  32,  DIR_DOWN,  LEVEL2, 0,  3, 1 },
    {   27,  32,  DIR_DOWN,  LEVEL4, 3, 27, 1 },
    {  226,  68,  DIR_LEFT,  LEVEL3, 3,  1, 1 },
    {  170,  32,  DIR_DOWN,  LEVEL3, 3, 25, 1 },
    {   65,  32,  DIR_DOWN,  LEVEL5, 2, 23, 1 },
    // Inn
    {  200,  32,  DIR_DOWN, LEVEL10, 1,  0, 3 },
    // Shop
    {    3,  60, DIR_RIGHT, LEVEL13, 2, 33, 1 },
    {    3,  95, DIR_RIGHT, LEVEL13, 2,  2, 1 },
    // Caves
    {  203,  32,  DIR_DOWN, LEVEL14, 0,  2, 5 },
    {  190,  32,  DIR_DOWN, LEVEL17, 1, 24, 1 },
    {  225,  32,  DIR_DOWN, LEVEL15, 2, 38, 1 },
    // Woods
    {  200, 576,  DIR_DOWN,  LEVEL8, 3,  1, 1 },
    {  230, 576,  DIR_DOWN,  LEVEL8, 3,  4, 1 },
    {  918, 193,    DIR_UP,  LEVEL8, 3,  1, 5 },
    {  480, 768,  DIR_DOWN, LEVEL11, 3, 21, 1 },
    { 1000, 288,  DIR_DOWN, LEVEL16, 2, 34, 1 },
    {  420, 160,  DIR_DOWN, LEVEL12, 3, 15, 1 },
    {  390, 160,  DIR_DOWN, LEVEL12, 3, 11, 1 },

    END_OF_CHEST,
};
