#include "CommonTypes.h"

bool HandRating::operator <(HandRating const& rhs) {
	if (values[0] != rhs.values[0])
		return values[0] < rhs.values[0];
	else if (values[1] != rhs.values[1])
		return values[1] < rhs.values[1];
	else if (values[2] != rhs.values[2])
		return values[2] < rhs.values[2];
	else if (values[3] != rhs.values[3])
		return values[3] < rhs.values[3];
	else if (values[4] != rhs.values[4])
		return values[4] < rhs.values[4];
	else if (values[5] != rhs.values[5])
		return values[5] < rhs.values[5];
	return false;
}
bool HandRating::operator == (HandRating const& rhs) {
	if (values[0] == rhs.values[0] &&
		values[1] == rhs.values[1] &&
		values[2] == rhs.values[2] &&
		values[3] == rhs.values[3] &&
		values[4] == rhs.values[4] &&
		values[5] == rhs.values[5])
		return true;
	return false;
}
void HandRating::reset() {
	values[0] = values[1] = values[2] = values[3] = values[4] = values[5] = 0;
}
