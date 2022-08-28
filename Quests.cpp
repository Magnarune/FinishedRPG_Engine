#include "Quests.h"
#include "Engine.h"

cScriptProcessor* cQuest::g_script = nullptr;
RPG_Engine* cQuest::g_engine = nullptr;

#define X(n) g_script->AddCommand(new cCommand_ ## n)
int IQuesttick = 0;
cQuest::cQuest()
{
}

bool cQuest::PopulateDynamics(std::vector<cDynamic*>& vecDyns, std::string sMap)
{
	return true;
}

bool cQuest::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature)
{
	return true;
}
bool cQuest_MainQuest::PopulateDynamics(std::vector<cDynamic*>& vecDyns, std::string sMap)
{
	if (sMap == "Coder Town" && IQuesttick != 1 )
	{
		cDynamic_Creature* c1 = new cDynamic_Creature("Gavin", RPG_Assets::get().GetSprite("purple"));
		c1->px = 6.0f;
		c1->py = 4.0f;
		c1->bFriendly = true;
		vecDyns.push_back(c1);
	}
	if (sMap == "Home")
	{
		cDynamic_Creature* c1 = new cDynamic_Creature("Hunter", RPG_Assets::get().GetSprite("skelly"));
		c1->px = 12.0f;
		c1->py = 4.0f;
		vecDyns.push_back(c1);
	}
	if (sMap == "Cherry Wood")
	{
		cDynamic_Sign* s0 = new cDynamic_Swordshop();
		s0->bSolidVsMap = false;
		s0->px = (int)9;
		s0->py = (int)22;
		s0->bFriendly = true;
		vecDyns.push_back(s0);
		cDynamic_Creature* c0 = new cDynamic_Creature("Villager", RPG_Assets::get().GetSprite("purple"));
		c0->px = 2.0f;
		c0->py = 18.0f;
		c0->bFriendly = true;
		vecDyns.push_back(c0);
		cDynamic_Creature* c1 = new cDynamic_Creature("Villager", RPG_Assets::get().GetSprite("purple"));
		c1->px = 9.0f;
		c1->py = 15.0f;
		c1->bFriendly = true;
		vecDyns.push_back(c1);
		cDynamic_Creature* c2 = new cDynamic_Creature("Villager", RPG_Assets::get().GetSprite("purple"));
		c2->px = 10.0f;
		c2->py = 24.0f;
		c2->bFriendly = true;
		vecDyns.push_back(c2);
		cDynamic_Creature* c3 = new cDynamic_Creature("Villager", RPG_Assets::get().GetSprite("purple"));
		c3->px = 12.0f;
		c3->py = 31.0f;
		c3->bFriendly = true;
		vecDyns.push_back(c3);
		cDynamic_Creature* c4 = new cDynamic_Creature("Villager", RPG_Assets::get().GetSprite("purple"));
		c4->px = 11.0f;
		c4->py = 34.0f;
		c4->bFriendly = true;
		vecDyns.push_back(c4);
		cDynamic_Creature* c5 = new cDynamic_Creature("Villager", RPG_Assets::get().GetSprite("purple"));
		c5->px = 22.0f;
		c5->py = 28.0f;
		c5->bFriendly = true;
		vecDyns.push_back(c5);
		cDynamic_Creature* c6 = new cDynamic_Creature("Villager", RPG_Assets::get().GetSprite("purple"));
		c6->px = 18.0f;
		c6->py = 24.0f;
		c6->bFriendly = true;
		vecDyns.push_back(c6);
		cDynamic_Creature* c7 = new cDynamic_Creature("Villager", RPG_Assets::get().GetSprite("purple"));
		c7->px = 16.0f;
		c7->py = 24.0f;
		c7->bFriendly = true;
		vecDyns.push_back(c7);
		cDynamic_Creature* c8 = new cDynamic_Creature("Villager", RPG_Assets::get().GetSprite("purple"));
		c8->px = 20.0f;
		c8->py = 14.0f;
		c8->bFriendly = true;
		vecDyns.push_back(c8);
		cDynamic_Creature* c9 = new cDynamic_Creature("Villager", RPG_Assets::get().GetSprite("purple"));
		c9->px = 23.0f;
		c9->py = 9.0f;
		c9->bFriendly = true;
		vecDyns.push_back(c9);
		cDynamic_Creature* c10 = new cDynamic_Creature("Villager", RPG_Assets::get().GetSprite("purple"));
		c10->px = 28.0f;
		c10->py = 15.0f;
		c10->bFriendly = true;
		vecDyns.push_back(c10);
		cDynamic_Creature* c11 = new cDynamic_Creature("Villager", RPG_Assets::get().GetSprite("purple"));
		c11->px = 21.0f;
		c11->py = 45.0f;
		c11->bFriendly = true;
		vecDyns.push_back(c11);
		cDynamic_Creature* c12 = new cDynamic_Creature("Villager", RPG_Assets::get().GetSprite("purple"));
		c12->px = 21.0f;
		c12->py = 44.0f;
		c12->bFriendly = true;
		vecDyns.push_back(c12);
		cDynamic_Creature* c13 = new cDynamic_Creature("Villager", RPG_Assets::get().GetSprite("purple"));
		c13->px = 17.0f;
		c13->py = 26.0f;
		c13->bFriendly = true;
		vecDyns.push_back(c13);
		cDynamic_Creature* c14 = new cDynamic_Creature("Dark Lando", RPG_Assets::get().GetSprite("DarkHero"));
		c14->px = 16.0f;
		c14->py = 21.0f;
		c14->bFriendly = true;
		vecDyns.push_back(c14);
	}
	return true;
}

