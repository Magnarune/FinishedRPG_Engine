#include "Dynamics.h"
#include "Engine.h"
#include "olcPixelGameEngine.h"
#include "Maps.h"
#include "fstream"
#include <random>

RPG_Engine* cDynamic::g_engine = nullptr;

cDynamic::cDynamic(std::string n)
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
}
cDynamic::~cDynamic()
{
}

cDynamic_Creature::cDynamic_Creature(std::string name, olc::Sprite* sprite) : cDynamic(name)
{
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

void cDynamic_Creature::Update(float fElapsedTime, cDynamic* player)
{
	if (m_fKnockBackTimer > 0.0f)
	{
		vx = m_fKnockBackDX * 10.0f;
		vy = m_fKnockBackDY * 10.0f;
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

void cDynamic_Creature::KnockBack(float dx, float dy, float dist)
{
	m_fKnockBackDX = dx;
	m_fKnockBackDY = dy;
	m_fKnockBackTimer = dist;
	bSolidVsDyn = false;
	bControllable = false;
	bIsAttackable = false;
}

void cDynamic_Creature::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
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
	gfx->DrawPartialSprite((px - ox) * 16.0f, (py - oy) * 16.0f, m_pSprite, nSheetOffsetX, nSheetOffsetY, 16, 16);
}

void cDynamic_Creature::Behaviour(float fElapsedTime, cDynamic* player)
{


	m_fStateTick -= fElapsedTime;
	if (m_fStateTick <= 0.0f)
	{
		vx = float(rand() % 3 - 1) * 1.5f; //You made it to your Random spot
		vy = float(rand() % 3 - 1) * 1.5f;
		m_fStateTick += 3.0f; // Ponder your life before you repeat
	}

	return;

}


cDynamic_Creature_Magnus::cDynamic_Creature_Magnus() : cDynamic_Creature("Magnus", RPG_Assets::get().GetSprite("player"))
{
	bFriendly = true;
	AttackCoolDown = 0;
	SpellCoolDown = 0;
	nHealth = 10;
	nHealthMax = 10;
	nMana = 10;
	nManaMax = 10;
	Level = 1;
	XP = 0;
	nGold = 0;
	m_fStateTick = 2.0f;
	Dead = false;
	Drops = false;
}

void cDynamic_Creature_Magnus::PerformAttack()
{
	
	
		if (pEquipedWeapon == nullptr)
			return;
			pEquipedWeapon->OnUse(this);

}
void cDynamic_Creature_Magnus::PerformSpell()
{
	if (pEquipedSpell == nullptr)
		return;
	if (nMana <= 0)
	{
		return;
	}
	pEquipedSpell->OnCast(this);

}

void cDynamic_Creature_Magnus::Behaviour(float fElapsedTime, cDynamic* player)
{ 
}

cDynamic_Creature_Skelly::cDynamic_Creature_Skelly() : cDynamic_Creature("Skelly", RPG_Assets::get().GetSprite("skelly"))
{
	bFriendly = false;
	Dead = false;
	Drops = false;
	nHealth = 10;
	nHealthMax = 10;
	m_fStateTick = 2.0f;
	pEquipedWeapon = (cWeapon*)RPG_Assets::get().GetItem("Basic Sword");
}

void cDynamic_Creature_Skelly::Behaviour(float fElapsedTime, cDynamic* player)
{
	//Only if dead
	if (nHealth <= 0)
	{
		vx = 0;
		vy = 0;
		bSolidVsDyn = false;
		bIsAttackable = false;
		Dead = true;
		if (Drops != true)
		{
			Drops = true;
		}
		return;
	}
	float fTargetX = player->px - px;
	float fTargetY = player->py - py;
	float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);
	m_fStateTick -= fElapsedTime;
	if (m_fStateTick <= 0.0f)
	{


		if (fDistance < 6.0f)
		{
			vx = (fTargetX / fDistance) * 2.0f;
			vy = (fTargetY / fDistance) * 2.0f;
			if (fDistance < 1.5f)
				PerformAttack();
		}
		else
		{
			vx = (rand() % 3 - 1) * 1.5; //You made it to your Random spot
			vy = (rand() % 3 - 1) * 1.5;
			m_fStateTick += 2.0f;
		}
		m_fStateTick += 1.0f;
	}
}

void cDynamic_Creature_Skelly::PerformAttack()
{
	if (pEquipedWeapon == nullptr)
		return;
	pEquipedWeapon->OnUse(this);
}
void cDynamic_Creature_Skelly::Loot()
{
}

cDynamic_Creature_MegaSkelly::cDynamic_Creature_MegaSkelly() : cDynamic_Creature("megaskelly", RPG_Assets::get().GetSprite("MegaSkelly"))
{

	bFriendly = false;
	Dead = false;
	Drops = false;
	nHealth = 50;
	nHealthMax = 50;
	m_fStateTick = 2.0f;
	pEquipedWeapon = (cWeapon*)RPG_Assets::get().GetItem("Basic Sword");
}

void cDynamic_Creature_MegaSkelly::Behaviour(float fElapsedTime, cDynamic* player)
{
	if (nHealth <= 0)
	{
		vx = 0;
		vy = 0;
		bSolidVsDyn = false;
		bIsAttackable = false;
		Dead = true;
		return;
	}

	float fTargetX = player->px - px;
	float fTargetY = player->py - py;
	float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);
	m_fStateTick -= fElapsedTime;
	if (m_fStateTick <= 0.0f)
	{


		if (fDistance < 8.0f)
		{
			vx = (fTargetX / fDistance) * 3.0f;
			vy = (fTargetY / fDistance) * 3.0f;
			if (fDistance < 1.5f)
				PerformAttack();
		}
		else
		{
			vx = (rand() % 3 - 1) * 1.5; //You made it to your Random spot
			vy = (rand() % 3 - 1) * 1.5;
			m_fStateTick += 3.0f;

		}
		m_fStateTick += 1.0f;
	}


}
void cDynamic_Creature_MegaSkelly::PerformAttack()
{
	if (pEquipedWeapon == nullptr)
		return;

	pEquipedWeapon->OnUse(this);
	//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("SwordAttack"));
	g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("SwordAttack"));
}

