#pragma once

enum gamePhase { preflop, flop, turn, river, over};
enum gameAction { notActed, blinds, fold, check, call, raise };

struct Card {
	int value; // 0-12,	
	int color; // 0-3
};

struct HandRating {
	enum Hand { undefined, highcard, pair, twoPair, three, straight, flush, house, four, straFlush };
	enum HandValue { bestHand, bestCard };
	int values[6]; // 1:highcard, 2:pair, 3:two-pair, 3:three, 4: straight, 5: flush, 6: house, 7:four, 8: stra flush
	bool operator <(HandRating const& rhs);
	bool operator ==(HandRating const& rhs);
	void reset();
	HandRating() {
		reset();
	}
};