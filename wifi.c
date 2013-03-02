/*
 * get wireless information
 * by mjheagle
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/wireless.h>
#include "wifi.h"

char *get_dev_essid(int skfd, const char *dev) {
        struct iwreq wrq;
        bool get_skfd = false;

        if (skfd == -1) {
                get_skfd = true;
                skfd = socket(AF_INET, SOCK_DGRAM, 0);
                if (skfd == -1)
                        return NULL;
        }

        char *essid = calloc(IW_ESSID_MAX_SIZE+1, sizeof(char));
        wrq.u.essid.pointer = (caddr_t) essid;
        wrq.u.essid.length = IW_ESSID_MAX_SIZE + 1;
        wrq.u.essid.flags = 0;
        strncpy(wrq.ifr_name, dev, IFNAMSIZ);
        if (ioctl(skfd, SIOCGIWESSID, &wrq) < 0) {
                free(essid);
                essid = NULL;
        }

        if (get_skfd)
                close(skfd);

        return essid;
}

char *get_essid() {
        int skfd;
        struct ifconf conf;
        struct ifreq *req;
        char buf[1024];
        char *essid = NULL;

        /* open kernel socket */
        skfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (skfd == -1)
                return NULL;

        /* get data */
        conf.ifc_len = sizeof(buf);
        conf.ifc_buf = buf;
        if (ioctl((long) skfd, SIOCGIFCONF, &conf) < 0)
                return NULL;
        req = conf.ifc_req;

        /* iterate through devices and determine active device */
        int i;
        for (i = conf.ifc_len / sizeof(struct ifreq); --i>=0; req++)
        {
                if (strcmp(req->ifr_name, "lo") == 0)
                        continue;

                /* get device essid */
                essid = get_dev_essid(skfd, req->ifr_name);
                if (essid != NULL)
                        goto done;
        }

        goto done;

done:
        /* close kernel socket */
        close((long) skfd);

        return essid;
}
