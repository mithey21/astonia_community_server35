/*
 * Part of Astonia Server 3.5 (c) Daniel Brockhaus. Please read license.txt.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <mysql/mysql.h>
#include <mysql/mysqld_error.h>
#define EXTERNAL_PROGRAM
#include "server.h"
#undef EXTERNAL_PROGRAM
#include "mail.h"
#include "statistics.h"
#include "clan.h"
#include "drdata.h"
#include "skill.h"
#include "depot.h"
#include "club.h"
#include "badip.h"
#include "argon.h"

static MYSQL mysql;
static char mysqlpass[80] = {"tgbdwf3h"};

int init_database(void) {
    // init database client
    if (!mysql_init(&mysql)) return 0;

    // try to login as root with our password
    if (!mysql_real_connect(&mysql, "localhost", "root", mysqlpass, "merc35", 0, NULL, 0)) {
        fprintf(stderr, "MySQL error: %s (%d)\n", mysql_error(&mysql), mysql_errno(&mysql));
        return 0;
    }

    return 1;
}

void exit_database(void) {
    mysql_close(&mysql);
}

int main(int argc, char **args) {
    char buf[512];
    char hash[256];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <email> <password>\n", args[0]);
        return 1;
    }

    if (!init_database()) {
        fprintf(stderr, "Cannot connect to database.\n");
        return 3;
    }

    if (argon2id_hash_password(hash, sizeof(hash), args[2], NULL)) {
        fprintf(stderr, "Argon failed. Call Mom!\n");
        return 2;
    }

    sprintf(buf, "insert account (email,password,creation_time) values ("
                 "'%s'," // email
                 "'%s'," // password
                 "%d)", // creation time
            args[1], hash, (int)time(NULL));

    if (mysql_query(&mysql, buf)) {
        fprintf(stderr, "Failed to create account: Error: %s (%d)", mysql_error(&mysql), mysql_errno(&mysql));
        return 2;
    }

    printf("Success. Account ID is %d.\n", (int)mysql_insert_id(&mysql));

    exit_database();

    return 0;
}
