// C++ standard libraries
#include <chrono>
#include <iostream>
#include <thread>

// Custom made libraries
#include "trees.h"
#include "tokens.h"

#include "debug.h"

// Including namespaces
using namespace std;
using namespace tokens;
using namespace trees;

// Delay time
#define DELAY 1
#define DELAY_P 100

extern int stage;

int main()
{
	token_tree <double> tr = token_tree <double> ();
        
        cout << "jere" << endl;
        tr.set_cursor(new ttwrapper <double> (operand <double> {324}));

        cout << "jere" << endl;
	tr.add_branch(new ttwrapper <double> (operand <double> {45}));
	tr.add_branch(new ttwrapper <double> (operand <double> {67}));

        cout << "jere" << endl;
	tr.move_down(0);

	tr.add_branch(new ttwrapper <double> (operand <double> {90}));

	tr.move_up();
	tr.move_right();

	tr.add_branch(new ttwrapper <double> (operand <double> {90}));
	tr.add_branch(new ttwrapper <double> (operand <double> {120}));

	tr.print();
}
