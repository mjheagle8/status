/*
 * time management
 * by mjheagle
 */

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "config.h"
#include "time.h"

/* global variables */
time_t curtime = 1;

/**
 * datePP
 * get the current date
 * and print it using strftime
 */
char *
datePP()
{
        struct tm *now;
        time_t cur;
        char *timestr;

#ifdef USE_DZEN
        dzen_color(DZEN_HIGHLIGHT, NULL);
        dzen_icon("/usr/share/icons/stlarch_icons/clock2.xbm");
        dzen_color(DZEN_FG, NULL);
        printf(" ");
#endif

        /* get current time */
        time(&cur);
        now = localtime(&cur);
        curtime = mktime(now);

        /* convert thte time to a formatted string */
        timestr = malloc(TIMELENGTH*sizeof(char));
        strftime(timestr, TIMELENGTH, TIMEFMT, now);

        return timestr;
}

/**
 * lsleep
 * wait until the next iteration
 */
void
lsleep()
{
        struct timespec tm;
        clock_gettime(CLOCK_REALTIME, &tm);
        const int sleeptime = ((INTERVAL*1000)-tm.tv_nsec)/1000;
        usleep(sleeptime);
}
