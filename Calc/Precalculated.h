#pragma once
#include "StartHandLogger.h"


struct MakeRanking {
	MakeRanking(int len) : len(len) {}
	void operator()(float *src, float *dst);
private:
	int len;
};



class Precalculated {
	// (cardPair, players) -> winchance [0,1]
	float* startPairWinchance; 
	
	// (cardPair, players) -> rank [0,1]
	// rank 0.7 means its better then 70% of the other starthands 
	float* startPairAbove;

	void rankStartPairs();
public:
	void writeResultsToFileBuffer(float* allWinChances, StartHandLogger* shl, int players);
	void save(StartHandLogger*, int nPlayers);
	void readFile();
	float *getWinChance(int players);
	float* GetAboveArray(int players);
	Precalculated(); 
	~Precalculated();
};

