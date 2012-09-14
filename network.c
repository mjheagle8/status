/**
 * get network statistics
 * by mjheagle
 */

#include <stdio.h>
#include <stdbool.h>
#include "config.h"
#include "network.h"

/**
 * if_up
 * check status of network card
 */
bool
if_up()
{
        bool ret = false;

        /* interface status file */
        static const char *fp = "/sys/class/net/" NIC "/operstate";

        /* open file */
        FILE *fd = fopen((char *)fp, "r");
        if (fd == NULL)
                return false;

        /* read file */
        char buffer[3];
        if (fgets(buffer, 3, fd))
                ret = buffer[0] == 'u' && buffer[1] == 'p';

        /* done */
        fclose(fd);

        return ret;
}
