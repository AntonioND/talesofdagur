// SPDX-License-Identifier: WTFPL
//
// SPDX-FileContributor: LiraNuna, 2006

// Level links

#include "background.h"

#define END_OF_LINK                 \
    {                               \
        0, 0, 0, NO_LEVEL, DIR_NONE \
    }

levelLink level1Link[] = {
    { 49, 58, 28, LEVEL2,   DIR_UP },
    { 50, 57, 32, LEVEL5, DIR_DOWN },
    { 15, 32, 98, LEVEL7, DIR_DOWN },
    END_OF_LINK,
};

levelLink level2Link[] = {
    {  3,  8, 24, LEVEL4,   DIR_UP },
    { 51, 56,  4, LEVEL3,   DIR_UP },
    { 26, 33, 52, LEVEL1, DIR_DOWN },
    END_OF_LINK,
};

levelLink level3Link[] = {
    { 3, 8, 52, LEVEL2, DIR_DOWN },
    END_OF_LINK,
};

levelLink level4Link[] = {
    { 23, 28, 4, LEVEL2, DIR_DOWN },
    END_OF_LINK,
};

levelLink level5Link[] = {
    { 29, 38, 52, LEVEL1, DIR_UP },
    END_OF_LINK,
};

levelLink level6Link[] = {
    { 26,  41,  33,  LEVEL7, DIR_RIGHT },
    { 83,  88,  81,  LEVEL7, DIR_RIGHT },
    { 91, 113, 100, LEVEL11,    DIR_UP },
    {  7,  26,  11, LEVEL12,  DIR_LEFT },
    END_OF_LINK,
};

levelLink level7Link[] = {
    { 26,  41, 33, LEVEL6, DIR_LEFT },
    { 83,  88, 81, LEVEL6, DIR_LEFT },
    { 97, 102, 22, LEVEL1,   DIR_UP },
    { 55,  68, 62, LEVEL8,   DIR_UP },
    END_OF_LINK,
};

levelLink level8Link[] = {
    {  55,  68,  60,  LEVEL7, DIR_DOWN },
    {  25,  39,  29, LEVEL11, DIR_LEFT },
    { 109, 122, 113, LEVEL11, DIR_LEFT },
    END_OF_LINK,
};

levelLink level9Link[] = {
    { 23, 28, 24, LEVEL10, DIR_UP },
    END_OF_LINK,
};

levelLink level10Link[] = {
    { 23, 28, 24, LEVEL9, DIR_DOWN },
    END_OF_LINK,
};

levelLink level11Link[] = {
    {  25,  38,  29,  LEVEL8, DIR_RIGHT },
    { 109, 124, 113,  LEVEL8, DIR_RIGHT },
    {  95, 109, 100,  LEVEL6,  DIR_DOWN },
    {  33,  46,  37, LEVEL16,  DIR_LEFT },
};

levelLink level12Link[] = {
    {  8,  23, 11,  LEVEL6, DIR_RIGHT },
    { 40, 123, 74, LEVEL16,    DIR_UP },
    END_OF_LINK,
};

levelLink level13Link[] = {
    END_OF_LINK,
};

levelLink level14Link[] = {
    END_OF_LINK,
};

levelLink level15Link[] = {
    END_OF_LINK,
};

levelLink level16Link[] = {
    { 33,  46, 36, LEVEL11, DIR_RIGHT },
    { 40, 115, 74, LEVEL12,  DIR_DOWN },
    END_OF_LINK,
};

levelLink level17Link[] = {
    END_OF_LINK,
};

levelLink level18Link[] = {
    END_OF_LINK,
};
