
/***************************************************************\
*   IEEE80.h							*
*   Convert between "double" and IEEE 80 bit format  		*
*   in machine independent manner.				*
*   Assumes array of char is a continuous region of 8 bit frames*
*   Assumes (unsigned long) has 32 useable bits			*
*   billg, dpwe @media.mit.edu					*
*   01aug91							*
*   19aug91  aldel/dpwe  workaround top bit problem in Ultrix   *
*                        cc's double->ulong cast		*
*   05feb92  dpwe/billg  workaround top bit problem in 		*
*                        THINKC4 + 68881 casting		*
\***************************************************************/
#define ULPOW2TO31	((unsigned long)0x80000000L)
#define DPOW2TO31	((double)2147483648.0)	/* 2^31 */

class ieee80 
{
private:
	static double   myUlongToDouble (unsigned long ul);
	static unsigned long myDoubleToUlong (double val);
public:
	static double ieee_80_to_double(unsigned char *p);
	static void double_to_ieee_80(double val, unsigned char *p);
};