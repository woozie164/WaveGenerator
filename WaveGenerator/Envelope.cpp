#include "StdAfx.h"
#include "Envelope.h"
#include <fstream>
#include "lerp.h"
#include <assert.h>

Envelope::Envelope(void)
{
}


Envelope::~Envelope(void)
{
}

void Envelope::LoadEnvelope(string filename)
{
	ifstream f(filename);
	if(!f.is_open())
	{
		cerr << "Unable to open file " << filename << endl;
		return;
	}
	while(true)
	{
		float t, v;
		f >> t;
		if(f.eof()) break;
		f >> v;	
		if(f.eof()) break;
		breakpoints.push_back(Breakpoint(t, v));
	}
}

void Envelope::SetDuration(double seconds)
{
	for(unsigned int i = 0; i < breakpoints.size(); i++)
	{
		breakpoints[i].t *= seconds;
	}
}

void Envelope::SetNumSamples(unsigned int numSamples)
{
	vector<Breakpoint> newBreakpoints;
	newBreakpoints.reserve(numSamples);

	unsigned int diff = numSamples / breakpoints.size();
	
	// Currently, the code assumes that there's an integer ratio between
	// the number of breakpoints and number of samples.
	assert(diff >= 1);
	
	// Check that numSamples / breakpoints.size() is not a fraction
	assert((numSamples / breakpoints.size()) % 1 == 0);

	int lastSample = 0;
	for(unsigned long i = 0; i < breakpoints.size(); i++)
	{
		// The data needed exists already, so just copy it over
		if(i % diff == 0)
		{
			lastSample = i;
			newBreakpoints.push_back(breakpoints[i]);
			//newBreakpoints.t = breakpoints[i].t;
			//newBreakpoints[i].v = breakpoints[i].v;
		}
		else // data missing. Get it by using linear interpolation
		{
			// Interpolate betwwen these two
			float t = (i - lastSample) * ((breakpoints[lastSample + 1].t - breakpoints[lastSample].t) / (numSamples - breakpoints.size() - 1));
			Breakpoint bkp;
			bkp.t = lerp(breakpoints[lastSample].t, breakpoints[lastSample + 1].t, t);
			bkp.v = lerp(breakpoints[lastSample].v, breakpoints[lastSample + 1].v, t);			
			newBreakpoints.push_back(bkp);
		}
	}
}
