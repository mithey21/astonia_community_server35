/*
 * Part of Astonia Server 3.5 (c) Daniel Brockhaus. Please read license.txt.
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include "server.h"
#include "mem.h"
#include "log.h"
#include "talk.h"
#include "player.h"
#include "motd.h"

static int motd_time;
static char *motd = NULL;

int read_motd(void) {
    struct stat st;
    int handle, len;

    if (stat("motd.txt", &st)) {
        elog("Error stat-ing motd");
        return 0;
    }
    if (st.st_mtime > motd_time) {
        handle = open("motd.txt", O_RDONLY);
        if (handle == -1) {
            elog("Error opening motd");
            return 0;
        }

        len = lseek(handle, 0, SEEK_END);
        lseek(handle, 0, SEEK_SET);
        motd = xrealloc(motd, len + 1, IM_BASE);
        read(handle, motd, len);
        motd[len] = 0;
        close(handle);

        xlog("Read MotD");

        motd_time = st.st_mtime;
    }
    return 1;
}

void show_motd(int nr) {
    char buf[1024], *a, *b;

    if (!motd) {
        log_player(nr, LOG_SYSTEM, "Welcome to your generic Astonia 3.5 server.");
        return;
    }

    for (a = motd, b = buf; *a; a++) {
        if (*a == '\n') {
            *b = 0;
            b = buf;
            log_player(nr, LOG_SYSTEM, "%s", buf);
        } else *b++ = *a;
    }
}
