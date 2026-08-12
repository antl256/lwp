#include "lwpwav.h"
#include "lwplog.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool is_wave(FILE *pfile) {
    char chunk_id[4];
    int chunk_size;

    fread(chunk_id, 4, sizeof(char), pfile);
    fread(&chunk_size, 1, sizeof(int), pfile);
    if (strncmp(chunk_id, "RIFF", 4) != 0) {
        lwp_log_error("Header not found: RIFF");
        return false;
    }

    fread(chunk_id, 4, sizeof(char), pfile);
    if (strncmp(chunk_id, "WAVE", 4) != 0) {
        lwp_log_error("Header not found: WAVE");
        return false;
    }

    return true;
}

static bool seek_chunk_start(FILE *pfile, const char *id) {
    char chunk_id[4];
    uint32_t chunk_size;
    while (feof(pfile) == 0) {
        fread(chunk_id, sizeof(char), 4, pfile);
        fread(&chunk_size, sizeof(uint32_t), 1, pfile);
        if (strncmp(chunk_id, id, 4) == 0) {
            fseek(pfile, -8, SEEK_CUR);
            return true;
        } else {
            fseek(pfile, chunk_size, SEEK_CUR);
        }
    }
    return false;
}

static bool parse_fmt(FILE *pfile, lwp_wav *wav) {
    char chunk_id[4];
    fread(chunk_id, sizeof(char), 4, pfile);
    fread(&wav->format.cbsize, sizeof(uint32_t), 1, pfile);
    if (strncmp(chunk_id, "fmt ", 4) != 0) {
        return false;
    }

    fread(&wav->format.format_tag, sizeof(uint16_t), 1, pfile);
    fread(&wav->format.channels, sizeof(uint16_t), 1, pfile);
    fread(&wav->format.samples_per_second, sizeof(uint32_t), 1, pfile);
    fread(&wav->format.avg_bytes_per_sec, sizeof(uint32_t), 1, pfile);
    fread(&wav->format.block_align, sizeof(uint16_t), 1, pfile);
    fread(&wav->format.bits_per_sample, sizeof(uint16_t), 1, pfile);

    return true;
}

static bool parse_data(FILE *pfile, lwp_wav *wav) {
    char chunk_id[4];
    fread(chunk_id, sizeof(char), 4, pfile);
    fread(&wav->data.cbsize, sizeof(uint32_t), 1, pfile);
    if (strncmp(chunk_id, "data", 4) != 0) {
        return false;
    }
    wav->data.raw = calloc(1, wav->data.cbsize);
    fread(wav->data.raw, wav->data.cbsize, 1, pfile);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/* API */
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

lwp_wav *lwp_wav_read(const char *path) {
    assert(path != NULL);
    assert(path[0] != '\0');

    FILE *pfile = fopen(path, "rb");
    if (pfile == NULL) {
        lwp_log_error("Failed to read wav file \"%s\"", path);
        return NULL;
    }

    if (is_wave(pfile) == false) {
        lwp_log_error("Not a valid WAV file");
        return NULL;
    }

    lwp_wav *wav = calloc(1, sizeof(lwp_wav));

    if (seek_chunk_start(pfile, "fmt ") == false) {
        lwp_log_error("Failed to find chunk: fmt");
    }
    parse_fmt(pfile, wav);

    if (seek_chunk_start(pfile, "data") == false) {
        lwp_log_error("Failed to find chunk: data");
    }
    parse_data(pfile, wav);

    fclose(pfile);

    lwp_log_info("Read wav file: \"%s\"", path);

    return wav;
}

void lwp_wav_free(lwp_wav *wav) {
    if (wav != NULL) {
        free(wav->data.raw);
        free(wav);
    }
}
