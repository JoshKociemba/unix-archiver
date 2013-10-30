#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include "myar.h"

int main(int argc, char **argv) {
	
	int opt;
	int sel_opt = 0;

	while ((opt = getopt(argc, argv, "vqxtdAw")) != -1) {
		switch (opt) {
			case 'v':
				sel_opt = 1;
				break;	
			case 'q':
				sel_opt = 2;
				break;
			case 'x':
				sel_opt = 3;
				break;
			case 't':
				sel_opt = 4;
				break;
			case 'd':
				sel_opt = 5;
				break;
			case 'A':
				sel_opt = 6;
			case 'w':
				sel_opt = 7;
			default:
				break;
				
		}
	}

	if (sel_opt == 0) {
		printf("Correct options not selected\n");
	}

	

}
