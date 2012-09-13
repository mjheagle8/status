/*
 * get cpu usage
 * by mjheagle
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "config.h"

#define LINELENGTH 128

/**
 * cpustat structure
 * idle - total number of cycles idle
 * busy - total number of cycles busy
 */
typedef struct _cpustat
{
        unsigned int idle;
        unsigned int busy;
} cpustat;

/* local functions */
static cpustat *parse_cpu_stat_line(const char *);

/**
 * cpuperc
 * get the percentage of the cpu that is being used
 */
float
cpuperc()
{
        float ret = -2;
        short i;
        static cpustat *laststat[NCPUS];
        static bool first = true;
        cpustat *thisstat[NCPUS];

        /* open stat file */
        FILE *fd = fopen("/proc/stat", "r");
        if (!fd)
                return -1;

        /* read relevant statistics from file */
        for (i=-1; i<NCPUS; i++)
        {
                char *line = calloc(LINELENGTH, sizeof(char));
                fgets(line, LINELENGTH, fd);
                printf("%s", line);
                if (i == -1 && NCPUS == 1)
                {
                        break;
                }
        }

        /* close stat file */
        fclose(fd);

        /* compute use percentages */

        /* clean up */
        if (!first)
        {
                for (i=0; i<NCPUS; i++)
                        free(laststat[i]);
        }

        /* return */
        if (first)
        {
                first = false;
                return -1;
        }
        return ret;
}

/**
 * parse_cpu_stat_line
 */
static cpustat *
parse_cpu_stat_line(const char *line)
{
}
