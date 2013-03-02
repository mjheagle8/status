/*
 * get number of mails
 * by mjheagle
 */

#define _GNU_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "checkmail.h"
#include "config.h"

/* count the number of files in a directory */
int countdir(const char *dir) {
        DIR *dp;
        struct dirent *ep;
        int ret = 0;

        /* open directory */
        dp = opendir(dir);
        if (dp == NULL)
                return 0;

        /* iterate through directory */
        while ((ep = readdir(dp)) != NULL) {
                if (*(ep->d_name) != '.')
                        ret++;
        }

        return ret;
}

/* count the number of mails in a root mail directory */
int countmails(const char *dir) {
        DIR *dp;
        struct dirent *ep;
        int ret = 0;

        /* open directory */
        dp = opendir(dir);
        if (dp == NULL)
                return -1;

        /* iterate through directory */
        while ((ep = readdir(dp)) != NULL) {
                if (*(ep->d_name) == '.')
                        continue;
                char *path = calloc(256, sizeof(char));
                snprintf(path, 255, "%s/%s/new", dir, ep->d_name);
                ret += countdir(path);
                free(path);
        }

        return ret;
}

/* generate a string with the number of mails in a directory */
char *mailcount(const char *dir) {
        static int mails = -2;
        static char *mailstr = NULL;
        extern time_t curtime;

        if (mails == -2 || (curtime % MAIL_INT) == 0) {
                if (mailstr)
                        free(mailstr);
                mails = countmails(dir);
                if (mails <= 0)
                        mailstr = NULL;
                else {
                        mailstr = calloc(5, sizeof(char));
                        snprintf(mailstr, 4, "%d ", mails);
                }
        }

        return mailstr;
}
