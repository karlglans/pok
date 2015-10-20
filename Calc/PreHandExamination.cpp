
#include "PreHandExamination.h"

void PreHandExamination::reset() {
	for (int v = 0; v < 13; v++){
		valueFreq[v] = 0;
	}
	for (int co = 0; co < 4; co++){
		colorFreq[co] = 0;
		colorHand[co] = 0;
	}
	uniHand = 0;
}

