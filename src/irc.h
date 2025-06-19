#ifndef IRC_H
#define IRC_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "buffer.h"

#define TWITCH_IRC_BUFF_SIZE 500
#define CR 0x0D
#define LF 0x0A

typedef struct {
    char buffer[TWITCH_IRC_BUFF_SIZE];
    size_t len;
} message_t;

typedef struct {
    char *prefix;
    char *command;
    char **params;
    char **trailing_params;
} raw_irc_message_t;

typedef struct {
    char *port;
    char *ip;
} twitch_irc_t;

typedef struct {
    int fd;
    char *auth_token;
    char *nickname;
    char *channel_name;
} irc_client_t;

/* ---- IRC Client Functionality ---  */
irc_client_t* establish_connection(char* ip, char *port);
bool authenticate_bot(irc_client_t *client);
void keep_alive();

/* messaging */
message_t create_pass_msg(char *msg);
message_t create_join_msg(char *msg);
message_t create_nick_msg(char *msg);

int send_irc_msg(int fd, void *buff, size_t n);
int recv_irc_msg(int fd, void *buff, size_t n);

void free_resources(void *res);
void print_irc_info(irc_client_t *irc);

#endif
