#pragma once
// represents a pair of start cards in a dence form. 
// 169 combinations from like AA, AKs, AKo
struct PairPos {
	int pos; // 0-168
	int set(int high, int low, bool suit);
	void get(int *high, int *low, bool *suit);
};
