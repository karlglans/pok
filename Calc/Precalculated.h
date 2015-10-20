#pragma once
#include "StartHandLogger.h"


class Precalculated {
	//unsigned char* startpairRaking;
	float* startPairWinchance;
public:
	void writeResultsToFileBuffer(float* allWinChances, StartHandLogger* shl, int players);
	void save(StartHandLogger*, int nPlayers);
	void readFile();
	float *getWinChance(int players);
	Precalculated(); 
	~Precalculated();
};

