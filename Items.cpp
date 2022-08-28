#include "Items.h"
#include "Engine.h"

RPG_Engine* cItem::g_engine = nullptr;

cItem::cItem(std::string name, olc::Sprite* sprite, std::string desc)
{
	sName = name; pSprite = sprite; sDescription = desc;
}
cItem_Health::cItem_Health() : cItem("Small Health", RPG_Assets::get().GetSprite("health"), "Restores 10 health")
{
}
bool cItem_Health::OnInteract(cDynamic* object)
{
	OnUse(object);
	return false; // Just absorb
}
bool cItem_Health::OnUse(cDynamic* object)
{
	if (object != nullptr)
	{
		cDynamic_Creature* dyn = (cDynamic_Creature*)object;
		dyn->nHealth = std::min(dyn->nHealth + 10, dyn->nHealthMax);
	}
	return true;
}
cItem_Coin::cItem_Coin() : cItem("5 gold", RPG_Assets::get().GetSprite("Coin"), "Gives 10 health")
{
}
bool cItem_Coin::OnInteract(cDynamic* object)
{
	OnUse(object);
	return false; // Just absorb
}
bool cItem_Coin::OnUse(cDynamic* object)
{
	//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("GOLD"));
	g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("GOLD"));
	if (object != nullptr)
	{
		cDynamic_Creature* dyn = (cDynamic_Creature*)object;
		dyn->nGold = dyn->nGold + 5;
	}
	return true;
}
cItem_HealthBoost::cItem_HealthBoost() :
	cItem("Health Boost", RPG_Assets::get().GetSprite("healthboost"), "Increases Max Health by 10")
{}
bool cItem_HealthBoost::OnInteract(cDynamic* object)
{
	
	//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("PotionPickup"));
	g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("PotionPickup"));
	return true; // Add to inventory
}
bool cItem_HealthBoost::OnUse(cDynamic* object)
{
	if (object != nullptr)
	{
		cDynamic_Creature* dyn = (cDynamic_Creature*)object;
		dyn->nHealthMax += 10;
		dyn->nHealth = dyn->nHealthMax;
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("PotionPickup"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("PotionPickup"));

	}
	return true; // Remove from inventory
}
cItem_ManaBoost::cItem_ManaBoost() : cItem("Mana Boost", RPG_Assets::get().GetSprite("Mana"), "Increases Max Mana by 10")
{}
bool cItem_ManaBoost::OnInteract(cDynamic* object)
{
	//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("PotionPickup"));
	g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("PotionPickup"));
	return true; // Add to inventory
}
bool cItem_ManaBoost::OnUse(cDynamic* object)
{
	if (object != nullptr)
	{
		cDynamic_Creature* dyn = (cDynamic_Creature*)object;
		dyn->nManaMax += 10;
		dyn->nMana = dyn->nManaMax;
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("PotionPickup"));
	g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("PotionPickup"));
	}
	return true; // Remove from inventory
}
cItem_Key::cItem_Key() : cItem("Key", RPG_Assets::get().GetSprite("healthboost"), "A 'Lost' Key")
{
	bKeyItem = true;

}
bool cItem_Key::OnInteract(cDynamic* object)
{
	//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Boom"));
	g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("Boom"));
	return true; // Add to inventory
}
bool cItem_Key::OnUse(cDynamic* object)
{
	return true; // Remove from inventory
}
cItem_RedKey::cItem_RedKey() : cItem("RedKey", RPG_Assets::get().GetSprite("healthboost"), "Opens Dungeon Door")
{
	bKeyItem = true;

}
bool cItem_RedKey::OnInteract(cDynamic* object)
{
	//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Boom"));
	g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("Boom"));
	return true; // Add to inventory
}
bool cItem_RedKey::OnUse(cDynamic* object)
{
	return true; // Remove from inventory
}
cWeapon::cWeapon(std::string name, olc::Sprite* sprite, std::string desc, int dmg) : cItem(name, sprite, desc)
{
	nDamage = dmg;
}
bool cWeapon::OnInteract(cDynamic* object)
{
	
	return false;
}
bool cWeapon::OnUse(cDynamic* object)
{
	return false;
}
cWeapon_Sword::cWeapon_Sword() :
	cWeapon("Basic Sword", RPG_Assets::get().GetSprite("Basic Sword"), "A wooden sword, 5 dmg", 5)
{
	
}
bool cWeapon_Sword::OnInteract(cDynamic* object)
{
;
	return true; // Add to inventory

}
bool cWeapon_Sword::OnUse(cDynamic* object)
{
	cDynamic_Creature* dyn = (cDynamic_Creature*)object;
	dyn->pEquipedWeapon = (cWeapon*)RPG_Assets::get().GetItem("Basic Sword");
	if (dyn->AttackCoolDown > 0)
	{
		return false;
	}
	dyn->AttackCoolDown = dyn->AttackCoolDown + 0.2; //ATTACK COOLDOWN
	//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("SwordAttack"));
	g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("SwordAttack"));
	// Get direction of attacker
	cDynamic_Creature* aggressor = (cDynamic_Creature*)object;
	// Determine attack origin
	float x, y, vx, vy;
	if (aggressor->GetFacingDirection() == 0) // South
	{
		x = aggressor->px;
		y = aggressor->py + 0.8f;
		vx = 0.0f; vy = 1.0f;
	}
	if (aggressor->GetFacingDirection() == 1) // East
	{
		x = aggressor->px - 0.7f;
		y = aggressor->py + 0.2;
		vx = -1.0f; vy = 0.0f;
	}
	if (aggressor->GetFacingDirection() == 2) // North
	{
		x = aggressor->px;
		y = aggressor->py - 0.8f;
		vx = 0.0f; vy = -1.0f;
	}
	if (aggressor->GetFacingDirection() == 3) // West
	{
		x = aggressor->px + 0.8f;
		y = aggressor->py + 0.2;
		vx = 1.0f; vy = 0.0f;
	}
	cDynamic_Projectile* p = new cDynamic_Projectile(x, y, aggressor->bFriendly, aggressor->vx, aggressor->vy, 0.1f, RPG_Assets::get().GetSprite("Basic Sword"), (aggressor->GetFacingDirection() + 3) % 4 + 1, 0.0f);
	p->bSolidVsMap = false;
	p->bSolidVsDyn = false;
	p->nDamage = 5;
	p->bOneHit = true;
	g_engine->AddProjectile(p);
	return true;//
}

