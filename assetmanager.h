#ifndef ASSETMANAGER_H_INCLUDED
#define ASSETMANAGER_H_INCLUDED

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include "framework/widget.h"
class Asset
{
public:
    Asset(SDL_Texture* tex);
    ~Asset();
    SDL_Texture* getTexture(){return m_texture;}
private:
    SDL_Texture* m_texture;
};

class AssetManager
{
public:
    AssetManager();
    ~AssetManager();

    bool loadAsset(const std::string& assetName, Widget* requester);
    Asset& getAsset(const std::string& assetName, Widget* requester);
private:
    std::map<std::string, Asset> m_assets;
};

#endif // ASSETMANAGER_H_INCLUDED