cDynamic_Creature_orc::cDynamic_Creature_orc() : cDynamic_Creature("orc", RPG_Assets::get().GetSprite("orc"))
{
	bFriendly = false;
	Dead = false;
	Drops = false;
	nHealth = 500;
	nHealthMax = 550;
	m_fStateTick = 2.0f;
	pEquipedWeapon = (cWeapon*)RPG_Assets::get().GetItem("Basic Sword");
}

void cDynamic_Creature_orc::Behaviour(float fElapsedTime, cDynamic* player)
{
	if (nHealth <= 0)
	{
		vx = 0;
		vy = 0;
		bSolidVsDyn = false;
		bIsAttackable = false;
		Dead = true;
		return;
	}

	float fTargetX = player->px - px;
	float fTargetY = player->py - py;
	float fDistance = sqrtf(fTargetX * fTargetX + fTargetY * fTargetY);
	m_fStateTick -= fElapsedTime;
	if (m_fStateTick <= 0.0f)
	{


		if (fDistance < 5.0f)
		{
			vx = (fTargetX / fDistance) * 4.0f;
			vy = (fTargetY / fDistance) * 4.0f;
			if (fDistance < 1.5f)
				PerformAttack();
		}
		else
		{
			vx = 0;
			vy = 0;

		}
		m_fStateTick += 1.0f;
	}


}
void cDynamic_Creature_orc::PerformAttack()
{
	if (pEquipedWeapon == nullptr)
		return;
	pEquipedWeapon->OnUse(this);
}


cDynamic_Creature_Slime::cDynamic_Creature_Slime() : cDynamic_Creature("Slime", RPG_Assets::get().GetSprite("Slime"))
{
	Dead = false;
	Drops = false;
	bFriendly = false;
	nHealth = 5;
	nHealthMax = 5;
	m_fStateTick = 2.0f;

}

