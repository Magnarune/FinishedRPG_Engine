#pragma once
#include "olcPixelGameEngine.h"
#include "olcSoundWaveEngine.h"
//#include "olcPGEX_Sound.h"
#include <list>


class RPG_Engine;

class SoundFilter
{
public:
	 SoundFilter();

	 static float ChannelFilter(int nChannel, float fGlobalTime, float fSample);
	
public:
	bool bToggle = false;
	static bool bSynthPlaying;
	static float fFilterVolume;
	static float fPreviousSamples[128];
	static int nSamplePos;
	

};
