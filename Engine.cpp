#include "Engine.h"

#define X(n) m_script.AddCommand(new cCommand_ ## n)

RPG_Engine::RPG_Engine()
{
    sAppName = "Witty Fantasy";
}

bool Leveled = false;
int nLayerBackround = 0;
int nLayerHud = 1;


float mTime = 0;
float pTime = 0;
float lvltime = 0;
float Dynamicdebug = 0;
float DeadTime;
int Second = 0;
bool SwordBought = false;
bool SPELLGIVEN = false;
olc::Pixel color = {123,122,111};

//WORLDMAP DATA
float fWorldVX = 0.0f;
float fWorldVY = 0.0f;
float fWorldA = 0.1f;
float fNear = 0.01f;
float fFar = 0.5f; //Fake Z;        
float PI = 3.14159f;
float fFoVHalf = 3.14159f / 4.0f;
float m_fTime = 0.0f;
bool INTRO = true;
bool S1 = true;
int Graphictimer = 0;
float lightingtimer;
//WORLDMAP DATA
bool RPG_Engine::OnUserCreate()
{
   // olc::SOUND::InitialiseAudio(44100, 1, 8, 512);// audio
   // olc::SOUND::SetUserFilterFunction(SoundFilter::ChannelFilter);//this?
    engine.InitialiseAudio();
    cCommand::g_engine = this;    
    cMap::g_script = &m_script;
    cQuest::g_script = &m_script; 
    cQuest::g_engine = this;
    cDynamic::g_engine = this;
    cItem::g_engine = this;
    cSpell::g_engine = this;
    cMap::g_engine = this;

    nLayerBackround = CreateLayer();
    // BACKROUND IS BLACK
    SetDrawTarget(nLayerBackround);
    Clear(olc::BLACK);
    SetPixelMode(olc::Pixel::NORMAL);
    EnableLayer(nLayerBackround, false);
    SetDrawTarget(nullptr);
    // END OF BACKROUND
    //Create Rain
    for (int i = 0; i < 100; i++)
    {
        RainDrops.push_back( {(float)(rand() % ScreenWidth()), (float)(rand() % ScreenHeight())} );
       
    }
    m_fLightingTimer = 5.0f;
    m_braining = false;
    //End Rain
    RPG_Assets::get().LoadSprites();
    RPG_Assets::get().LoadMaps();
    RPG_Assets::get().LoadItems();
    RPG_Assets::get().LoadSpells();
    RPG_Assets::get().LoadSounds();
    
    SetPixelMode(olc::Pixel::ALPHA);
    m_sprFont = RPG_Assets::get().GetSprite("font");
    m_listQuests.push_front(new cQuest_MainQuest());
    m_pPlayer = new cDynamic_Creature_Magnus();
    m_pPlayer->Dead = false;
    m_nGameMode = MODE_WORLD_MAP;
    m_pPlayer->Worldpx = 0.15f;
    m_pPlayer->Worldpy = 0.5f;
    return true;
}

