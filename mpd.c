/*
 * get mpd now playing
 * by mjheagle
 */

#include <stdbool.h>
#include <stdio.h>
#include <mpd/client.h>
#include "config.h"

/* global variables */
struct mpd_connection *connection = NULL;

/**
 * mpd_status
 * print the now playing information
 */
bool
mpd_status()
{
        /* connect if there is no connection */
        if (!connection)
                connection = mpd_connection_new(MPD_HOST, MPD_PORT, MPD_TIMEOUT);

        /* get status */
        struct mpd_status *status = NULL;
        status = mpd_run_status(connection);
        /* attempt to reconnect if status call failed */
        if (!status)
        {
                mpd_connection_free(connection);
                connection = mpd_connection_new(MPD_HOST, MPD_PORT, MPD_TIMEOUT);
                status = mpd_run_status(connection);
                /* if reconnection failed, error out */
                if (!status)
                {
                        printf("MPD: connection failed ");
                        return false;
                }
        }

        /* get play state */
        enum mpd_state playstate = mpd_status_get_state(status);
        if (playstate == MPD_STATE_UNKNOWN)
        {
                printf("MPD: state unknown");
                return false;
        }
        else if (playstate == MPD_STATE_STOP)
        {
                printf("MPD: stopped ");
                return true;
        }
        else if (playstate == MPD_STATE_PAUSE)
                printf("MPD: paused ");
        else if (playstate == MPD_STATE_PLAY)
                printf("MPD: playing ");

        mpd_status_free(status);
        return true;
}
