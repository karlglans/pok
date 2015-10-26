// Pok.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
#include <vector>
#include <algorithm>
#include "PairPos.h"
#include "PreHandExamination.h"
#include "MiniSim.h"
#include "Precalculated.h"


int runSimForLogger(int nPlayers, int laps) {
	std::cout << "Runsim, players: " << nPlayers << " laps: " << laps << "\n";
	MiniSim ms;
	time_t now;
	time(&now); 
	StartHandLogger* shl = new StartHandLogger();
	ms.runForLogger(nPlayers, laps, shl);

	struct StartPair{
		float winchance;
		int count;
		int wins;
		int handPair;
		int high;
		int low;
		bool suit;
	};
	std::vector<StartPair> vec(169);

	StartPair sp;  PairPos hp;
	int cDetected = 0;
	for (int pair = 0; pair < 169; pair++){
		sp.count = shl->getStartHandCount(nPlayers, pair);
		if (sp.count > 0) {
			sp.wins = shl->getWinningPairCount(nPlayers, pair);
			sp.winchance = sp.wins / (float)sp.count;
			sp.handPair = pair;
			hp.pos = pair;
			hp.get(&sp.high, &sp.low, &sp.suit);
			cDetected++;
			vec[pair] = sp;
		}
	}

	std::sort(vec.begin(), vec.end(),
		[](const StartPair & a, const StartPair & b) -> bool
	{
		return a.winchance > b.winchance;
	});

	std::cout << "detected: " << cDetected << "\n";
	char buffer[100];
	char *suited = "s", *offsiut = "", *sss = 0;

	for (int r = 0; r < 20; r++) {
		//std::cout << " : " << vec[r].winchance << " c: "<< vec[r].count << "\n";
		if (vec[r].suit)
			sss = suited;
		sss = vec[r].suit ? suited : offsiut;
		sprintf_s(buffer, "%2.i %2.i %02.i%s\t %5.2f %u \n", r + 1, vec[r].high, vec[r].low, sss, vec[r].winchance, vec[r].count);
		printf(buffer);
	}

	Precalculated precalced;
	//precalced.save(shl, nPlayers);

	time_t done;
	time(&done);
	double seconds = difftime(done, now);
	printf("%.f seconds", seconds);

	delete shl;
	return 0;
}

int runSimCompair(int nPlayers, int laps) {
	// 2 3 4 5 6 7 8 9 0 k q k e
	// 0 1 2 3 4 5 6 7 8 9 0 1 2
	std::cout << "runSimCompair(), players: " << nPlayers << " laps: " << laps << "\n";
	MiniSim ms;
	time_t now;
	time(&now);  /* get current time; same as: now = time(NULL)  */

	CardDeck cd;  MiniSimResults result;
	Card cards[] = { { 0, 0 }, { 5, 2 } };
	
	cd.SetPlayerCards(cards);
	
	float winChance = ms.runSim(&cd, nPlayers, laps, &result);
	result.print();
	time_t done;
	time(&done);
	double seconds = difftime(done, now);
	printf("%.f seconds", seconds);
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	printf("RUNNING DEBUG BUILD\n");
#endif

	//for (int pl = 2; pl <= 8; pl++) runSim(pl, 1000 * 1000 * 1);
	
	runSimForLogger(4, 1 * 1000 * 1000); 
	//runSim(9, 1000 * 1000 * 1000); 
	//runSimCompair(9, 1000*1000*1);
	
	// done 1000M: 3, 4, 10,
	// done 200M: 9

	char cc;
	std::cin.get(cc);
	
	return 0;
}

