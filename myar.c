/**
 * Author: Joshua Kociemba (kociembj), joshua.kociemba@oregonstate.edu
 * Created: 2013-10-29 18:01:09
 * Filename: myar.c
 *
 * Description: UNIX Archive Utility
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>
#include <time.h>
#include "myar.h"

struct arch_file {
	char mode[7];
	char uid[6];
	char gid[6];
	char date[20];
	char name[17];
	char perms[10];
	char size[10];
};

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

bool arch_isValid(char *archpath);

bool arch_hasValidHdr(int fd);

void arch_close(int filedesc);

/**
 * Attempts to write to the archive's global header
 * Returns 1 on success
 * Returns -1 on failure
 **/
int arch_write_ghdr(int filedesc);

int arch_open(char *archpath) {
	
	int fd;

	fd = open(archpath, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	if(fd == -1) {
		printf("File could not be opened.\n");
		return -1;
	}

	if(arch_isValid(archpath)) {

		lseek(fd, 0, SEEK_SET);
		
		if(arch_hasValidHdr(fd) == false) {
			printf("Bad global header.\n");
			arch_close(fd);
			return -1;
		}	
	}
	else {
		
	}

	return fd;
}

bool arch_isValid(char *archpath) {
	
	struct stat buffer;
	
	if(stat(archpath, &buffer) == 0) {
		return false;
	}
	else {
		return true;
	}
}

bool arch_hasValidHdr(int fd) {
	
	char buffer[SARMAG];

	if(lseek(fd, 0, SEEK_SET) != 0) {
		return false;
	}

	if(read(fd, buffer, SARMAG) != SARMAG) {
		return false;
	}

	if(strncmp(buffer, ARMAG, sizeof(buffer)) != 0) {
		return false;
	}

	return true;
}

void arch_close(int fd) {
	if(fd != 0) {
		close(fd);
	}
}


bool arch_writeHdr(int fd) {

	int success;

	if(fd != 0) {
		lseek(fd, 0, SEEK_SET);
		success = write(fd, ARMAG, SARMAG);

		if(success == -1) {
			printf("An error occured while writing");
			return -1;
		}
		
		else {
			return 1;
		}
	}

	return -1;
}

struct arch_file cln_prms(struct arch_file archy) {
	int i = 3;
	int j = 0;
	archy.perms[9] = '\0';

	while (i < 6) {
		if (archy.mode[i] == '0') {
			archy.perms[j] = '-';
			archy.perms[j+1] = '-';
			archy.perms[j+2] = '-';		
		}
		if (archy.mode[i] == '1') {
			archy.perms[j] = '-';
			archy.perms[j+1] = '-';
			archy.perms[j+2] = 'x';
		}
		if (archy.mode[i] == '2') {
			archy.perms[j] = '-';
			archy.perms[j+1] = 'w';
			archy.perms[j+2] = '-';
		}
		if (archy.mode[i] == '3') {
			archy.perms[j] = '-';
			archy.perms[j+1] = 'w';
			archy.perms[j+2] = 'x';
		}
		if (archy.mode[i] == '4') {
			archy.perms[j] = 'r';
			archy.perms[j+1] = '-';
			archy.perms[j+2] = '-';
		}
		if (archy.mode[i] == '5') {
			archy.perms[j] = 'r';
			archy.perms[j+1] = '-';
			archy.perms[j+2] = 'x';
		}
		if (archy.mode[i] == '6') {
			archy.perms[j] = 'r';
			archy.perms[j+1] = 'w';
			archy.perms[j+2] = '-';
		}
		if (archy.mode[i] == '7') {
			archy.perms[j] = 'r';
			archy.perms[j+1] = 'w';
			archy.perms[j+2] = 'x';
		}
		i++;
		j+=3;
	}
	
	return archy;
}

struct arch_file cln_str(struct arch_file archy) {
	int i = 0;
	int st;
	long t;
	char temp[20];
	time_t arch_time;

	while (i < 17) {
		if (archy.name[i] == '/') {
			archy.name[i] = '\0';
		}
		i++;
	}

	i = 0;

	while (i < 9) {
		if (archy.size[i] == ' ') {
			temp[i] = '\0';
			i = 9;
		}
		else {
			temp[i] = archy.size[i];
			i++;
		}
	}


	strcpy(archy.size, temp);

	t = atol(archy.date);
	arch_time = (time_t)t;

	strftime(temp, 20, "%b %e %R %Y", localtime(&arch_time));

	strcpy(archy.date, temp);

	archy.date[19] = '\0';
	archy.gid[5] = '\0';
	archy.uid[5] = '\0';
	archy.mode[6] = '\0';

	archy = cln_prms(archy);

	return archy;
}

struct arch_file arch_read(int fd) {
	struct arch_file archy;
	
	read(fd, archy.name, 16);

	read(fd, archy.date, 12);

	read(fd, archy.gid, 5);

	lseek(fd, 1, SEEK_CUR);

	read(fd, archy.uid, 5);

	lseek(fd, 1, SEEK_CUR);

	read(fd, archy.mode, 6);

	lseek(fd, 2, SEEK_CUR);

	read(fd, archy.size, 10);

	archy = cln_str(archy);

	return archy;
}

void arch_seek_next(int fd, int hsize) {
	
	hsize += 2;

	lseek(fd, hsize, SEEK_CUR);

	if ((lseek(fd, 0, SEEK_CUR) % 2) == 1) {
		lseek(fd, 1, SEEK_CUR);
	}
}

void arch_print_table(int fd, bool verbose) {
	struct arch_file archy;
	int arch_size = lseek(fd, 0, SEEK_END);
	int hsize;

	lseek(fd, 8, SEEK_SET);
	
	while (lseek(fd, 0, SEEK_CUR) < arch_size) {
		archy = arch_read(fd);

		if (verbose) {
			printf("%s ", archy.perms);
			printf("%s/%s", archy.gid, archy.uid);
			printf("\t  %s ", archy.size);
			printf("%s ", archy.date);
			printf("%s\n", archy.name);
		}
		else {
			printf("%s\n", archy.name);
		}
		
		hsize = atoi(archy.size);
		arch_seek_next(fd, hsize);
	}
}

void usage(void) {

	printf( "Usage: myar [-][vqxtdAw] archive file\n"
		"-v\t- print a verbose version of the output\n"
		"-q\t- quickly append named files to archive\n"
		"-x\t- extract named files\n"
		"-t\t- print a concise table of contents of the archive\n"
		"-d\t- delete named files from archive\n"
		"-A\t- quickly append all regular files in the current directory\n"
	      );
}

int main(int argc, char **argv) {

	char arch_path[] = "";
	int opt = 0;
	int fd;

	strcpy(arch_path, argv[2]);
	
	fd = arch_open(arch_path);
	
	while ((opt = getopt(argc, argv, "vqxtdA")) != -1) {
		switch (opt) {
		case 't':
			arch_print_table(fd, false);
			break;
		case 'v':
			arch_print_table(fd, true);
			break;
		default:
			usage();
			break;
		}
	}

	arch_close(fd);

	return 0;
}
