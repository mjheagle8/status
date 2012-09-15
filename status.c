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
#include "cpu.h"
#include "mem.h"
#include "network.h"
#include "alsa.h"

/* macros */
#define delimiter() printf(":: ")

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
                /* get network status */
                if (if_up())
                {
                        unsigned long long int tx = transmit_bytes();
                        unsigned long long int rx = download_bytes();
                        print_data_rates(rx, tx);
                        if (rx || tx)
                                delimiter();
                }

                /* get volume */
#ifdef GET_VOLUME
                int volume = getvolume();
                printf("vol:%d%% ", volume);
                delimiter();
#endif

                /* get cpu usage */
                float perc[NCPUS];
                if (cpuperc(perc))
                        cpuPP(perc, cpufreq());

                /* get memory */
                const int mem = memused();
                memPP(mem);
                delimiter();

                /* print date */
                datePP();

                /* wait for next iteration */
                fflush(stdout);
                usleep(INTERVAL);
#ifdef GET_VOLUME
                alsa_refresh(INTERVAL/1000);
#endif
                printf("\n");
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