void cDynamic_Creature_Slime::Behaviour(float fElapsedTime, cDynamic* player)
{

	//Only if dead
	if (nHealth <= 0)
	{
		vx = 0;
		vy = 0;
		bSolidVsDyn = false;
		bIsAttackable = false;
		Dead = true;
		if (Drops != true)
		{
			Drops = true;
		}
		return;
	}
	m_fStateTick -= fElapsedTime;
	if (m_fStateTick <= 0.0f)
	{
		vx = (rand() % 3 - 1) * 1.5; //You made it to your Random spot
		vy = (rand() % 3 - 1) * 1.5;
		m_fStateTick += 3.0f; // Ponder your life before you repeat
	}
	return;
}
void cDynamic_Creature_Slime::Loot()
{

}




cDynamic_Teleport::cDynamic_Teleport(float x, float y, std::string mapName, float tx, float ty) : cDynamic("Teleport")
{
	px = x;
	py = y;
	fMapPosX = tx;
	fMapPosY = ty;
	sMapName = mapName;
	bSolidVsDyn = false;
	bSolidVsMap = false;
}

void cDynamic_Teleport::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
}

void cDynamic_Teleport::Update(float fElapsedTime, cDynamic* player)
{
}

cDynamic_Item::cDynamic_Item(float x, float y, cItem* i) : cDynamic("pickup")
{
	px = x;
	py = y;
	bSolidVsDyn = false;
	bSolidVsMap = false;
	bFriendly = true;
	bCollected = false;
	item = i;
}

void cDynamic_Item::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
	if (bCollected)
		return;

	gfx->DrawPartialSprite((px - ox) * 16.0f, (py - oy) * 16.0f, item->pSprite, 0, 0, 16, 16);
}

void cDynamic_Item::OnInteract(cDynamic* player)
{
	if (bCollected)
		return;

	if (item->OnInteract(player))
	{
		// Add item to inventory
		g_engine->GiveItem(item);
	}

	bCollected = true;
}



cDynamic_Projectile::cDynamic_Projectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, olc::Sprite* sprite, float tx, float ty) : cDynamic("projectile")
{
	pSprite = sprite;
	fSpriteX = tx;
	fSpriteY = ty;
	fDuration = duration;
	px = ox;
	py = oy;
	vx = velx;
	vy = vely;
	bSolidVsDyn = false;
	bSolidVsMap = true;
	bIsProjectile = true;
	bIsAttackable = false;
	bFriendly = bFriend;
}

void cDynamic_Projectile::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
	gfx->DrawPartialSprite((px - ox) * 16, (py - oy) * 16, pSprite, fSpriteX * 16, fSpriteY * 16, 16, 16);

}

void cDynamic_Projectile::Update(float fElapsedTime, cDynamic* player)
{
	fDuration -= fElapsedTime;
	if (fDuration <= 0.0f)
		bRedundant = true;
}

cDynamic_Sign::cDynamic_Sign(std::string name, olc::Sprite* sprite) : cDynamic(name)
{
	bSolidVsMap = false;
	m_pSprite = sprite;
	bIsAttackable = false;
}
void cDynamic_Sign::Behaviour(float fElapsedTime, cDynamic* player)
{

}

cDynamic_Swordshop::cDynamic_Swordshop() :
	cDynamic_Sign("Sign", RPG_Assets::get().GetSprite("orc"))
{

}

void cDynamic_Swordshop::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
}

void cDynamic_Swordshop::Update(float fElapsedTime, cDynamic* player)
{
}

void cDynamic_Swordshop::Behaviour(float fElapsedTime, cDynamic* player)
{
}


cDynamic_WORLD_Teleport::cDynamic_WORLD_Teleport(float x, float y, float Wx, float Wy) : cDynamic("WorldTeleport")
{
	bSolidVsDyn = false;
	bSolidVsMap = false;
	px = x;
	py = y;
	Worldpx = Wx;
	Worldpy = Wy;
}

void cDynamic_WORLD_Teleport::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{
	gfx->DrawCircle(((px + 0.5f) - ox) * 16.0f, ((py + 0.5f) - oy) * 16.0f, 0.5f * 16.0f);
}

void cDynamic_WORLD_Teleport::Update(float fElapsedTime, cDynamic* player)
{
}
