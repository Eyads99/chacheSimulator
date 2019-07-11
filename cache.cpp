#include <iostream>
#include  <iomanip>

using namespace std;

#define		DBG				1
#define		DRAM_SIZE		(64*1024*1024)
#define		CACHE_SIZE		(64*1024) //fixed

unsigned int LINESIZE;

enum cacheResType { MISS = 0, HIT = 1 };

/* The following implements a random number generator */
unsigned int m_w = 0xABCCAB55;    /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0x05786902;    /* must not be zero, nor 0x9068ffff */

unsigned int rand_()
{
	m_z = 36969 * (m_z & 65535) + (m_z >> 16);
	m_w = 18000 * (m_w & 65535) + (m_w >> 16);
	return (m_z << 16) + m_w;  /* 32-bit result */
}

unsigned int memGenA()//memory generator that always increses the memory location by one
{
	static unsigned int addr = 0;
	return (addr++) % (DRAM_SIZE / 4);
}

unsigned int memGenB()
{
	static unsigned int addr = 0;
	return  rand_() % (64 * 1024);
}

unsigned int memGenC()
{
	static unsigned int a1 = 0, a0 = 0;
	a0++;
	if (a0 == 512) { a1++; a0 = 0; }
	if (a1 == 128) a1 = 0;
	return(a1 + a0 * 128);
}

unsigned int memGenD()
{
	static unsigned int addr = 0;
	return (addr++) % (1024 * 4);
}

unsigned int memGenE()
{
	static unsigned int addr = 0;
	return (addr++) % (1024 * 16);
}

unsigned int memGenF()
{
	static unsigned int addr = 0;
	return (addr += 64) % (128 * 1024);
}


// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr)
{
	// This function accepts the memory address for the memory transaction and
	// returns whether it caused a cache miss or a cache hit
	//New stuff from Eyad cache will work by having the valid bit as the least significant bit then the tag then the data
	unsigned int NoL = CACHE_SIZE / LINESIZE;
	static unsigned int cache[CACHE_SIZE] = {};
	unsigned int map = addr % NoL;  //LINESIZE undefined TODO    //(#Blocks in cache)
	if (cache[map] % 2 == 0)//if invalid
	{
		offesb= log2(LINESIZE);
		cache[map] = addr / CACHE_SIZE;//this gets the tag
		/*cache[map]=*/cache[map] << 1;//making space for the one bit for valid
		cache[map] += 1;//making valid
		//TODO DATA PART
		cout << "¡CACHE VALIDATED!" << endl;
		return MISS;
	}
	else if ((cache[map] >> 1)&0xffff) != (addr / CACHE_SIZE))//if tag is incorrect
	{
		cache[map] = addr /NoL;//this gets the tag
		cache[map] << 1;//making space for the one bit for valid
		cache[map] += 1;//making valid
		cout << "¡CACHE OVERWRITE!" << endl;
		return MISS;

	}
	else
		return HIT;
	// The current implementation assumes there is no cache; so, every transaction is a miss

	//return MISS;
}


char *msg[2] = { (char*)"Miss",(char*)"Hit" };//Array to cout if something is a hit or a miss

#define		NO_OF_Iterations	10000	// Change to 1,000,000
int main()
{
	unsigned int hit = 0;
	cacheResType r;
	unsigned int addr;

	cout << "What linesize do you want to use in bytes? (binary number from 8-128)\n";
	cin >> LINESIZE;
	cout << "Direct Mapped Cache Simulator\n";

	for (int inst = 0; inst < NO_OF_Iterations; inst++)
	{
		addr = memGenB();
		r = cacheSimDM(addr);
		if (r == HIT) hit++;
		cout << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r] << ")\n";
	}
	//cout<<"hit count= " <<dec<<hit<<endl;
	cout << "Hit ratio = " << (100.0*hit / NO_OF_Iterations) << endl;
	//cout<<CACHE_SIZE/LINESIZE;
}