bool RPG_Engine::OnUserUpdate(float fElapsedTime)
{    
    if (GetKey(olc::Key::F1).bReleased)
    {
        ConsoleShow(olc::Key::ESCAPE, true);
    }
    switch (m_nGameMode)
    {
    case MODE_TITLE:
        return UpdateTitleScreen(fElapsedTime);
    case MODE_LOCAL_MAP:
        return UpdateLocalMap(fElapsedTime);
    case MODE_WORLD_MAP:
        return UpdateWorldMap(fElapsedTime);
    case MODE_INVENTORY:
        return UpdateInventory(fElapsedTime);
    case MODE_SHOP: 
        return UpdateShop(fElapsedTime);
    case MODE_SHOP2:
        return UpdateHunterShop(fElapsedTime);
    case MODE_OPTIONS:
        return UpdateOptions(fElapsedTime);
    case MODE_MUSIC:
        return UpdateMusic(fElapsedTime);
    case MODE_SPELLS:
        return UpdateSPELL(fElapsedTime);
    case MODE_LEVEL:
        return UpdateLevelUp(fElapsedTime);
    case MODE_HERO:
        return UpdateHero(fElapsedTime);
    }
    return true;
}
bool RPG_Engine::UpdateLocalMap(float fElapsedTime)
{
    m_script.ProcessCommands(fElapsedTime);
    m_vecProjectiles.erase(
        remove_if(m_vecProjectiles.begin(), m_vecProjectiles.end(),
            [](const cDynamic* d) {return ((cDynamic_Projectile*)d)->bRedundant; }), m_vecProjectiles.end());
    //Dead
    if (m_pPlayer->nHealth <= 0)
    {
        m_script.bUserControlEnabled = false;
        DisplayDialog({ "YOU ARE DEAD", "Press Space to Restart" }, ScreenWidth() / 4.0, ScreenHeight() / 2.0);

        if (GetKey(olc::Key::SPACE).bReleased)
        {
            m_pPlayer->nHealth = 9.0;
            m_pPlayer->px = 5.0;
            m_pPlayer->py = 5.0;
            m_bShowDialog = false;

            return true;
        }
        return true;
    }
    if (m_script.bUserControlEnabled)
    {
        m_pPlayer->vx = 0.0f;
        m_pPlayer->vy = 0.0f;
        if (IsFocused())
        {
            if (GetKey(olc::Key::UP).bHeld || GetKey(olc::Key::W).bHeld)
            {
                m_pPlayer->vy = -4.0f;
                if (GetKey(olc::Key::UP).bHeld || GetKey(olc::Key::W).bHeld && GetKey(olc::Key::SHIFT).bHeld)
                {
                    m_pPlayer->vy = -4.0f * 1.5f;
                }
            }


            if (GetKey(olc::Key::DOWN).bHeld || GetKey(olc::Key::S).bHeld)
            {
                m_pPlayer->vy = 4.0f;
                if (GetKey(olc::Key::DOWN).bHeld || GetKey(olc::Key::S).bHeld && GetKey(olc::Key::SHIFT).bHeld)
                {
                    m_pPlayer->vy = 4.0f * 1.5f;
                }
            }
            if (GetKey(olc::Key::LEFT).bHeld || GetKey(olc::Key::A).bHeld)
            {
                m_pPlayer->vx = -4.0f;
                if (GetKey(olc::Key::LEFT).bHeld || GetKey(olc::Key::A).bHeld && GetKey(olc::Key::SHIFT).bHeld)
                {
                    m_pPlayer->vx = -4.0f * 1.5f;
                }
            }
            if (GetKey(olc::Key::RIGHT).bHeld || GetKey(olc::Key::D).bHeld)
            {
                m_pPlayer->vx = 4.0f;
                if (GetKey(olc::Key::RIGHT).bHeld || GetKey(olc::Key::D).bHeld && GetKey(olc::Key::SHIFT).bHeld)
                {
                    m_pPlayer->vx = 4.0f * 1.5f;
                }
            }
            if (GetKey(olc::Key::TAB).bReleased)
            {
                m_nGameMode = MODE_WORLD_MAP;
            }
            if (GetKey(olc::Key::F).bReleased && m_pPlayer->pEquipedSpell != nullptr)
            {
                m_pPlayer->PerformSpell();
            }
            if (GetKey(olc::Key::ESCAPE).bReleased)
            {
                m_nGameMode = MODE_OPTIONS;
               // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("open_backpack"));
                  engine.PlayWaveform(RPG_Assets::get().GetSound("open_backpack"));

            }
            if (GetKey(olc::Key::O).bReleased)
            {
                if (m_pPlayer->Level >= 2)
                {
                    m_nGameMode = MODE_SPELLS;
                   // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("open_backpack"));
                      engine.PlayWaveform(RPG_Assets::get().GetSound("open_backpack"));
                }
            }
            if (GetKey(olc::Key::C).bReleased)
            {
                if (Leveled == true)
                {
                   // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Fanfare"));
                     engine.PlayWaveform(RPG_Assets::get().GetSound("Fanfare"));
                    m_nGameMode = MODE_LEVEL;
                }
                else
                {
                    m_nGameMode = MODE_HERO;
                }
            }
            if (GetKey(olc::Key::Z).bReleased)
            {
                m_nGameMode = MODE_INVENTORY;
                //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("open_backpack"));   
                 engine.PlayWaveform(RPG_Assets::get().GetSound("open_backpack"));
            }
            if (GetKey(olc::Key::M).bReleased)
            {
                //olc::SOUND::StopSample(RPG_Assets::get().GetSound("Intro"));
                 engine.PlayWaveform(RPG_Assets::get().GetSound("Intro"));
            }
            if (GetKey(olc::Key::H).bReleased)
            {
                m_pPlayer->XP += 100;
            }
            if (GetKey(olc::Key::Y).bReleased)
            {               
                m_pPlayer->nHealth += 100;
                m_pPlayer->nGold += 100;
                m_pPlayer->nMana += 100;
                m_pPlayer->AttackCoolDown = 0;
                m_pPlayer->SpellCoolDown = 0;
                if (m_pPlayer->nHealth > m_pPlayer->nHealthMax || m_pPlayer->nMana > m_pPlayer->nManaMax)
                {
                    m_pPlayer->nHealth = m_pPlayer->nHealthMax;
                    m_pPlayer->nMana = m_pPlayer->nManaMax;
                }
            }
            if (GetKey(olc::Key::SPACE).bReleased && m_pPlayer->pEquipedWeapon != nullptr)
            {
                m_pPlayer->PerformAttack();//with sword
            }
            if (GetKey(olc::Key::E).bReleased)
            {
                float fTestX, fTestY;
                if (m_pPlayer->GetFacingDirection() == 0) // South
                {
                    fTestX = m_pPlayer->px + 0.5f;
                    fTestY = m_pPlayer->py + 1.5f;
                }

                if (m_pPlayer->GetFacingDirection() == 1) // West
                {
                    fTestX = m_pPlayer->px - 0.5f;
                    fTestY = m_pPlayer->py + 0.5f;
                }

                if (m_pPlayer->GetFacingDirection() == 2) // North
                {
                    fTestX = m_pPlayer->px + 0.5f;
                    fTestY = m_pPlayer->py - 0.5f;
                }

                if (m_pPlayer->GetFacingDirection() == 3) // East
                {
                    fTestX = m_pPlayer->px + 1.5f;
                    fTestY = m_pPlayer->py + 0.5f;
                }
                bool bHitSomething = false;
                for (auto dyns : m_vecDynamics)
                {
                    if (fTestX > dyns->px && fTestX < (dyns->px + 1.0f) && fTestY > dyns->py && fTestY < (dyns->py + 1.0f))
                    {
                        if (dyns->bFriendly)
                        {
                            for (auto& quest : m_listQuests)
                                if (quest->OnInteraction(m_vecDynamics, dyns, cQuest::TALK))
                                {
                                    bHitSomething = true;
                                    break;
                                }
                            dyns->OnInteract(m_pPlayer);
                            m_pCurrentMap->OnInteraction(m_vecDynamics, dyns, cMap::TALK);
                        }
                        else
                        {
                        }
                    }
                }
                if (!bHitSomething)
                {
                }
            }
        }
    }
    else
    {
        if (m_bShowDialog)
        {
            if (GetKey(olc::Key::E).bReleased)
            {
                m_bShowDialog = false;
                m_script.CompleteCommand();
            }
        }
    }


    

    Dynamicdebug += fElapsedTime;
    //COLLISION================================
    bool bWorkingWithProjectiles = false;

    for (auto& source : { &m_vecDynamics, &m_vecProjectiles })
    {
        for (auto& object : *source)
        {
            float fNewObjectPosX = object->px + object->vx * fElapsedTime;
            float fNewObjectPosY = object->py + object->vy * fElapsedTime;
            // Collision//////////////////////////////////////////////////////////////////////////////////////////////
            float fBorder = 0.1f;

            bool bCollisionWithMap = false;
            bool bCollisionWithMapX = false;
            bool bCollisionWithMapY = false;
            if (object->vx <= 0) // Moving Left
            {
                if (m_pCurrentMap->GetSolid(fNewObjectPosX + fBorder, object->py + fBorder + 0.0f) || m_pCurrentMap->GetSolid(fNewObjectPosX + fBorder, object->py + (1.0f - fBorder)))
                {
                    fNewObjectPosX = (int)fNewObjectPosX + 1;
                    object->vx = 0;
                    bCollisionWithMap = true;
                    bCollisionWithMapX = true;
                }
            }
            else // Moving Right
            {
                if (m_pCurrentMap->GetSolid(fNewObjectPosX + (1.0f - fBorder), object->py + fBorder + 0.0f) || m_pCurrentMap->GetSolid(fNewObjectPosX + (1.0f - fBorder), object->py + (1.0f - fBorder)))
                {
                    fNewObjectPosX = (int)fNewObjectPosX;
                    object->vx = 0;
                    bCollisionWithMap = true;
                    bCollisionWithMapX = true;
                }
            }
            if (object->vy <= 0) // Moving Up
            {
                if (m_pCurrentMap->GetSolid(fNewObjectPosX + fBorder + 0.0f, fNewObjectPosY + fBorder) || m_pCurrentMap->GetSolid(fNewObjectPosX + (1.0f - fBorder), fNewObjectPosY + fBorder))
                {
                    fNewObjectPosY = (int)fNewObjectPosY + 1;
                    object->vy = 0;
                    bCollisionWithMap = true;
                    bCollisionWithMapY = true;
                }
            }
            else // Moving Down
            {
                if (m_pCurrentMap->GetSolid(fNewObjectPosX + fBorder + 0.0f, fNewObjectPosY + (1.0f - fBorder)) || m_pCurrentMap->GetSolid(fNewObjectPosX + (1.0f - fBorder), fNewObjectPosY + (1.0f - fBorder)))
                {
                    fNewObjectPosY = (int)fNewObjectPosY;
                    object->vy = 0;
                    bCollisionWithMap = true;
                    bCollisionWithMapY = true;
                }
            }
            if (object->bIsProjectile && bCollisionWithMap)
            {
                object->bRedundant = true;
            }
            float fDynamicObjectPosX = fNewObjectPosX;
            float fDynamicObjectPosY = fNewObjectPosY;
            // Object V Object Collisions
            for (auto& dyn : m_vecDynamics)
            {
                if (dyn != object)
                {
                    if (dyn->bSolidVsDyn && object->bSolidVsDyn)
                    {
                        if (fDynamicObjectPosX < (dyn->px + 1.0f) && (fDynamicObjectPosX + 1.0f) > dyn->px &&
                            object->py < (dyn->py + 1.0f) && (object->py + 1.0f) > dyn->py)
                        {
                            // First Check Horizontally - Check Left
                            if (object->vx <= 0)
                                fDynamicObjectPosX = dyn->px + 1.0f;
                            else
                                fDynamicObjectPosX = dyn->px - 1.0f;
                        }

                        if (fDynamicObjectPosX < (dyn->px + 1.0f) && (fDynamicObjectPosX + 1.0f) > dyn->px &&
                            fDynamicObjectPosY < (dyn->py + 1.0f) && (fDynamicObjectPosY + 1.0f) > dyn->py)
                        {
                            // First Check Vertically - Check Left
                            if (object->vy <= 0)
                                fDynamicObjectPosY = dyn->py + 1.0f;
                            else
                                fDynamicObjectPosY = dyn->py - 1.0f;
                        }
                    }
                    else
                    {
                        if (object == m_vecDynamics[0])
                        {
                            if (fDynamicObjectPosX < (dyn->px + 1.0f) && (fDynamicObjectPosX + 1.0f) > dyn->px &&
                                object->py < (dyn->py + 1.0f) && (object->py + 1.0) > dyn->py)
                            {
                                for (auto& quest : m_listQuests)
                                    if (quest->OnInteraction(m_vecDynamics, dyn, cQuest::WALK))
                                        break;
                                m_pCurrentMap->OnInteraction(m_vecDynamics, dyn, cMap::WALK);

                                dyn->OnInteract(object);
                            }
                        }
                        else
                        {
                            if (bWorkingWithProjectiles)
                            {
                                if (fDynamicObjectPosX < (dyn->px + 1.0f) && (fDynamicObjectPosX + 1.0f) > dyn->px &&
                                    fDynamicObjectPosY < (dyn->py + 1.0f) && (fDynamicObjectPosY + 1.0f) > dyn->py)
                                {
                                    if (dyn->bFriendly != object->bFriendly)
                                    {			
                                        cDynamic_Creature* _dyn;							
                                        if (dyn->bIsAttackable && (_dyn = dynamic_cast<cDynamic_Creature*>(dyn)))
                                        {
                                            Damage((cDynamic_Projectile*)object, _dyn);
                                        }
                                    }
                                }
                            }
                        }
                    }

                }
            }
            if (bCollisionWithMapX != true && Dynamicdebug > 0.5)
            {
                object->px = fDynamicObjectPosX;
            }
            if (bCollisionWithMapY != true && Dynamicdebug > 0.5)
            {
                object->py = fDynamicObjectPosY;
            }
        }
        bWorkingWithProjectiles = true;
    }
    for (auto& dyns : m_vecDynamics)
        dyns->Update(fElapsedTime, m_pPlayer);
    for (auto& dyns : m_vecProjectiles)
        dyns->Update(fElapsedTime, m_pPlayer);
    auto i = remove_if(m_listQuests.begin(), m_listQuests.end(), [](const cQuest* d) { return d->bCompleted; });
    if (i != m_listQuests.end())
        m_listQuests.erase(i);
    
    fCameraPosX = m_pPlayer->px;
    fCameraPosY = m_pPlayer->py;
    //Draw level
    int nTileWidth = 16;
    int nTileHeight = 16;
    int nVisibleTilesX = ScreenWidth() / nTileWidth;
    int nVisibleTilesY = ScreenHeight() / nTileHeight;
    //Calculate top leftmost visible tile
    float fOffsetX = fCameraPosX - (float)(nVisibleTilesX / 2.0f);
    float fOffsetY = fCameraPosY - (float)(nVisibleTilesY / 2.0f);
    //Clamp camera
    if (fOffsetX < 0) fOffsetX = 0;
    if (fOffsetY < 0) fOffsetY = 0;
    if (fOffsetX > m_pCurrentMap->nWidth - nVisibleTilesX) fOffsetX = m_pCurrentMap->nWidth - nVisibleTilesX;
    if (fOffsetY > m_pCurrentMap->nHeight - nVisibleTilesY) fOffsetY = m_pCurrentMap->nHeight - nVisibleTilesY;
    //Get offsets for smooth movement
    float fTileOffsetX = (fOffsetX - (int)fOffsetX) * nTileWidth;
    float fTileOffsetY = (fOffsetY - (int)fOffsetY) * nTileHeight;
    //Draw visible tile map
    for (int x = -1; x < nVisibleTilesX + 1; x++)
    {
        for (int y = -1; y < nVisibleTilesY + 1; y++)
        {
            int idx = m_pCurrentMap->GetIndex(x + fOffsetX, y + fOffsetY);
            int sx = idx % 10;
            int sy = idx / 10;
            DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, m_pCurrentMap->pSprite, sx * nTileWidth, sy * nTileHeight, nTileWidth, nTileHeight);
        }
    }
    
    // Draw Ojects
    for (auto& dyns : m_vecDynamics)
        dyns->DrawSelf(this, fOffsetX, fOffsetY);
    for (auto& dyns : m_vecProjectiles)
        dyns->DrawSelf(this, fOffsetX, fOffsetY);





    std::string sHealth = "HP:" + std::to_string(m_pPlayer->nHealth) + "/" + std::to_string(m_pPlayer->nHealthMax);
    float healthpercent = (float)m_pPlayer->nHealth / (float)m_pPlayer->nHealthMax;
    FillRect(159, 0, healthpercent*90, 10,olc::DARK_RED);
    DrawRect(158, 0, 90, 10, olc::DARK_GREY);
    DrawBigText(sHealth, 160, 2);
    //MANA
    float manapercent = (float)m_pPlayer->nMana / (float)m_pPlayer->nManaMax;
    FillRect(159, 10, manapercent * 90, 5, olc::DARK_BLUE);
    DrawRect(158, 10, 90, 5, olc::DARK_GREY);
    //Debug info
    if (m_pPlayer->pEquipedWeapon != nullptr)
    {
        DrawRect(143, 0, 16, 15, olc::DARK_GREY);
        DrawPartialSprite(143, 0, m_pPlayer->pEquipedWeapon->pSprite, 0, 0, 16, 16);
    }

    std::string Debug = "PX:" + std::to_string((int)m_pPlayer->px) + " PY:" + std::to_string((int)m_pPlayer->py);
   // DisplayDialog({ Debug }, 165, 200);

    std::string Mapname = "Map: " + std::string(m_pCurrentMap->sName);
    DisplayDialog({ Mapname }, 2, 230);
    pTime = pTime + fElapsedTime;
    float XptoLevel = m_pPlayer->XP / (10 * m_pPlayer->Level);
    FillRect(139, 230, 115, 9, olc::DARK_YELLOW);
    FillRect(139, 230, (XptoLevel * 115), 9, olc::YELLOW);
    lvltime =  lvltime + fElapsedTime;
   
    if (Leveled == true)
    {
        
        if (lvltime >= 0.2)
        {
            lvltime = 0.0;
            color = olc::Pixel(rand() % 128 + 128, rand() % 128 + 128, rand() % 128 + 128);
           
        }
        FillRect(139, 230, 115, 9, color);
    }
    else
    {
        DrawRect(139, 230, 115, 9, olc::WHITE);
    }
    DrawString(185, 232, std::to_string(m_pPlayer->Level), olc::RED);
      
    if (pTime > 0)
    {
        for (auto& object : m_vecDynamics)
        {
            cDynamic_Creature* dyn = dynamic_cast<cDynamic_Creature*>(object);
            if(dyn == nullptr) continue;
            if (dyn->AttackCoolDown > 0)
                dyn->AttackCoolDown -= pTime;
            if (dyn->SpellCoolDown > 0)
                dyn->SpellCoolDown -= pTime;
            if (dyn->AttackCoolDown < 0)
                dyn->AttackCoolDown = 0;
            if (dyn->SpellCoolDown < 0)
                dyn->SpellCoolDown = 0;
        }
        pTime = 0;
    }
    mTime += fElapsedTime;
    if (mTime > 1)
    {
        if (m_pPlayer->nMana < m_pPlayer->nManaMax)
            m_pPlayer->nMana += 1;
        mTime = 0;
    }
    
    DeadTime += fElapsedTime;

    for (auto it = m_vecDynamics.begin(); it != m_vecDynamics.end(); ++it)
    {
        auto object = *it;
        cDynamic_Creature* dyn = dynamic_cast<cDynamic_Creature*>(object);
        if(dyn == nullptr) continue;

        if (dyn->Dead == true && dyn != m_pPlayer)
        {
            *it = new cDynamic_Item(dyn->px, dyn->py, RPG_Assets::get().GetItem("5 gold"));
            delete object;
            m_pPlayer->XP += 2;
        }
    }
    if (m_pPlayer->XP >= 10 * m_pPlayer->Level)
    {
        Leveled = true;
        //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("LevelUp"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("LevelUp"));

        m_pPlayer->Level += 1;
        m_pPlayer->nHealthMax += 10;
        m_pPlayer->nHealth = m_pPlayer->nHealthMax;
        m_pPlayer->nManaMax += 5;
        m_pPlayer->XP = 0;
    }
    lightingtimer += fElapsedTime;
    if (m_braining == true)
    {

        if (lightingtimer > m_fLightingTimer)
        {
            if((int)(rand() % 2) == 1)
                Clear(olc::WHITE);

        }
        if (lightingtimer > m_fLightingTimer + 0.1f)
        {
            lightingtimer = 0.0f;
            if((int)(rand() % 2)== 1)
                engine.PlayWaveform(RPG_Assets::get().GetSound("Lighting"));

            if((int)(rand() % 2) == 1)
            {
                m_braining = false;
            }
        }
        UpdateRain(fElapsedTime);
    }
    if (m_pCurrentMap->sName == "River")
        m_braining = true;
    
    if (m_bShowDialog)
    {
        DisplayDialog(m_vecDialogToShow, 15, 15);
    }
    return true;
}
void RPG_Engine::ShowDialog(std::vector<std::string> vecLines)
{
    m_vecDialogToShow = vecLines;
    m_bShowDialog = true;

}
void RPG_Engine::DisplayDialog(std::vector<std::string> vecText, int x, int y)
{
    int nMaxLineLength = 0;
    int nLines = vecText.size();
    for (auto l : vecText) if (l.size() > nMaxLineLength) nMaxLineLength = l.size();

    FillRect(x - 1, y - 1, nMaxLineLength * 8 + 2, nLines * 8 + 2, olc::DARK_BLUE);
    DrawLine(x - 2, y - 2, x - 2, y + nLines * 8 + 1);
    DrawLine(x + nMaxLineLength * 8 + 1, y - 2, x + nMaxLineLength * 8 + 1, y + nLines * 8 + 1);
    DrawLine(x - 2, y - 2, x + nMaxLineLength * 8 + 1, y - 2);
    DrawLine(x - 2, y + nLines * 8 + 1, x + nMaxLineLength * 8 + 1, y + nLines * 8 + 1);

    for (int l = 0; l < vecText.size(); l++)
        DrawBigText(vecText[l], x, y + l * 8);
}
void RPG_Engine::DrawBigText(std::string sText, int x, int y)
{
    int i = 0;
    for (auto c : sText)
    {
        int sx = ((c - 32) % 16) * 8;
        int sy = ((c - 32) / 16) * 8;
        DrawPartialSprite(x + i * 8, y, m_sprFont, sx, sy, 8, 8);
        i++;
    }

}
void RPG_Engine::ChangeMap(std::string sMapName, float x, float y)
{
    // Destroy all dynamics
    m_vecDynamics.clear();
    m_vecDynamics.push_back(m_pPlayer);

    // Set current map
    m_pCurrentMap = RPG_Assets::get().GetMap(sMapName);

    // Update player location
    m_pPlayer->px = x;
    m_pPlayer->py = y;

    m_pCurrentMap->PopulateDynamics(m_vecDynamics);

    for (auto q : m_listQuests)
        q->PopulateDynamics(m_vecDynamics, m_pCurrentMap->sName);
}
void RPG_Engine::ChangeMode(int GameMode)
{
   // m_nGameMode = MOD
    m_nGameMode = GameMode;

}
void RPG_Engine::AddQuest(cQuest* quest)
{
    m_listQuests.push_front(quest);
}
bool RPG_Engine::GiveItem(cItem* item)
{
    m_listItems.push_back(item);
    return true;
}
bool RPG_Engine::TakeItem(cItem* item)
{
    if (item != nullptr)
    {
        m_listItems.erase(find(m_listItems.begin(), m_listItems.end(), item));
        return true;
    }
    else
        return false;
}
bool RPG_Engine::HasItem(cItem* item)
{
    if (item != nullptr)
        return find(m_listItems.begin(), m_listItems.end(), item) != m_listItems.end();
    else
        if (item!= nullptr)
            return find(m_CharacterItems.begin(), m_CharacterItems.end(), item) != m_CharacterItems.end();
        else
            if (item!= nullptr)
                return find(m_KeyItems.begin(), m_KeyItems.end(), item) != m_KeyItems.end();
            else
                return false;
}



