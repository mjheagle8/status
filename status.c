/*
 * memused
 * a simplified version of free that only finds mem usage
 * by mjheagle
 */

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"

int
main ()
{
        const int mem = memused();
        memPP(mem);
        return 0;
}
