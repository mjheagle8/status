/**
 * get network statistics
 * by mjheagle
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "color.h"
#include "dzen.h"
#include "network.h"

/* local functions */
unsigned long long int read_bytes(const char *);
char *print_data_rate(unsigned long long int);

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

        return ret;
}

/**
 * print_data_rate
 * pretty print a data rate with units
 * b - bytes
 */
char *
print_data_rate(unsigned long long int b)
{
        static const double KB = 1024;
        static const double MB = 1024*1024;
        static const double GB = 1024*1024*1024;

        const double br = (double)(1000000*(double)b/INTERVAL);
        const int buflen = 8;
        char *buf = calloc(buflen, sizeof(char));

        if (br<KB)
                snprintf(buf, buflen, "%.1fB", br);
        else if (br<MB)
                snprintf(buf, buflen, "%.2fK", br/KB);
        else if (br<GB)
                snprintf(buf, buflen, "%.2fM", br/MB);
        else
                snprintf(buf, buflen, "%.2fG", br/GB);

        return buf;
}

/**
 * print_data_rates
 * rx - received bytes
 * tx - transmit bytes
 */
char *
print_data_rates(unsigned long long int rx, unsigned long long int tx)
{
        const int buflen = 25;
        char *buf = calloc(buflen, sizeof(char));

        if (rx)
        {
                char *tmp = NULL;
                append_color(2, buf);
                if (fields[P_NET_DN])
                        strcat(buf, fields[P_NET_DN]);
                else
                        strcat(buf, "d:");
                append_color(1, buf);
                tmp = print_data_rate(rx);
                strcat(buf, tmp);
                strcat(buf, " ");
                free(tmp);
        }
        if (tx)
        {
                char *tmp = NULL;
                append_color(2, buf);
                if (fields[P_NET_UP])
                        strcat(buf, fields[P_NET_UP]);
                else
                        strcat(buf, "u:");
                append_color(1, buf);
                tmp = print_data_rate(tx);
                strcat(buf, tmp);
                strcat(buf, " ");
                free(tmp);
        }

        return buf;
}

/**
 * transmit_bytes
 * get the current transmit bytes
 */
unsigned long long int
transmit_bytes()
{
        static bool first = true;
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

        /* dont print initial data */
        if (first)
        {
                first = false;
                return 0;
        }

        return ret;
}

/**
 * download_bytes
 * get the current download bytes
 */
unsigned long long int
download_bytes()
{
        static bool first = true;
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

        /* dont print initial data */
        if (first)
        {
                first = false;
                return 0;
        }

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

        /* close file */
        fclose(fd);

        return ret;
}
