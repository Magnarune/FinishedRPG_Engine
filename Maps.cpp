#include "Maps.h"
#include "Engine.h"
#include <fstream>
RPG_Engine* cMap::g_engine = nullptr;

cScriptProcessor* cMap::g_script = nullptr;
#define X(n) g_script->AddCommand(new cCommand_ ## n)

cMap::cMap()
{
	
	pSprite = nullptr;
	nWidth = 0;
	nHeight = 0;
	m_solids = nullptr;
	m_indices = nullptr;
}


cMap::~cMap()
{
	delete[] m_solids;
	delete[] m_indices;
}

int cMap::GetIndex(int x, int y)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		return m_indices[y * nWidth + x];
	else
		return 0;
}

bool cMap::GetSolid(int x, int y)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		return m_solids[y * nWidth + x];
	else
		return true;
}

bool cMap::Create(std::string fileData, olc::Sprite* sprite, std::string name)
{
	sName = name;
	pSprite = sprite;
	std::ifstream data(fileData, std::ios::in | std::ios::binary);
	if (data.is_open())
	{
		data >> nWidth >> nHeight;
		m_solids = new bool[nWidth * nHeight];
		m_indices = new int[nWidth * nHeight];
		for (int i = 0; i < nWidth * nHeight; i++)
		{
			data >> m_indices[i];
			data >> m_solids[i];
		}
		return true;
	}

	return false;
}



cMap_Village1::cMap_Village1()
{
	Create("assets/rpgdata/map/village.lvl", RPG_Assets::get().GetSprite("village"), "Coder Town");
}

bool cMap_Village1::PopulateDynamics(std::vector<cDynamic*>& vecDyns)
{
	vecDyns.push_back(new cDynamic_Teleport(12.0f, 6.0f, "Home", 5.0f, 12.0f));
	vecDyns.push_back(new cDynamic_Teleport(63.0f, 30.0f, "River", 2.0f, 5.0f));
	vecDyns.push_back(new cDynamic_Teleport(63.0f, 29.0f, "River", 2.0f, 6.0f));
	//vecDyns.push_back(new cDynamic_Teleport(36.0f, 26.0f, "House", 21.0f, 21.0f));
	//vecDyns.push_back(new cDynamic_Teleport(8.0f, 26.0f, "Creepy House", 2.0f, 9.0f));
	for (int i = 0; i < 3; i++)
	{
		cDynamic* g1 = new cDynamic_Creature_Skelly();
		vecDyns.push_back(g1);
		g1->px = rand() % 10 + 15.0f;
		g1->py = rand() % 10 + 20.0f;
	}
	for (int i = 0; i < 10; i++)
	{
		cDynamic* g2 = new cDynamic_Creature_Skelly();
		vecDyns.push_back(g2);
		g2->px = rand() % 15 + 29;
		g2->py = rand() % 10 + 19.0f;
	}
	for (int i = 0; i < 10; i++)
	{
		cDynamic* g2 = new cDynamic_Creature_Slime();
		vecDyns.push_back(g2);
		g2->px = rand() % 10 + 5;
		g2->py = rand() % 10 + 5;
	}
	for (int i = 0; i < 20; i++)
	{
		cDynamic* g2 = new cDynamic_Creature_Slime();
		vecDyns.push_back(g2);
		g2->px = rand() % 15 + 30;
		g2->py = rand() % 15 + 5;
	}
	cDynamic_Creature* c1 = new cDynamic_Creature_MegaSkelly();
	c1->px = 35.0f;
	c1->py = 16.0f;
	vecDyns.push_back(c1);
	cDynamic_Creature* c2 = new cDynamic_Creature_orc();
	c2->px = 32.0f;
	c2->py = 11.0f;
	vecDyns.push_back(c2);
	return true;

}
bool cMap_Village1::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature)
{
	if (target->sName == "Teleport")
	{
		if (target->px == 12.0f && target->py == 6.0f && g_engine->HasItem(RPG_Assets::get().GetItem("Key")))
		{
			//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Dooropen"));
			g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("Dooropen"));
			g_script->AddCommand(new cCommand_ChangeMap(
				((cDynamic_Teleport*)target)->sMapName,
				((cDynamic_Teleport*)target)->fMapPosX,
				((cDynamic_Teleport*)target)->fMapPosY));
		}
	}
	if(target->sName == "Teleport" && target->px != 12.0f && target->py != 6.0f)
	{
		{
			//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Walk"));
			g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("Walk"));
			g_script->AddCommand(new cCommand_ChangeMap(
				((cDynamic_Teleport*)target)->sMapName,
				((cDynamic_Teleport*)target)->fMapPosX,
				((cDynamic_Teleport*)target)->fMapPosY));
		}
	}
	return false;
}






cMap_Home1::cMap_Home1()
{
	Create("assets/rpgdata/map/home.lvl", RPG_Assets::get().GetSprite("hitech"), "Home");
}
bool cMap_Home1::PopulateDynamics(std::vector<cDynamic*>& vecDyns)
{
	vecDyns.push_back(new cDynamic_Teleport(5.0f, 13.0f, "Coder Town", 12.0f, 7.0f));
	vecDyns.push_back(new cDynamic_Teleport(4.0f, 13.0f, "Coder Town", 12.0f, 7.0f));
	return true;
}
bool cMap_Home1::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature)
{
	if (target->sName == "Teleport")
	{
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Dooropen"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("Dooropen"));
		g_script->AddCommand(new cCommand_ChangeMap(
			((cDynamic_Teleport*)target)->sMapName,
			((cDynamic_Teleport*)target)->fMapPosX,
			((cDynamic_Teleport*)target)->fMapPosY));
	}
	return false;
}

