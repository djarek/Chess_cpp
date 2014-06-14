#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED
#include "configmanager.h"
#include "application.h"
#include "game.h"
#include "assetmanager.h"

extern Application g_Application;
extern ConfigManager g_ConfigManager;
extern Game g_Game;
extern MoveGenerator g_MoveGenerator;
extern AssetManager g_AssetManager;
extern AIEngine g_AIEngine;
#endif // GLOBALS_H_INCLUDED
