#pragma once
#include "Assets.h"
#include "olcPixelGameEngine.h"

class RPG_Engine;
class cItem;
class cWeapon;
class cSpell;
class cDynamic
{
public:
	cDynamic(std::string n);
	virtual ~cDynamic();

public:
	float px, py;
	float vx, vy;
	float Worldpx, Worldpy;
	bool bSolidVsMap;
	bool bSolidVsDyn;
	bool bFriendly;
	bool bRedundant;
	bool bIsProjectile;
	bool bIsAttackable;
	bool Dead;
	bool Drops;
	std::string sName;
public:
	virtual void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) {}
	virtual void Update(float fElapsedTime, cDynamic* player = nullptr) {}
	virtual void OnInteract(cDynamic* player = nullptr) {}
	static RPG_Engine* g_engine;
};
class cDynamic_Creature : public cDynamic
{

public:
	cDynamic_Creature(std::string n, olc::Sprite* sprite);

protected:
	olc::Sprite* m_pSprite;
	olc::Decal* m_pDecal;
	float m_fTimer;
	int m_nGraphicCounter;
	enum { SOUTH = 0, WEST = 1, NORTH = 2, EAST = 3 } m_nFacingDirection;
	enum { STANDING, WALKING, CELEBRATING, DEAD } m_nGraphicState;

public:
	int nHealth;
	int nHealthMax;
	float AttackCoolDown;
	float SpellCoolDown;
	int nMana;
	int nManaMax;
	int Level;
	float XP;
	int nGold;
	int bControllable = true;
public:
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void Update(float fElapsedTime, cDynamic* player = nullptr) override;
	virtual void Behaviour(float fElapsedTime, cDynamic* player = nullptr);
	int GetFacingDirection() { return m_nFacingDirection; };
	virtual void PerformAttack() {};
	virtual void PerformSpell() {};
	void KnockBack(float dx, float dy, float dist);


	cWeapon* pEquipedWeapon = nullptr;
	cSpell* pEquipedSpell = nullptr;

protected:
	float m_fStateTick;
	float m_fKnockBackTimer = 0.0f;
	float m_fKnockBackDX = 0.0f;
	float m_fKnockBackDY = 0.0f;
};
class cDynamic_Creature_Skelly : public cDynamic_Creature
{
public:
	cDynamic_Creature_Skelly();

	void Behaviour(float fElapsedTime, cDynamic* player = nullptr) override;
	void PerformAttack() override;
	void Loot();
};
class cDynamic_Creature_Slime : public cDynamic_Creature
{
public:
	cDynamic_Creature_Slime();
	void Loot();
	void Behaviour(float fElapsedTime, cDynamic* player = nullptr) override;

};
class cDynamic_Creature_MegaSkelly : public cDynamic_Creature
{
public:
	cDynamic_Creature_MegaSkelly();

	void Behaviour(float fElapsedTime, cDynamic* player = nullptr) override;
	void PerformAttack() override;
};
class cDynamic_Creature_orc : public cDynamic_Creature
{
public:
	cDynamic_Creature_orc();

	void Behaviour(float fElapsedTime, cDynamic* player = nullptr) override;
	void PerformAttack() override;
};



class cDynamic_Creature_Magnus : public cDynamic_Creature
{
public:
	cDynamic_Creature_Magnus();

public:
	void PerformAttack() override;
	void PerformSpell() override;
	void Behaviour(float fElapsedTime, cDynamic* player = nullptr) override;


};

class cDynamic_Teleport : public cDynamic
{
public:
	cDynamic_Teleport(float x, float y, std::string sMapName, float tx, float ty);
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void Update(float fElapsedTime, cDynamic* player = nullptr) override;
public:
	std::string sMapName;
	float fMapPosX;
	float fMapPosY;
};


class cDynamic_WORLD_Teleport : public cDynamic
{
public:
	cDynamic_WORLD_Teleport(float x, float y, float Wx, float Wy);
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void Update(float fElapsedTime, cDynamic* player = nullptr) override;
public:
	float WorldMapPosX;
	float WorldMapPosY;
};





class cDynamic_Item : public cDynamic
{
public:
	cDynamic_Item(float x, float y, cItem* item);
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void OnInteract(cDynamic* player = nullptr) override;

public:
	cItem* item;
	bool bCollected = false;
};

class cDynamic_Projectile : public cDynamic
{
public:
	cDynamic_Projectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, olc::Sprite* sprite, float tx, float ty);
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void Update(float fElapsedTime, cDynamic* player = nullptr) override;

public:
	olc::Sprite* pSprite = nullptr;
	float fSpriteX;
	float fSpriteY;
	float fDuration;
	bool bOneHit = true;
	int nDamage = 0;
};

class cDynamic_Sign : public cDynamic
{

public:
	cDynamic_Sign(std::string name, olc::Sprite* sprite);
	virtual void Behaviour(float fElapsedTime, cDynamic* player = nullptr);
protected:
	olc::Sprite* m_pSprite;
};
class cDynamic_Swordshop : public cDynamic_Sign
{
public:
	cDynamic_Swordshop();
	void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) override;
	void Update(float fElapsedTime, cDynamic* player = nullptr) override;
	void Behaviour(float fElapsedTime, cDynamic* player = nullptr) override;
};
