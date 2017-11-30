#include "function.h"

using namespace std;

void intValue2Name(string& name, int value) {

	if (value < 10) {
		name = "0000" + to_string(value);
	}
	else if (value < 100) {
		name = "000" + to_string(value);
	}
	else if (value < 1000) {
		name = "00" + to_string(value);
	}
	else if (value < 10000) {
		name = "0" + to_string(value);
	}
	else {
		name = to_string(value);
	}
}