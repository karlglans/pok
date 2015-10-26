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
			Assert::IsFalse(a < b);

			a.values[0] = 11;	b.values[0] = 10;
			Assert::IsFalse(a < b);

			a.values[0] = 9;	b.values[0] = 10;
			Assert::IsTrue(a < b);
		}
	};
}