/*
 * color printing
 * by mjheagle
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "color.h"

/*
 * get_color
 * build a color string for whatever output
 * is being used  * and return a string
 * containing it (requires free'ing)
 */
char *get_color(int c)
{
        char *cl;

#ifdef USE_DWM
        cl = calloc(2, sizeof(char));
        *cl = c;
        return cl;
#endif

        cl = calloc(1, sizeof(char));
        return cl;
}

/**
 * append color
 * write a color into the buffer
 */
void append_color(int c, char *buf)
{
        char *cl = get_color(c);
        if (*cl)
                strcat(buf, cl);
        free(cl);
}
