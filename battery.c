/*
 * get battery status
 * by mjheagle
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "battery.h"
#include "color.h"
#include "dzen.h"

/* local functions */
static float read_float_file(const char *);
static char charge_status();
static float charge_pct();

/**
 * batteryPP
 * get the battery status
 * returns a alloc'd string that needs to be free'd
 */
char *
batteryPP()
{
        /* initial declarations */
        static short counter = 0;
        static char status = -1;
        static float pct = -1;
        char *buf;

        /* check whether we should recompute values this time */
        if (counter == BATTERY_INT || status < 0)
        {
                counter = 0;
                status = charge_status();
                pct = charge_pct();
        }

        /* check for failure parsing */
        if (!status)
                return NULL;

        buf = calloc(32, sizeof(char));
        append_color(2, buf);
        if (fields[P_BAT] && status == 'D')
                strcat(buf, fields[P_BAT]);
        else if (fields[P_AC] && status == 'C')
                strcat(buf, fields[P_AC]);
        else
                snprintf(buf, 3, "%c ", status);

        if (pct>25)
                append_color(1, buf);
        else
                append_color(3, buf);
        char *tmp = calloc(10, sizeof(char));
        snprintf(tmp, 10, "%.1f%% ", pct);
        strcat(buf, tmp);
        free(tmp);

        /* increment counter */
        counter++;

        return buf;
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
