/*
 * ttimer.cpp
 *
 *  Created on: 2015-01-30
 *      Author: sergueik
 */

#include <iostream>

#include <libhrt/timer.h>

using namespace std;
#define NREP (100*1000*1000)

uint64_t ft(uint64_t x){
	uint64_t y=x;
	return y;
}

int main() {
	hrt_init(); hrt_sync_clock();
	hrt_timer x = hrt_timer_now();
	hrt_timer t1=hrt_timer_now();
	for ( int i=0; i<NREP; i++ ){
		static hrt_timer sprev = x;
		x=ft(i)+1;
		sprev=x;
	}
	int64_t elapsed = hrt_timer_get_elapsed( &t1 );
	cout 	<< hrt_to_seconds(elapsed) << " s\t"
			<< (hrt_to_nano_seconds(elapsed) / NREP) <<" ns\t"
			<< x << endl;

	t1 = x = hrt_timer_now();
	for ( int i=0; i<NREP; i++ ){
		static hrt_timer sprev = x;
		hrt_timer e = hrt_timer_now();
		x=e-sprev;
		sprev=e;
	}
	elapsed = hrt_timer_get_elapsed( &t1 );
	cout 	<< hrt_to_seconds(elapsed) << " s\t"
			<< (hrt_to_nano_seconds(elapsed) / NREP) <<" ns\t"
			<< x << endl;

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}


