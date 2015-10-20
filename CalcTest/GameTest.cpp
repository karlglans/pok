#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include "PreHandExamination.h"
#include "MiniSim.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CalcTest
{
	TEST_CLASS(WinTests)
	{
	public:

		TEST_METHOD(winTest_winByHighCard)
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