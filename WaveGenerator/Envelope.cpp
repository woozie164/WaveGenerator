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
