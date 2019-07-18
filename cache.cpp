#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

#define		DBG				1
#define		DRAM_SIZE		(64*1024*1024)
#define		CACHE_SIZE		(64*1024) //fixed

unsigned int LINESIZE;
static signed int cache[16384]={0};//cache at it's maximum number of lines 

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

unsigned int memGenA()
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

unsigned int memGenVal0()//validation memory generator that always increses the memory location by 4
{
	static unsigned int addr = 0;
	return (addr+=4) % (DRAM_SIZE / 4); 
}

unsigned int memGenVal1()//validation memory generator that always increses the memory location by 64
{
	static unsigned int addr = 0;
	return (addr+=127	) % (DRAM_SIZE / 4); 
}


// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr)
{
	// This function accepts the memory address for the memory transaction and
	// returns whether it caused a cache miss or a cache hit
	unsigned int NoL = CACHE_SIZE / LINESIZE;//number of lines
	//static signed int cache[16384] = {};//initializing the cache as all 0s to make all locations invalid and putting in the max NoL
	unsigned int offesb = (log2(LINESIZE));//log2: Returns the binary (base-2) logarithm of x MIGHT BE TOO LARGE

	//addr = addr>>2;//this removes the byte offset of the data 
	unsigned int map = (addr >> (int)offesb) % NoL;  //LINESIZE undefined TODO    //(#Blocks in cache)
	if (cache[map] % 2 == 0)//if invalid
	{
		cache[map] = (addr>>16);//this gets the tag while ignoring the byte and word offset
		cache[map] = cache[map] << 1;//making space for the one bit for valid
		cache[map] += 1;//making valid
		
		cout << "CACHE VALIDATED!" << endl;
		return MISS;
	}
	else if (((cache[map] >> 1)/*&0xffff*/) != ((addr >> 16/*offesb) / NoL*/)))//if tag is incorrect
	{
		cache[map] = addr >> 16/*offesb) /NoL*/;//this gets the tag while ignoring the byte and word offset
		cache[map] = cache[map] << 1;//making space for the one bit for valid
		cache[map] += 1;//making valid
		cout << "CACHE OVERWRITE!" << endl;
		return MISS;
	}
	else
	{	
		//cout << map << endl;
		//cout<< (addr>>16)<<endl;
		//cout<< (addr>>offesb) /NoL<<endl;
		return HIT;
	}
}

char *msg[2] = { (char*)"Miss",(char*)"Hit" };//Array to cout if something is a hit or a miss

#define		NO_OF_Iterations	1000000
int main()
{
	unsigned int hit = 0;
	cacheResType r;
	unsigned int addr;
	char MG;//To select between the Memory Generators
	
	//fill_n(cache, 16384, -1);
	cout << "What linesize do you want to use in bytes? (binary number from 4-128)\n";
	cin >> LINESIZE;
	cout << "What Memory generator would you like (between A-F)\n";
	cin >> MG;
	cout << "Direct Mapped Cache Simulator\n";

	for (int inst = 0; inst < NO_OF_Iterations; inst++)//picking which memGen to use
	{
		switch (MG)
		{
		case 'a':
		case 'A':
			addr = memGenA(); break;
		case 'b':
		case 'B':
			addr = memGenB(); break;
		case 'c':
		case 'C':
			addr = memGenC(); break;
		case 'd':
		case 'D':
			addr = memGenD(); break;
		case 'e':
		case 'E':
			addr = memGenE(); break;
		case 'f':
		case 'F':
			addr = memGenF(); break;
		case '0':
			addr= memGenVal0();break;//the validation memory Generator 0
		case '1':
			addr= memGenVal1();break;//the validation memory Generator 1
		}
		r = cacheSimDM(addr);
		if (r == HIT) hit++;
		cout << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r] << ")\n";
	}
	cout << "Hit ratio = " << (100.0*hit / NO_OF_Iterations) << endl;
	cout << "Miss ratio = " << 100-(100.0*hit / NO_OF_Iterations) << endl;
	//cout<<CACHE_SIZE/LINESIZE;
}
