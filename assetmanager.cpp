#include "assetmanager.h"
#include "globals.h"
#include <SDL2/SDL_image.h>
Asset::Asset(SDL_Texture* tex)
    : m_texture(tex)
{

}

Asset::~Asset()
{
    SDL_DestroyTexture(m_texture);
}

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{

}

bool AssetManager::loadAsset(const std::string& assetName, Widget* requester)
{
    SDL_Texture* newTexture = nullptr;
    auto path = g_ConfigManager.getConfigValue<std::string>(assetName + "_Path");
    auto renderer = requester->getRenderer();
    if(!renderer)
        return false;
    newTexture = IMG_LoadTexture(renderer, path.c_str());
    if(!newTexture)
        return false;
    m_assets.emplace(assetName, newTexture);
    return true;
}

Asset& AssetManager::getAsset(const std::string& assetName, Widget* requester)
{
    auto it = m_assets.find(assetName);
    if(it == m_assets.end())
    {
        if(!loadAsset(assetName, requester))
            throw;
        else
            it = m_assets.find(assetName);
    }
    return it->second;
}
