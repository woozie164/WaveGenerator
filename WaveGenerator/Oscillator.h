#ifndef __OSCILLATOR_H__
#define __OSCILLATOR_H__

#include "StdAfx.h"

/*
	Hanterar en oscillator som anv�nds av OscGenerator/WaveForm f�r att
	generera en ljudv�g.

	author: Johan Hagelb�ck (jhg@bth.se)
*/
class Oscillator
{
private:
	
public:
	Oscillator(unsigned long sampleRate);
	Oscillator(unsigned long sampleRate, double phase);

	double twoPI_ovr_sr;

	unsigned long sampleRate;
	double curFreq;
	double curPhase;
	double incr;
};

#endif