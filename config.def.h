/*
 * configuration file for status
 * by mjheagle
 */

#ifndef _STATUS_CONFIG_H
#define _STATUS_CONFIG_H

/* general */
#define INTERVAL        800000          /* amount of time between prints (in microseconds) */

/* cpu */
#define NCPUS           1               /* number of processors */
#define GET_CPUFREQ

/* time */
#define TIMEFMT         "%H:%M:%S %a %b %d"
#define TIMELENGTH      32              /* number of characters in the date string */

/* network */
#define NIC             "wlan0"         /* network interface */

/* sound (alsa) */
#define SOUNDCARD       "default"
#define ALSAMIXER       "Master"
#define GET_VOLUME

/* status outputs */
#define USE_DWM

/* field prefixes */
enum {P_MPD = 0, P_NET_UP, P_NET_DN, P_VOL, P_MEM, P_BAT, P_DATE};

#ifdef _STATUS_MAIN
const char *field_prefixes[] =
{
        [P_MPD] = "mpd: ",
};
#endif

#endif
