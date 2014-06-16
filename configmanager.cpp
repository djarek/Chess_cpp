#include "configmanager.h"

void ConfigManager::loadConfig()
{
    configMap["defaultWindowWidth"] = (int32_t)640;
    configMap["defaultWindowHeight"] = (int32_t)640;
    configMap["defaultWindowXPos"] = (int32_t)0;
    configMap["defaultWindowYPos"] = (int32_t)0;
    configMap["defaultWindowName"] = "(Prawie) Mistrz Szachow";
    configMap["black_pawn_Path"] = "textures/black_pawn.png";
    configMap["white_pawn_Path"] = "textures/white_pawn.png";

    configMap["black_rook_Path"] = "textures/black_rook.png";
    configMap["white_rook_Path"] = "textures/white_rook.png";

    configMap["black_bishop_Path"] = "textures/black_bishop.png";
    configMap["white_bishop_Path"] = "textures/white_bishop.png";

    configMap["black_knight_Path"] = "textures/black_knight.png";
    configMap["white_knight_Path"] = "textures/white_knight.png";

    configMap["black_queen_Path"] = "textures/black_queen.png";
    configMap["white_queen_Path"] = "textures/white_queen.png";

    configMap["black_king_Path"] = "textures/black_king.png";
    configMap["white_king_Path"] = "textures/white_king.png";
}
