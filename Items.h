#pragma once
#include "Dynamics.h"

class RPG_Engine;

class cItem
{
public:
	cItem(std::string name, olc::Sprite* sprite, std::string desc);
	virtual bool OnInteract(cDynamic* object) { return false; }
	virtual bool OnUse(cDynamic* object) { return false; }
public:
	std::string sName;
	std::string sDescription;
	olc::Sprite* pSprite;
	bool bKeyItem = false;
	bool bEquipable = false;
	bool bWeapon = false;

	static RPG_Engine* g_engine;
};
class cItem_Health : public cItem // give player 10hp
{
public:
	cItem_Health();
	bool OnInteract(cDynamic* object) override;
	bool OnUse(cDynamic* object) override;
};
class cItem_Coin : public cItem // give player 10hp
{
public:
	cItem_Coin();
	bool OnInteract(cDynamic* object) override;
	bool OnUse(cDynamic* object) override;
};


class cItem_HealthBoost : public cItem // raise max hp 10
{
public:
	cItem_HealthBoost();
	bool OnInteract(cDynamic* object) override;
	bool OnUse(cDynamic* object) override;
};
class cItem_ManaBoost : public cItem // raise max hp 10
{
public:
	cItem_ManaBoost();
	bool OnInteract(cDynamic* object) override;
	bool OnUse(cDynamic* object) override;
};
class cItem_Key : public cItem
{
public:
	cItem_Key();
	bool OnInteract(cDynamic* object) override;
	bool OnUse(cDynamic* object) override;
};
class cItem_RedKey : public cItem
{
public:
	cItem_RedKey();
	bool OnInteract(cDynamic* object) override;
	bool OnUse(cDynamic* object) override;
};

class cWeapon : public cItem
{
public:
	cWeapon(std::string name, olc::Sprite* sprite, std::string desc, int dmg);
	bool OnInteract(cDynamic* object) override;
	bool OnUse(cDynamic* object) override;
	

public:
	
	int nDamage = 0;
	int nCooldown = 0;
	bool IsWeapon = true;
};

class cWeapon_Sword : public cWeapon
{
public:
	cWeapon_Sword();
public:
	bool OnInteract(cDynamic* object);
	bool OnUse(cDynamic* object) override;
};

class cWeapon_FireSword : public cWeapon
{
public:
	cWeapon_FireSword();
public:
	bool OnInteract(cDynamic* object);
	bool OnUse(cDynamic* object) override;
};


