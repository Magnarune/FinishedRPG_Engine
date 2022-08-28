#pragma once
#include "Dynamics.h"

class RPG_Engine;

class cSpell
{
public:
	cSpell(std::string name, olc::Sprite* sprite, std::string desc , int dmg, int ManaCost);
	virtual bool OnInteract(cDynamic* object) { return false; }
	virtual bool OnCast(cDynamic* object) { return false; }


public:
	std::string sName;
	std::string sDescription;
	olc::Sprite* pSprite;
	bool bKeyItem = false;
	bool bEquipable = false;
	int nManaCost = 0;
	int nDamage = 0;
	int nCooldown = 0;
	bool IsSpell = true;

	static RPG_Engine* g_engine;
};
class cSpell_FireBalls : public cSpell
{
public:
	cSpell_FireBalls();
	bool OnInteract(cDynamic* object) override;
	bool OnCast(cDynamic* object) override;
};
class cSpell_FireStorm : public cSpell
{
public:
	cSpell_FireStorm();
	bool OnInteract(cDynamic* object) override;
	bool OnCast(cDynamic* object) override;
};
class cSpell_FireStream : public cSpell
{
public:
	cSpell_FireStream();
	bool OnInteract(cDynamic* object) override;
	bool OnCast(cDynamic* object) override;
};
class cSpell_Blink : public cSpell
{
public:
	cSpell_Blink();
	bool OnInteract(cDynamic* object) override;
	bool OnCast(cDynamic* object) override;
};