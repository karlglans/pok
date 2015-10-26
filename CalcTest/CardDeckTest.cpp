#include "stdafx.h"
#include "CppUnitTest.h"
#include "CardDeck.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CalcTest
{
	TEST_CLASS(WinTests)
	{
	public:

		TEST_METHOD(CarDeck_addCards)
		{
			CardDeck newDeck;
			Card pair[] = { { 0, 2 }, { 1, 3 } }; // {value, color}

			// add first player cards
			newDeck.SetPlayerCards(pair);
			Assert::AreEqual(2, newDeck.countAddedCards());

			// add more cards from the game. It will internaly filter out previusly added cards

			Card flop[] = { { 1, 2 }, { 3, 2 }, { 4, 2 }, { 5, 2 }, {6, 2 }};
			newDeck.SetTableCards(flop, 3); // add 3 more cards
			Assert::AreEqual(5, newDeck.countAddedCards());

			newDeck.SetTableCards(flop, 4); // add 1 more cards
			Assert::AreEqual(6, newDeck.countAddedCards());

			newDeck.SetTableCards(flop, 5); // add 1 more cards
			Assert::AreEqual(7, newDeck.countAddedCards());
		}

		TEST_METHOD(CarDeck_deal_canDealAll5TableCards)
		{
			CardDeck newDeck;
			Card pair[] = { { 0, 2 }, { 1, 3 } }; // {value, color}
			newDeck.SetPlayerCards(pair);

			int nonsensValue = 20;
			newDeck._cards[0].value = nonsensValue;
			newDeck._cards[1].value = nonsensValue;
			newDeck._cards[2].value = nonsensValue;
			newDeck._cards[3].value = nonsensValue;
			newDeck._cards[4].value = nonsensValue;
			newDeck._cards[5].value = nonsensValue; // player card1, keep

			newDeck.deal(2); // 2 = number of players, irrelevant for the test

			// should have changed after a deal
			Assert::AreNotEqual(nonsensValue, newDeck._cards[0].value);
			Assert::AreNotEqual(nonsensValue, newDeck._cards[1].value);
			Assert::AreNotEqual(nonsensValue, newDeck._cards[2].value);
			Assert::AreNotEqual(nonsensValue, newDeck._cards[3].value);
			Assert::AreNotEqual(nonsensValue, newDeck._cards[4].value);
			Assert::AreEqual(nonsensValue, newDeck._cards[5].value); // should not have changed
		}

		TEST_METHOD(CarDeck_deal_canDealWithouthPlayerCards)
		{
			CardDeck newDeck;
			int tableCard4 = 3, idxPalyerFirstCard = 5, idxOpp2FirstCard = 9, idxOpp3FirstCard = 11;

			int nonsensValue = 20;
			newDeck._cards[tableCard4].value = nonsensValue;
			newDeck._cards[idxPalyerFirstCard].value = nonsensValue;
			newDeck._cards[idxOpp2FirstCard].value = nonsensValue;
			newDeck._cards[idxOpp3FirstCard].value = nonsensValue;

			newDeck.deal(3); // there should be an opponent 2, but no opponent 3

			Assert::AreNotEqual(nonsensValue, newDeck._cards[tableCard4].value);
			Assert::AreNotEqual(nonsensValue, newDeck._cards[idxPalyerFirstCard].value);
			Assert::AreNotEqual(nonsensValue, newDeck._cards[idxOpp2FirstCard].value);
			Assert::AreEqual(nonsensValue, newDeck._cards[idxOpp3FirstCard].value);
		}

		TEST_METHOD(CarDeck_deal_canDealCardsForOpponent)
		{
			CardDeck newDeck;
			Card pair[] = { { 0, 2 }, { 1, 3 } }; // {value, color}
			newDeck.SetPlayerCards(pair);

			int nonsensValue = 20;
			int idxOpp1FirstCard = 7, idxOpp2FirstCard = 9;
			newDeck._cards[idxOpp1FirstCard].value = nonsensValue; // opponent 1 first card
			newDeck._cards[idxOpp2FirstCard].value = nonsensValue; // opponent 2 first card

			newDeck.deal(2); // 2 = number of players

			Assert::AreNotEqual(nonsensValue, newDeck._cards[idxOpp1FirstCard].value); // should have changed
			Assert::AreEqual(nonsensValue, newDeck._cards[idxOpp2FirstCard].value); // should not have been changed, sinc only 2 players in game
		}
	};
}