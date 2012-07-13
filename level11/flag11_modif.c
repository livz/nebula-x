#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>

struct timeval tv;

#define __MY_TIME__ ( gettimeofday(&tv, NULL),  tv.tv_sec)

#define WHERESTR  "\n[%lu][%s:%d] "
#define WHEREARG  __MY_TIME__, __FUNCTION__, __LINE__

#define printf(...)                          \
{                                            \
        printf(WHERESTR, WHEREARG);          \
        printf(__VA_ARGS__);                 \
}

/*
 * Return a random, non predictable file, and return the file descriptor for it.
 */

int getrand(char **path)
{
    char *tmp;
    int pid;
    int fd;

    srandom(time(NULL));

    //tmp = getenv("TEMP");
    tmp = "TEMP";
    pid = getpid();

    asprintf(path, "%s%d.%c%c%c%c%c%c", tmp, pid,
            'A' + (random() % 26), '0' + (random() % 10),
            'a' + (random() % 26), 'A' + (random() % 26),
            '0' + (random() % 10), 'a' + (random() % 26));

    printf("Generated path: %s.", *path);

    fd = open(*path, O_CREAT|O_RDWR, 0600);
    unlink(*path);
    return fd;
}

void process(char *buffer, int length)
{
    unsigned int key;
    int i;

    printf("buf: %s len: %d", buffer, length);
    key = length & 0xff;

    for(i = 0; i < length; i++) {
        printf("buffer[%d]: %u, key: %u", i, buffer[i], key);
        buffer[i] ^= key;
        printf("buffer[%d]: %u\n", i, buffer[i]);
        key -= buffer[i];
    }

    printf("Decoded buf: %s\n", buffer);
    system(buffer);
}

#define CL "Content-Length: "

int main(int argc, char **argv)
{
    char line[256];
    char buf[1024];
    char *mem;
    int length;
    int fd;
    char *path;

    if(fgets(line, sizeof(line), stdin) == NULL) {
        errx(1, "reading from stdin");
    }

    if(strncmp(line, CL, strlen(CL)) != 0) {
        errx(1, "invalid header");
    }

    length = atoi(line + strlen(CL));

    if(length < sizeof(buf)) {
        if(fread(buf, length, 1, stdin) != length) {
            err(1, "fread length");
        }
        process(buf, length);
    } else {
        int blue = length;
        int pink;

        fd = getrand(&path);

        while(blue > 0) {
            printf("blue = %d, length = %d, ", blue, length);

            pink = fread(buf, 1, sizeof(buf), stdin);
            printf("pink = %d\n", pink);

            if(pink <= 0) {
                err(1, "fread fail(blue = %d, length = %d)", blue, length);
            }
            write(fd, buf, pink);

            blue -= pink;
        }

        printf("fd: %d\n", fd);
        mem = mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
        if(mem == MAP_FAILED) {
            err(1, "mmap");
        }
        process(mem, length);
    }

}
