/*
 * status
 * a simple text-output status monitor
 * by mjheagle
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "config.h"
#include "alsa.h"
#include "battery.h"
#include "cpu.h"
#include "dzen.h"
#include "mem.h"
#include "mpd.h"
#include "network.h"

/* macros */
#ifdef USE_DZEN
#define delimiter()
#else
#define delimiter() printf(":: ")
#endif

/* function prototypes */
char *datePP();

int
main()
{
        /* initialization */
#ifdef GET_VOLUME
        init_alsa();
#endif

        /* main loop */
        while (1)
        {
                /* get network status */
                if (if_up())
                {
                        const unsigned long long int tx = transmit_bytes();
                        const unsigned long long int rx = download_bytes();
                        print_data_rates(rx, tx);
                        if (rx || tx)
                        {
                                delimiter();
                        }
                }

                /* get mpd status */
#ifdef MPD_HOST
                mpd_status();
                delimiter();
#endif

                /* get volume */
#ifdef GET_VOLUME
#ifdef USE_DZEN
                dzen_color(DZEN_HIGHLIGHT, NULL);
                dzen_icon("/usr/share/icons/stlarch_icons/vol4.xbm");
                dzen_color(DZEN_FG, NULL);
                printf(" ");
#else
                printf("vol:");
#endif
                printf("%d%% ", getvolume());
                delimiter();
#endif

                /* get cpu usage */
                float perc[NCPUS];
                if (cpuperc(perc))
                {
                        cpuPP(perc, cpufreq());
                        delimiter();
                }

                /* get memory */
                memPP(memused());
                delimiter();

                /* get battery */
#ifdef GET_BATTERY
                batteryPP();
                delimiter();
#endif

                /* print date */
                char *date = datePP();
                printf(date);
                free(date);

                /* wait for next iteration */
                printf("\n");
                fflush(stdout);
                usleep(INTERVAL);
#ifdef GET_VOLUME
                alsa_refresh(INTERVAL/1000);
#endif
        }

        return 0;
}

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

        /* convert thte time to a formatted string */
        timestr = malloc(TIMELENGTH*sizeof(char));
        strftime(timestr, TIMELENGTH, TIMEFMT, now);

        return timestr;
}
