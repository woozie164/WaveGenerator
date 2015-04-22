#pragma once
#include <vector>

class Envelope
{
	struct Breakpoint
	{
		float t;
		float v;

		Breakpoint(float t, float v)
		{
			this->t = t;
			this->v = v;
		}
	};

public:
	std::vector<Breakpoint> breakpoints;

	Envelope(void);
	~Envelope(void);

	void LoadEnvelope(string filename);
};