cWeapon_FireSword::cWeapon_FireSword() :
	cWeapon("Fire Sword", RPG_Assets::get().GetSprite("Fire Sword"), "A Dev sword, 25 dmg", 25)
{

}
bool cWeapon_FireSword::OnInteract(cDynamic* object)
{
	
	return true;
	 // Equipe
}
bool cWeapon_FireSword::OnUse(cDynamic* object)
{
	cDynamic_Creature* dyn = (cDynamic_Creature*)object;
	dyn->pEquipedWeapon = (cWeapon*)RPG_Assets::get().GetItem("Fire Sword");
	if (dyn->AttackCoolDown > 0)
	{
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Tired"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("Tired"));
		return false;
	}
	dyn->AttackCoolDown = dyn->AttackCoolDown + 0.5;
	//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("FireBlade"));
	g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("FireBlade"));
	// Get direction of attacker
	cDynamic_Creature* aggressor = (cDynamic_Creature*)object;
	// Determine attack origin
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
	if (aggressor->nMana >= 2)
	{
		// Beam sword
		aggressor->nMana -= 2;
		cDynamic_Projectile* p = new cDynamic_Projectile(x, y, aggressor->bFriendly, vx * 15.0f, vy * 15.0f, 1.0f, RPG_Assets::get().GetSprite("Fire Sword"), (aggressor->GetFacingDirection() + 3) % 4 + 1, 1.0f);
		p->bSolidVsMap = true;
		p->bSolidVsDyn = false;
		p->nDamage = 25;
		p->bOneHit = true;
		g_engine->AddProjectile(p);
	}
	cDynamic_Projectile* p = new cDynamic_Projectile(x, y, aggressor->bFriendly, aggressor->vx, aggressor->vy, 0.1f, RPG_Assets::get().GetSprite("Fire Sword"), (aggressor->GetFacingDirection() + 3) % 4 + 1, 0.0f);
	p->bSolidVsMap = false;
	p->bSolidVsDyn = false;
	p->nDamage = 25;
	p->bOneHit = true;
	g_engine->AddProjectile(p);
	return true;
}
