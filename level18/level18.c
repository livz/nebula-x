#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/time.h>
#include <sys/resource.h>

struct {
	FILE *debugfile;
	int verbose;
	int loggedin;
} globals;

#define dprintf(...) if(globals.debugfile) fprintf(globals.debugfile, __VA_ARGS__)
#define dvprintf(num, ...) if(globals.debugfile && globals.verbose >= num) fprintf(globals.debugfile, __VA_ARGS__)

#define PWFILE "/home/level18/password"

void login(char *pw)
{
	FILE *fp;

	fp = fopen(PWFILE, "r");
	// fp is not close. After maximum opened files is reached, open() call will fail
	// Check the number of available descriptors.
	if(fp) {
		char file[64];

		if(fgets(file, sizeof(file) - 1, fp) == NULL) {
			dprintf("Unable to read password file %s\n", PWFILE);
			return;
		}

		dprintf("got from pass file: [%s]", file); 
		if(strcmp(pw, file) != 0) return;		
	}
	printf("logged in successfully (with%s password file)\n", fp == NULL ? "out" : "");
	
	globals.loggedin = 1;

}

void notsupported(char *what)
{
	char *buffer = NULL;
	asprintf(&buffer, "--> [%s] is unsupported at this current time.\n", what);
	dprintf(what);
	free(buffer);
}

void setuser(char *user)
{
	char msg[128];

	sprintf(msg, "unable to set user to '%s' -- not supported.\n", user);
	printf("%s\n", msg);

}

int main(int argc, char **argv, char **envp)
{
	char c;

	while((c = getopt(argc, argv, "d:v")) != -1) {
		switch(c) {
			case 'd':
				globals.debugfile = fopen(optarg, "w+");
				if(globals.debugfile == NULL) err(1, "Unable to open %s", optarg);
				setvbuf(globals.debugfile, NULL, _IONBF, 0);
				break;
			case 'v':
				globals.verbose++;
				break;
		}
	}

	dprintf("Starting up. Verbose level = %d\n", globals.verbose);

	// print max file descriptors limit
	struct rlimit file_lim;
	getrlimit(RLIMIT_NOFILE, &file_lim);
	printf("Soft lim: %d, hard lim: %d\n", file_lim.rlim_cur, file_lim.rlim_max);


	setresgid(getegid(), getegid(), getegid());
	setresuid(geteuid(), geteuid(), geteuid());
	
	while(1) {
		char line[256];
		char *p, *q;

		q = fgets(line, sizeof(line)-1, stdin);
		if(q == NULL) break;
		p = strchr(line, '\n'); if(p) *p = 0;
		p = strchr(line, '\r'); if(p) *p = 0;

		dvprintf(2, "got [%s] as input\n", line);

		if(strncmp(line, "login", 5) == 0) {
			dvprintf(3, "attempting to login\n");
			login(line + 6);
		} else if(strncmp(line, "logout", 6) == 0) {
			globals.loggedin = 0;
		} else if(strncmp(line, "shell", 5) == 0) {
			printf("attempting to start shell\n");
			printf("logged in: %d\n", globals.loggedin);
			if(globals.loggedin) {
				int j=0;
				for(j=0; j<argc; j++) {
					printf("param %d: %s\n", j, argv[j]);
				}
				printf("params: %s			
				execve("/bin/sh", argv, envp);
				printf("?\n");
				err(1, "unable to execve");
			}
			printf("Permission denied\n");
		} else if(strncmp(line, "logout", 4) == 0) {
			globals.loggedin = 0;
		} else if(strncmp(line, "closelog", 8) == 0) {
			if(globals.debugfile) fclose(globals.debugfile);
			globals.debugfile = NULL;
		} else if(strncmp(line, "site exec", 9) == 0) {
			notsupported(line + 10);
		} else if(strncmp(line, "setuser", 7) == 0) {
			setuser(line + 8);
		}
	}

	return 0;
}

