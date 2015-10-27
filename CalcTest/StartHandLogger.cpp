#include "stdafx.h"
#include "CppUnitTest.h"
#include "StartHandLogger.h"
#include "Precalculated.h"
#include "PairPos.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CalcTest
{
	TEST_CLASS(StartHandLoggerTest)
	{
	public:

		TEST_METHOD(PairPos_GetSet)
		{
			StartHandLogger shl;  PairPos pp;
			int highOut, lowOut; // 0-12
			bool suitedOut;
			for (int high = 0; high < 13; high++){
				for (int low = 0; low < 13; low++){
					if (low <= high){
						// test all possible suited cards:
						if (high != low) { // suited cards cant be pairs
							pp.set(high, low, true);
							Assert::IsTrue(pp.pos < 169);
							pp.get(&highOut, &lowOut, &suitedOut);
							Assert::AreEqual(high, highOut);
							Assert::AreEqual(low, lowOut);
							Assert::AreEqual(true, suitedOut);

						}
						// test all possible offsuit cards:
						pp.set(high, low, false);
						Assert::IsTrue(pp.pos < 169);
						pp.get(&highOut, &lowOut, &suitedOut);
						Assert::AreEqual(high, highOut);
						Assert::AreEqual(low, lowOut);
						Assert::AreEqual(false, suitedOut);
					}
				}
			}
		}

		TEST_METHOD(StartHandLogger_makeStartHandValue_testAllCombiations)
		{
			StartHandLogger shl;
			int cardPairValue;
			//int highest, lowest; 
			Card cards[] = { { 1000, 0 }, { 1000, 0 } }; // {value, color}
			for (int color = 0; color < 2; color++) {
				for (int v0 = 0; v0 < 13; v0++) {
					for (int v1 = 0; v1 < 13; v1++){
						cards[0].value = v0;
						cards[1].value = v1;
						cards[0].color = 0;
						if (v0 == v1)
							cards[1].color = 1;
						else
							cards[1].color = color; // either 0 or 1
						cardPairValue = shl.makeCardPairValue(cards); // make a cardPairValue
						Assert::IsTrue(cardPairValue < 169);
					}
				}
			}
		}

		TEST_METHOD(StartHandLogger_addStartHands)
		{
			StartHandLogger shl;
			// startHandPair1 same type as startHandPair3, both: 23o
			//               startHandPair1:     startHandPair2:     startHandPair3:     startHandPair4:
			//				 23s                 45s                 23s                 AA
			Card cards[] = { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 }, { 0, 4 }, { 1, 4 }, { 12, 4 }, { 12, 1 } }; // {value, color}

			int numberOfPlayers = 4;
			shl.addCardPair(cards, numberOfPlayers, false);
			shl.addCardPair(&cards[2], numberOfPlayers, false);
			shl.addCardPair(&cards[4], numberOfPlayers, false);
			shl.addCardPair(&cards[6], numberOfPlayers, false);

			//                     pair, KK
			Card notYetAdded[] = { { 11, 4 }, { 11, 3 } };

			int startHandValue_23s = shl.makeCardPairValue(&cards[0]);
			int startHandValue_45s = shl.makeCardPairValue(&cards[2]);
			int startHandValue_AA = shl.makeCardPairValue(&cards[6]);
			int startHandValue_KK = shl.makeCardPairValue(&notYetAdded[0]);

			int countPair_23s = shl.getStartHandCount(numberOfPlayers, startHandValue_23s);
			int countPair_45s = shl.getStartHandCount(numberOfPlayers, startHandValue_45s);
			int countPair_AA = shl.getStartHandCount(numberOfPlayers, startHandValue_AA);
			int countPair_KK = shl.getStartHandCount(numberOfPlayers, startHandValue_KK);

			Assert::AreEqual(2, countPair_23s);
			Assert::AreEqual(1, countPair_45s);
			Assert::AreEqual(1, countPair_AA);
			Assert::AreEqual(0, countPair_KK);
		}

		TEST_METHOD(StartHandLogger_addStartHands_addToDiffNumPlayers)
		{
			StartHandLogger shl;
			//				 QKs                   45s                 45s   
			Card cards[] = { { 10, 1 }, { 11, 1 }, { 2, 1 }, { 3, 1 }, { 2, 4 }, { 3, 4 } }; // {value, color}
			int twoPlayers = 2, threePlayes = 3;
			// add 2 startPairs to a 2 player Game
			shl.addCardPair(&cards[0], twoPlayers, false);
			shl.addCardPair(&cards[2], twoPlayers, false);
			// add 3 startPairs to a 3 player Game
			shl.addCardPair(&cards[0], threePlayes, false);
			shl.addCardPair(&cards[2], threePlayes, false);
			shl.addCardPair(&cards[4], threePlayes, false);

			int startHandValue_QKs = shl.makeCardPairValue(&cards[0]);
			int startHandValue_45s = shl.makeCardPairValue(&cards[2]);
			int countPair_QKs_in2playerGame = shl.getStartHandCount(twoPlayers, startHandValue_QKs);
			int countPair_QKs_in3playerGame = shl.getStartHandCount(threePlayes, startHandValue_QKs);
			int countPair_45s_in3playerGame = shl.getStartHandCount(threePlayes, startHandValue_45s);

			Assert::AreEqual(1, countPair_QKs_in2playerGame);
			Assert::AreEqual(1, countPair_QKs_in3playerGame);
			Assert::AreEqual(2, countPair_45s_in3playerGame);
		}

		TEST_METHOD(Precalculated_prepairResultsBuffer)
		{
			float resultsBuffer[9 * 169]; // resultsBuffer[(nubPlayers-2)*offset + startPairValue] = winchance ranking (0-168) of this hand
			memset(resultsBuffer, 0, 8 * 169);
			StartHandLogger shl;
			int *pStartHandsCount = 0, *pWinningPairCount = 0;
			shl.getBuffers(&pWinningPairCount, &pStartHandsCount);

			int nPlayers = 7;
			pStartHandsCount = &pStartHandsCount[169 * (nPlayers - 2)]; // move to game 8
			for (int i = 0; i < 169; i++)
				pStartHandsCount[i] = 2; // every start added twice, so there is an count for every cardpair

			// just checking if all hands have been seet to count 2, 
			Assert::AreEqual(0, shl.getStartHandCount(nPlayers, 169)); // belongs 9 player games, should still be 0
			Assert::AreEqual(2, shl.getStartHandCount(nPlayers, 168));
			Assert::AreEqual(2, shl.getStartHandCount(nPlayers, 0));

			// add best card as winner wice: (register 3 wins)
			Card bestCards_22[] = { { 0, 2 }, { 0, 3 } }; // 22
			int cardPairIdx_22 = shl.addCardPair(bestCards_22, nPlayers, true); // add one winning pair
			shl.addCardPair(bestCards_22, nPlayers, true); //
			shl.addCardPair(bestCards_22, nPlayers, true); // 

			// add best card as winner wice: (register 2 wins)
			Card nextBestCards_33[] = { { 1, 2 }, { 1, 3 } }; // 33
			int cardPairIdx_33 = shl.addCardPair(nextBestCards_33, nPlayers, true);
			shl.addCardPair(nextBestCards_33, nPlayers, true);

			// add best card as winner wice: (register 1 win)
			Card nextBestCards_QKo[] = { { 10, 2 }, { 11, 3 } }; // 33
			int cardPairIdx_QKo = shl.addCardPair(nextBestCards_QKo, nPlayers, true);

			Precalculated pre;
			pre.writeResultsToFileBuffer(resultsBuffer, &shl, nPlayers);

			float spotForPair_22 = resultsBuffer[cardPairIdx_22];
			float spotForPair_33 = resultsBuffer[cardPairIdx_33];
			float spotForPair_QKo = resultsBuffer[cardPairIdx_QKo];

			Assert::IsTrue(spotForPair_22 > spotForPair_33); // best winchance > next best winchance
			Assert::IsTrue(spotForPair_33 > spotForPair_QKo); // next best winchance
			Assert::IsTrue(spotForPair_QKo > 0.0f); // 3rd place
		}

		TEST_METHOD(MakeRanking_)
		{
			float rating[169];
			float winchance[] = { 0.01f, 0.02f, 0.5f, 0.04f, 0.05f }; // no order

			MakeRanking aa(5); // functor, 5 items rated
			aa(winchance, rating);

			Assert::IsTrue(rating[4] > 0.5); // next higest, should be equal or better then 50%
			Assert::IsTrue(rating[2] == 1.0); // highest, should be equal or better then 100%
			Assert::IsTrue(rating[0] < 0.5); // lowest, should be less then 50%
			Assert::IsTrue(rating[1] < 0.5); // lowest, should be less then 50%
		}

	};
}