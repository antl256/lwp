#ifndef LWP_WAV_H
#define LWP_WAV_H

#include <stdint.h>

typedef struct lwp_wav {
    struct {
        uint32_t cbsize;
        uint16_t format_tag;
        uint16_t channels;
        uint32_t samples_per_second;
        uint32_t avg_bytes_per_sec;
        uint16_t block_align;
        uint16_t bits_per_sample;
    } format;

    struct {
        uint32_t cbsize;
        uint8_t *raw;
    } data;
} lwp_wav;

lwp_wav *lwp_wav_read(const char *path);
void lwp_wav_free(lwp_wav *wav);

#endif /* LWP_WAV_H */