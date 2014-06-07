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
extern BitBoard BOTTOM_RIGHT_DIAGONAL;
extern BitBoard BOTTOM_LEFT_DIAGONAL;
uint64_t rotateRight (uint64_t x, int s) {return (x << s) | (x >> (64-s));}
uint64_t rotateLeft(uint64_t x, int s) {return (x >> s) | (x << (64-s));}

int main(int argc, char** argv)
{
    test();
    g_ConfigManager.loadConfig();
    g_Application.init(argc, argv);
    g_Application.run();
    printf("Exited cleanly\n");
    return 0;
}
