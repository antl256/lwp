#ifndef LWP_PLAYER_H
#define LWP_PLAYER_H

#include "lwp_wav.h"

#if !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 1
#endif /* _POSIX_C_SOURCE */

#include <alsa/asoundlib.h>
#include <stdint.h>

typedef struct lwp_stream {
    lwp_wav *wav;
    int16_t *buffer;
    size_t buffer_cbsize;
    snd_pcm_t *pcm;
    int64_t total_seconds;
    int64_t total_frames;
    int64_t total_uploads;
    int64_t frames_per_write;
    snd_htimestamp_t timestamp_start;
    snd_htimestamp_t timestamp_end;
} lwp_stream;

lwp_stream *lwp_player_create(lwp_wav **wav);
void lwp_player_free(lwp_stream *stream);
void lwp_player_upload(lwp_stream *stream, float amplitude);
void lwp_player_drain(lwp_stream *stream);

#endif /* LWP_PLAYER_H */