bool cQuest_MainQuest::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature)
{
	if (target->sName == "Gavin")
	{
		if (g_engine->HasItem(RPG_Assets::get().GetItem("Key")))
		{
			g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("hello"));
			X(ShowDialog({ "[Gav]", "Thanks, the Hunter sells many", " good wears!" }));
			IQuesttick = 1;
		}
		else
		{
			//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("hello"));
			g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("hello"));
			X(ShowDialog({ "[Gav]", "I lost the key..."}));
			X(ShowDialog({ "[Gav]", "Can you find it?" }));
		}
	}



	if (target->sName == "Hunter")
	{
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("HunterShop"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("HunterShop"));
		g_engine->ChangeMode(RPG_Engine::MODE_SHOP2);
	}
	if (target->sName == "Villager")
	{
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Whatisit"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("Whatisit"));
		if ((int)(rand() % 2) == 1)
		{
			X(ShowDialog({ "[Townsmen]", "Who are you?", "No one thats who!" }));
			X(ShowDialog({ "[Townsmen]", "Go away now!" }));
		}
		else
		{
			X(ShowDialog({ "[Townsmen]", "I heard", "the River in the South/East!" }));
			X(ShowDialog({ "[Townsmen]", "Is Cursed!!" }));
		}
	}
	if (target->sName == "Dark Lando")
	{		
		if (g_engine->HasItem(RPG_Assets::get().GetItem("Key")))
		{
			//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("NoRefund"));
			g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("NoRefund"));
			X(ShowDialog({ "[Dark Lisa]", "Hehe..No refunds" }));
		}
		else
		{
			//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("TheifShop"));
			g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("TheifShop"));
			g_engine->ChangeMode(RPG_Engine::MODE_SHOP);
		}
	}
	if (target->sName == "Sign")
	{
		X(ShowDialog({ "Sword Shop" }));
	}
	return false;
}
bool cQuest_BobsQuest::PopulateDynamics(std::vector<cDynamic*>& vecDyns, std::string sMap)
{
	return true;
}
bool cQuest_BobsQuest::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature)
{
	if (target->sName == "sarah")
	{
		X(ShowDialog({ "[Sarah]", "You are doing Bob's", "quest!" }));
		return true;
	}
	return false;
}
