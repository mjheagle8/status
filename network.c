/**
 * get network statistics
 * by mjheagle
 */

#include <stdio.h>
#include <stdbool.h>
#include "config.h"
#include "network.h"

/* local functions */
unsigned long long int read_bytes(const char *);

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
        {
                fprintf(stderr, "error opening %s\n", fp);
                return false;
        }

        /* read file */
        char buffer[3];
        if (fgets(buffer, 3, fd))
                ret = buffer[0] == 'u' && buffer[1] == 'p';

        /* done */
        fclose(fd);

        /* debug */
        printf("d: %Ld %Ld\n", download_bytes(), transmit_bytes());

        return ret;
}

/**
 * transmit_bytes
 * get the current transmit bytes
 */
unsigned long long int
transmit_bytes()
{
        int ret = 0;

        /* interface rx_bytes file */
        static const char *fp = "/sys/class/net/" NIC "/statistics/tx_bytes";

        /* byte counters */
        static unsigned long long int last_bytes = 0;
        unsigned long long int this_bytes;

        /* read file */
        this_bytes = read_bytes(fp);
        if (!this_bytes)
                return 0;

        /* done */
        ret = this_bytes - last_bytes;
        last_bytes = this_bytes;

        return ret;
}

/**
 * download_bytes
 * get the current download bytes
 */
unsigned long long int
download_bytes()
{
        int ret = 0;

        /* interface rx_bytes file */
        static const char *fp = "/sys/class/net/" NIC "/statistics/rx_bytes";

        /* byte counters */
        static unsigned long long int last_bytes = 0;
        unsigned long long int this_bytes;

        /* read file */
        this_bytes = read_bytes(fp);
        if (!this_bytes)
                return 0;

        /* done */
        ret = this_bytes - last_bytes;
        last_bytes = this_bytes;

        return ret;
}

/**
 * read_bytes
 * fp - handle path of file to read
 */
unsigned long long int read_bytes(const char *fp)
{
        long long int  ret;
        /* open file */
        FILE *fd = fopen((char *)fp, "r");
        if (fd == NULL)
        {
                fprintf(stderr, "error opening %s\n", fp);
                return 0;
        }

        /* read data */
        if (fscanf(fd, "%Ld", &ret) == 0)
        {
                fprintf(stderr, "error reading %s\n", fp);
                return 0;
        }

        return ret;
}
