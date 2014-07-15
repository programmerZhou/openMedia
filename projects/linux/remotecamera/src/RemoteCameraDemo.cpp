//============================================================================
// Name        : remotecamera.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <getopt.h>
#include <iostream>

#include "Monitor.h"

using namespace std;

bool isTerminate = false;

int main() {

	const char* file = "test.jpg";

	Monitor **monitor = 0;

	Monitor::LoadFileMonitors(file, monitor, Monitor::CAPTURE);

	if(monitor[0]->PrimeCapture() < 0){
		printf("prime capture is less than zero\n");
	}

	while(!isTerminate){

		sleep(1);

		if(true){

			monitor[0]->PreCapture();

			monitor[0]->Capture();

			monitor[0]->PostCapture();
		}
	}


	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
