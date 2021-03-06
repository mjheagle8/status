/*
 * status
 * a simple text-output status monitor
 * by mjheagle
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <X11/Xlib.h>
#define _STATUS_MAIN
#include "config.h"
#undef _STATUS_MAIN
#include "alsa.h"
#include "battery.h"
#include "checkmail.h"
#include "color.h"
#include "cpu.h"
#include "dzen.h"
#include "mem.h"
#include "mpd.h"
#include "network.h"
#include "time.h"
#include "wifi.h"

/* macros */
#ifdef USE_DZEN
#define delimiter()
#else
#define delimiter()
#endif

/* function prototypes */
void sigh(int);
void append_field(int, char *);

/* global variables */
#ifdef USE_DWM
static Display *dpy;
static Window root;
static int screen;
#endif

int
main()
{
        /* initialize Xorg interface */
#ifdef USE_DWM
        dpy = XOpenDisplay(NULL);
        if (!dpy)
        {
                fprintf(stderr, "fatal: unable to open display\n");
                exit(2);
        }
        screen = DefaultScreen(dpy);
        root = RootWindow(dpy, screen);
#endif
        signal(SIGINT, sigh);
        signal(SIGTERM, sigh);

        /* initialize for main loop */
#ifdef GET_VOLUME
        init_alsa();
#endif
        char *mails = NULL;

        /* main loop */
        while (1)
        {
                /* allocate buffers */
                char *buf = calloc(1024, sizeof(char));
                char *tmp;


                /* get network status */
                if (if_up())
                {
                        append_color(2, buf);
                        append_field(P_NET, buf);
                        append_color(1, buf);

#ifdef NIC
                        char *essid = get_dev_essid(-1, NIC);
#else
                        char *essid = get_essid();
#endif
                        strcat(buf, essid);
                        strcat(buf, " ");

#ifdef MAILDIR
                        mails = mailcount(MAILDIR);
#endif
                        free(essid);

                        const unsigned long long int tx = transmit_bytes();
                        const unsigned long long int rx = download_bytes();
                        char *net = print_data_rates(rx, tx);
                        strcat(buf, net);
                        free(net);
                        if (rx || tx)
                        {
                                delimiter();
                        }
                }

                /* get mpd status */
#ifdef MPD_HOST
                append_color(2, buf);
                append_field(P_MPD, buf);
                append_color(1, buf);
                char *mpd = mpd_status();
                strcat(buf, mpd);
                free(mpd);
                delimiter();
#endif

                /* print mails */
                if (mails != NULL) {
                        append_color(2, buf);
                        append_field(P_MAIL, buf);
                        append_color(1, buf);
                        strcat(buf, mails);
                }

                /* get volume */
#ifdef GET_VOLUME
                const int vol = getvolume();
                append_color(2, buf);
                if (vol>50)
                        append_field(P_VOL_HI, buf);
                else if (vol>0)
                        append_field(P_VOL_LO, buf);
                else
                        append_field(P_VOL_MUTE, buf);
                append_color(1, buf);
                tmp = calloc(16, sizeof(char));
                snprintf(tmp, 16, "%d%% ", vol);
                strcat(buf, tmp);
                free(tmp);
                delimiter();
#endif

                /* get cpu usage */
                append_color(2, buf);
                append_field(P_CPU, buf);
                append_color(1, buf);
                float perc[NCPUS];
                if (cpuperc(perc))
                {
                        char *cpu = cpuPP(perc, cpufreq());
                        strcat(buf, cpu);
                        free(cpu);
                        delimiter();
                }

                /* get memory */
                append_color(2, buf);
                append_field(P_MEM, buf);
                append_color(1, buf);
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
                append_color(2, buf);
                append_field(P_DATE, buf);
                append_color(1, buf);
                char *date = datePP();
                strcat(buf, date);
                free(date);

                /* wait for next iteration */
#ifdef USE_DWM
                XStoreName(dpy, root, buf);
                XFlush(dpy);
#else
                printf("%s\n", buf);
#endif
                free(buf);
                fflush(stdout);
                lsleep();
#ifdef GET_VOLUME
                alsa_refresh();
#endif
        }

        XCloseDisplay(dpy);
        return 0;
}


/**
 * sigh
 * handle signals sent to this process
 */
void sigh(int sig)
{
    fprintf(stderr, "Signal %d, exiting...\n", sig);
#ifdef USE_DWM
    XCloseDisplay(dpy);
#endif
    exit(1);
}

/**
 * append field
 * write a field prefix into the buffer
 */
void append_field(int field, char *buf)
{
        if (fields[field])
                strcat(buf, fields[field]);
}
