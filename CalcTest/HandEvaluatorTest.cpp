#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include "PreHandExamination.h"
#include "MiniSim.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CalcTest
{
	TEST_CLASS(HandEvaluatorTest)
	{
	public:

		TEST_METHOD(HandEvaluator_addCardCount)
		{
			HandEvaluator ms;
			Card someCards[4];
			someCards[0].value = 1;	/*<--*/	someCards[0].color = 1;
			someCards[1].value = 1;	/*<--*/	someCards[1].color = 2;
			someCards[2].value = 1;	/*<--*/	someCards[2].color = 0; //<--
			someCards[3].value = 3;			someCards[3].color = 0; //<--

			PreHandExamination result; result.reset();
			ms.addCardCount(someCards, 4, &result);

			Assert::AreEqual(3, result.valueFreq[1]); // found 3 x value 1
			Assert::AreEqual(2, result.colorFreq[0]); // found 2 x color 2
		}

		TEST_METHOD(HandEvaluator_addCardCount_CanAccumelateResult)
		{
			PreHandExamination phe; HandEvaluator he;
			Card cards[] = { { 0, 0 }, { 12, 3 } };
			he.addCardCount(cards, 2, &phe);

			// checking if counting color and value was ok
			Assert::AreEqual(1, phe.colorFreq[0]);
			Assert::AreEqual(0, phe.valueFreq[1]);
			Assert::AreEqual(1, phe.valueFreq[12]);

			// test 2, adding same cards again 
			he.addCardCount(cards, 2, &phe);
			Assert::AreEqual(2, phe.colorFreq[0]);
			Assert::AreEqual(0, phe.valueFreq[1]);
			Assert::AreEqual(2, phe.valueFreq[12]);
		}

		TEST_METHOD(HandEvaluator_preExamCards)
		{
			PreHandExamination* phe = 0; HandEvaluator he; int player1 = 0, player2 = 1;
			Card cards[] = { { 0, 1 }, { 1, 1 }, { 10, 2 }, { 11, 2 }, { 12, 2 } // <-- shared
			, { 10, 1 }, { 2, 3 } // <-- player 1, gets a exra 10
			, { 9, 2 }, { 8, 2 } // <-- player 2, has more color 2
			};
			phe = he.preExamCards(cards, 2);
			// expect differences between hands
			Assert::AreNotEqual(phe[player1].colorFreq[2], phe[player2].colorFreq[2]);
			Assert::AreNotEqual(phe[player1].valueFreq[10], phe[player2].valueFreq[10]);
			// expect same, both has just one card value 0.
			Assert::AreEqual(phe[player1].valueFreq[0], phe[player1].valueFreq[0]);
		}

		TEST_METHOD(HandEvaluator_addColorBits_ValuesToBits)
		{
			Card card[5];

			// should add up to:   0001 0000 0101, for color 0
			// should add up to:   0000 0101 0000, for color 3
			// should add up to:   0001 0101 0101, for union
			card[0].value = 0;			card[0].color = 0;
			card[1].value = 2;			card[1].color = 0;
			card[2].value = 4;			card[2].color = 3;
			card[3].value = 6;			card[3].color = 3;
			card[4].value = 8;			card[4].color = 0;

			PreHandExamination phe;
			HandEvaluator he;
			he.addColorBits(card, 5, &phe);

			Assert::AreEqual((unsigned int)bin<100000101>::value, phe.colorHand[0]);
			Assert::AreEqual((unsigned int)bin<1010000>::value, phe.colorHand[3]);
			Assert::AreEqual((unsigned int)bin<101010101>::value, phe.uniHand);
		}
		
		// 0  -->  1
		// 1  -->  10
		// 2  -->  100
		// n  -->  2^n

		TEST_METHOD(HandEvaluator_addColorBits_WillAccumelateResult)
		{
			Card card[5];
			card[0].color = 3;			card[0].value = 4; // 10000
			card[1].color = 3;			card[1].value = 5; // 100000
			PreHandExamination phe;
			HandEvaluator he;

			// accumelating result from bits made from cards to result in last arg
			he.addColorBits(&card[0], 1, &phe); // 1st card added to bit-structure
			he.addColorBits(&card[1], 1, &phe); // 2nd card added to bit-structure

			Assert::AreEqual((unsigned int)bin<110000>::value, phe.colorHand[3]);
			Assert::AreEqual((unsigned int)bin<110000>::value, phe.uniHand);
		}

		TEST_METHOD(HandEvaluator_makeRatedFlushHand)
		{
			HandEvaluator he;
			HandRating* hr = he.makeRatedFlushHand(bin<1>::value);
			Assert::AreEqual(1, hr->values[HandRating::bestCard]);
			
			int aceBit = 4096; // too high for macro
			int bitHand = aceBit + bin<110111>::value; // ace bit is not showing in macro, but still added
			hr = he.makeRatedFlushHand(bitHand);

			// only 5 cards in a flush hand. The last card will be ignored
			Assert::AreEqual(aceBit, hr->values[HandRating::bestCard]);
			Assert::AreEqual((int)bin<100000>::value, hr->values[HandRating::bestCard + 1]);
			Assert::AreEqual((int)bin<10000>::value, hr->values[HandRating::bestCard + 2]);
			Assert::AreEqual((int)bin<100>::value, hr->values[HandRating::bestCard + 3]);
			Assert::AreEqual((int)bin<10>::value, hr->values[HandRating::bestCard + 4]); // lowest card is the 3, 
		}

		TEST_METHOD(HandEvaluator_findStraightFlush)
		{
			PreHandExamination phe; HandEvaluator he; HandRating* handRated = NULL;

			// Case 1: There is a straight flush
			phe.colorFreq[2] = 5;
			phe.colorHand[2] = (unsigned int)bin<11111000>::value; // higher straighs will have higher value
			handRated = he.findStraightFlush(&phe); 
			Assert::AreEqual((int)HandRating::straFlush, (int)handRated->values[HandRating::bestHand]);
			int sumOfLastValues = handRated->values[HandRating::bestCard + 1] + handRated->values[HandRating::bestCard + 2] +
				handRated->values[HandRating::bestCard + 3] + handRated->values[HandRating::bestCard + 4];
			Assert::AreEqual(0, sumOfLastValues); // last values will be 0

			// Case 2: there is no flush to be found
			phe.colorFreq[2] = 4;
			phe.colorHand[2] = (unsigned int)bin<111001000>::value;
			handRated = he.findStraightFlush(&phe);
			Assert::AreEqual((int)HandRating::undefined, handRated->values[HandRating::bestHand]);
		}

		TEST_METHOD(HandEvaluator_findStraight)
		{
			PreHandExamination phe; HandEvaluator he; HandRating* handRated = NULL;

			// Case 1: Basic, There is a straight
			phe.uniHand = (unsigned int)bin<11111>::value; // higher straighs will have higher value
			handRated = he.findStraightFlush(&phe);
			Assert::AreEqual((int)HandRating::straight, handRated->values[HandRating::bestHand]);
			int nextLowestStraight = (int)handRated->values[HandRating::bestCard];
			int sumOfLastValues = handRated->values[HandRating::bestCard + 1] + handRated->values[HandRating::bestCard + 2] +
				handRated->values[HandRating::bestCard + 3] + handRated->values[HandRating::bestCard + 4];
			Assert::AreEqual(0, sumOfLastValues); // last values will all be 0

			// Case 2: Negative, there is no straight
			phe.uniHand = (unsigned int)bin<111011000>::value;
			handRated = he.findStraightFlush(&phe);
			Assert::AreEqual((int)HandRating::undefined, handRated->values[HandRating::bestHand]);

			// Case 3: Top straight
			phe.uniHand = (unsigned int)7936; // 111110000000
			handRated = he.findStraightFlush(&phe);
			Assert::AreEqual((int)HandRating::straight, handRated->values[HandRating::bestHand]);
			int topStraightValue = (int)handRated->values[HandRating::bestCard];

			// Case 4: Bottom straight, ace as 1
			phe.uniHand = (unsigned int)4111; // 100000001111
			handRated = he.findStraightFlush(&phe);
			Assert::AreEqual((int)HandRating::straight, handRated->values[HandRating::bestHand]);
			int lowestStraight = (int)handRated->values[HandRating::bestCard];

			// compair straights
			Assert::IsTrue(nextLowestStraight > lowestStraight);
			Assert::IsTrue(topStraightValue > nextLowestStraight);
		}

		TEST_METHOD(HandEvaluator_findFlush)
		{
			PreHandExamination phe; HandEvaluator he; HandRating* handRated = NULL;

			// Case 1: There is a straight
			int someColor = 3;
			phe.colorFreq[someColor] = 5;
			phe.colorHand[someColor] = (unsigned int)bin<11101100>::value;
			handRated = he.findStraightFlush(&phe);
			Assert::AreEqual((int)HandRating::flush, handRated->values[HandRating::bestHand]);
			// the 5 cards in this flush hand:
			Assert::AreEqual((int)bin<10000000>::value, handRated->values[HandRating::bestCard]);
			Assert::AreEqual((int)bin<1000000>::value, handRated->values[HandRating::bestCard + 1]);
			Assert::AreEqual((int)bin<100000>::value, handRated->values[HandRating::bestCard + 2]);
			Assert::AreEqual((int)bin<1000>::value, handRated->values[HandRating::bestCard + 3]);
			Assert::AreEqual((int)bin<100>::value, handRated->values[HandRating::bestCard + 4]);
		}


		TEST_METHOD(HandEvaluator_findHands_findFour)
		{
			PreHandExamination phe; HandEvaluator he; HandRating* handRated = NULL;

			// Case 1: basic case
			// 4x3					1x12					redundant
			phe.valueFreq[3] = 4;	phe.valueFreq[12] = 1;	phe.valueFreq[5] = 2;
			handRated = he.findHouse(&phe);
			Assert::AreEqual((int)HandRating::four, handRated->values[HandRating::bestHand]);
			Assert::AreEqual(3, handRated->values[HandRating::bestCard]);
			Assert::AreEqual(12, handRated->values[HandRating::bestCard + 1]);
			phe.reset();

			// Case 2: Should find last (*) card from a pair
			// 4x3					2x12 *					redundant
			phe.valueFreq[3] = 4;	phe.valueFreq[12] = 2;	phe.valueFreq[5] = 1;
			handRated = he.findHouse(&phe);
			Assert::AreEqual((int)HandRating::four, (int)handRated->values[HandRating::bestHand]);
			Assert::AreEqual(3, (int)handRated->values[HandRating::bestCard]);
			Assert::AreEqual(12, (int)handRated->values[HandRating::bestCard + 1]);
			phe.reset();

			// Case 3: Should find last (*) card from a three
			// 4x3					3x11 *
			phe.valueFreq[3] = 4;	phe.valueFreq[11] = 3;
			handRated = he.findHouse(&phe);
			Assert::AreEqual((int)HandRating::four, (int)handRated->values[HandRating::bestHand]);
			Assert::AreEqual(3, (int)handRated->values[HandRating::bestCard]);
			Assert::AreEqual(11, (int)handRated->values[HandRating::bestCard + 1]);
		}

		TEST_METHOD(HandEvaluator_findHands_findHouse)
		{
			PreHandExamination phe; HandEvaluator he; HandRating* handRated = NULL;

			// Case1: the 3s are lower value then the 2s:
			// 3x3					2x12					redundant
			phe.valueFreq[3] = 3;	phe.valueFreq[12] = 2;	phe.valueFreq[4] = 2;
			handRated = he.findHouse(&phe);
			Assert::AreEqual((int)HandRating::house, (int)handRated->values[HandRating::bestHand]);
			Assert::AreEqual(3, (int)handRated->values[HandRating::bestCard]);
			Assert::AreEqual(12, (int)handRated->values[HandRating::bestCard + 1]);

			phe.reset();
			// Case2: the 3s are higher value then the 2s:
			// 3x6					2x5						redundant
			phe.valueFreq[6] = 3;	phe.valueFreq[5] = 2;	phe.valueFreq[4] = 2;
			handRated = he.findHouse(&phe);
			Assert::AreEqual((int)HandRating::house, (int)handRated->values[HandRating::bestHand]);
			Assert::AreEqual(6, (int)handRated->values[HandRating::bestCard]);
			Assert::AreEqual(5, (int)handRated->values[HandRating::bestCard + 1]);

			phe.reset();
			// Case3: tho 3s
			// 3x6					3x5						redundant
			phe.valueFreq[6] = 3;	phe.valueFreq[5] = 3;	phe.valueFreq[4] = 2;
			handRated = he.findHouse(&phe);
			Assert::AreEqual((int)HandRating::house, (int)handRated->values[HandRating::bestHand]);
			Assert::AreEqual(6, (int)handRated->values[HandRating::bestCard]);
			Assert::AreEqual(5, (int)handRated->values[HandRating::bestCard + 1]);
		}

		TEST_METHOD(HandEvaluator_findHands_findThree)
		{
			PreHandExamination phe; HandEvaluator he; HandRating* handRated = NULL;
			// 3x4					1x12					1x5
			phe.valueFreq[4] = 3;	phe.valueFreq[12] = 1;	phe.valueFreq[5] = 1;
			handRated = he.findHouse(&phe);
			Assert::AreEqual((int)HandRating::three, (int)handRated->values[HandRating::bestHand]);
			Assert::AreEqual(4, (int)handRated->values[HandRating::bestCard]);
			Assert::AreEqual(12, (int)handRated->values[HandRating::bestCard + 1]);
			Assert::AreEqual(5, (int)handRated->values[HandRating::bestCard + 2]);
		}

		TEST_METHOD(HandEvaluator_findHands_findTwoPair)
		{
			PreHandExamination phe; HandEvaluator he; HandRating* handRated = NULL;
			// case 1: basic case, 2 pairs
			// 2x11					2x3						1x5
			phe.valueFreq[11] = 2;	phe.valueFreq[3] = 2;	phe.valueFreq[5] = 1;
			handRated = he.findHouse(&phe);
			Assert::AreEqual((int)HandRating::twoPair, (int)handRated->values[HandRating::bestHand]);
			Assert::AreEqual(11, (int)handRated->values[HandRating::bestCard]);
			Assert::AreEqual(3, (int)handRated->values[HandRating::bestCard + 1]);
			Assert::AreEqual(5, (int)handRated->values[HandRating::bestCard + 2]);
			phe.reset();

			// case 2: last card (*) also from a pair
			// 2x11					2x5						2x2	*					1x1
			phe.valueFreq[11] = 2;	phe.valueFreq[5] = 2;	phe.valueFreq[2] = 2;	phe.valueFreq[1] = 1;
			handRated = he.findHouse(&phe);
			Assert::AreEqual((int)HandRating::twoPair, (int)handRated->values[HandRating::bestHand]);
			Assert::AreEqual(11, (int)handRated->values[HandRating::bestCard]);
			Assert::AreEqual(5, (int)handRated->values[HandRating::bestCard + 1]);
			Assert::AreEqual(2, (int)handRated->values[HandRating::bestCard + 2]);
		}

		TEST_METHOD(HandEvaluator_findHands_findPair)
		{
			PreHandExamination phe; HandEvaluator he; HandRating* handRated = NULL;
			// 2x0 (0=two)			1x12					1x5						1x3
			phe.valueFreq[0] = 2;	phe.valueFreq[12] = 1;	phe.valueFreq[5] = 1;	phe.valueFreq[3] = 1;
			handRated = he.findHouse(&phe);
			Assert::AreEqual((int)HandRating::pair, (int)handRated->values[HandRating::bestHand]);
			Assert::AreEqual(0, (int)handRated->values[HandRating::bestCard]);
			Assert::AreEqual(12, (int)handRated->values[HandRating::bestCard + 1]);
			Assert::AreEqual(5, (int)handRated->values[HandRating::bestCard + 2]);
			Assert::AreEqual(3, (int)handRated->values[HandRating::bestCard + 3]);
		}

		TEST_METHOD(HandEvaluator_findHands_highCard)
		{
			PreHandExamination phe; HandEvaluator he; HandRating* handRated = NULL;
			phe.valueFreq[11] = 1;	phe.valueFreq[10] = 1;	phe.valueFreq[6] = 1;	phe.valueFreq[5] = 1;	phe.valueFreq[2] = 1;
			handRated = he.findHouse(&phe);
			Assert::AreEqual((int)HandRating::highcard, (int)handRated->values[HandRating::bestHand]);
			Assert::AreEqual(11, (int)handRated->values[HandRating::bestCard]);
			Assert::AreEqual(10, (int)handRated->values[HandRating::bestCard + 1]);
			Assert::AreEqual(6, (int)handRated->values[HandRating::bestCard + 2]);
			Assert::AreEqual(5, (int)handRated->values[HandRating::bestCard + 3]);
			Assert::AreEqual(2, (int)handRated->values[HandRating::bestCard + 4]);
		}

		TEST_METHOD(HandEvaluator_won)
		{
			PreHandExamination* phe = 0; HandEvaluator he;

			// case 1: some1 else has a stromger hand
			Card deal1cards[] = { { 0, 1 }, { 1, 1 }, { 10, 2 }, { 11, 2 }, { 12, 2 } // <-- shared
			, { 10, 1 }, { 2, 3 } // <-- player, gets a extra 10
			, { 9, 2 }, { 8, 2 } // <-- opponent 1, who has a straight flush
			};
			HandRating bestHand;
			bool won = he.won(2, deal1cards, &bestHand);
			Assert::IsFalse(won);

			// case 2: player has the best hand
			Card cards_v2[] = { { 0, 1 }, { 1, 1 }, { 10, 2 }, { 11, 2 }, { 12, 2 } // <-- shared
			, { 9, 2 }, { 8, 2 } // <-- player 0, has a flush
			, { 10, 1 }, { 2, 3 } // <-- opponent, gets a extra 10
			};
			won = he.won(2, cards_v2, &bestHand);
			Assert::IsTrue(won);


			// case 3: player looses to a straight
			Card cards_v3[] = { { 0, 1 }, { 1, 1 }, { 10, 2 }, { 11, 2 }, { 12, 2 } // <-- shared
			, { 10, 1 }, { 2, 3 } // <-- player, pair 10
			, { 9, 2 }, { 8, 1 } // <-- opponent, straight
			};
			won = he.won(2, cards_v3, &bestHand);
			Assert::IsFalse(won);


			// case 4: player looses to a hight card hand
			Card cards_v4[] = { { 0, 1 }, { 1, 1 }, { 4, 2 }, { 5, 2 }, { 6, 2 } // <-- shared
			, { 8, 1 }, { 9, 3 } // <-- player, HK9
			, { 8, 3 }, { 9, 1 } // <-- opponent, HK9
			, { 8, 2 }, { 10, 1 } // <-- opponent, HK 10
			};
			won = he.won(3, cards_v4, &bestHand);
			Assert::AreEqual((int)HandRating::Hand::highcard, bestHand.values[0]);
		}

		TEST_METHOD(HandEvaluator_won_oneWinner)
		{
			PreHandExamination* phe = 0; HandEvaluator he; StartHandLogger shl;

			// case 1: some1 else has a stromger hand
			Card deal1cards[] = { { 0, 1 }, { 1, 1 }, { 10, 2 }, { 11, 2 }, { 12, 2 } // <-- shared
			, { 10, 1 }, { 2, 3 } // <-- Q4o, player, gets a extra 10
			, { 9, 2 }, { 8, 2 } // <-- Kn10s, opponent 1, who has a straight flush
			};
			HandRating bestHand;
			int numbPlayers = 2;
			bool won = he.won(numbPlayers, deal1cards, &bestHand, &shl);
			Assert::IsFalse(won);

			int cardPair_Q4o = shl.makeCardPairValue(&deal1cards[5]);
			int cardPair_Kn10s = shl.makeCardPairValue(&deal1cards[5 + 2]);

			int Q4o_count = shl.getStartHandCount(numbPlayers, cardPair_Q4o);
			int Kn10s_count = shl.getStartHandCount(numbPlayers, cardPair_Kn10s);
			int Q4o_WinCount = shl.getWinningPairCount(numbPlayers, cardPair_Q4o);
			int Kn10s_WinCount = shl.getWinningPairCount(numbPlayers, cardPair_Kn10s);

			Assert::AreEqual(1, Q4o_count);
			Assert::AreEqual(1, Kn10s_count);
			Assert::AreEqual(0, Q4o_WinCount);
			Assert::AreEqual(1, Kn10s_WinCount);
		}

		TEST_METHOD(HandEvaluator_won_multipleWinners)
		{
			PreHandExamination* phe = 0; HandEvaluator he; StartHandLogger shl;

			// case 1: some1 else has a stromger hand
			Card deal1cards[] = { { 0, 1 }, { 1, 1 }, { 10, 2 }, { 11, 2 }, { 12, 2 } // <-- shared
			, { 10, 1 }, { 2, 3 } // <-- Q4o, player, gets a extra 10
			, { 10, 3 }, { 2, 1 } // <-- Q4o, opponent, same hand
			};
			HandRating bestHand;
			int numbPlayers = 2;
			bool won = he.won(numbPlayers, deal1cards, &bestHand, &shl);
			Assert::IsTrue(won);

			int cardPair_Q4o = shl.makeCardPairValue(&deal1cards[5]);

			int Q4o_count = shl.getStartHandCount(numbPlayers, cardPair_Q4o);
			int Q4o_WinCount = shl.getWinningPairCount(numbPlayers, cardPair_Q4o);

			Assert::AreEqual(2, Q4o_count);
			Assert::AreEqual(2, Q4o_WinCount);
		}

		TEST_METHOD(HandEvaluator_winTest_winByHighCard)
		{
			PreHandExamination* phe = 0; HandEvaluator he; HandRating bestHand;
			Card cards_v4[] = { { 0, 1 }, { 1, 1 }, { 4, 2 }, { 5, 2 }, { 6, 2 } // <-- shared
			, { 8, 1 }, { 9, 3 } // <-- player, HK9
			, { 8, 3 }, { 9, 1 } // <-- opponent, HK9
			, { 8, 2 }, { 10, 1 } // <-- opponent, HK 10
			};
			bool won = he.won(3, cards_v4, &bestHand);
			Assert::IsFalse(won);
			Assert::AreEqual((int)HandRating::Hand::highcard, bestHand.values[0]);
			Assert::AreEqual((int)10, bestHand.values[HandRating::bestCard]);
		}

	};
}