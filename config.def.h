/*
 * configuration file for status
 * by mjheagle
 */

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

/* dzen */
#define USE_DZEN
#define DZEN_FG         "#AAAAAA"
#define DZEN_BG         "#222222"
#define DZEN_FADE       "#777777"
#define DZEN_HIGHLIGHT  "#0055FF"
