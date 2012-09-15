/*
 * dzen print formats
 * by mjheagle
 */

#include <stdio.h>
#include "dzen.h"

/**
 * dzen_icon
 * icon - icon file to print
 */
void
dzen_icon(const char *icon)
{
        printf("^i(%s)", icon);
}

/**
 * dzen_color
 * fg - color to set as foreground
 * bg - color to set as background
 */
void
dzen_color(const char *fg, const char *bg)
{
        if (fg)
                printf("^fg(%s)", fg);
        if (bg)
                printf("^bg(%s)", bg);
}
