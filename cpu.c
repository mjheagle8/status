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
        unsigned long int idle;
        unsigned long int busy;
} cpustat;

/* local functions */
static cpustat *parse_cpu_stat_line(FILE *fd);

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
        char buffer[LINELENGTH];
        float perc[NCPUS];

        /* open stat file */
        FILE *fd = fopen("/proc/stat", "r");
        if (!fd)
                return -1;

        /* read relevant statistics from file */
        fgets(buffer, LINELENGTH, fd); /* skip first line */
        for (i=0; i<NCPUS; i++)
                thisstat[i] = parse_cpu_stat_line(fd);

        /* close stat file */
        fclose(fd);

        /* compute percentages */
        if (!first)
        {
                for (i=0; i<NCPUS; i++)
                {
                        int idle = thisstat[i]->idle - laststat[i]->idle;
                        int busy = thisstat[i]->busy - laststat[i]->busy;
                        perc[i] = 100*(float)busy/((float)idle + (float)busy);
                        printf("perc: %0.1f\n", perc[i]);
                        free(laststat[i]);
                }
        }

        /* copy thisstat into laststat */
        for (i=0; i<NCPUS; i++)
                laststat[i] = thisstat[i];

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
 * fd - the file handle for /proc/stat
 */
static cpustat *
parse_cpu_stat_line(FILE *fd)
{
        unsigned long int user, usernice, system, idle, iowait, irq, softirq,
                     steal, guest;
        int ret;
        static const char *format = "%*s %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %Ld %*s\n";

        /* run fscanf */
        ret = fscanf(fd, format, &user, &usernice, &system, &idle, &iowait,
                        &irq, &softirq, &steal, &guest);
        if (ret != 9)
                return 0;

        cpustat *this = calloc(1, sizeof(cpustat));
        this->idle = idle + iowait;
        this->busy = user + usernice + system + irq + softirq + steal + guest;

        return this;
}
