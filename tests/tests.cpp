#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include <complex>
#include <type_traits>

#include <gmpxx.h>

/*
#include "../engine/barn.h"
#include "../engine/node.h"
#include "../engine/polynomial.h"
#include "../engine/Rational.h"
#include "../engine/Complex.h"
*/

#include <barn.h>
#include <node.h>
#include <polynomial.h>
#include <rational.h>
#include <complex.h>

using namespace std;

int main()
{
	string line;

	while (getline(cin, line)) {
		cout << "In: " << line << endl;

		node <double, int> nd = string(line);

		cout << "Out: " << nd.display() << "\n" << endl;
	}
}
