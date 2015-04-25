// WaveGenerator.cpp : main project file.

#include "stdafx.h"
#include "WaveForm.h"
#include "OscGenerator.h"
#include <time.h>

#include "Envelope.h"

/*
	Main klass för WaveGenerator.

	author: Johan Hagelbäck (jhg@bth.se)
*/
int main(int a, char *args[])
{
	Envelope env;
	env.LoadEnvelope("expDecay.brk");

	Envelope env2;
	env2.LoadEnvelope("ADSRenvelope.brk");

	clock_t startTime = clock();

	//Generera fyrkantsvåg, 880 Hz med 4 oscillatorer (harmonier).
	OscGenerator og = OscGenerator(44100, 880, 8, 0.8, 4, SQUARE, "square_ampADSRenv.wav", &env2, nullptr);	
	og.generateToWav();

	OscGenerator og2 = OscGenerator(44100, 880, 8, 0.8, 4, SQUARE, "square_freqADSRenv.wav", nullptr, &env2);	
	og2.generateToWav();

	OscGenerator og3 = OscGenerator(44100, 880, 8, 0.8, 4, SQUARE, "square_ampfreqADSRenv.wav", &env2, &env2);	
	og3.generateToWav();

	OscGenerator og4 = OscGenerator(44100, 880, 8, 0.8, 4, SQUARE, "square_ampfreqADSRenvTableLookup.wav", &env2, &env2);	
	og4.SetOscillatorType(TABLE_LOOKUP);
	og4.generateToWav();

	OscGenerator og5 = OscGenerator(44100, 880, 8, 0.8, 4, TRI, "tri_ampfreqADSRenvTableLookup.wav", &env2, &env2);	
	og5.SetOscillatorType(TABLE_LOOKUP);
	og5.generateToWav();

	clock_t endTime = clock();
	double elapsed = (endTime - startTime) / (double)CLOCKS_PER_SEC;
	cout << "Elapsed: " << elapsed << " sec" << endl;

    system("pause");

    return 0;
}
