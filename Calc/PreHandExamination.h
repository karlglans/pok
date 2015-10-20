#pragma once

struct ValueFreq {
	int value; // 0-12
	int freq; // 0-4,
};

struct ColorFreq {
	int color; // 0-4
	int freq; // 0-7,
};

// Used for storing data that is to be used for evaluating hands
struct PreHandExamination {
	static const int mostFrequent = 1;
	int valueFreq[13];
	int colorFreq[4];
	unsigned int colorHand[4]; // bitMasks. One bit for each card.
	unsigned int uniHand; // bitMask for card valuses, all colors incuded
	void reset();
	PreHandExamination() { reset();  }
};

