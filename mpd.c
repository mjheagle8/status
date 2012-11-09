/*
 * get mpd now playing
 * by mjheagle
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpd/client.h>
#include "config.h"
#include "dzen.h"

/* global variables */
struct mpd_connection *connection = NULL;

/**
 * mpd_status
 * print the now playing information
 */
char *
mpd_status()
{
        struct mpd_status *status = NULL;
        struct mpd_song *song = NULL;
        char *buf = calloc(256, sizeof(char));
        char *state;
        bool fetch_song = true;

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
                        strncat(buf+5, "connection failed", 249);
                        return buf;
                }
        }

        /* get play state */
        enum mpd_state playstate = mpd_status_get_state(status);
        if (playstate == MPD_STATE_STOP)
        {
                if (fields[P_MPD_STOP])
                        state = (char *)fields[P_MPD_STOP];
                else
                        state = "stopped";
                fetch_song = false;
        }
        else if (playstate == MPD_STATE_PAUSE)
        {
                if (fields[P_MPD_PAUSE])
                        state = (char *)fields[P_MPD_PAUSE];
                else
                        state = "paused:";
        }
        else if (playstate == MPD_STATE_PLAY)
        {
                if (fields[P_MPD_PLAY])
                        state = (char *)fields[P_MPD_PLAY];
                else
                        state = "playing:";
        }
        else
        {
                state = "?";
                fetch_song = false;
        }
        mpd_status_free(status);

        /* get song */
        if (fetch_song)
        {
                song = mpd_run_current_song(connection);
                const char *title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
                const char *artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
                snprintf(buf, 256, "%s %s - %s ", state, artist, title);
                mpd_song_free(song);
        }
        else
                snprintf(buf, 256, "%s", state);

        return buf;
}
