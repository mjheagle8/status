/*
 * get memory used
 * by mjheagle
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * memused
 * determine the amount of memory in use
 * by reading /proc/meminfo
 */
int
memused()
{
        FILE *f;
        char *line;
        int mem, tmp;

        f = fopen("/proc/meminfo", "r");

        line = malloc(32*sizeof(char));
        fgets(line, 30, f);
        sscanf(line, "MemTotal: %d kB", &tmp);
        mem = tmp;
        free(line);

        line = malloc(32*sizeof(char));
        fgets(line, 30, f);
        sscanf(line, "MemFree: %d kB", &tmp);
        mem -= tmp;
        free(line);

        line = malloc(32*sizeof(char));
        fgets(line, 30, f);
        sscanf(line, "Buffers: %d kB", &tmp);
        mem -= tmp;
        free(line);

        line = malloc(32*sizeof(char));
        fgets(line, 30, f);
        sscanf(line, "Cached: %d kB", &tmp);
        mem -= tmp;
        free(line);

        fclose(f);

        return mem;
}

/**
 * memPP
 * pretty-print the memory used
 * mem - memory used in KB
 */
void
memPP (const int mem)
{
        if (mem<1024)
                printf("%dK", mem);
        else if (mem<1024*1024)
                printf("%.1fM", (float)mem / 1024);
        else
                printf("%.1fG", (float)mem / (1024*1024));
}
