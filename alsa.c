/*
 * get alsa status
 * (mostly from cloudef)
 * by mjheagle
 */

#include <alsa/asoundlib.h>
#include "config.h"

/* global variables */
snd_mixer_t *alsa = NULL;
snd_mixer_elem_t *alsamixer = NULL;

/**
 * init_alsa
 * must be called before other alsa functions
 * only needs to be called once
 * initializes alsa handle
 */
void
init_alsa()
{
        snd_mixer_open(&alsa, 0);
        snd_mixer_attach(alsa, SOUNDCARD);
        snd_mixer_selem_register(alsa, NULL, NULL);
        snd_mixer_load(alsa);

        if (alsa == NULL)
        {
                fprintf(stderr, "error opening sound card " SOUNDCARD);
                return;
        }

        snd_mixer_selem_id_t *sid;
        snd_mixer_selem_id_alloca(&sid);
        snd_mixer_selem_id_set_index(sid, 0);
        snd_mixer_selem_id_set_name(sid, ALSAMIXER);
        alsamixer = snd_mixer_find_selem(alsa, sid);

        if (alsamixer == NULL)
        {
                fprintf(stderr, "error opening alsa mixer " ALSAMIXER);
                return;
        }
}

/**
 * getvolume
 * determine the current volume
 */
int
getvolume()
{
        static long min = 0;
        static long max = 0;
        long volume;
        int mute;

        /* get min/max volume if necessary */
        if (!min && !max)
                snd_mixer_selem_get_playback_volume_range(alsamixer, &min, &max);

        /* get volume and mute */
        snd_mixer_selem_get_playback_volume(alsamixer, SND_MIXER_SCHN_MONO, &volume);
        snd_mixer_selem_get_playback_switch(alsamixer, SND_MIXER_SCHN_MONO, &mute);

        return !mute?0:volume>min?volume<max?(volume*100)/max:100:0;
}

/**
 * alsa_sleep
 * wait for mixer to be ready over sleep interval
 * interval - ms to sleep
 */
void
alsa_sleep(const int interval)
{
        snd_mixer_wait(alsa, interval);
        snd_mixer_handle_events(alsa);
}
