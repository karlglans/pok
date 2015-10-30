#include <stdexcept>
#include "PairPos.h"


int PairPos::set(int high, int low, bool suit) {
	pos = 0;
	if (suit == false) {
		for (int h = 0; h < 13; h++){
			for (int l = 0; l <= h; l++){
				if (h == high && l == low)
					return pos;
				pos++;
			}
		}
		return pos;
	}
	pos = 91; // 13*13/2 + 13 = 78 + 13
	for (int h = 1; h < 13; h++){
		for (int l = 0; l < h; l++){
			if (h == l) // suited cards cant be pairs
				continue;
			if (h == high && l == low)
				return pos;
			pos++;
		}
	}
	return pos;
}

void PairPos::get(int *high, int *low, bool *suit) {
	int poss = 0;
	if (pos < 91) {
		*suit = false;
		for (int h = 0; h < 13; h++){
			for (int l = 0; l <= h; l++){
				if (poss == pos) {
					*high = h;
					*low = l;
					return;
				}
				poss++;
			}
		}
	}
	*suit = true;
	poss = 91; // 13*13/2 + 13 = 78 + 13
	for (int h = 1; h < 13; h++){
		for (int l = 0; l < h; l++){
			if (h == l) // suited cards cant be pairs
				continue;
			if (poss == pos) {
				*high = h;
				*low = l;
				return;
			}
			poss++;
		}
	}
}

// the structure can be used as key to index between 169 fields
int PairPos::set(const Card* pCards) {
	int highest = pCards[0].value;
	int lowest = pCards[1].value;
	int b;
	if (lowest > highest) {
		b = lowest;
		lowest = highest;
		highest = b;
	}
#ifdef _DEBUG
	if (lowest == highest && pCards[0].color == pCards[1].color)
		throw std::invalid_argument("Suited pair can not be pairs");
#endif
	return set(highest, lowest, pCards[0].color == pCards[1].color);
}
