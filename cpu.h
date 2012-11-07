/*
 * get cpu usage
 * by mjheagle
 */

#ifndef _STATUS_CPU_H
#define _STATUS_CPU_H

float *cpuperc();
char *cpuPP(const float *, unsigned int);
unsigned int cpufreq();

#endif
