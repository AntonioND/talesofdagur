#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <nds/arm9/console.h>
#include <nds/arm9/sound.h>
using namespace std;
#include <list>
#include <vector>

#define DEBUG 0
#define DISP_Y REG_VCOUNT

#ifndef __MAIN_H__
#define __MAIN_H__

	// Other code parts
#include "sprites.h"
#include "background.h"
#include "text.h"
#include "handlers.h"
#include "database.h"
#include "events.h"
#include "Sound9.h"

	// Variables
extern volatile u32 frameCounter;
extern volatile u16 KEYS;
extern volatile u16 HKEYS;

	// Functions
extern int main(void);

#endif
