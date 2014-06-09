#include "application.h"
#include "configmanager.h"
#include "game.h"
#include "assetmanager.h"
#include "moves.h"
#include "bitboard.h"
#include "moves.h"

Application g_Application;
ConfigManager g_ConfigManager;
Game g_Game;
MoveGenerator g_MoveGenerator;
AssetManager g_AssetManager;

int main(int argc, char** argv)
{
    g_MoveGenerator.init();
    g_ConfigManager.loadConfig();
    g_Application.init(argc, argv);
    g_Application.run();
    printf("Exited cleanly\n");
    return 0;
}
