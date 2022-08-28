#include "Assets.h"
#include "Maps.h"
#include "Items.h"
#include "Spells.h"
#include "Hero.h"



RPG_Assets::RPG_Assets()
{
	//s_engine;
}
RPG_Assets::~RPG_Assets()
{
	for(auto& [_,p] : m_mapSprites) delete p;//not yet p and _
	for(auto& [_,p] : m_mapMaps) delete p;
	for(auto& [_,p] : m_mapItems) delete p;
	for(auto& [_,p] : m_mapSpells) delete p;
	m_mapSprites.clear();
	m_mapMaps.clear();
	m_mapItems.clear();
	m_mapSpells.clear();
}

void RPG_Assets::LoadSounds()
{
	
	auto load = [&](std::string sName, std::string sFileName)
	{
		olc::sound::Wave* B = new olc::sound::Wave(sFileName);
		

		m_mapSounds[sName] = B;
	};

	load("Intro", "assets/rpgdata/Sounds/Intro.wav");
	load("SwordAttack", "assets/rpgdata/Sounds/AttackSwipe.wav");
	load("PlayerHurt",  "assets/rpgdata/Sounds/Player_hurt.wav");
	load("DeadPlayer",  "assets/rpgdata/Sounds/PlayerDead.wav");
	load("HurtSkelly1", "assets/rpgdata/Sounds/SkellyH1.wav");
	load("HurtSkelly2", "assets/rpgdata/Sounds/SkellyH2.wav");
	load("Squish",		"assets/rpgdata/Sounds/Squish.wav");
	load("Whatisit",	"assets/rpgdata/Sounds/Whatisit.wav");
	load("open_backpack", "assets/rpgdata/Sounds/Bagopen.wav");
	load("select_backpack", "assets/rpgdata/Sounds/BagSelect.wav");
	load("menu click", "assets/rpgdata/Sounds/Tok.wav");
	load("Pig", "assets/rpgdata/Sounds/Grunt.wav");
	load("hello", "assets/rpgdata/Sounds/Hello.wav");
	load("Thankyou", "assets/rpgdata/Sounds/thanks.wav");
	load("Walk", "assets/rpgdata/Sounds/footsteps.wav");
	load("Dooropen", "assets/rpgdata/Sounds/dooropen.wav");
	load("FireBlade", "assets/rpgdata/Sounds/Fireblade.wav");
	load("SFireBall", "assets/rpgdata/Sounds/Fireball.wav");
	load("ZeroMana", "assets/rpgdata/Sounds/No_Mana.wav");
	load("NotEnoughMana", "assets/rpgdata/Sounds/NotEnoughMana.wav");
	load("PotionPickup", "assets/rpgdata/Sounds/Potion.wav");
	load("Walkgrass", "assets/rpgdata/Sounds/Walking.wav");
	load("Boom", "assets/rpgdata/Sounds/TheBoom.wav");
	load("SpellCooldown", "assets/rpgdata/Sounds/Spellcooldown.wav");
	load("Tired", "assets/rpgdata/Sounds/AttackCooldown.wav");
	load("GOLD", "assets/rpgdata/Sounds/Gold.wav");
	load("LevelUp", "assets/rpgdata/Sounds/LevelUp.wav");
	load("TheifShop", "assets/rpgdata/Sounds/TheifShop.wav");
	load("NoRefund", "assets/rpgdata/Sounds/NoRefund.wav");
	load("Fanfare", "assets/rpgdata/Sounds/FanFare.wav");
	load("Magic Equip", "assets/rpgdata/Sounds/MagEquip.wav");
	load("HunterShop", "assets/rpgdata/Sounds/ShopHunter.wav");
	load("BlinkSound", "assets/rpgdata/Sounds/BlinkSound.wav");
	load("Ahhh", "assets/rpgdata/Sounds/ScreamLink.wav");
	load("Thud", "assets/rpgdata/Sounds/Thud.wav");
	load("Lighting", "assets/rpgdata/Sounds/Thunder.wav");
}

void RPG_Assets::LoadSprites()
{
	auto load = [&](std::string sName, std::string sFileName)
	{
		olc::Sprite* s = new olc::Sprite(sFileName);
		m_mapSprites[sName] = s;
	};

	load("village", "assets/rpgdata/gfx/spritesheetdark.spr.png");//backround
	load("hitech",  "assets/rpgdata/gfx/tileset_modernish.png"); //moderish
	load("font",	"assets/rpgdata/gfx/nesfont8x8.spr.edit.rbg.png");//font
	load("Creep",   "assets/rpgdata/gfx/NewCreepdisign.png");
	load("skelly",  "assets/rpgdata/gfx/skelly.PNG"); //skelly
	load("player",  "assets/rpgdata/gfx/CharacterSprites.spr.rbg.png");
	load("worldmap","assets/rpgdata/gfx/worldmap1.png");
	load("skymap",  "assets/rpgdata/gfx/sky1.png");
	load("title",   "assets/rpgdata/gfx/title3.png");
	load("balloon", "assets/rpgdata/gfx/balloon1.png");
	load("sword",   "assets/rpgdata/gfx/Sword.spr");
	load("MegaSkelly", "assets/rpgdata/gfx/Redskelly.png");
	load("purple", "assets/rpgdata/gfx/purple.spr.rbg.png");
	load("Porc", "assets/rpgdata/gfx/Orc.png");
	load("Slime", "assets/rpgdata/gfx/Sedit.png");
	load("Zelda", "assets/rpgdata/gfx/Loz.png");
	load("health", "assets/rpgdata/gfx/item_health.spr.rbg.png");// heart
	load("healthboost", "assets/rpgdata/gfx/sprites_items.png");//boost
	load("Fire Sword", "assets/rpgdata/gfx/Fire_Sword.png");
	load("Basic Sword", "assets/rpgdata/gfx/weapon_basic_sword.spr.rbg.png"); //Sword
	load("Sword Model Left", "assets/rpgdata/gfx/Sword.spr.png");
	load("Sword Model Right", "assets/rpgdata/gfx/Sword_Right.spr.png");
	load("orc", "assets/rpgdata/gfx/Zelda_orc.png");
	load("Coin", "assets/rpgdata/gfx/Coin5.png");
	load("DarkHero", "assets/rpgdata/gfx/heroDark.PNG");
	load("Mana", "assets/rpgdata/gfx/Mana&key.png");
	load("fireballspr", "assets/rpgdata/gfx/Fireballspr.png");
	load("Blinkspr", "assets/rpgdata/gfx/Blink.png");
}
void RPG_Assets::LoadMaps()
{
	auto load = [&](cMap* m)
	{
		m_mapMaps[m->sName] = m;
	};
	load(new cMap_Village1());
	load(new cMap_Home1());
	load(new cMap_River());
	load(new cMap_Dungeon());
	load(new cMap_Smallhouse());
	load(new cMap_Town());
}
void RPG_Assets::LoadItems()
{
	auto load = [&](cItem* i)
	{
		m_mapItems[i->sName] = i;
	};
	load(new cItem_Health());
	load(new cItem_Coin());
	load(new cItem_HealthBoost());
	load(new cItem_ManaBoost());
	load(new cItem_Key());
	load(new cItem_RedKey());	
	load(new cWeapon_Sword());
	load(new cWeapon_FireSword());
}
void RPG_Assets::LoadSpells()
{
	auto load = [&](cSpell* i)
	{
		m_mapSpells[i->sName] = i;
	};
	load(new cSpell_FireBalls());
	load(new cSpell_FireStorm());
	load(new cSpell_FireStream());
	load(new cSpell_Blink());
}