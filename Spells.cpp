#include "Spells.h"
#include "Engine.h"
#include "Maps.h"
RPG_Engine* cSpell::g_engine = nullptr;

cSpell::cSpell(std::string name, olc::Sprite* sprite, std::string desc, int dmg , int ManaCost)
{
	sName = name; pSprite = sprite; sDescription = desc; nDamage = dmg; nManaCost = ManaCost;
}
cSpell_FireBalls::cSpell_FireBalls() :cSpell("Fire Ball", RPG_Assets::get().GetSprite("Fire Sword"), "Fire Ball 40 dmg", 40, 2)
{
}
bool cSpell_FireBalls::OnInteract(cDynamic* object)
{
	return false;
}
bool cSpell_FireBalls::OnCast(cDynamic* object)
{	
	cDynamic_Creature* dyn = (cDynamic_Creature*)object;
	//int ManaCost = 2;
	if (dyn->nMana < nManaCost)
	{
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("NotEnoughMana"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("NotEnoughMana"));
		return false;
	}	
	if (dyn->SpellCoolDown > 0)
	{
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("SpellCooldown"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("SpellCooldown"));
		return false;
	}
	dyn->SpellCoolDown += 1;
	dyn->nMana = dyn->nMana - nManaCost;
	//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("SFireBall"));
	g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("SFireBall"));
	cDynamic_Creature* aggressor = (cDynamic_Creature*)object;
	float x, y, vx, vy;
	if (aggressor->GetFacingDirection() == 0) // South
	{
		x = aggressor->px;
		y = aggressor->py + 1.0f;
		vx = 0.0f; vy = 1.0f;
	}
	if (aggressor->GetFacingDirection() == 1) // East
	{
		x = aggressor->px - 1.0f;
		y = aggressor->py;
		vx = -1.0f; vy = 0.0f;
	}
	if (aggressor->GetFacingDirection() == 2) // North
	{
		x = aggressor->px;
		y = aggressor->py - 1.0f;
		vx = 0.0f; vy = -1.0f;
	}
	if (aggressor->GetFacingDirection() == 3) // West
	{
		x = aggressor->px + 1.0f;
		y = aggressor->py;
		vx = 1.0f; vy = 0.0f;
	}
	cDynamic_Projectile* p = new cDynamic_Projectile(x, y, aggressor->bFriendly, vx * 15.0f, vy * 15.0f, 1.0f, RPG_Assets::get().GetSprite("Fire Sword"), (aggressor->GetFacingDirection() + 3) % 4 + 1, 1.0f);
	p->bSolidVsMap = true;
	p->bSolidVsDyn = false;
	p->nDamage = 40;
	p->bOneHit = true;
	g_engine->AddProjectile(p);
	return false;
}

cSpell_FireStorm::cSpell_FireStorm() : cSpell(std::string("FireStorm"), RPG_Assets::get().GetSprite("Fire Sword"),"Lauches fire balls in all directions", 100, 100)
{
}

bool cSpell_FireStorm::OnInteract(cDynamic* object)
{
	return false;
}