cMap_River::cMap_River()
{
	Create("assets/rpgdata/map/introduction.lvl", RPG_Assets::get().GetSprite("village"), "River");
}

bool cMap_River::PopulateDynamics(std::vector<cDynamic*>& vecDyns)
{
	vecDyns.push_back(new cDynamic_Teleport(1.0f, 5.0f, "Coder Town", 62.0f, 30.0f));
	vecDyns.push_back(new cDynamic_Teleport(1.0f, 6.0f, "Coder Town", 62.0f, 29.0f));
	vecDyns.push_back(new cDynamic_Teleport(28.0f, 5.0f, "Cherry Wood", 3.0f, 51.0f));
	vecDyns.push_back(new cDynamic_Teleport(28.0f, 6.0f, "Cherry Wood", 3.0f, 52.0f));
	
	for (int i = 0; i < 3; i++)
	{
		cDynamic* g1 = new cDynamic_Creature_Skelly();
		vecDyns.push_back(g1);
		g1->px = rand() % 4 + 16.0f;
		g1->py =  6.0f;
	}
	return true;
}
bool cMap_River::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature)
{
	if (target->sName == "Teleport")
	{
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Walk"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("Walk"));
		g_script->AddCommand(new cCommand_ChangeMap(
			((cDynamic_Teleport*)target)->sMapName,
			((cDynamic_Teleport*)target)->fMapPosX,
			((cDynamic_Teleport*)target)->fMapPosY));
	}
	return false;
}

///////////////////////////////////
cMap_Dungeon::cMap_Dungeon()
{
	Create("assets/rpgdata/map/loz_dungeon.lvl", RPG_Assets::get().GetSprite("Zelda"), "House");
}

bool cMap_Dungeon::PopulateDynamics(std::vector<cDynamic*>& vecDyns)
{
	return true;
}
bool cMap_Dungeon::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature)
{
	if (target->sName == "Teleport")
	{
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Walk"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("Walk"));
		g_script->AddCommand(new cCommand_ChangeMap(
			((cDynamic_Teleport*)target)->sMapName,
			((cDynamic_Teleport*)target)->fMapPosX,
			((cDynamic_Teleport*)target)->fMapPosY));
	}
	return false;
}

cMap_Smallhouse::cMap_Smallhouse()
{
	Create("assets/rpgdata/map/SmallHousev2.lvl", RPG_Assets::get().GetSprite("Creep"), "Creepy House");
}

bool cMap_Smallhouse::PopulateDynamics(std::vector<cDynamic*>& vecDyns)
{
	return true;
}
bool cMap_Smallhouse::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature)
{
	if (target->sName == "Teleport")
	{
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Walk"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("Walk"));
		g_script->AddCommand(new cCommand_ChangeMap(
			((cDynamic_Teleport*)target)->sMapName,
			((cDynamic_Teleport*)target)->fMapPosX,
			((cDynamic_Teleport*)target)->fMapPosY));
	}
	return false;
}


cMap_Town::cMap_Town()
{
	Create("assets/rpgdata/map/LocalTown.lvl", RPG_Assets::get().GetSprite("village"), "Cherry Wood");
}
bool Once = true;
bool cMap_Town::PopulateDynamics(std::vector<cDynamic*>& vecDyns)
{
	
 	vecDyns.push_back(new cDynamic_Teleport(0.0f, 51.0f, "River", 27.0f, 5.0f));
	vecDyns.push_back(new cDynamic_Teleport(0.0f, 52.0f, "River", 27.0f, 6.0f));
	if ( Once  == true)
	{
		vecDyns.push_back(new cDynamic_Item(16, 18, RPG_Assets::get().GetItem("Basic Sword")));
		Once = false;
	}	
	cDynamic* g1 = new cDynamic_Creature_Slime();
	vecDyns.push_back(g1);
	g1->px = 29;
	g1->py = 34;
	cDynamic* g2 = new cDynamic_Creature_Slime();
	vecDyns.push_back(g2);
	g2->px = 29;
	g2->py = 36;
	cDynamic* g3 = new cDynamic_Creature_Slime();
	vecDyns.push_back(g3);
	g3->px = 28;
	g3->py = 35;
	return true;
}
bool cMap_Town::OnInteraction(std::vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature)
{
	if (target->sName == "Teleport")
	{
		g_script->AddCommand(new cCommand_ChangeMap(
			static_cast<cDynamic_Teleport*>(target)->sMapName,
			static_cast<cDynamic_Teleport*>(target)->fMapPosX,
			static_cast<cDynamic_Teleport*>(target)->fMapPosY)
		);
		//olc::SOUND::PlaySample(RPG_Assets::get().GetSound("Walk"));
		g_engine->engine.PlayWaveform(RPG_Assets::get().GetSound("Walk"));
	}
	if (target->sName == "WorldTeleport")
	{
		g_script->AddCommand(new cCommand_ChangeMODE(
			((cDynamic_WORLD_Teleport*)target)->WorldMapPosX,
			((cDynamic_WORLD_Teleport*)target)->WorldMapPosY));
	}
	return false;
}
