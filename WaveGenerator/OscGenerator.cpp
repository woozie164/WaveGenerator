#include "StdAfx.h"
#include "OscGenerator.h"
#include "portsf.h"
#include <math.h>
#include "lerp.h"

OscGenerator::OscGenerator(unsigned long sampleRate, double freq, double duration, double ampfac, int noOscillators, int waveFormType, char* filename, Envelope * ampEnv, Envelope * freqEnv)
{
	this->sampleRate = sampleRate;
	this->freq = freq;
	this->duration = duration;
	this->ampfac = ampfac;
	this->waveFormType = waveFormType;
	this->filename = filename;

	for (int i = 0; i < noOscillators; i++)
	{
		oscs.push_back(new Oscillator(sampleRate));
	}
	oscamps = (double*) malloc(noOscillators * sizeof(double));
	oscfreqs = (double*) malloc(noOscillators * sizeof(double));

	nsamps = (unsigned long)(duration * sampleRate);

	init();

	this->ampEnv = ampEnv;
	if(ampEnv)
	{
		ampEnv->SetDuration(duration);
		ampEnv->SetNumSamples(nsamps);
	}	

	this->freqEnv = freqEnv;
	if(freqEnv)
	{
		freqEnv->SetDuration(duration);
		freqEnv->SetNumSamples(nsamps);
	}

	this->oscType = GENERATOR;

	unsigned int tableSize = 1024 * 2;
	sineTable.reserve(tableSize);
	double step = TWOPI / tableSize;
	for(unsigned int i = 0; i < tableSize; i++)
	{
		sineTable.push_back(sin(step * i));
	}
}

void OscGenerator::init()
{
	double ampfac = 1.0;
	double freqfac = 1.0;
	double ampadjust = 0.0;
	double phase = 0.0;

	if (waveFormType == TRI)
	{
		for (int i = 0; i < (int)oscs.size(); i++)
		{
			ampfac = 1.0 / (freqfac*freqfac);
			oscamps[i] = ampfac;
			oscfreqs[i] = freqfac;
			freqfac += 2.0; /* odd harmonics only */
			ampadjust += ampfac;
		}
		phase = 0.25;
	}
	if (waveFormType == SQUARE)
	{
		for (int i = 0; i < (int)oscs.size(); i++)
		{
			ampfac = 1.0 / freqfac;
			oscamps[i] = ampfac;
			oscfreqs[i] = freqfac;
			freqfac += 2.0;
			ampadjust += ampfac;
		}
	}
	if (waveFormType == SAWU || waveFormType == SAWD)
	{
		for (int i = 0; i < (int)oscs.size(); i++)
		{
			ampfac = 1.0 / freqfac;
			oscamps[i] = ampfac;
			oscfreqs[i] = freqfac;
			freqfac += 1.0;
			ampadjust += ampfac;
		}
		if (waveFormType == SAWU)
		{
			ampadjust = -ampadjust; /* inverts the waveform */
		}
	}

	/* Rescale amplitudes so they add to 1.0 */
	/* Set correct phase (sine/cos) for different wavetypes */
	for (int i = 0; i < (int)oscs.size(); i++)
	{
		oscamps[i] /= ampadjust;
		oscs.at(i)->curPhase = phase;
	}
}

OscGenerator::~OscGenerator()
{
	for (int i = 0; i < (int)oscs.size(); i++)
	{
		delete oscs.at(i);
	}
}

void OscGenerator::setFilename(char* filename)
{
	this->filename = filename;
}

int OscGenerator::generateToText()
{
	//Open output file
	FILE* fp = fopen(filename, "w");
	if (fp == NULL) 
	{
		cout << "Unable to open file '" << filename << "'" << endl;
		return 1;
	}

	//Oscillator loop
	for (unsigned long i = 0; i < nsamps; i++)
	{
		double val = 0.0;
		
		for (int k = 0; k < (int)oscs.size(); k++)
		{
			val += oscamps[k] * tick_sine(oscs[k], freq*oscfreqs[k]);
		}
		float samp = (float)(val * ampfac);

		fprintf(fp,"%.21f\n",samp);
	}

	//Close file
	fclose(fp);

	return 0;
}

int OscGenerator::generateToWav()
{
	PSF_PROPS props;
	props.srate = sampleRate;
	props.chans = 1;
	props.samptype = PSF_SAMP_IEEE_FLOAT;
	props.format = PSF_WAVE_EX;
	props.chformat = MC_MONO;
	portsf psf;
	
	int ofd = psf.psf_sndCreate(filename, &props, 1, 0, PSF_CREATE_RDWR);
	if (ofd < 0)
	{
		cout << "Unable to open file '" << filename << "'" << endl;
		return 1;
	}

	for (unsigned long i = 0; i < nsamps; i++)
	{
		double val = 0.0;

		// Apply frequency envelope
		float freq = this->freq;
		if(freqEnv)
		{
			freq *= freqEnv->breakpoints[i].v;
		}

		for (int k = 0; k < (int)oscs.size(); k++)
		{
			val += oscamps[k] * tick_sine(oscs[k], freq*oscfreqs[k]);			
		}
		float samp = (float)(val * ampfac);

		// Apply amplitude envelope
		if(ampEnv)
		{
			samp *= ampEnv->breakpoints[i].v;
		}

		float* frame = (float*) malloc(props.chans * sizeof(float));
		frame[0] = samp;
		psf.psf_sndWriteFloatFrames(ofd, frame, 1);
	}

	//Close file
	psf.psf_sndClose(ofd);

	return 0;
}

double OscGenerator::tick_sine(Oscillator* osc, double cFreq)
{
	double val;

	//Samp calculation
	switch(oscType)
	{
	case GENERATOR:
		val = sin(osc->curPhase);		
		break;

	case TABLE_LOOKUP:
		val = sine_lookup(osc->curPhase);
		break;
	}

	if (osc->curFreq != cFreq)
	{
		osc->curFreq = cFreq;
		osc->incr = osc->twoPI_ovr_sr * cFreq;
	}
	osc->curPhase += osc->incr;
	if (osc->curPhase >= TWOPI)
	{
		osc->curPhase -= TWOPI;
	}
	if (osc->curPhase < 0.0)
	{
		osc->curPhase += TWOPI;
	}

	return val;
}

double OscGenerator::sine_lookup(double curPhase)
{
	int i = (int)curPhase;
	int inext = i + 1;
	if(inext >= sineTable.size())
		inext -= sineTable.size();
	double frac = curPhase - i;
	return lerp(sineTable[i], sineTable[inext], frac);	
}

void OscGenerator::ApplyAmpEnvelope(Envelope & env)
{
	env.SetDuration(duration);
	env.SetNumSamples(nsamps);
}

void OscGenerator::ApplyFreqEnvelope(const Envelope & env)
{

}

void OscGenerator::SetOscillatorType(OSCILLATOR_TYPE oscType)
{
	this->oscType = oscType;
}