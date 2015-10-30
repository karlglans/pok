#include "stdafx.h"
#include "CppUnitTest.h"
#include "GameSim.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CalcTest
{
	TEST_CLASS(GameSimTest)
	{
	public:

		TEST_METHOD(GameSim_RunPreFlopPhase_foldWithBadPairs)
		{
			int nSeats = 3, dealer = 0; // player is the dealer
			Precalculated precalc;  SeatActionHistory sh;  CardDeck deck;  GameSim sm(&precalc);
		
			Card playerCards[] = { { 12, 2 }, { 12, 3 } }; // AA {value, color}
			Card badPair1[] = { { 0, 0 }, { 1, 1 } }; // 23o
			Card badPair2[] = { { 0, 2 }, { 1, 3 } }; // 23o

			// should have come from file read
			float* above = precalc.GetAboveArray(nSeats);
			above[90] = 1.00f; // AA
			above[1] = 0.01f; // 23o
			
			deck.setCardsForSeat(badPair1, 1);
			deck.setCardsForSeat(badPair2, 2);
			deck.SetPlayerCards(playerCards);
			
			sh.newGame(nSeats, dealer, 50, 100);
			sh.insertBet(1000); // player bets

			int nActiveSeates = sm.runPreflopPhase(&sh, &precalc, &deck);
			int nPlayesLeft = sh.countActiveSeats();

			SeatStatus reportPlayerSeat, reportSeat1, reportSeat2;
			sh.getSeatStatus(&reportPlayerSeat, 0);
			sh.getSeatStatus(&reportSeat1, 1);
			sh.getSeatStatus(&reportSeat2, 2);

			Assert::AreEqual(true, reportPlayerSeat.stillActive);
			Assert::AreEqual(false, reportSeat1.stillActive);
			Assert::AreEqual(false, reportSeat2.stillActive);

			// expect seat 1,2 to fold
			Assert::AreEqual(nPlayesLeft, 1);
		}

		TEST_METHOD(GameSim_RunPreFlopPhase_callWithGoodPairs)
		{
			int nSeats = 4, dealer = 0; // player is the dealer
			Precalculated precalc;  SeatActionHistory sh;  CardDeck deck;  GameSim sm(&precalc);

			Card playerCards[] = { { 12, 2 }, { 12, 3 } }; // {value, color}
			Card goodPair1[] = { { 11, 0 }, { 11, 1 } }; // KK 
			Card goodPair2[] = { { 11, 2 }, { 11, 3 } }; // KK

			// should have come from file. 
			float* above = precalc.GetAboveArray(nSeats);
			above[90] = 1.00f; // AA
			above[77] = 0.9f; // KK

			deck.setCardsForSeat(goodPair1, 1);
			deck.setCardsForSeat(goodPair2, 2);
			deck.SetPlayerCards(playerCards);

			sh.newGame(nSeats, dealer, 50, 100);
			sh.insertBet(1000); // player bets

			
			sm.begin(playerCards, nSeats, dealer, 100, 200);

			int nActiveSeates = sm.runPreflopPhase(&sh, &precalc, &deck);
			int nPlayesLeft = sh.countActiveSeats();

			SeatStatus reportPlayerSeat, reportSeat1, reportSeat2;
			sh.getSeatStatus(&reportPlayerSeat, 0);
			sh.getSeatStatus(&reportSeat1, 1);
			sh.getSeatStatus(&reportSeat2, 2);

			Assert::AreEqual(true, reportPlayerSeat.stillActive);
			Assert::AreEqual(true, reportSeat1.stillActive);
			Assert::AreEqual(true, reportSeat2.stillActive);

			// expect seat 1,2 to fold
			//Assert::AreEqual(nPlayesLeft, 3);
		}


		
	};
}