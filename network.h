/**
 * get network statistics
 * by mjheagle
 */

#ifndef _STATUS_NETWORK_H
#define _STATUS_NETWORK_H

#include <stdbool.h>

bool if_up();
unsigned long long int download_bytes();
unsigned long long int transmit_bytes();
void print_data_rates(unsigned long long int, unsigned long long int);

#endif
