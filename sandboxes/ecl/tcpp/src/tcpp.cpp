//============================================================================
// Name        : tcpp.cpp
// Author      : Serguei Koubli
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

char buff[3] = {0, 0, 0};

int main() {

	buff[0] = 'D';
	buff[1] = 'c';

	cout << "shm-cons" << buff <<endl; // prints shm-cons
	return 0;
}
