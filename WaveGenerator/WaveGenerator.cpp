// WaveGenerator.cpp : main project file.

#include "stdafx.h"
#include "WaveForm.h"
#include "OscGenerator.h"
#include <time.h>

/*
	Main klass f�r WaveGenerator.

	author: Johan Hagelb�ck (jhg@bth.se)
*/
int main(int a, char *args[])
{
	clock_t startTime = clock();

	//Generera fyrkantsv�g, 880 Hz med 4 oscillatorer (harmonier).
	OscGenerator og = OscGenerator(44100, 880, 8, 0.8, 4, SQUARE, "square.wav");
	og.generateToWav();

	clock_t endTime = clock();
	double elapsed = (endTime - startTime) / (double)CLOCKS_PER_SEC;
	cout << "Elapsed: " << elapsed << " sec" << endl;

    system("pause");

    return 0;
}
