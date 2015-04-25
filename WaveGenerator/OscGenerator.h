#ifndef __OSCGENERATOR_H__
#define __OSCGENERATOR_H__

#include "StdAfx.h"
#include <vector>
#include "Oscillator.h"
#include "Envelope.h"

/*
	Skapar en syntetiserad ljudvåg av valfri typ och valfritt antal
	oscillatorer (harmonier).

	author: Johan Hagelbäck (jhg@bth.se)
*/	
enum OSCILLATOR_TYPE
{
	TABLE_LOOKUP,
	GENERATOR
};

class OscGenerator
{
private:
	//Parameters
	unsigned long sampleRate;
	double freq;
	double duration;
	double ampfac;
	char* filename;

	//Additional variables and methods
	vector<Oscillator*> oscs;
	double* oscamps;
	double* oscfreqs;
	int waveFormType;
	Envelope * ampEnv; 
	Envelope * freqEnv;
	void init();

	unsigned long nsamps;
	double tick_sine(Oscillator* osc, double cFreq);
	
	OSCILLATOR_TYPE oscType;

public:
	OscGenerator(unsigned long sampleRate, double freq, double duration, double ampfac, int noOscillators, int waveFormType, char* filename, Envelope * ampEnv, Envelope * freqEnv);
	~OscGenerator();

	int generateToText();
	int generateToWav();

	void setFilename(char* filename);
	void SetOscillatorType(OSCILLATOR_TYPE oscType);

	void ApplyAmpEnvelope(Envelope & env);
	void ApplyFreqEnvelope(const Envelope & env);
};

#endif


	