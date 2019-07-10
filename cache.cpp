#include <iostream>
#include  <iomanip>

using namespace std;

#define		DBG				1
#define		DRAM_SIZE		(64*1024*1024)
#define		CACHE_SIZE		(64*1024)

enum cacheResType {MISS=0, HIT=1};

/* The following implements a random number generator */
unsigned int m_w = 0xABCCAB55;    /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0x05786902;    /* must not be zero, nor 0x9068ffff */
unsigned int rand_()
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;  /* 32-bit result */
}

unsigned int memGenA()
{
	static unsigned int addr=0;
	return (addr++)%(DRAM_SIZE/4);
}

unsigned int memGenB()
{
	static unsigned int addr=0;
	return  rand_()%(64*1024);
}

unsigned int memGenC()
{
	static unsigned int a1=0, a0=0;
	a0++;
	if(a0 == 512) { a1++; a0=0; }
	if(a1 == 128) a1=0;
	return(a1 + a0*128);
}

unsigned int memGenD()
{
	static unsigned int addr=0;
	return (addr++)%(1024*4);
}

unsigned int memGenE()
{
	static unsigned int addr=0;
	return (addr++)%(1024*16);
}

unsigned int memGenF()
{
	static unsigned int addr=0;
	return (addr+=64)%(128*1024);
}


// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr)
{	
	// This function accepts the memory address for the memory transaction and 
	// returns whether it caused a cache miss or a cache hit

	// The current implementation assumes there is no cache; so, every transaction is a miss
	return MISS;
}


char *msg[2] = {(char*)"Miss",(char*)"Hit"};

#define		NO_OF_Iterations	100		// Change to 1,000,000
int main()
{
	unsigned int hit = 0;
	cacheResType r;
	
	unsigned int addr;
	cout << "Direct Mapped Cache Simulator\n";

	for(int inst=0;inst<NO_OF_Iterations;inst++)
	{
		addr = memGenB();
		r = cacheSimDM(addr);
		if(r == HIT) hit++;
		cout <<"0x" << setfill('0') << setw(8) << hex << addr <<" ("<< msg[r] <<")\n";
	}
	cout << "Hit ratio = " << (100*hit/NO_OF_Iterations)<< endl;
}