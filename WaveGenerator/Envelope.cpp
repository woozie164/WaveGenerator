#include "StdAfx.h"
#include "Envelope.h"
#include <fstream>

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

void Envelope::SetDuration(int double)
{
	for(int i = 0; i < breakpoints.size(); i++)
	{
		breakpoints.t *= seconds;
	}
}

void Envelope::SetNumSamples(unsinged long numSamples)
{
	vector<Breakpoint> newBreakpoints(numSamples);

	unsinged long diff = numSamples / breakpoints.size();
	int lastSample = 0;
	for(unsingedn long i = 0; i < breakpoints.size(); i += diff)
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
			breakpoints[lastSample];
			breakpoints[lastSample + diff];
			
		}


	}
}
