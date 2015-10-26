#include "stdafx.h"
#include "CppUnitTest.h"
#include "MiniSim.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CalcTest
{
	TEST_CLASS(WinTests)
	{
	public:

		TEST_METHOD(MiniSim_rumSim)
		{
			CardDeck deck;
			Card pair[] = { { 12, 2 }, { 12, 3 } }; // {value, color}
			deck.SetPlayerCards(pair);
			MiniSimResults msr;

			MiniSim ms;
			float win = ms.runSim(&deck, 3, 100, &msr);
			Assert::IsTrue(win > 0.0f);
		}
	};
}