bool cSpell_FireStorm::OnCast(cDynamic* object)
{
	cDynamic_Creature* dyn = (cDynamic_Creature*)object;
	if (dyn->nMana < nManaCost)
	{
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("NotEnoughMana"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("NotEnoughMana"));
		return false;
	}
	if (dyn->SpellCoolDown > 0)
	{
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("SpellCooldown"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("SpellCooldown"));
		return false;
	}
	dyn->SpellCoolDown += 12;
	dyn->nMana = dyn->nMana - nManaCost;
	//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Boom"));
	g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("Boom"));
	cDynamic_Creature* aggressor = (cDynamic_Creature*)object;
	float x, y;
	x = aggressor->px;
	y = aggressor->py;	
	cDynamic_Projectile* p0 = new cDynamic_Projectile(x, y, aggressor->bFriendly, 1 * 15.0f, 1 * 15.0f, 1.0f, RPG_Assets::get().GetSprite("Fire Sword"), (aggressor->GetFacingDirection() + 3) % 4 + 1, 1.0f);
	p0->bSolidVsMap = true;
	p0->bSolidVsDyn = false;
	p0->nDamage = 100;
	p0->bOneHit = true;
	g_engine->AddProjectile(p0);//Z--
	cDynamic_Projectile* p1 = new cDynamic_Projectile(x, y, aggressor->bFriendly, -1 * 15.0f, -1 * 15.0f, 1.0f, RPG_Assets::get().GetSprite("Fire Sword"), (aggressor->GetFacingDirection() + 3) % 4 + 1, 1.0f);
	p1->bSolidVsMap = true;
	p1->bSolidVsDyn = false;
	p1->nDamage = 100;
	p1->bOneHit = true;
	g_engine->AddProjectile(p1);//Z++
	cDynamic_Projectile* p2 = new cDynamic_Projectile(x, y, aggressor->bFriendly, 1 * 15.0f, -1 * 15.0f, 1.0f, RPG_Assets::get().GetSprite("Fire Sword"), (aggressor->GetFacingDirection() + 3) % 4 + 1, 1.0f);
	p2->bSolidVsMap = true;
	p2->bSolidVsDyn = false;
	p2->nDamage = 100;
	p2->bOneHit = true;
	g_engine->AddProjectile(p2);//Z+-
	cDynamic_Projectile* p3 = new cDynamic_Projectile(x, y, aggressor->bFriendly, -1 * 15.0f, 1 * 15.0f, 1.0f, RPG_Assets::get().GetSprite("Fire Sword"), (aggressor->GetFacingDirection() + 3) % 4 + 1, 1.0f);
	p3->bSolidVsMap = true;
	p3->bSolidVsDyn = false;
	p3->nDamage = 100;
	p3->bOneHit = true;
	g_engine->AddProjectile(p3);//Z-+
	cDynamic_Projectile* p4 = new cDynamic_Projectile(x, y, aggressor->bFriendly, -1 * 15.0f, 0 * 15.0f, 1.0f, RPG_Assets::get().GetSprite("Fire Sword"), (aggressor->GetFacingDirection() + 3) % 4 + 1, 1.0f);
	p4->bSolidVsMap = true;
	p4->bSolidVsDyn = false;
	p4->nDamage = 100;
	p4->bOneHit = true;
	g_engine->AddProjectile(p4);//X
	cDynamic_Projectile* p5 = new cDynamic_Projectile(x, y, aggressor->bFriendly, 1 * 15.0f, 0 * 15.0f, 1.0f, RPG_Assets::get().GetSprite("Fire Sword"), (aggressor->GetFacingDirection() + 3) % 4 + 1, 1.0f);
	p5->bSolidVsMap = true;
	p5->bSolidVsDyn = false;
	p5->nDamage = 100;
	p5->bOneHit = true;
	g_engine->AddProjectile(p5);//-X
	cDynamic_Projectile* p6 = new cDynamic_Projectile(x, y, aggressor->bFriendly, 0 * 15.0f, 1 * 15.0f, 1.0f, RPG_Assets::get().GetSprite("Fire Sword"), (aggressor->GetFacingDirection() + 3) % 4 + 1, 1.0f);
	p6->bSolidVsMap = true;
	p6->bSolidVsDyn = false;
	p6->nDamage = 100;
	p6->bOneHit = true;
	g_engine->AddProjectile(p6);//Y
	cDynamic_Projectile* p7 = new cDynamic_Projectile(x, y, aggressor->bFriendly, 0 * 15.0f, -1 * 15.0f, 1.0f, RPG_Assets::get().GetSprite("Fire Sword"), (aggressor->GetFacingDirection() + 3) % 4 + 1, 1.0f);
	p7->bSolidVsMap = true;
	p7->bSolidVsDyn = false;
	p7->nDamage = 100;
	p7->bOneHit = true;
	g_engine->AddProjectile(p7);//-Y
	return false;
}

cSpell_FireStream::cSpell_FireStream() : cSpell(std::string("FireStream"), RPG_Assets::get().GetSprite("Fire Sword"), "Lauches fire balls in a direction", 5, 75)
{
}

bool cSpell_FireStream::OnInteract(cDynamic* object)
{
	return false;
}

