/*
 * get mpd now playing
 * by mjheagle
 */

#include <stdbool.h>
#include <stdio.h>
#include <mpd/client.h>
#include "config.h"
#include "dzen.h"

/* global variables */
struct mpd_connection *connection = NULL;

/**
 * mpd_status
 * print the now playing information
 */
bool
mpd_status()
{
        bool ret = true;
        struct mpd_status *status = NULL;
        struct mpd_song *song = NULL;

        /* print prefix */
        printf("MPD: ");

        /* connect if there is no connection */
        if (!connection)
                connection = mpd_connection_new(MPD_HOST, MPD_PORT, MPD_TIMEOUT);

        /* get status */
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
                        printf("connection failed ");
                        return false;
                }
        }

        /* get play state */
        enum mpd_state playstate = mpd_status_get_state(status);
        if (playstate == MPD_STATE_UNKNOWN)
        {
                printf("state unknown");
                return false;
        }
        else if (playstate == MPD_STATE_STOP)
        {
                printf("stopped ");
                goto done;
        }
        else if (playstate == MPD_STATE_PAUSE)
                printf("(paused) ");
        else if (playstate == MPD_STATE_PLAY)
                printf("(playing) ");

        /* get song */
        song = mpd_run_current_song(connection);
        const char *title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
        const char *artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
        printf("%s - %s ", artist, title);
        goto done;

done:
        /* clean up */
        if (song)
                mpd_song_free(song);
        if (status)
                mpd_status_free(status);
        return ret;
}
