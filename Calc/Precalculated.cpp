#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

#include "PairPos.h"
#include "Precalculated.h"

void Precalculated::writeResultsToFileBuffer(float* allWinChances, StartHandLogger* shl, int players) {
	struct CardPair {
		float winChance;
		int pairValue; // 0-168, StartHandLogger::numb_starthands
	};
	vector<CardPair> vec(StartHandLogger::numb_starthands);
	int nWins, nCount, nSpotted = 0;

	for (int i = 0; i < StartHandLogger::numb_starthands; i++) {
		vec[i].pairValue = i;
		nWins = shl->getWinningPairCount(players, i);
		nCount = shl->getStartHandCount(players, i);
		
		if (nCount == 0) {
			vec[i].winChance = 0;
		} else {
			nSpotted++;
			vec[i].winChance = nWins / (float)nCount;
		}
	}

	if (nSpotted != 169)
		__debugbreak(); // all startpairs should be there
 
	sort(vec.begin(), vec.end(), [](const CardPair & a, const CardPair & b) -> bool{
		return a.winChance > b.winChance;
	});

	int firstResultIdx = StartHandLogger::numb_starthands *(players - 2);
	float* pResultsInIntervall = &allWinChances[firstResultIdx];

	// ** pResultsInIntervall[pairValue] --> handPos (orderd by winchance, highest first)
	for (int i = 0; i < StartHandLogger::numb_starthands; i++) {
		int pairValue = vec[i].pairValue;
		pResultsInIntervall[pairValue] = vec[i].winChance;
	}
}

void Precalculated::save(StartHandLogger* shl, int nPlayers) {
	int bufferSize = sizeof(float)* StartHandLogger::numb_starthands;
	float* pairRanking = new float[StartHandLogger::numb_starthands * 9];
	memset(pairRanking, 1, bufferSize * 9);
	writeResultsToFileBuffer(pairRanking, shl, nPlayers);
	int startPoint = StartHandLogger::numb_starthands*(nPlayers - 2); // offset from 0
	fstream file;
	file.open("pairs.bin", ios::binary | std::fstream::in | std::fstream::out); // 
	file.seekg(startPoint * sizeof(float), file.beg);
	file.write((const char*)&pairRanking[startPoint], bufferSize);
	file.close();
}

Precalculated::Precalculated() {
	startPairWinchance = new float[StartHandLogger::numb_starthands * 9];
	readFile();
}
Precalculated::~Precalculated() {
	delete[] startPairWinchance;
}

float* Precalculated::getWinChance(int nPlayers) {
	int startPoint = StartHandLogger::numb_starthands*(nPlayers - 2);
	return &startPairWinchance[startPoint];
}

void Precalculated::readFile() {
	fstream file;
	file.open("pairs.bin", ios::binary | std::fstream::in);
	file.seekg(0, file.beg);
	file.read((char*)startPairWinchance, sizeof(float)*StartHandLogger::numb_starthands * 9);
	file.close();
}