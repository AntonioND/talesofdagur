// SPDX-License-Identifier: WTFPL
//
// SPDX-FileContributor: LiraNuna, 2006

#include <nds.h>

#include "database.h"
#include "main.h"
#include "sprites.h"

const Item itemDB[] = {
    {      "Potion",  { 100, false, 0, false, 0, 0, 0, 0, 0, 0 },        POTION,  150 },
    {   "Hi-Potion",  { 500, false, 0, false, 0, 0, 0, 0, 0, 0 },        POTION,  500 },
    {   "Full Pot.",     { 0, true, 0, false, 0, 0, 0, 0, 0, 0 },        POTION,  850 },
    {       "Ether",   { 0, false, 20, false, 0, 0, 0, 0, 0, 0 },        POTION,  200 },
    {   "Hi. Ether",  { 0, false, 100, false, 0, 0, 0, 0, 0, 0 },        POTION,  500 },
    {  "Full Ether",     { 0, false, 0, true, 0, 0, 0, 0, 0, 0 },        POTION, 1000 },
    {      "Elixer",      { 0, true, 0, true, 0, 0, 0, 0, 0, 0 },        POTION, 1500 },
    {       "Sword",    { 0, false, 0, false, 2, 2, 0, 0, 0, 1 },  WEAPON_SWORD,  100 },
    {   "En. Sword",    { 0, false, 0, false, 3, 2, 1, 0, 0, 1 },  WEAPON_SWORD,  500 },
    {       "Spear",    { 0, false, 0, false, 3, 1, 4, 0, 0, 1 },  WEAPON_SPEAR,  250 },
    {   "En. Spear",    { 0, false, 0, false, 4, 1, 5, 0, 0, 2 },  WEAPON_SPEAR,  700 },
    {         "Axe",  { 0, false, 0, false, 5, 5, -2, 0, 0, -1 },    WEAPON_AXE,  350 },
    {     "En. Axe",  { 0, false, 0, false, 7, 6, -1, 0, 0, -1 },    WEAPON_AXE, 1050 },
    {      "Dagger",   { 0, false, 0, false, 3, -2, 5, 0, 0, 2 }, WEAPON_DAGGER,  450 },
    {  "En. Dagger",    { 0, false, 0, false, 5, 1, 6, 1, 0, 2 }, WEAPON_DAGGER,  950 },
    {         "Bow",   { 0, false, 0, false, 5, -3, 2, 0, 0, 3 },    WEAPON_BOW,  800 },
    {     "En. Bow",   { 0, false, 0, false, 4, -1, 3, 5, 0, 5 },    WEAPON_BOW, 1200 },
    {         "Gun",  { 0, false, 0, false, 6, -1, -1, 0, 0, 1 },    WEAPON_GUN, 1050 },
    {     "En. Gun",  { 0, false, 0, false, 10, 0, 1, -2, 0, 2 },    WEAPON_GUN, 1800 },
    {        "Club",  { 0, false, 0, false, 4, 1, -1, 0, 0, -1 },   WEAPON_CLUB, 1500 },
    {    "En. Club",   { 0, false, 0, false, 5, 2, 0, 0, 0, -1 },   WEAPON_CLUB, 3050 },
    {       "Staff", { 0, false, 0, false, -4, 2, -1, 0, 10, 3 },  WEAPON_STAFF,  150 },
    {   "En. Staff",  { 0, false, 0, false, -4, 3, 0, 4, 11, 4 },  WEAPON_STAFF,  400 },
    {      "Shield", { 0, false, 0, false, 0, 10, -2, 0, 0, -1 },        SHIELD,  200 },
    {  "En. Shield",   { 0, false, 0, false, 0, 10, 0, 2, 0, 1 },        SHIELD,  450 },
    {        "Helm",   { 0, false, 0, false, 1, 5, -1, 2, 0, 1 },     HEAD_GEAR,  300 },
    {    "En. Helm",    { 0, false, 0, false, 2, 6, 1, 2, 0, 1 },     HEAD_GEAR,  750 },
    {         "Hat",    { 0, false, 0, false, 0, 2, 1, 0, 3, 0 },     HEAD_GEAR,  150 },
    {   "Circlette",   { 0, false, 0, false, -3, 3, 0, 0, 5, 3 },     HEAD_GEAR,  250 },
    {  "Full Armor",  { 0, false, 0, false, 2, 10, 0, 0, 0, -1 },         ARMOR,  800 },
    {  "Mega Armor",   { 0, false, 0, false, 3, 12, 0, 4, 0, 1 },         ARMOR, 1850 },
    {     "Cuirass",   { 0, false, 0, false, 1, 5, -1, 0, 0, 1 },         ARMOR,  750 },
    { "En. Cuirass",    { 0, false, 0, false, 2, 6, 0, 0, 0, 2 },         ARMOR, 1200 },
    {        "Robe",   { 0, false, 0, false, -4, 2, 0, 0, 5, 2 },         ARMOR,  600 },
    {  "Magic Robe",   { 0, false, 0, false, -2, 3, 0, 0, 6, 2 },         ARMOR,  850 },
    {        "Ring",    { 0, false, 0, false, 2, 0, 2, 2, 2, 2 },     ACCESSORY,  150 },
    {     "Pendant",    { 0, false, 0, false, 0, 2, 2, 2, 2, 2 },     ACCESSORY,  550 },
    {   "Wristband",    { 0, false, 0, false, 2, 0, 2, 2, 2, 2 },     ACCESSORY,  350 },
    {        "Cape",    { 0, false, 0, false, 1, 1, 2, 0, 0, 0 },         ARMOR,  350 },
    {   "Green Orb",    { 0, false, 0, false, 2, 0, 0, 0, 0, 0 },           ORB,  200 },
    {  "Purple Orb",    { 0, false, 0, false, 0, 2, 0, 0, 0, 0 },           ORB,  200 },
    {     "Red Orb",    { 0, false, 0, false, 0, 0, 2, 0, 0, 0 },           ORB,  200 },
    {    "Teal Orb",    { 0, false, 0, false, 0, 0, 0, 2, 0, 0 },           ORB,  200 },
    {  "Orange Orb",    { 0, false, 0, false, 0, 0, 0, 0, 2, 0 },           ORB,  200 },
    {  "Yellow Orb",    { 0, false, 0, false, 0, 0, 0, 0, 0, 2 },           ORB,  200 },
};

