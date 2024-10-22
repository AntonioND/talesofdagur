// SPDX-License-Identifier: WTFPL
//
// SPDX-FileContributor: LiraNuna, 2006

// ----- Change log -----
// 05/08/05 - Created.
// ----------------------

#ifndef MODFILE_H
#define MODFILE_H

#include <nds.h>

#ifdef __cplusplus
extern "C" {
#endif

// ----- Functions -----

struct _MOD; // Forward declaration

u32 ModFileLoad(const void *modFile, struct _MOD *dest, void *memPool, u32 memPoolSize);

// ----------

#endif // MODFILE_H