bool RPG_Engine::UpdateTitleScreen(float fElapsedTime)
{
    INTRO = false;
    olc::vf2d zeros = { 0, 0 };
    if (S1 == true)
    {
       // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Thud"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("Thud"));
        S1 = false;
    }

    DrawSprite(zeros, RPG_Assets::get().GetSprite("title"));
    if (GetKey(olc::Key::SPACE).bReleased)

    {
        ChangeMap("Cherry Wood", 15, 15);
       //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Intro"), true);
      // engine.PlayWaveform(RPG_Assets::get().GetSound("Intro"));
        m_nGameMode = MODE_LOCAL_MAP;
    }
    return true;
}
bool RPG_Engine::UpdateOptions(float fElapsedTime)
{
    //bool Click_Zone;
    if (GetKey(olc::Key::ESCAPE).bReleased)
    {
        m_nGameMode = MODE_LOCAL_MAP;
       // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("menu click"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("menu click"));
    }
    int nMouseX = GetMouseX();
    int nMouseY = GetMouseY();
    auto PointInRect = [&](int x, int y, int rx, int ry, int rw, int rh)
    {
        return x >= rx && x < (rx + rw) && y >= ry && y < (ry + rh);
    };
    //BACK TO GAME ICON
    FillRect(65, 4, 140, 24, olc::BLACK);
    DrawRect(65, 4, 140, 24);
    DrawString(80, 10, "Back to Game");
    //MUSIC ICON
    FillRect(65, 32, 140, 24, olc::BLACK);
    DrawRect(65, 32, 140, 24);
    DrawString(93, 40, "Music");
    //SAVE ICON
    FillRect(65, 60, 140, 24, olc::BLACK);
    DrawRect(65, 60, 140, 24);
    DrawString(94, 70, "Save");
    //LOAD ICON
    FillRect(65, 88, 140, 24, olc::BLACK);
    DrawRect(65, 88, 140, 24);
    DrawString(94, 100, "Load");
    //Main Menu
    FillRect(65, 116, 140, 24, olc::BLACK);
    DrawRect(65, 116, 140, 24);
    DrawString(80, 130, "Main Menu");

    if(PointInRect(nMouseX, nMouseY, 65, 4, 140, 24))
    {
        FillRect(65, 4, 140, 24, olc::RED);
        DrawRect(65, 4, 140, 24, olc::DARK_GREY);
        DrawString(80, 10, "Back to Game");
        if (GetMouse(0).bPressed && PointInRect(nMouseX, nMouseY, 65, 4, 140, 24))
        {
            m_nGameMode = MODE_LOCAL_MAP;
           // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("menu click"));
             engine.PlayWaveform(RPG_Assets::get().GetSound("menu click"));
        }
    }

    if (PointInRect(nMouseX, nMouseY, 65, 32, 140, 24))
    {
        FillRect(65, 32, 140, 24, olc::RED);
        DrawRect(65, 32, 140, 24, olc::DARK_GREY);
        DrawString(93, 40, "Music");
        if (GetMouse(0).bPressed && PointInRect(nMouseX, nMouseY, 65, 32, 140, 24))
        {
            m_nGameMode = MODE_MUSIC;//Mode_Music
            //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("menu click"));
             engine.PlayWaveform(RPG_Assets::get().GetSound("menu click"));
        }
    }
    return true;
}
bool RPG_Engine::UpdateMusic(float fElapsedTime)
{
    if (GetKey(olc::Key::ESCAPE).bReleased)
    {
        m_nGameMode = MODE_LOCAL_MAP;
        //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("menu click"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("menu click"));
        engine.PlayWaveform(RPG_Assets::get().GetSound("menu click"));
    }
    int nMouseX = GetMouseX();
    int nMouseY = GetMouseY();
    auto PointInRect = [&](int x, int y, int rx, int ry, int rw, int rh)
    {
        return x >= rx && x < (rx + rw) && y >= ry && y < (ry + rh);
    };
    FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::VERY_DARK_BLUE);
    //BACK TO GAME ICON
    FillRect(65, 4, 140, 24, olc::BLACK);
    DrawRect(65, 4, 140, 24);
    DrawString(80, 10, "Back to Game");
    //MUSIC ICON
    FillRect(65, 32, 140, 24, olc::BLACK);
    DrawRect(65, 32, 140, 24);
    DrawString(90, 38, "Music Volume");
    if (PointInRect(nMouseX, nMouseY, 65, 4, 140, 24))
    {
        FillRect(65, 4, 140, 24, olc::RED);
        DrawRect(65, 4, 140, 24, olc::DARK_GREY);
        DrawString(80, 10, "Back to Game");
        if (GetMouse(0).bPressed && PointInRect(nMouseX, nMouseY, 65, 4, 140, 24))
        {
            m_nGameMode = MODE_LOCAL_MAP;
            //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("menu click"));
             engine.PlayWaveform(RPG_Assets::get().GetSound("menu click"));
        }
    }
    DrawRect(60, 78, 90, 24);
    DrawString(64, 82, "Volume +");
    DrawRect(160, 78, 90, 24);
    DrawString(164, 82, "Volume -");
    if (GetMouse(0).bHeld && PointInRect(nMouseX, nMouseY, 60, 78, 90, 24))
    {
        //float Volume = 0;
        //Volume += 2.0f * fElapsedTime;
        
       
    }

    if (GetMouse(0).bHeld && PointInRect(nMouseX, nMouseY, 160, 78, 90, 24))
    {
      //  float Volume = 0;
       // Volume += 2.0f * fElapsedTime;
        //SoundFilter::fFilterVolume -= 2.0f * fElapsedTime;
    }
   // DrawString(134, 110, "Volume: " + std::to_string((int)(SoundFilter::fFilterVolume * 10.0f)));

   // if (SoundFilter::fFilterVolume < 0.0f) SoundFilter::fFilterVolume = 0.0f;
   // if (SoundFilter::fFilterVolume > 1.0f) SoundFilter::fFilterVolume = 1.0f;
    return true;
}
bool RPG_Engine::UpdateInventory(float fElapsedTime)
{
    FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);
    FillRect(6, 18, 81, 81, olc::DARK_BLUE);
    DrawBigText("Inventory", 4, 4);
    int i = 0;
    cItem* highlighted = nullptr;

    // Draw Consumables
    for (auto& item : m_listItems)
    {
        int x = i % 4;
        int y = i / 4;
        i++;
    
        if (item->sName == "Key")
        {
            DrawPartialSprite(8 + x * 20, 20 + y * 20, item->pSprite, 16, 0, 16, 16);
        }
        else if(item->sName == "RedKey" || item->sName == "BlueKey")
        {
            DrawPartialSprite(8 + x * 20, 20 + y * 20, item->pSprite, 32, 0, 16, 16);
        }
        else
        {
            DrawPartialSprite(8 + x * 20, 20 + y * 20, item->pSprite, 0, 0, 16, 16);
        }
        if (m_nInvSelectX == x && m_nInvSelectY == y)
            highlighted = item;
    }
    // Draw selection reticule
    DrawLine(6 + (m_nInvSelectX) * 20, 18 + (m_nInvSelectY) * 20, 6 + (m_nInvSelectX + 1) * 20, 18 + (m_nInvSelectY) * 20);
    DrawLine(6 + (m_nInvSelectX) * 20, 18 + (m_nInvSelectY + 1) * 20, 6 + (m_nInvSelectX + 1) * 20, 18 + (m_nInvSelectY + 1) * 20);
    DrawLine(6 + (m_nInvSelectX) * 20, 18 + (m_nInvSelectY) * 20, 6 + (m_nInvSelectX) * 20, 18 + (m_nInvSelectY + 1) * 20);
    DrawLine(6 + (m_nInvSelectX + 1) * 20, 18 + (m_nInvSelectY) * 20, 6 + (m_nInvSelectX + 1) * 20, 18 + (m_nInvSelectY + 1) * 20);


    if (GetKey(olc::Key::LEFT).bPressed || GetKey(olc::Key::A).bPressed) 
    { 
        m_nInvSelectX--; 
       // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("select_backpack"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("select_backpack"));

    }
    if (GetKey(olc::Key::RIGHT).bPressed || GetKey(olc::Key::D).bPressed)
    {
        m_nInvSelectX++;
       // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("select_backpack"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("select_backpack"));
    }
    if (GetKey(olc::Key::UP).bPressed || GetKey(olc::Key::W).bPressed)
    {
        m_nInvSelectY--;
        //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("select_backpack"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("select_backpack"));
    }
    if (GetKey(olc::Key::DOWN).bPressed || GetKey(olc::Key::S).bPressed)
    {
        m_nInvSelectY++;
        //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("select_backpack"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("select_backpack"));
    }
    if (GetKey(olc::Key::O).bReleased)
    {
        if (m_pPlayer->Level >= 2)
        {
            m_nGameMode = MODE_SPELLS;
           // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("open_backpack"));
             engine.PlayWaveform(RPG_Assets::get().GetSound("open_backpack"));
        }
    }
    if (m_nInvSelectX < 0) m_nInvSelectX = 3;
    if (m_nInvSelectX >= 4) m_nInvSelectX = 0;
    if (m_nInvSelectY < 0) m_nInvSelectY = 3;
    if (m_nInvSelectY >= 4) m_nInvSelectY = 0;
    if (GetKey(olc::Key::Z).bReleased || (GetKey(olc::Key::ESCAPE).bReleased))
    {
        m_nGameMode = MODE_LOCAL_MAP;
        //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("open_backpack"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("open_backpack"));
    }
    DrawBigText("SELECTED:", 8, 160);
    if (highlighted != nullptr)
    {
        DrawBigText("SELECTED:", 8, 160);
        DrawBigText(highlighted->sName, 8, 170);

        DrawBigText("DESCRIPTION:", 8, 190);
        DrawBigText(highlighted->sDescription, 8, 200);

        if (!highlighted->bKeyItem)
        {
            DrawBigText("(Press SPACE to use)", 80, 160);
        }    
        if (GetKey(olc::Key::SPACE).bReleased)
        {
            // Use selected item 
            if (!highlighted->bKeyItem)
            {
                if (highlighted->bWeapon)
                {
                    m_pPlayer->pEquipedWeapon = (cWeapon*)highlighted; //Equips Weapon and removes from Itemlist                       
                }

                if (highlighted->OnUse(m_pPlayer))
                {
                    TakeItem(highlighted);
                }
            }
            else
            {
            }
        }
        else
        {

        }


    }
    DrawBigText("LOCATION:", 128, 8);
    DrawString(128,16, m_pCurrentMap->sName, olc::GREEN);
    DrawString(146, 60, "Gold: " + std::to_string(m_pPlayer->nGold), olc::YELLOW);
    DrawSprite({ 128,56 }, RPG_Assets::get().GetSprite("Coin"));
    DrawSprite({ 113,29 }, RPG_Assets::get().GetSprite("health"));
    DrawString(128, 32, "HEALTH: " + std::to_string(m_pPlayer->nHealth),olc::RED);
    DrawString(128, 44, "MAX HEALTH: " + std::to_string(m_pPlayer->nHealthMax),olc::RED);
    DrawString(146, 72 ,"Mana: " + std::to_string(m_pPlayer->nMana), olc::BLUE);
    DrawPartialSprite({ 111,42 }, RPG_Assets::get().GetSprite("healthboost"), { 0,0 }, { 16,16 });

    return true;

}
bool RPG_Engine::UpdateShop(float fElapsedTime)
{
    if (GetKey(olc::Key::ESCAPE).bReleased)
    {
        m_nGameMode = MODE_LOCAL_MAP;
        //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("menu click"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("menu click"));
    }
    int nMouseX = GetMouseX();
    int nMouseY = GetMouseY();
    auto PointInRect = [&](int x, int y, int rx, int ry, int rw, int rh)
    {
        return x >= rx && x < (rx + rw) && y >= ry && y < (ry + rh);
    };
    FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::VERY_DARK_GREY);
    DrawBigText("Not a Thief's", 142, 4);
    DrawBigText("Shop", 193, 12);
    FillRect(180, 180, 70, 70, olc::BLUE);
    DrawRect(180, 180, 70, 70, olc::YELLOW);
    DrawPartialSprite(185, 185, RPG_Assets::get().GetSprite("DarkHero"), 81, 0, 15, 16,4.0);
    DrawString(142, 20, "Gold: " + std::to_string(m_pPlayer->nGold), olc::YELLOW);
    FillRect(0, 4, 140, 24, olc::BLACK);
    DrawRect(0, 4, 140, 24);
    DrawString(0, 10, "Back to Game");
    FillRect(0, 88, 140, 24, olc::BLACK);
    DrawRect(0, 88, 140, 24);
    DrawPartialSprite(142, 92, RPG_Assets::get().GetSprite("healthboost"), 16, 0, 16, 16);
    DrawString(0, 100, "Key: 150");
    if (PointInRect(nMouseX, nMouseY, 0, 4, 140, 24))
    {
        FillRect(0, 4, 140, 24, olc::RED);
        DrawRect(0, 4, 140, 24, olc::DARK_GREY);
        DrawString(2, 10, "Back to Game");
        if (GetMouse(0).bPressed && PointInRect(nMouseX, nMouseY, 0, 4, 140, 24))
        {
            m_nGameMode = MODE_LOCAL_MAP;
           // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("menu click"));
             engine.PlayWaveform(RPG_Assets::get().GetSound("menu click"));
        }
    }
    if (PointInRect(nMouseX, nMouseY, 0, 88, 140, 24))
    {
        FillRect(0, 88, 140, 24, olc::RED);
        DrawRect(0, 88, 140, 24, olc::DARK_GREY);
        DrawString(2, 96, "Buy Key:150");

        if (GetMouse(0).bPressed && PointInRect(nMouseX, nMouseY, 0, 88, 140, 24) && m_pPlayer->nGold >= 150)
        {
            m_pPlayer->nGold -= 150;
            m_listItems.push_back(RPG_Assets::get().GetItem("Key"));
            //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Boom"));
             engine.PlayWaveform(RPG_Assets::get().GetSound("Boom"));
        }
    }
    return true;
}
bool RPG_Engine::UpdateHunterShop(float fElapsedTime)
{
    if (GetKey(olc::Key::ESCAPE).bReleased)
    {
        m_nGameMode = MODE_LOCAL_MAP;
       // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("menu click"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("menu click"));
    }
    int nMouseX = GetMouseX();
    int nMouseY = GetMouseY();
    auto PointInRect = [&](int x, int y, int rx, int ry, int rw, int rh)
    {
        return x >= rx && x < (rx + rw) && y >= ry && y < (ry + rh);
    };
    //
    FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::VERY_DARK_GREY);

    DrawBigText("Hunter's Shop", 142, 4);
    DrawString(142, 20, "Gold: " + std::to_string(m_pPlayer->nGold), olc::YELLOW);
    FillRect(180, 180, 70, 70, olc::BLUE);
    DrawRect(180, 180, 70, 70, olc::YELLOW);
    DrawPartialSprite(185, 185, RPG_Assets::get().GetSprite("skelly"), 64, 0, 16, 16, 4.0);
    //BACK TO GAME ICON
    FillRect(0, 4, 140, 24, olc::BLACK);
    DrawRect(0, 4, 140, 24);
    DrawString(0, 10, "Back to Game");
    //MUSIC ICON
    FillRect(0, 32, 140, 24, olc::BLACK);
    DrawRect(0, 32, 140, 24);
    DrawPartialSprite(142, 34, RPG_Assets::get().GetSprite("healthboost"), 0, 0, 16, 16);
    DrawString(0, 40, "Buy Potion: 20");
    FillRect(0, 88, 140, 24, olc::BLACK);
    DrawRect(0, 88, 140, 24);
    DrawPartialSprite(142, 92, RPG_Assets::get().GetSprite("healthboost"), 32, 0, 16, 16);
    DrawString(0, 100, "RedKey: 300");
    
    FillRect(0, 116, 140, 24, olc::BLACK);
    DrawRect(0, 116, 140, 24);
    DrawString(0, 130, "ManaUP");
    DrawPartialSprite(142, 120, RPG_Assets::get().GetSprite("Mana"), 0, 0, 16, 16);
    
    if (PointInRect(nMouseX, nMouseY, 0, 4, 140, 24))
    {
        FillRect(0, 4, 140, 24, olc::RED);
        DrawRect(0, 4, 140, 24, olc::DARK_GREY);
        DrawString(2, 10, "Back to Game");
        if (GetMouse(0).bPressed)
        {
            m_nGameMode = MODE_LOCAL_MAP;
            //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("menu click"));
             engine.PlayWaveform(RPG_Assets::get().GetSound("menu click"));

        }
    }
    if (PointInRect(nMouseX, nMouseY, 0, 32, 140, 24))
    {
        FillRect(0, 32, 140, 24, olc::RED);
        DrawRect(0, 32, 140, 24, olc::DARK_GREY);
        DrawString(2, 40, "Buy Potion: 20");

        if (GetMouse(0).bPressed && m_pPlayer->nGold >= 20)
        {
            m_pPlayer->nGold -= 20;
            m_listItems.push_back(RPG_Assets::get().GetItem("Health Boost"));

            //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("PotionPickup"));
             engine.PlayWaveform(RPG_Assets::get().GetSound("PotionPickup"));
        }
    }

    FillRect(0, 60, 140, 24, olc::BLACK);
    DrawRect(0, 60, 140, 24);
    DrawPartialSprite(142, 62, RPG_Assets::get().GetSprite("Fire Sword"), 0, 0, 16, 16);
    DrawString(0, 70, "Buy FireSword:100");
    if (PointInRect(nMouseX, nMouseY, 0, 60, 140, 24))
    {
        FillRect(0, 60, 140, 24, olc::RED);
        DrawRect(0, 60, 140, 24, olc::DARK_GREY);
        DrawString(2, 70, "Buy FireSword:100");

        if (GetMouse(0).bPressed && m_pPlayer->nGold >= 100 && SwordBought != true)
        {
            m_pPlayer->nGold -= 100;
            m_listItems.push_back(RPG_Assets::get().GetItem("Fire Sword"));

            //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Boom"));
             engine.PlayWaveform(RPG_Assets::get().GetSound("Boom"));
            SwordBought = true;
        }
    }

    if (PointInRect(nMouseX, nMouseY, 0, 88, 140, 24))
    {
        FillRect(0, 88, 140, 24, olc::RED);
        DrawRect(0, 88, 140, 24, olc::DARK_GREY);
        DrawString(2, 96, "Buy RedKey:300");

        if (GetMouse(0).bPressed && m_pPlayer->nGold >= 300)
        {
            m_pPlayer->nGold -= 300;
            m_listItems.push_back(RPG_Assets::get().GetItem("RedKey"));
            //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Boom"));
             engine.PlayWaveform(RPG_Assets::get().GetSound("Boom"));
        }
    }
    
    if (PointInRect(nMouseX, nMouseY, 0, 116, 140, 24))
    {
        FillRect(0, 116, 140, 24, olc::RED);
        DrawRect(0, 116, 140, 24, olc::DARK_GREY);
        DrawString(2, 130, "Buy Manaup:60");

        if (GetMouse(0).bPressed && PointInRect(nMouseX, nMouseY, 0, 116, 140, 24) && m_pPlayer->nGold >= 60)
        {
            m_pPlayer->nGold -= 60;
            m_listItems.push_back(RPG_Assets::get().GetItem("Mana Boost"));

            //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Boom"));
             engine.PlayWaveform(RPG_Assets::get().GetSound("Boom"));

        }
    }
    
    return true;
}
bool RPG_Engine::UpdateSPELL(float fElapsedTime)
{
    if (GetKey(olc::Key::ESCAPE).bReleased || GetKey(olc::Key::O).bReleased)
    {
        m_nGameMode = MODE_LOCAL_MAP;
       // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("menu click"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("menu click"));
    }
    FillRect(0, 0, ScreenWidth() / 2, ScreenHeight(), olc::BLACK);
    DrawRect(0, 0, ScreenWidth() / 2, ScreenHeight(), olc::DARK_GREY);
    DrawBigText("Spell List", 4, 4);
    int i = 0;
    cSpell*highlighted = nullptr;
    for (auto& Spell : m_listSpells)
    {
        int x = i % 4;
        int y = i / 4;
        i++;
        
            DrawPartialSprite(8 + x * 20, 20 + y * 20, Spell->pSprite, 0, 0, 16, 16);
        
        if (m_nInvSelectX == x && m_nInvSelectY == y)
            highlighted = Spell;
    }
    DrawLine(6 + (m_nInvSelectX) * 20, 18 + (m_nInvSelectY) * 20, 6 + (m_nInvSelectX + 1) * 20, 18 + (m_nInvSelectY) * 20);
    DrawLine(6 + (m_nInvSelectX) * 20, 18 + (m_nInvSelectY + 1) * 20, 6 + (m_nInvSelectX + 1) * 20, 18 + (m_nInvSelectY + 1) * 20);
    DrawLine(6 + (m_nInvSelectX) * 20, 18 + (m_nInvSelectY) * 20, 6 + (m_nInvSelectX) * 20, 18 + (m_nInvSelectY + 1) * 20);
    DrawLine(6 + (m_nInvSelectX + 1) * 20, 18 + (m_nInvSelectY) * 20, 6 + (m_nInvSelectX + 1) * 20, 18 + (m_nInvSelectY + 1) * 20);

    if (GetKey(olc::Key::LEFT).bPressed || GetKey(olc::Key::A).bPressed)
    {
        m_nInvSelectX--;
        //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("select_backpack"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("select_backpack"));
    }
    if (GetKey(olc::Key::RIGHT).bPressed || GetKey(olc::Key::D).bPressed)
    {
        m_nInvSelectX++;
       //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("select_backpack"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("select_backpack"));
    }
    if (GetKey(olc::Key::UP).bPressed || GetKey(olc::Key::W).bPressed)
    {
        m_nInvSelectY--;
       // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("select_backpack"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("select_backpack"));
    }
    if (GetKey(olc::Key::DOWN).bPressed || GetKey(olc::Key::S).bPressed)
    {
        m_nInvSelectY++;
        //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("select_backpack"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("select_backpack"));
    }
    if (m_nInvSelectX < 0) m_nInvSelectX = 3;
    if (m_nInvSelectX >= 4) m_nInvSelectX = 0;
    if (m_nInvSelectY < 0) m_nInvSelectY = 3;
    if (m_nInvSelectY >= 4) m_nInvSelectY = 0;
    DrawBigText("SELECTED:", 8, 160);
    if (highlighted != nullptr)
    {
        DrawBigText("SELECTED:", 8, 160);
        DrawBigText(highlighted->sName, 8, 170);

        DrawBigText("DAMAGE:", 8, 190);
        DrawBigText(std::to_string(highlighted->nDamage), 8, 200);
        DrawBigText("ManaCost:", 8, 210);
        DrawBigText(std::to_string(highlighted->nManaCost), 8, 220);

        if (GetKey(olc::Key::SPACE).bReleased)
        {
            // Use selected item 
            if (!highlighted->bKeyItem)
            {
               // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Magic Equip"));
                 engine.PlayWaveform(RPG_Assets::get().GetSound("Magic Equip"));
                m_pPlayer->pEquipedSpell = highlighted;
            }
        }
    }
    return true;
}
bool RPG_Engine::UpdateLevelUp(float fElapsedTime)
{
    if (GetKey(olc::Key::ESCAPE).bReleased || GetKey(olc::Key::C).bReleased)
    {
        Leveled = false;
        m_nGameMode = MODE_LOCAL_MAP;
       // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("menu click"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("menu click"));

    }
    FillRect(0, 0, ScreenWidth() / 2, ScreenHeight(), olc::BLACK);
    DrawRect(0, 0, ScreenWidth() / 2, ScreenHeight(), olc::DARK_GREY);
    //DrawSprite(ScreenWidth() / 4, 8, RPG_Assets::get().GetSprite("player"), 2);
    DrawPartialSprite(ScreenWidth() / 6, 8, RPG_Assets::get().GetSprite("player"), 64, 0, 16, 16, 2);

    DrawString(ScreenWidth() / 8, 40, "Level:" + std::to_string(m_pPlayer->Level), olc::Pixel(255, 255, 255));
    //DrawString(ScreenWidth() / 8, 40, "Level:" + to_string(m_pPlayer->Level), olc::Pixel(12, 12, 12));
    if (m_pPlayer->Level == 2)
    {
        DrawSprite(ScreenWidth() / 10, 48, RPG_Assets::get().GetSprite("fireballspr"), 0.001);
        DrawString(ScreenWidth() / 9, 50, "FireBall", olc::Pixel(255, 255, 255));
        DrawString(ScreenWidth() / 9, 60, "Unlocked!", olc::Pixel(255, 255, 255));
        DrawString(8, 80, "Press O to Open", olc::Pixel(255, 255, 255));
        DrawString(15, 90, "SpellBook!", olc::Pixel(255, 255, 255));
        if (Leveled != false)
        {
            m_listSpells.push_back(RPG_Assets::get().GetSpell("Fire Ball"));
            Leveled = false;
        }
    }
    if (m_pPlayer->Level == 5)
    {
        DrawSprite(ScreenWidth() / 10, 48, RPG_Assets::get().GetSprite("fireballspr"), 0.001);
        DrawString(ScreenWidth() / 9, 50, "FireStream", olc::Pixel(255, 255, 255));
        DrawString(ScreenWidth() / 9, 60, "Unlocked!", olc::Pixel(255, 255, 255));
        DrawString(8, 80, "Press O to Open", olc::Pixel(255, 255, 255));
        DrawString(15, 90, "SpellBook!", olc::Pixel(255, 255, 255));
        if (Leveled != false)
        {
            m_listSpells.push_back(RPG_Assets::get().GetSpell("FireStream"));
            Leveled = false;
        }
    }
    if (m_pPlayer->Level == 10)
    {
        DrawSprite(ScreenWidth() / 10, 48, RPG_Assets::get().GetSprite("fireballspr"), 0.001);
        DrawString(ScreenWidth() / 9, 50, "FireStorm", olc::Pixel(255, 255, 255));
        DrawString(ScreenWidth() / 9, 60, "Unlocked!", olc::Pixel(255, 255, 255));
        DrawString(8, 80, "Press O to Open", olc::Pixel(255, 255, 255));
        DrawString(15, 90, "SpellBook!", olc::Pixel(255, 255, 255));
        if (Leveled != false)
        {
            m_listSpells.push_back(RPG_Assets::get().GetSpell("FireStorm"));
            Leveled = false;
        }
    }
    if (m_pPlayer->Level == 15)
    {
        DrawPartialSprite(ScreenWidth() / 10, 48, RPG_Assets::get().GetSprite("Blinkspr"),0,0,16,16, 2);
        DrawString(ScreenWidth() / 9, 50, "Blink", olc::Pixel(255, 255, 255));
        DrawString(ScreenWidth() / 9, 60, "Unlocked!", olc::Pixel(255, 255, 255));
        DrawString(8, 80, "Press O to Open", olc::Pixel(255, 255, 255));
        DrawString(15, 90, "SpellBook!", olc::Pixel(255, 255, 255));
        if (Leveled != false)
        {
            m_listSpells.push_back(RPG_Assets::get().GetSpell("Blink"));
            Leveled = false;
        }
    }


    return true;
}
bool RPG_Engine::UpdateHero(float fElapsedTime)
{
    if (GetKey(olc::Key::ESCAPE).bReleased || GetKey(olc::Key::C).bReleased)
    {
        Leveled = false;
        m_nGameMode = MODE_LOCAL_MAP;
       // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("menu click"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("menu click"));
    }
    FillRect(0, 0, ScreenWidth() / 2, ScreenHeight(), olc::BLACK);
    DrawRect(0, 0, ScreenWidth() / 2, ScreenHeight(), olc::DARK_GREY);
    //DrawSprite(ScreenWidth() / 4, 8, RPG_Assets::get().GetSprite("player"), 2);
    DrawPartialSprite(ScreenWidth() / 6, 10, RPG_Assets::get().GetSprite("player"), 64, 0, 16, 16, 2);
    FillRect((ScreenWidth() / 6) - 4, 0, 70, 9, olc::DARK_BLUE);
    DrawRect((ScreenWidth() / 6) - 4, 0, 70, 9, olc::BLUE);
    DrawString((ScreenWidth() / 6) - 4, 1, "Level:" + std::to_string(m_pPlayer->Level), olc::Pixel(255, 255, 255));
    //DrawString(160, 0, "Character:" + to_string(char))
    if (m_pPlayer->pEquipedWeapon != nullptr)
    {
        FillRect(0, 10, 32, 32, olc::DARK_GREY);
        DrawRect(0, 10, 32, 32, olc::GREY);
        DrawPartialSprite(0, 10, m_pPlayer->pEquipedWeapon->pSprite, 0, 0, 16, 16, 2);
    }
    else
    {
        FillRect(0, 10, 32, 32, olc::BLACK);
        DrawRect(0, 10, 32, 32, olc::WHITE);

    }
    if (m_pPlayer->pEquipedSpell != nullptr)
    {
        FillRect((ScreenWidth() / 6) + 32, 10, 32, 32, olc::DARK_GREY);
        DrawRect((ScreenWidth() / 6) + 32, 10, 32, 32, olc::GREY);
        DrawPartialSprite((ScreenWidth() / 6) + 32, 10, m_pPlayer->pEquipedSpell->pSprite, 0, 0, 16, 16, 2);
    }
    else
    {
        FillRect((ScreenWidth() / 6) + 32, 10, 32, 32, olc::BLACK);
        DrawRect((ScreenWidth() / 6) + 32, 10, 32, 32, olc::WHITE);

    }
    return true;
}
bool RPG_Engine::UpdateWorldMap(float fElapsedTime)
{
    Clear(olc::CYAN);
    if (GetKey(olc::Key::ESCAPE).bReleased)
    {
        m_nGameMode = MODE_OPTIONS;
        //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("open_backpack"));
         engine.PlayWaveform(RPG_Assets::get().GetSound("open_backpack"));

    }
    if (GetKey(olc::Q).bHeld) fNear += 0.1f * fElapsedTime;
    if (GetKey(olc::E).bHeld) fNear -= 0.1f * fElapsedTime;
    if (GetKey(olc::W).bHeld) fFar += 0.1f * fElapsedTime;
    if (GetKey(olc::S).bHeld) fFar -= 0.1f * fElapsedTime;
    if (GetKey(olc::Z).bHeld) fFoVHalf += 0.1f * fElapsedTime;
    if (GetKey(olc::X).bHeld) fFoVHalf -= 0.1f * fElapsedTime;
    float fFarX1  = m_pPlayer->Worldpx + cosf(fWorldA - fFoVHalf) * fFar;
    float fFarY1  = m_pPlayer->Worldpy + sinf(fWorldA - fFoVHalf) * fFar;
    float fFarX2  = m_pPlayer->Worldpx + cosf(fWorldA + fFoVHalf) * fFar;
    float fFarY2  = m_pPlayer->Worldpy + sinf(fWorldA + fFoVHalf) * fFar;
    float fNearX1 = m_pPlayer->Worldpx + cosf(fWorldA - fFoVHalf) * fNear;
    float fNearY1 = m_pPlayer->Worldpy + sinf(fWorldA - fFoVHalf) * fNear;
    float fNearX2 = m_pPlayer->Worldpx + cosf(fWorldA + fFoVHalf) * fNear;
    float fNearY2 = m_pPlayer->Worldpy + sinf(fWorldA + fFoVHalf) * fNear;
    for (int y = 0; y < ScreenHeight() / 2; y++)
    {
        float fSampleDepth = (float)y / ((float)ScreenHeight() / 2.0f);
        float fStartX = (fFarX1 - fNearX1) / ( fSampleDepth)+fNearX1;
        float fStartY = (fFarY1 - fNearY1) / ( fSampleDepth)+fNearY1;
        float fEndX = (fFarX2 - fNearX2) / (fSampleDepth)+fNearX2;
        float fEndY = (fFarY2 - fNearY2) / (fSampleDepth)+fNearY2;
        for (int x = 0; x < ScreenWidth(); x++)
        {
            float fSampleWidth = (float)x / (float)ScreenWidth();
            float fSampleX = (fEndX - fStartX) * fSampleWidth + fStartX;
            float fSampleY = (fEndY - fStartY) * fSampleWidth + fStartY;
            //fSampleX = fmod(fSampleX, 1.0f);
           // fSampleY = fmod(fSampleY, 1.0f);
            olc::Pixel col = RPG_Assets::get().GetSprite("worldmap")->Sample(fSampleX, fSampleY);
            Draw(x, y + ScreenHeight() / 2, col);
            col = RPG_Assets::get().GetSprite("skymap")->Sample(fSampleX, fSampleY);
            Draw(x, ScreenHeight() / 2 - y, col);
        }
    }
    DrawLine(0, ScreenHeight() / 2, ScreenWidth(), ScreenHeight() / 2, olc::CYAN);
    DrawLine(0, ScreenHeight() / 2 +1, ScreenWidth(), ScreenHeight() / 2 +1, olc::CYAN);    
    if (GetKey(olc::LEFT).bHeld)
    {
        fWorldA -= 0.1f;
    }
    if (GetKey(olc::RIGHT).bHeld)
    {
        fWorldA += 0.1f;
    }
    if (GetKey(olc::UP).bHeld)
    {
        m_pPlayer->Worldpx += cosf(fWorldA) * 0.1f * fElapsedTime;
        m_pPlayer->Worldpy += sinf(fWorldA) * 0.1f * fElapsedTime;
    }
    if (GetKey(olc::DOWN).bHeld)
    {
        m_pPlayer->Worldpx -= cosf(fWorldA) * 0.1f * fElapsedTime;
        m_pPlayer->Worldpy -= sinf(fWorldA) * 0.1f * fElapsedTime;
    }
    if (GetKey(olc::SPACE).bPressed)
    {
        fFar += 0.2f;
    }
    if (GetKey(olc::Key::ESCAPE).bReleased && INTRO != true)
    {
        m_nGameMode = MODE_OPTIONS;
        //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("open_backpack"));
        engine.PlayWaveform(RPG_Assets::get().GetSound("open_backpack"));
    }
    DrawString(0, 10, "WorldX:" + std::to_string(m_pPlayer->Worldpx), olc::BLACK);
    DrawString(0, 20, "WorldY:" + std::to_string(m_pPlayer->Worldpy), olc::BLACK);
    DrawString(0, 30, "ANGLE:" + std::to_string(fWorldA), olc::BLACK);
    DrawString(0, 40, "Hight:" + std::to_string(fFar), olc::BLACK);
    
    //INTRODUCTION
    if (INTRO == true)
    {
        if (S1 == true)
        {
            engine.PlayWaveform(RPG_Assets::get().GetSound("Ahhh"));
           // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Ahhh"));
            S1 = false;
        }
        m_fTime += fElapsedTime;

        if (m_fTime > 1 && fFar > 0.0244)
        {
            fWorldA += 0.02f;
        }
        if (fFar <= 0.0254)
        {
            DrawPartialSprite(ScreenWidth() / 2, ScreenHeight() - 16, RPG_Assets::get().GetSprite("player"), 64, 16, 16, 16, 0.5);
        }
        else
        {
            DrawPartialSprite(ScreenWidth() / 2, ScreenHeight() - 16, RPG_Assets::get().GetSprite("player"), 32, 0, 16, 16, 0.5);
        }
    }
    if (INTRO == true)
    {
        if (m_fTime > 5)
        {
            m_nGameMode = MODE_TITLE;
            INTRO = false;
            S1 = true;
        }
    }
    else
    {
        DrawPartialSprite(ScreenWidth() / 2, ScreenHeight() - 16, RPG_Assets::get().GetSprite("player"), 32, 0, 16, 16, 0.5);
    }
    fFar -= 0.1f * fElapsedTime;
    if (fFar <= 0.0244)
    {
        fFar = 0.0244;//Ground
    }
    if (m_pPlayer->Worldpx >= 1) 
        m_pPlayer->Worldpx = 1.0f;
    if (m_pPlayer->Worldpy >= 1)
        m_pPlayer->Worldpy = 1.0f;
    if (m_pPlayer->Worldpx <= 0) 
        m_pPlayer->Worldpx = 0.0f;
    if (m_pPlayer->Worldpy <= 0) 
        m_pPlayer->Worldpy = 0.0f;
    //float ModWorldAngle;
    if (fWorldA >= 2 * PI)
    {
        fWorldA = 0;
    }
    FillCircle(ScreenWidth() / 10, ScreenHeight() / 2, 10, olc::BLACK);
    DrawCircle(ScreenWidth() / 10, ScreenHeight() / 2, 10, olc::BLUE);
    DrawLine(ScreenWidth() / 10, ScreenHeight() / 2,fabs( ScreenWidth() / 10 +  sinf(fWorldA) * -10), fabs(ScreenHeight() / 2 + cosf(fWorldA) * -10));
    
    if(INTRO == false)
        DrawPartialSprite(ScreenWidth() / 2, ScreenHeight() - 16, RPG_Assets::get().GetSprite("player"), 32 ,0, 16 ,16, 0.5);

    if (GetKey(olc::TAB).bReleased && INTRO != true)
    {
        m_nGameMode = MODE_LOCAL_MAP;
    }
    return true;
}