// clang-format off
const Monster monsterDB[] = {
    // Normal monsters
    { "Chicken", { 40, 40, 5, 5, 2, 2, 1, 2, 2, 1, 15, 0, 20 },
      (u8 *)chickenBattlerTiles, (u16 *)chickenBattlerPal },
    { "Deviling", { 76, 76, 7, 7, 3, 3, 2, 3, 2, 2, 30, 0, 25 },
     (u8 *)devilBattlerTiles, (u16 *)devilBattlerPal },
    { "Bat", { 82, 82, 8, 6, 4, 5, 5, 3, 3, 3, 55, 0, 40 },
     (u8 *)batBattlerTiles, (u16 *)batBattlerPal },
    { "Bee", { 96, 96, 6, 4, 5, 4, 3, 3, 2, 3, 65, 0, 40 },
     (u8 *)beeBattlerTiles, (u16 *)beeBattlerPal },
    { "Ghost", { 210, 210, 12, 12, 8, 6, 7, 5, 6, 5, 105, 0, 50 },
     (u8 *)ghostBattlerTiles, (u16 *)ghostBattlerPal },
    { "Zombie", { 310, 310, 15, 15, 7, 5, 6, 5, 6, 6, 188, 0, 70 },
     (u8 *)zombieBattlerTiles, (u16 *)zombieBattlerPal },
    { "Scorpion", { 390, 390, 15, 15, 5, 7, 8, 7, 3, 6, 134, 0, 80 },
     (u8 *)scropionBattlerTiles, (u16 *)scropionBattlerPal },
    { "Aquados", { 480, 480, 20, 13, 10, 1, 8, 8, 6, 9, 510, 0, 100 },
     (u8 *)squidBattlerTiles, (u16 *)squidBattlerPal },
    { "Cursed Tree", { 640, 540, 15, 16, 12, 11, 8, 6, 7, 7, 842, 0, 120 },
     (u8 *)treeBattlerTiles, (u16 *)treeBattlerPal },
    { "Evil Slime", { 620, 620, 18, 15, 14, 12, 7, 6, 7, 9, 1147, 0, 150 },
     (u8 *)aquticBattlerTiles, (u16 *)aquticBattlerPal },

    // Bosses
    { "Dong", { 1500, 1500, 150, 150, 13, 14, 7, 9, 7, 5, 3000, 0, 300 },
     (u8 *)fireBattlerTiles, (u16 *)fireBattlerPal },
    { "Dang", { 1500, 1500, 150, 150, 12, 13, 8, 8, 6, 8, 3000, 0, 300 },
     (u8 *)waterBattlerTiles, (u16 *)waterBattlerPal },
    { "Ding", { 1500, 1500, 150, 150, 13, 15, 6, 7, 8, 6, 3000, 0, 300 },
     (u8 *)earthBattlerTiles, (u16 *)earthBattlerPal },
    { "Varda", { 2000, 2000, 200, 200, 12, 7, 8, 6, 2, 4, 5000, 0, 500 },
     (u8 *)vardaBattlerTiles, (u16 *)vardaBattlerPal },
    { "Demitri", { 3000, 3000, 300, 300, 14, 12, 10, 9, 8, 7, 10000, 0, 1000 },
     (u8 *)demitriBattlerTiles, (u16 *)demitriBattlerPal },
};

const Attributes playerStartAttr[ATTR_SIZE] = {
#if DEBUG
    { 999, 999, 999, 999, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, },
#else
    { 122, 122, 4, 4, 2, 2, 2, 2, 2, 2, 0, 88, 100, 1, },
#endif
};
// clang-format on
