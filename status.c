/*
 * status
 * a simple text-output status monitor
 * by mjheagle
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "mem.h"
#include "config.h"

/* macros */
#define delimiter() printf(":: ")

/* function prototypes */
void datePP();

int
main ()
{
        while (1)
        {
                /* get memory */
                const int mem = memused();
                memPP(mem);
                delimiter();

                /* print date */
                datePP();

                /* wait for next iteration */
                printf("\n");
                usleep(INTERVAL);
        }

        return 0;
}

/**
 * datePP
 * get the current date
 * and print it using strftime
 */
void
datePP()
{
        struct tm *now;
        time_t cur;
        char *timestr;

        /* get current time */
        time(&cur);
        now = localtime(&cur);

        /* convert thte time to a formatted string */
        timestr = malloc(TIMELENGTH*sizeof(char));
        strftime(timestr, TIMELENGTH, "%a %b %d", now);

        printf("%s", timestr);
}
