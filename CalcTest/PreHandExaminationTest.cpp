#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include "PreHandExamination.h"
#include "MiniSim.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CalcTest
{
	TEST_CLASS(PreHandExamination)
	{
	public:

		TEST_METHOD(HandRating_lessThen)
		{
			HandRating a, b;
			a.values[0] = b.values[0] = 10;
			a.values[1] = b.values[1] = 10;
			a.values[2] = b.values[2] = 10;
			a.values[3] = b.values[3] = 10;
			a.values[4] = b.values[4] = 10;

			a.values[5] = 11;	b.values[5] = 10;
			//Assert::AreEqual(a < b, false);
			Assert::IsFalse(a < b);

			a.values[0] = 11;	b.values[0] = 10;
			//Assert::AreEqual(a < b, false);
			Assert::IsFalse(a < b);

			a.values[0] = 9;	b.values[0] = 10;
			Assert::IsTrue(a < b);
		}

		// flytta
		TEST_METHOD(dealCards)
		{
			//                keep:     keep:     change:       ignore:
			Card cards[] = { { 0, 1 }, { 1, 2 }, { 100, 100 }, { 101, 101 } };
			Deck d; d.reset();
			d.deal(cards, 2, 1); // deal 1 more card after first 2
			Assert::AreNotEqual(100, cards[2].value); // should have been changed
			Assert::AreEqual(101, cards[3].value); // should NOT have been changed

			// spot for card2 should have been marked as taken
			int takenPos = cards[1].color * 13 + cards[1].value;
			Assert::AreEqual((char)1, d.taken[takenPos]);

			// spot for card3 should have been marked as taken
			takenPos = cards[2].color * 13 + cards[2].value;
			Assert::AreEqual((char)1, d.taken[takenPos]);
		}


		
		
	};
}