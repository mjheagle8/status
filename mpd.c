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
        /* print prefix */
#ifdef USE_DZEN
        printf("^i(/usr/share/icons/stlarch_icons/note1.xbm)");
#else
        printf("MPD: ");
#endif

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
                return true;
        }
        else if (playstate == MPD_STATE_PAUSE)
#ifdef USE_DZEN
                printf("^i(/usr/share/icons/stlarch_icons/pause1.xbm) ");
#else
                printf("(paused) ");
#endif
        else if (playstate == MPD_STATE_PLAY)
#ifdef USE_DZEN
                printf("^i(/usr/share/icons/stlarch_icons/play1.xbm) ");
#else
                printf("(playing) ");
#endif

        /* get song */
        struct mpd_song *song = NULL;
        song = mpd_run_current_song(connection);
        const char *title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
        const char *artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
        printf("%s - %s ", artist, title);

        /* clean up */
        mpd_song_free(song);
        mpd_status_free(status);
        return true;
}
