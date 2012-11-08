/*
 * status
 * a simple text-output status monitor
 * by mjheagle
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#define delimiter() strcat(buf, ":: ")
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
                /* allocate buffers */
                char *buf = calloc(1024, sizeof(char));
                char *tmp;

                /* get network status */
                if (if_up())
                {
                        const unsigned long long int tx = transmit_bytes();
                        const unsigned long long int rx = download_bytes();
                        char *net = print_data_rates(rx, tx);
                        sprintf(buf, "%s", net);
                        free(net);
                        if (rx || tx)
                        {
                                delimiter();
                        }
                }

                /* get mpd status */
#ifdef MPD_HOST
                char *mpd = mpd_status();
                strcat(buf, mpd);
                free(mpd);
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
                strcat(buf, "vol:");
#endif
                tmp = calloc(16, sizeof(char));
                snprintf(tmp, 16, "%d%% ", getvolume());
                strcat(buf, tmp);
                free(tmp);
                delimiter();
#endif

                /* get cpu usage */
                float perc[NCPUS];
                if (cpuperc(perc))
                {
                        char *cpu = cpuPP(perc, cpufreq());
                        strcat(buf, cpu);
                        free(cpu);
                        delimiter();
                }

                /* get memory */
                char *mem = memPP(memused());
                strcat(buf, mem);
                free(mem);
                delimiter();

                /* get battery */
#ifdef GET_BATTERY
                char *bat = batteryPP();
                strcat(buf, bat);
                free(bat);
                delimiter();
#endif

                /* print date */
                char *date = datePP();
                strcat(buf, date);
                free(date);

                /* wait for next iteration */
                printf("%s\n", buf);
                free(buf);
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
