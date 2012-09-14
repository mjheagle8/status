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
void print_data_rate(unsigned long long int);

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
void
print_data_rate(unsigned long long int b)
{
        static const double KB = 1024;
        static const double MB = 1024*1024;
        static const double GB = 1024*1024*1024;

        const double br = (double)(1000000*(double)b/INTERVAL);

        if (br<KB)
                printf("%.1fB", br);
        else if (br<MB)
                printf("%.2fK", br/KB);
        else if (br<GB)
                printf("%.2fM", br/MB);
        else
                printf("%.2fG", br/GB);
}

/**
 * print_data_rates
 * rx - received bytes
 * tx - transmit bytes
 */
void
print_data_rates(unsigned long long int rx, unsigned long long int tx)
{
        if (rx)
        {
                printf("d:");
                print_data_rate(rx);
                if (tx)
                        printf("/");
        }
        if (tx)
        {
                printf("u:");
                print_data_rate(tx);
        }

        if (rx || tx)
                printf(" ");
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

        return ret;
}