bool cSpell_FireStream::OnCast(cDynamic* object)
{
	cDynamic_Creature* dyn = (cDynamic_Creature*)object;
	if (dyn->nMana < nManaCost)
	{
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("NotEnoughMana"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("NotEnoughMana"));
		return false;
	}
	if (dyn->SpellCoolDown > 0)
	{
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("SpellCooldown"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("SpellCooldown"));
		return false;
	}
	dyn->SpellCoolDown += 12;
	dyn->nMana = dyn->nMana - nManaCost;
	//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Boom"));
	g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("Boom"));
	cDynamic_Creature* aggressor = (cDynamic_Creature*)object;
	float x, y, vx,vy;
	x = aggressor->px;
	y = aggressor->py;
	if (aggressor->GetFacingDirection() == 0) // South
	{
		x = aggressor->px;
		y = aggressor->py + 1.0f;
		vx = 0.0f; vy = 1.0f;
		aggressor->vx += vx;
		aggressor->vy += vy;
	}
	if (aggressor->GetFacingDirection() == 1) // East
	{
		x = aggressor->px - 1.0f;
		y = aggressor->py;
		vx = -1.0f; vy = 0.0f;
		aggressor->vx += vx;
		aggressor->vy += vy;
	}
	if (aggressor->GetFacingDirection() == 2) // North
	{
		x = aggressor->px;
		y = aggressor->py - 1.0f;
		vx = 0.0f; vy = -1.0f;
		aggressor->vx += vx;
		aggressor->vy += vy;
	}
	if (aggressor->GetFacingDirection() == 3) // West
	{
		x = aggressor->px + 1.0f;
		y = aggressor->py;
		vx = 1.0f; vy = 0.0f;
		aggressor->vx += vx;
		aggressor->vy += vy;
	}	
	for (int i = 5; i < 13; i++)
	{
		cDynamic_Projectile* p = new cDynamic_Projectile(x, y, aggressor->bFriendly, vx * 1.5f*i, vy * 1.5f*i, 1.0f, RPG_Assets::get().GetSprite("Fire Sword"), (aggressor->GetFacingDirection() + 3) % 4 + 1, 1.0f);
		p->bSolidVsMap = true;
		p->bSolidVsDyn = false;
		p->nDamage = 40;
		p->bOneHit = true;
		g_engine->AddProjectile(p);
	}
	
	return false;
}
cSpell_Blink::cSpell_Blink() : cSpell(std::string("Blink"), RPG_Assets::get().GetSprite("Blinkspr"), "Blinks 3 units", 1, 75)
{
}

bool cSpell_Blink::OnInteract(cDynamic* object)
{
	return false;
}
bool cSpell_Blink::OnCast(cDynamic* object)
{
	cDynamic_Creature* dyn = (cDynamic_Creature*)object;
	//int ManaCost = 100;
	if (dyn->nMana < nManaCost)
	{
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("NotEnoughMana"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("NotEnoughMana"));
		return false;
	}

	if (dyn->SpellCoolDown > 0)
	{
	//	olc::SOUND::PlaySample(RPG_Assets::get().GetSound("SpellCooldown"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("SpellCooldown"));
		return false;
	}
	dyn->SpellCoolDown += 3;
	dyn->nMana = dyn->nMana - nManaCost;
	cDynamic_Creature* aggressor = (cDynamic_Creature*)object;
	float x, y;
	x = aggressor->px;
	y = aggressor->py;
	
	if (aggressor->px <= 3 || aggressor->py <= 3)
	{
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("NotEnoughMana"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("NotEnoughMana"));
		return false;
	}
	if (aggressor->GetFacingDirection() == 0) // South
	{
		x = aggressor->px;
		y = aggressor->py + 3.0f;
	}
	if (aggressor->GetFacingDirection() == 1) // East
	{
		x = aggressor->px - 3.0f;
		y = aggressor->py;
	}
	if (aggressor->GetFacingDirection() == 2) // North
	{
		x = aggressor->px;
		y = aggressor->py - 3.0f;
	}
	if (aggressor->GetFacingDirection() == 3) // West
	{
		x = aggressor->px + 3.0f;
		y = aggressor->py;
	}
	//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("BlinkSound"));
	g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("BlinkSound"));
	aggressor->px = x;
	aggressor->py = y;
	return false;
}
