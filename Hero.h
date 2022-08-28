/*
#pragma once
#include "Assets.h"
#include "olcPixelGameEngine.h"
#include "Dynamics.h"
class cDynamic;

class Hero : public cDynamic
{
	public:
		Hero(std::string n, olc::Sprite* sprite);
		~Hero();

	public:
		float px, py;
		float vx, vy;
		bool bSolidVsMap;
		bool bSolidVsDyn;
		bool bFriendly;
		bool bRedundant;
		bool bIsProjectile;
		bool bIsAttackable;
		bool Dead;
		bool Drops;
		std::string sName;
		//static RPG_Engine* g_engine;
	public:
		virtual void DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy) {}
		virtual void Update(float fElapsedTime, cDynamic* player = nullptr) {}
		virtual void OnInteract(cDynamic* player = nullptr) {}

		static RPG_Engine* g_engine;
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
	int nGold;
	int bControllable = true;
public:
	virtual void Behaviour(float fElapsedTime, cDynamic* player = nullptr);
	int GetFacingDirection() { return m_nFacingDirection; };
	virtual void PerformAttack() {};
	virtual void PerformSpell() {};
	void KnockBack(float dx, float dy, float dist);

protected:
	float m_fStateTick;
	float m_fKnockBackTimer = 0.0f;
	float m_fKnockBackDX = 0.0f;
	float m_fKnockBackDY = 0.0f;
	
};

class cHero_Magnus : public Hero
{
public:
	cHero_Magnus();

public:
	void PerformAttack() override;
	void PerformSpell() override;
	void Behaviour(float fElapsedTime, cDynamic* player = nullptr) override;


};
*/
