#pragma once

#include "olcPixelGameEngine.h"
#include "olcSoundWaveEngine.h"
//#include "olcPGEX_Sound.h"
#include <map>

class cMap;
class cItem;
class cSpell;

class RPG_Assets
{
public:
	olc::sound::Wave sample1;

	static RPG_Assets& get()
	{
		static RPG_Assets me;
		return me;
	}
	RPG_Assets(RPG_Assets const&) = delete;
	void operator=(RPG_Assets const&) = delete;
	olc::Sprite* GetSprite(std::string name)
	{
		return m_mapSprites[name];
	}
	cMap* GetMap(std::string name)
	{
		return m_mapMaps[name];
	}
	cItem* GetItem(std::string name)
	{
		return m_mapItems[name];
	}
	cSpell* GetSpell(std::string name)
	{
		return m_mapSpells[name];
	}
	
	olc::sound::Wave* GetSound(std::string name)
	{
		return m_mapSounds[name];
	}
	
	void LoadSprites();
	void LoadMaps();
	void LoadSounds();
	void LoadItems();
	void LoadSpells();
private:
	RPG_Assets();
	~RPG_Assets();//0.0
	std::map<std::string, olc::Sprite*> m_mapSprites;
	std::map<std::string, olc::sound::Wave*> m_mapSounds;
	std::map<std::string, cMap*> m_mapMaps;
	std::map<std::string, cItem*> m_mapItems;
	std::map<std::string, cSpell*> m_mapSpells;
};