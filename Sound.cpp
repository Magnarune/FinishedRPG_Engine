#include "Sound.h"


SoundFilter::SoundFilter()
{	
}

float SoundFilter::ChannelFilter(int nChannel, float fGlobalTime, float fSample)

{
	float fOutput = fSample* fFilterVolume;

	return fOutput;

}