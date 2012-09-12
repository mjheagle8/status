/*
 * status
 * a simple text-output status monitor
 * by mjheagle
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mem.h"
#include "config.h"

int
main ()
{
        while (1)
        {
                /* get memory */
                const int mem = memused();
                memPP(mem);

                /* wait for next iteration */
                printf("\n");
                usleep(INTERVAL);
        }

        return 0;
}
