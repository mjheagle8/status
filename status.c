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
#include "cpu.h"
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
void datePP();

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
                /* get mpd status */
#ifdef MPD_HOST
                mpd_status();
                delimiter();
#endif
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

                /* get volume */
#ifdef GET_VOLUME
                printf("vol:%d%% ", getvolume());
                delimiter();
#endif

                /* get cpu usage */
                float perc[NCPUS];
                if (cpuperc(perc))
                        cpuPP(perc, cpufreq());

                /* get memory */
                memPP(memused());
                delimiter();

                /* print date */
                datePP();

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
        strftime(timestr, TIMELENGTH, TIMEFMT, now);

        printf("%s", timestr);
        free(timestr);
}
