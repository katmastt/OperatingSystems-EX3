#include <stdio.h>
#include <stdlib.h>
#include "interface_sim.h"

int main(int argc, char *argv[]) {
	
	int S,i, max_pid;
	double l1=1, l2=0.4, l3=0.4, k ;
	S=atoi(argv[1]);
	k=atof(argv[2]);
	max_pid=atoi(argv[3]);
	
	simulation(S , l1, l2, l3, k, max_pid);


    return 0;

}
