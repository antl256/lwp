#include "wp_def.h"
#include "wp_player.h"
#include "wp_wav.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FLAG_HELP    "--help"
#define FLAG_VERSION "--version"

#define checkarg(arg, str) (strncmp(arg, str, sizeof(str)) == 0)

static wp_wav *s_wav = NULL;
static wp_stream *s_stream = NULL;

static void sigint() {
    fprintf(stdout, "\n");
    exit(EXIT_SUCCESS);
}

static void cleanup(void) {
    wp_player_free(s_stream);
    wp_wav_free(s_wav);
    s_stream = NULL;
    s_wav = NULL;
}

static void help() {
    printf("usage: %s WAVFILE VOLUME\n", WP_NAME);
}

static void version() {
    printf("%s version %d.%d.%d\n", WP_NAME, WP_MAJOR, WP_MINOR, WP_PATCH);
}

int main(int argc, const char **argv) {

    atexit(cleanup);
    signal(SIGINT, sigint);

    if (argc < 2) {
        help();
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];
        /* Process flags. */
        if (checkarg(arg, FLAG_HELP)) {
            help();
            exit(EXIT_SUCCESS);
        } else if (checkarg(arg, FLAG_VERSION)) {
            version();
            exit(EXIT_SUCCESS);
        }
    }

    const char *path = NULL;
    const char *volume = NULL;

    if (argv[1][0] != '\0') path = argv[1];
    if (argv[2][0] != '\0') volume = argv[2];

    if (argc < 3 || path == NULL || volume == NULL) {
        help();
        exit(EXIT_FAILURE);
    }

    s_wav = wp_wav_read(path);
    s_stream = wp_player_create(&s_wav);

    wp_player_upload(s_stream, strtof(volume, NULL));
    wp_player_drain(s_stream);

    exit(EXIT_SUCCESS);
}
