/*
 * get cpu usage
 * by mjheagle
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
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
 * perc - the array to store the percentages in
 */
bool
cpuperc(float perc[NCPUS])
{
        short i;
        static cpustat *laststat[NCPUS];
        static bool first = true;
        cpustat *thisstat[NCPUS];
        char buffer[LINELENGTH];

        /* open stat file */
        FILE *fd = fopen("/proc/stat", "r");
        if (!fd)
        {
                fprintf(stderr, "failed to open %s\n", "/proc/stat");
                return false;
        }

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
                        if (!idle && !busy)
                                perc[i] = 0;
                        else
                                perc[i] = 100*(float)busy/((float)idle + (float)busy);
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
                return false;
        }
        return true;
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
        static const char *format = "%*s %ld %ld %ld %ld %ld %ld %ld %ld %ld %*s\n";

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

/**
 * cpufreq
 * get the frequency of the first cpu
 */
unsigned int
cpufreq()
{
        char buffer[256];
        unsigned int ret = 0;
        float tmp;

        /* open file */
        FILE *fd = fopen("/proc/cpuinfo", "r");
        if (!fd)
        {
                fprintf(stderr, "failed to open %s\n", "/proc/stat");
                return false;
        }

        /* read file */
        do
        {
                memset(buffer, 0, 256);
                fgets(buffer, 256, fd);
        } while (buffer[4] != 'M');

        /* close file */
        fclose(fd);

        /* parse buffer */
        if (sscanf(buffer, "cpu MHz : %f", &tmp) == 1)
                ret = (unsigned int)tmp;
        return ret;
}

/**
 * cpuPP
 * pretty print the cpu percentage
 * stat - the array of cpu percentages
 */
void
cpuPP(const float *stat)
{
        int i;
        for (i=0; i<NCPUS; i++)
        {
                if (i>0)
                        printf("/");
                if (stat[i]<10)
                        printf("%0.2f%%", stat[i]);
                else
                        printf("%0.1f%%", stat[i]);
        }
        printf(" :: ");
}
