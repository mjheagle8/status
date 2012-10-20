/*
 * get battery status
 * by mjheagle
 */

#include <stdio.h>
#include "battery.h"
#include "config.h"
#include "dzen.h"

/* local functions */
static float read_float_file(const char *);
static char charge_status();
static float charge_pct();

/**
 * batteryPP
 * get the battery status and print it
 */
void
batteryPP()
{
        /* initial declarations */
        static short counter = 0;
        static char status = -1;
        static float pct = -1;

        /* check whether we should recompute values this time */
        if (counter == BATTERY_INT || status < 0)
        {
                counter = 0;
                status = charge_status();
                pct = charge_pct();
        }

        /* check for failure parsing */
        if (!status)
                return;

#ifdef USE_DZEN
        dzen_color(DZEN_HIGHLIGHT, NULL);
        if (status == 'C')
                dzen_icon("/usr/share/icons/stlarch_icons/ac9.xbm");
        else
                dzen_icon("/usr/share/icons/stlarch_icons/batt5full.xbm");
        dzen_color(DZEN_FG, NULL);
        printf(" ");
        if (pct > 50)
                dzen_color("#93D44F", NULL);
        else if (pct > 25)
                dzen_color("#F5B915", NULL);
        else
                dzen_color("#BF1E2D", NULL);
#else
        printf("%c ", status);
#endif
        printf("%.1f%% ", pct);
#ifdef USE_DZEN
        dzen_color(DZEN_FG, NULL);
#endif

        /* increment counter */
        counter++;
}

/**
 * charge_status
 * determine whether the battery is charging or discharging
 * returns a 'C' if charging, 'D' if discharging
 * returns a 0 if failed
 */
char
charge_status()
{
        static const char *fp = "/sys/class/power_supply/" BATTERY "/status";
        char ret;

        /* open file */
        FILE *fd = fopen(fp, "r");
        if (!fd)
                return 0;

        /* get character from file */
        ret = (char)fgetc(fd);

        /* close file */
        fclose(fd);

        return ret;
}

/**
 * charge_pct
 * get the percentage the battery is charged
 */
float
charge_pct()
{
        static const char *full_path = "/sys/class/power_supply/" BATTERY "/charge_full";
        static const char *now_path  = "/sys/class/power_supply/" BATTERY "/charge_now";

        return (float)100*read_float_file(now_path)/read_float_file(full_path);
}

/**
 * read_float_file
 * helper function to read a float from a file
 * fp - file path to read
 */
float
read_float_file(const char *fp)
{
        float ret;

        /* open file */
        FILE *fd = fopen(fp, "r");
        if (!fd)
                return -1;

        /* read file */
        if (!fscanf(fd, "%f", &ret))
                ret = 0;

        /* cloae file */
        fclose(fd);

        return ret;
}
