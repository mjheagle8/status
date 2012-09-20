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
        char status = charge_status();
        if (!status)
                return;

        float pct = charge_pct();

#ifdef USE_DZEN
        dzen_color(DZEN_HIGHLIGHT, NULL);
        if (status == 'C')
                dzen_icon("/usr/share/icons/stlarch_icons/ac9.xbm");
        else
                dzen_icon("/usr/share/icons/stlarch_icons/batt5full.xbm");
        dzen_color(DZEN_FG, NULL);
        printf(" ");
#else
        printf("%c ", status);
#endif
        printf("%.1f%% ", pct);
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
                return 0;

        /* read file */
        if (!fscanf(fd, "%f", &ret))
                ret = 0;

        /* cloae file */
        fclose(fd);

        return ret;
}
