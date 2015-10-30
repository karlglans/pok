#include "stdafx.h"
#include "CppUnitTest.h"
#include "SeatActionHistory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CalcTest
{
	TEST_CLASS(SeatActionHistoryTests)
	{
	public:

		TEST_METHOD(SeatActionHistory_findNextActiveSeat_canSkipFoldedSeats)
		{
			int nSeats = 5, dealer = 1;
			SeatActionHistory seatActHist;
			//                  seat:   dealer:1  sb:2, bb:3  
			seatActHist.newGame(nSeats, dealer,   50,   100);
			seatActHist.insertBet(1000); // seat4
			seatActHist.insertBet(0); // seat0, folds

			// since Seat0 folds, the next seat after 4 should be 1
			Assert::AreEqual(1, seatActHist.findNextActiveSeat(4));
		}

		TEST_METHOD(SeatActionHistory_countActiveSeats)
		{
			int nSeats = 3, dealer = 1;
			SeatActionHistory seatActHist;
			seatActHist.newGame(nSeats, dealer, 50, 100);

			// every1 should be active
			Assert::AreEqual(3, seatActHist.countActiveSeats());

			// fold 1
			seatActHist.insertBet(1000); // seat2, raise
			seatActHist.insertBet(0); // seat0, closes
			Assert::AreEqual(2, seatActHist.countActiveSeats());
		}

		TEST_METHOD(SeatActionHistory_insertBet_getToFlopPhaseByCall)
		{
			int nSeats = 3, dealer = 1;
			SeatActionHistory seatActHist;
			//                 seat:   dealer:1  sb:2, bb:0  
			seatActHist.newGame(nSeats, dealer,  50,   100);

			seatActHist.insertBet(1000); // seat1, raise
			seatActHist.insertBet(1000); // seat2, call
			Assert::AreEqual((int)gamePhase::preflop, (int)seatActHist.getGamePhase()); // still preflopp
			seatActHist.insertBet(1000); // seat0, call <-- raiser s1 called
			Assert::AreEqual((int)gamePhase::flop, (int)seatActHist.getGamePhase());
		}

		TEST_METHOD(SeatActionHistory_insertBet_getToFlopPhaseByFold)
		{
			int nSeats = 3, dealer = 1;
			SeatActionHistory seatActHist;
			//                 seat:   dealer:1  sb:2, bb:0  
			seatActHist.newGame(nSeats, dealer, 50, 100);
			seatActHist.insertBet(1000); // seat1, raise
			seatActHist.insertBet(1000); // seat2, call
			seatActHist.insertBet(0); // seat0, fold <-- raiser s1, still called
			Assert::AreEqual((int)gamePhase::flop, (int)seatActHist.getGamePhase());
		}

		TEST_METHOD(SeatActionHistory_insertBet_getToFlopPhaseByCheck){
			int nSeats = 3, dealer = 1;
			SeatActionHistory seatActHist;
			//                 seat:   dealer:1  sb:2, bb:0  
			seatActHist.newGame(nSeats, dealer, 50, 100);
			seatActHist.insertBet(100); // seat1, call
			seatActHist.insertBet(0); // seat2, fold
			Assert::AreEqual((int)gamePhase::preflop, (int)seatActHist.getGamePhase());
			seatActHist.insertBet(100); // seat0, check <-- 
			Assert::AreEqual((int)gamePhase::flop, (int)seatActHist.getGamePhase());
		}

		TEST_METHOD(SeatActionHistory_insertBet_canRun2ndTurnInPreflopPhase)
		{
			int nSeats = 3, dealer = 1;
			SeatActionHistory seatActHist;
			//                 seat:   dealer:1  sb:2, bb:0  
			seatActHist.newGame(nSeats, dealer, 50, 100);
			seatActHist.insertBet(1000); // seat1, raise
			seatActHist.insertBet(1000); // seat2, call
			seatActHist.insertBet(2000); // seat0, raise 
			seatActHist.insertBet(2000); // seat1, call
			Assert::AreEqual((int)gamePhase::preflop, (int)seatActHist.getGamePhase());
			seatActHist.insertBet(2000); // seat2, call <-- raiser s0 called
			Assert::AreEqual(2, seatActHist.getActiveSeatPos());
			Assert::AreEqual((int)gamePhase::flop, (int)seatActHist.getGamePhase());
		}


		TEST_METHOD(SeatActionHistory_firstActiveSeatIsAfterBigBlind)
		{
			int nSeats = 4, dealer;
			SeatActionHistory seatActHist;  SeatStatusReport report;

			// case 1: basic case, next seat to act is 3 steps after dealer
			// dealer	sb		bb		first
			// s0		s1		s2		s3
			dealer = 0;
			seatActHist.newGame(nSeats, dealer, 50, 100);
			seatActHist.getStatus(&report);
			Assert::AreEqual(dealer + 3, report.currSeatIdx);

			// case 2: can wrap
			// dealer	sb		bb		first
			// s3		s4		s0		s1
			dealer = 2; nSeats = 4;
			seatActHist.newGame(nSeats, dealer, 50, 100);
			seatActHist.getStatus(&report);
			Assert::AreEqual(1, report.currSeatIdx);

			// case 3: two players, small blind should be next to act
			// dealer	sb		bb		
			// s1		s0		s1
			dealer = 1;  nSeats = 2;
			seatActHist.newGame(nSeats, dealer, 50, 100);
			seatActHist.getStatus(&report);
			Assert::AreEqual(0, report.currSeatIdx);
		}

		TEST_METHOD(SeatActionHistory_flopPhasefirstActivePlayer)
		{
			int nSeats = 3, dealer = 1;
			SeatActionHistory seatActHist;  SeatStatusReport report;
			//                 seat:   dealer:1  sb:2, bb:0  
			seatActHist.newGame(nSeats, dealer, 50, 100);
			seatActHist.insertBet(100); // seat1, call
			seatActHist.insertBet(100); // seat2, call
			Assert::AreEqual((int)gamePhase::preflop, (int)seatActHist.getGamePhase());
			seatActHist.insertBet(100); // seat2, call
			seatActHist.getStatus(&report);
			Assert::AreEqual((int)gamePhase::flop, (int)report.phase);
			Assert::AreEqual(dealer + 1, report.currSeatIdx);
		}


		//TEST_METHOD(SeatActionHistory_cantRaseMoreThenOnce)
		//{
		//	int nSeats = 3, dealer = 1;
		//	SeatActionHistory seatActHist;  SeatStatusReport report;
		//	                 seat:   dealer:1  sb:2, bb:0  
		//	seatActHist.newGame(nSeats, dealer, 50, 100);
		//	seatActHist.insertBet(100); // seat1, betRound0, call
		//	seatActHist.insertBet(200); // seat2, betRound0, raise
		//	seatActHist.insertBet(200); // seat0, betRound0, call
		//	seatActHist.insertBet(400); // seat1, betRound1, re-raise

		//	 Now its time for seat2 to act:

		//	seatActHist.getStatus(&report);
		//	Assert::AreEqual((int)gamePhase::preflop, (int)report.phase); // still pre-flopp
		//	Assert::AreEqual(false, report.canRaise); // seat2 cant raise again
		//	seatActHist.insertBet(400); // seat2, call

		//	 Now its time for seat2 to act:

		//	seatActHist.getStatus(&report);
		//	Assert::AreEqual((int)gamePhase::preflop, (int)report.phase); // still pre-flopp
		//	Assert::AreEqual(false, report.canRaise); // seat0 cant raise
		//}


		TEST_METHOD(SeatActionHistory_checkRaise)
		{
			int nSeats = 3, dealer = 1;
			SeatActionHistory seatActHist;  SeatStatusReport report;
			//                 seat:   dealer:1  sb:2, bb:0  
			seatActHist.newGame(nSeats, dealer, 50, 100);

			seatActHist.insertBet(100); // seat1, betRound0, call
			seatActHist.insertBet(100); // seat2, betRound0, raise
			seatActHist.insertBet(100); // seat0, betRound0, call

			seatActHist.getStatus(&report);
			Assert::AreEqual((int)gamePhase::flop, (int)report.phase); 

			// pahse: flop

			//	2	0	1
			//	ch	ch	r
			//	cR	cR	RR    <-- betting round2
			//	c*	c	-

			// round 1:
			seatActHist.insertBet(0); // seat2, check
			seatActHist.insertBet(0); // seat0, check
			seatActHist.insertBet(100); // seat1, raise

			// round 2:
			seatActHist.getStatus(&report);
			Assert::AreEqual(true, report.canRaise);
			seatActHist.insertBet(200); // seat2, check-raise
			seatActHist.getStatus(&report);
			Assert::AreEqual(true, report.canRaise);
			seatActHist.insertBet(300); // seat0, check-raise
			seatActHist.getStatus(&report);
			Assert::AreEqual(true, report.canRaise);
			seatActHist.insertBet(600); // seat1,  re-raise

			// round 3:
			seatActHist.getStatus(&report);
			Assert::AreEqual((int)gamePhase::flop, (int)report.phase);
			Assert::AreEqual(false, report.canRaise);
			seatActHist.insertBet(600); // seat2, call
			seatActHist.insertBet(600); // seat0, call, raiser now called

			// phase: turn
			seatActHist.getStatus(&report);
			Assert::AreEqual((int)gamePhase::turn, (int)report.phase);
			Assert::AreEqual(2, report.currSeatIdx);
		}

	};
}