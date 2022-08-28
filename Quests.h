#pragma once

#include "Dynamics.h"
#include "Commands.h"

class RPG_Engine;

class cQuest
{
public:
	enum NATURE
	{
		TALK = 0,
		ATTACK = 1,
		KILL = 2,
		WALK = 3
	};
public:
	cQuest();

public:
	virtual bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature);
	virtual bool PopulateDynamics(std::vector<cDynamic*>& vecDyns, std::string sMap);
public:
	std::string sName;
	bool bCompleted = false;
	static cScriptProcessor* g_script;
	static RPG_Engine* g_engine;
};


class cQuest_MainQuest : public cQuest
{
public:
	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns, std::string sMap) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature) override;

private:
	int m_nPhase = 0;
};

class cQuest_BobsQuest : public cQuest
{
public:
	bool PopulateDynamics(std::vector<cDynamic*>& vecDyns, std::string sMap) override;
	bool OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature) override;

private:
	int m_nPhase = 0;
};


