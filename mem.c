/*
 * get memory used
 * by mjheagle
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "dzen.h"

/**
 * memused
 * determine the amount of memory in use
 * by reading /proc/meminfo
 */
int
memused()
{
        FILE *f;
        char line[32];
        int mem, tmp;

        f = fopen("/proc/meminfo", "r");

        memset(line, 0, 32);
        fgets(line, 30, f);
        sscanf(line, "MemTotal: %d kB", &tmp);
        mem = tmp;

        memset(line, 0, 32);
        fgets(line, 30, f);
        sscanf(line, "MemFree: %d kB", &tmp);
        mem -= tmp;

        memset(line, 0, 32);
        fgets(line, 30, f);
        sscanf(line, "Buffers: %d kB", &tmp);
        mem -= tmp;

        memset(line, 0, 32);
        fgets(line, 30, f);
        sscanf(line, "Cached: %d kB", &tmp);
        mem -= tmp;

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
#ifdef USE_DZEN
        dzen_color(DZEN_HIGHLIGHT, NULL);
        dzen_icon("/usr/share/icons/stlarch_icons/mem2.xbm");
        dzen_color(DZEN_FG, NULL);
        printf(" ");
#endif
        if (mem<1024)
                printf("%dK ", mem);
        else if (mem<1024*1024)
                printf("%.1fM ", (float)mem / 1024);
        else
                printf("%.1fG ", (float)mem / (1024*1024));
}
