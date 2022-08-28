/*
#include "Dynamics.h"
#include "Hero.h"
#include "olcPixelGameEngine.h"
#include "Maps.h"
#include "fstream"
#include <random>

Hero::Hero(std::string n, olc::Sprite* sprite) : cDynamic( n)
{
	sName = n;
	px = 0.0f;
	py = 0.0f;
	vx = 0.0f;
	vy = 0.0f;
	bSolidVsMap = true;
	bSolidVsDyn = true;
	bFriendly = true;
	Dead = false;
	Drops = false;
	bRedundant = false;
	bIsAttackable = false;
	bIsProjectile = false;
	m_pSprite = sprite;
	m_pDecal = new olc::Decal(sprite);
	nHealth = 10;
	nHealthMax = 10;
	m_nFacingDirection = SOUTH;
	m_nGraphicState = STANDING;
	m_nGraphicCounter = 0;
	m_fTimer = 0.0f;
	Drops = false;
	Dead = false;
	bIsAttackable = true;
}

Hero::~Hero()
{
}
void Hero::Update(float fElapsedTime, cDynamic* player)
{
	if (m_fKnockBackTimer > 0.0f)
	{
		vx = m_fKnockBackDX * 10.0f;
		vy = m_fKnockBackDY * 10.0f;
		//DrawSelf((olc::PixelGameEngine*) ("Basic Sword"), player->px, player->py);
		bIsAttackable = false;
		m_fKnockBackTimer -= fElapsedTime;
		if (m_fKnockBackTimer <= 0.0f)
		{
			m_fStateTick = 0.0f;
			bControllable = true;
			bIsAttackable = true;
		}
	}
	else
	{
		bSolidVsDyn = true;
		m_fTimer += fElapsedTime;
		if (m_fTimer >= 0.2f)
		{
			m_fTimer -= 0.2f;
			m_nGraphicCounter++;
			m_nGraphicCounter %= 2;
		}

		if (fabs(vx) > 0 || fabs(vy) > 0)
			m_nGraphicState = WALKING;
		else
			m_nGraphicState = STANDING;

		if (nHealth <= 0)
			m_nGraphicState = DEAD;

		if (vx < -0.1f) m_nFacingDirection = WEST;
		if (vx > 0.1f) m_nFacingDirection = EAST;
		if (vy < -0.1f) m_nFacingDirection = NORTH;
		if (vy > 0.1f) m_nFacingDirection = SOUTH;

		Behaviour(fElapsedTime, player);
	}
}
void Hero::KnockBack(float dx, float dy, float dist)
{
	m_fKnockBackDX = dx;
	m_fKnockBackDY = dy;
	m_fKnockBackTimer = dist;
	bSolidVsDyn = false;
	bControllable = false;
	bIsAttackable = false;
}
void Hero::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
	olc::Decal* Demo = nullptr;
	int nSheetOffsetX = 0;
	int nSheetOffsetY = 0;

	switch (m_nGraphicState)
	{
	case STANDING:
		nSheetOffsetX = m_nFacingDirection * 16;
		break;

	case WALKING:
		nSheetOffsetX = m_nFacingDirection * 16;
		nSheetOffsetY = m_nGraphicCounter * 16;
		break;

	case CELEBRATING:
		nSheetOffsetX = 4 * 16;
		break;

	case DEAD:
		nSheetOffsetX = 4 * 16;
		nSheetOffsetY = 1 * 16;
		break;

	}
	//	Demo = new olc::Decal(m_pSprite);

		//gfx->DrawPartialDecal({ (px - ox) ,(py - oy) }, { 16,16 }, m_pDecal, { nSheetOffsetX, nSheetOffsetY });

	gfx->DrawPartialSprite((px - ox) * 16.0f, (py - oy) * 16.0f, m_pSprite, nSheetOffsetX, nSheetOffsetY, 16, 16);

}



cHero_Magnus::cHero_Magnus() : Hero("Magnus", RPG_Assets::get().GetSprite("player"))
{
	bFriendly = true;
	AttackCoolDown = 0;
	SpellCoolDown = 0;
	nHealth = 10;
	nHealthMax = 10;
	nMana = 10;
	nManaMax = 10;
	nGold = 0;
	m_fStateTick = 2.0f;
	Dead = false;
	Drops = false;
	//pEquipedWeapon = (cWeapon*)RPG_Assets::get().GetItem("Basic Sword");
	//pEquipedSpell = (cSpell*)RPG_Assets::get().GetSpell("Fire Ball");
	if (nHealth <= 0)
	{
		vx = 0;
		vy = 0;
		bSolidVsDyn = false;
		bIsAttackable = false;
		Dead = true;
		return;
	}
}
void cHero_Magnus::PerformAttack()
{
/*
	if (pEquipedWeapon == nullptr)
		return;

	pEquipedWeapon->OnUse(this);
	*/
//}
//void cHero_Magnus::PerformSpell()
//{
	/*
	if (pEquipedSpell == nullptr)
		return;
	if (nMana <= 0)
	{
		olc::SOUND::PlaySample(RPG_Assets::get().GetSound("ZeroMana"));
		return;
	}
	pEquipedSpell->OnCast(this);
	//nMana =nMana - 2;
	
}

void cHero_Magnus::Behaviour(float fElapsedTime, cDynamic* player)
{
}
*/