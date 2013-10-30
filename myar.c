#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "myar.h"

/**
 * Attempts to open the archive at the given path
 * If no archive exists then it will try to create
 * Calls to arch_write_ghdr
 * Returns file descriptor on success
 * Returns -1 on failure
 **/ 
int arch_open(char *archpath);

/**
 * Attempts to close the archive attached to filedesc
 * Checks to make sure filedesc exists before continuing
 * Calls close and ends
 **/
void arch_close(int filedesc);

/**
 * Attempts to write to the archive's global header
 * Returns 1 on success
 * Returns -1 on failure
 **/
int arch_write_ghdr(int filedesc);

int arch_open(char *archpath) {
	
	struct stat buf;
	int exists;
	int filedesc;

	if(stat(archpath, &buf) == -1) {
		exists = 0;
	}
	else {
		exists = 1;
	}

	filedesc = open(archpath, O_CREAT | O_RDWR);

	if (filedesc == -1) {
		printf("Could not open nor create archive. Failed.\n");
		return -1;
	}

	else {
		lseek(filedesc, 0, SEEK_SET);
		
		if (exists) {
			if(arch_write_ghdr(filedesc) == -1) {
				printf("Arch_write_ghdr failed.\n");
				arch_close(filedesc);
				return -1;
			}
			
		}
	}

	return filedesc;
}

void arch_close(int filedesc) {
	if (filedesc != 0) {
		close(filedesc);
	}
}

int arch_write_ghdr(int filedesc) {

	return 0;
}

int main(int argc, char **argv) {
	
	int opt;
	int sel_opt = 0;
	char *archpath = NULL;
	int validar;

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
				break;
			case 'w':
				sel_opt = 7;
				break;
			default:
				break;
				
		}
	}

	if (sel_opt == 0) {
		printf("Correct options not selected.\n");
	}

	if (optind < argc) {
		archpath = (char *)malloc((strlen(argv[optind]) + 1) * sizeof(char));
		if (archpath == NULL) {
			printf("Allocated memory not created for archive path string.\n");
			exit(0);
		}

		strcpy(archpath, argv[optind++]);
	}
	else {
		printf("File path not found\n");
	}
	
	validar = arch_open(archpath);
	if (validar == -1) {
		printf("Archive was not found.");
		return -1;
	}

}
