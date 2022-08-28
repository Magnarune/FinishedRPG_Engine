#pragma once
#include <iostream>
#include <string>
#include "olcSoundWaveEngine.h" 
#include "olcPixelGameEngine.h"
//#include "olcPGEX_Sound.h" // Death time
#include "Assets.h"
#include "Maps.h"
#include "Dynamics.h"
#include "Commands.h"
#include "Quests.h"
#include "Items.h"
#include "Spells.h"


#define X(n) m_script.AddCommand(new cCommand_ ## n)

class RPG_Engine : public olc::PixelGameEngine
{
public:
    RPG_Engine();

    olc::sound::WaveEngine engine;
    
private:
    cMap* m_pCurrentMap = nullptr;
    cDynamic_Creature* m_pPlayer = nullptr;
    std::vector<cDynamic*> m_vecDynamics; //Fixed
    std::vector<cDynamic*> m_vecProjectiles; //Trans

    cScriptProcessor m_script;
    std::list<cQuest*> m_listQuests;
    std::list<cItem*> m_listItems;
    std::list<cItem*> m_CharacterItems;
    std::list<cItem*> m_KeyItems;
    std::list<cSpell*> m_listSpells;
    

    float fCameraPosX = 0.0f;
    float fCameraPosY = 0.0f;
    olc::Sprite* m_sprFont = nullptr;
    //RAIN
    struct Rain
    {
        float PositionX;
        float PositionY;
        
       //white
    };
    std::vector<Rain> RainDrops;
    float m_fLightingTimer;
    bool m_braining;
    bool EnableRain;
    void UpdateRain(float FelapsedTime)
    {
        for (auto& Raindrops : RainDrops)
        {
            Raindrops.PositionX += 1.0;
            Raindrops.PositionY += 1.0;
            if (Raindrops.PositionX > ScreenWidth())
            {
                Raindrops.PositionX = 0.0f;
            }
            if (Raindrops.PositionY > ScreenHeight())
            {
                Raindrops.PositionY = 0.0f;
            }
            
            Draw(Raindrops.PositionX, Raindrops.PositionY);
        }
    }
    //End of Rain
public://I changed this from private is that a problem?
    enum
    {
        MODE_TITLE,
        MODE_LOCAL_MAP,
        MODE_WORLD_MAP,
        MODE_INVENTORY,
        MODE_SPELLS,
        MODE_OPTIONS ,
        MODE_MUSIC ,
        MODE_SHOP ,
        MODE_SHOP2 ,
        MODE_LEVEL,
        MODE_HERO
    };
public://HERE
    int m_nGameMode = MODE_LOCAL_MAP;

    int m_nInvSelectX = 0;
    int m_nInvSelectY = 0;

protected:
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
    bool OnUserDestroy() override;

    bool UpdateTitleScreen(float fElapsedTime);
    bool UpdateLocalMap(float fElapsedTime);
    bool UpdateWorldMap(float fElapsedTime);
    bool UpdateSPELL(float fElapsedTime);
    bool UpdateInventory(float fElapsedTime);
    bool UpdateShop(float fElapsedTime);
    bool UpdateHunterShop(float fElapsedTime);
    bool UpdateLevelUp(float fElapsedTime);
    bool UpdateOptions(float fElaspedTime);
    bool UpdateMusic(float fElapsedTime);
    bool UpdateHero(float fElapsedTime);
protected:
    std::vector<std::string> m_vecDialogToShow;
    bool m_bShowDialog = false;
    float m_fDialogX = 0.0f;
    float m_fDialogY = 0.0f;
public:
    void ShowDialog(std::vector<std::string> vecLines);
    void DisplayDialog(std::vector<std::string> vecText, int x, int y);
    void DrawBigText(std::string sText, int x, int y);
    void ChangeMap(std::string sMapName, float x, float y);
    void AddQuest(cQuest* quest);
    void ChangeMode(int m_nGameMode);
    void AddProjectile(cDynamic_Projectile* proj);
    bool GiveItem(cItem* item);
    bool TakeItem(cItem* item);
    bool HasItem(cItem* item);
    void Attack(cDynamic_Creature* aggressor, cWeapon* weapon);
    void Damage(cDynamic_Projectile* projectile, cDynamic_Creature* victim);
    //void OnTextEntryComplete(const std::string& sText) override;
};