bool RPG_Engine::OnUserDestroy()
{
   // olc::SOUND::DestroyAudio();
    return true; // true for successful close
}
void RPG_Engine::Attack(cDynamic_Creature* aggressor, cWeapon* weapon)
{
    weapon->OnUse(aggressor);
}
void RPG_Engine::AddProjectile(cDynamic_Projectile* proj)
{
    m_vecProjectiles.push_back(proj);
}
void RPG_Engine::Damage(cDynamic_Projectile* projectile, cDynamic_Creature* victim)
{
    if (victim != nullptr)
    {
        victim->nHealth -= projectile->nDamage;
        if (victim->sName == "Magnus" && victim->nHealth > 0)
        {
           // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("PlayerHurt"));
             engine.PlayWaveform(RPG_Assets::get().GetSound("PlayerHurt"));
        }
        if (victim->sName == "Magnus" && victim->nHealth <= 0)
        {
           // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("DeadPlayer"));
            engine.PlayWaveform(RPG_Assets::get().GetSound("DeadPlayer"));
        }
        if (victim->sName == "Skelly" || victim->sName == "megaskelly")
        {
            int counter = 1;

            if (counter == 1)
            {
                //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("HurtSkelly1"));
                 engine.PlayWaveform(RPG_Assets::get().GetSound("HurtSkelly1"));
                counter += 1;
            }
            if (counter == 2)
            {
               // olc::SOUND::PlaySample(RPG_Assets::get().GetSound("HurtSkelly2"));
                 engine.PlayWaveform(RPG_Assets::get().GetSound("HurtSkelly2"));
                counter -= 1;
            }
        }
        if (victim->sName == "Slime")
        {
            //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Squish"));
            engine.PlayWaveform(RPG_Assets::get().GetSound("Squish"));
        }
        if (victim->sName == "orc")
        {
            //olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Pig"));
            engine.PlayWaveform(RPG_Assets::get().GetSound("Pig"));
        }
        float tx = victim->px - projectile->px;
        float ty = victim->py - projectile->py;
        float d = sqrtf(tx * tx + ty * ty);
        olc::vf2d victimpoc = { victim->px, victim->py };
        if (victim->Dead != true)
        {
            if (d < 1) d = 1.0f;
            victim->bIsAttackable = false;
            victim->KnockBack(tx / d, ty / d, 0.2f);
            victim->bIsAttackable = true;
            if (victim != m_pPlayer)
            {
                victim->OnInteract(m_pPlayer);
            }
            else
            {
                victim->bSolidVsDyn = true;
            }
        }
        else
        {
        }
        if (projectile->bOneHit)
            projectile->bRedundant = true;
    }
}

