#pragma once

#include "olcPixelGameEngine.h"
#include "Assets.h"
#include "Dynamics.h"
#include "Commands.h"
#define X(n) m_script.AddCommand(new cCommand_ ## n)

class RPG_Engine;
class cMap
{
public:
	enum NATURE
	{
		TALK,
		WALK
	};
public:
	cMap();
	virtual ~cMap();
public:
	int nWidth;
	int nHeight;
	std::string sName;
	olc::Sprite* pSprite;
	
	int GetIndex(int x, int y);
	bool GetSolid(int x, int y);
	bool Create(std::string fileData, olc::Sprite* sprite, std::string name);
	virtual bool PopulateDynamics(std::vector<cDynamic*>& vecDyns)
	{
		return false;
	}
	virtual bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature)
	{
		return false;
	}
private:
	int* m_indices = nullptr;
	bool* m_solids = nullptr;
public:
	static cScriptProcessor* g_script;
	static RPG_Engine* g_engine;	
};
class cMap_Village1 : public cMap
{
public:
	cMap_Village1();
	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature) override;
};
class cMap_Home1 : public cMap
{
public:
	cMap_Home1();
	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature) override;

};
class cMap_River : public cMap
{
public:
	cMap_River();
	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature) override;

};
class cMap_Dungeon : public cMap
{
public:
	cMap_Dungeon();
	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature) override;
};
class cMap_Smallhouse : public cMap
{
public:
	cMap_Smallhouse();
	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature) override;
};
class cMap_Town : public cMap
{
public:
	cMap_Town();
	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature) override;
};

