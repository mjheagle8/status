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

/* mpd */
#define MPD_HOST        "localhost"
#define MPD_PORT        6600
#define MPD_TIMEOUT     1000

/* battery */
#define GET_BATTERY
#define BATTERY         "BAT0"
#define BATTERY_INT     30              /* interval for fetching power status */

/* mail directory */
#define MAILDIR         "/mnt/develop/mail/gmail"
#define MAIL_INT        60

/* status outputs */
#define USE_DWM

/* field headers */
enum {P_MPD = 0, P_MPD_PLAY, P_MPD_PAUSE, P_MPD_STOP,
        P_MAIL,
        P_CPU,
        P_NET, P_NET_UP, P_NET_DN,
        P_VOL_HI, P_VOL_LO, P_VOL_MUTE,
        P_MEM,
        P_BAT, P_AC,
        P_DATE};

#ifdef _STATUS_MAIN
const char *fields[] =
{
        [P_MAIL]      = " ",
        [P_MPD]       = " ",
        [P_MPD_PLAY]  = "",
        [P_MPD_PAUSE] = "",
        [P_MPD_STOP]  = " ",
        [P_NET]       = " ",
        [P_NET_UP]    = " ",
        [P_NET_DN]    = " ",
        [P_DATE]      = " ",
        [P_MEM]       = " ",
        [P_VOL_LO]    = " ",
        [P_VOL_HI]    = " ",
        [P_VOL_MUTE]  = " ",
        [P_CPU]       = " ",
        [P_BAT]       = " ",
        [P_AC]        = " ",
};
#else
extern const char *fields[];
#endif
#endif
