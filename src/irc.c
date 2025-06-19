#include "irc.h"

/**
    * z-twitch-client <twitch_channel_name>
    * arg length = 2
**/

int main(int argc, char **argv) {
    if(argc != 2) {
        fprintf(stderr, "usage: z-twitch-client <twitch_channel_name>\n");
        return -1;
    }
    
    twitch_irc_t twitch_irc_server;
    twitch_irc_server.ip = "irc.chat.twitch.tv";
    twitch_irc_server.port = "6667";

    irc_client_t* irc_client = establish_connection(twitch_irc_server.ip, twitch_irc_server.port);
    irc_client->channel_name = argv[1];
    irc_client->nickname = "TestingTestingZai";
    irc_client->auth_token = getenv("TWITCH_AUTH_TOKEN"); 
    if(irc_client->auth_token == NULL) {
        fprintf(stderr, "TWITCH_AUTH_TOKEN env variable is not set!\n");
        return -1;
    }
   
    // for now this will ALWAYS return true
    if(!authenticate_bot(irc_client)) {
        fprintf(stderr, "Failed to authenticate bot\n");
        return -1;
    }
     
    return 0;
}

irc_client_t* establish_connection(char *ip, char *port) {
    printf("Establishing connection..\n");
    irc_client_t *zai = malloc(sizeof(irc_client_t));
    if(!zai) {
        fprintf(stderr, "Failed to allocate memory for {twitch_irc_t}\n");
        exit(EXIT_FAILURE);
    }
    
    zai->fd = 0;
    struct addrinfo hints, *res;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(ip, port, &hints, &res) != 0) {
        perror("getaddrinfo");
        return NULL;
    }
    
    if((zai->fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    if(connect(zai->fd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect");
        return NULL;
    }
    
    printf("Connected.\n");
    return zai;
}

bool authenticate_bot(irc_client_t *client) {
    puts("Authenticating Bot...");
    message_t pass = create_pass_msg(client->auth_token);
    message_t nick = create_nick_msg(client->nickname);
    
    message_t recv;
    recv.len = TWITCH_IRC_BUFF_SIZE;
    
    printf("%s", pass.buffer);
    printf("%s", nick.buffer);
   
    /*
        * Sending the following IRC Messages
        * PASS oauth:<access_token>
        * NICK <nickname of twitch bot>
    */
    send_irc_msg(client->fd, pass.buffer, pass.len);
    send_irc_msg(client->fd, nick.buffer, nick.len);

    /*
        * Since my access token expired, I should expect the following message:
        * if response buffer == :tmi.twitch.tv NOTICE * :Login authentication failed : failed to auth
        * otherwise: did not fail
        *
    */
    //      0          1   2         3           4       5        6 
    // :tmi.twitch.tv 421 you QPMLCJ8FHSTBLNME :Unknown command /r/n
    recv_irc_msg(client->fd, recv.buffer, recv.len);
    printf("%s\n", recv.buffer);
    
    // iterate through the buffer until we hit \r\n
    #if 0
    size_t len = strlen(recv.buffer);
    for(int i = 0; i < len; i++) {
        printf("{%d} %d\n", i, (int)recv.buffer[i]);
    }
    #endif

    // parse raw irc message
    string_t* list = init_string();
    char *delim = " ";
    char *token = strtok(recv.buffer, delim);
   
    while(token != NULL) {
        append_string(list, token);
        token = strtok(NULL, token);
    }
    
    printf("List size: %zu\n", list->size);
    for(int i = 0; i < list->size; i++) {
        printf("{%d} %s\n", i, list->data[i]);
    }
    
    free(list->data);
    return true;
}

message_t create_pass_msg(char *msg) {
    message_t irc_msg;
    irc_msg.len = TWITCH_IRC_BUFF_SIZE;
    snprintf(irc_msg.buffer, irc_msg.len, "PASS oauth:%s\r\n", msg);
    return irc_msg;
}

message_t create_join_msg(char *msg) {
    message_t irc_msg;
    irc_msg.len = TWITCH_IRC_BUFF_SIZE;
    snprintf(irc_msg.buffer, irc_msg.len, "JOIN #%s\r\n", msg);
    return irc_msg;
}

message_t create_nick_msg(char *msg) {
    message_t irc_msg;
    irc_msg.len = TWITCH_IRC_BUFF_SIZE;
    snprintf(irc_msg.buffer, irc_msg.len, "NICK %s\r\n", msg);
    return irc_msg;
}

int send_irc_msg(int fd, void *buff, size_t n) {
    ssize_t bytes_sent = send(fd, buff, n, 0);
    return bytes_sent;
}

int recv_irc_msg(int fd, void *buff, size_t n) {
    ssize_t bytes_recv = recv(fd, buff, n, 0);
    return bytes_recv;
}

void print_irc_info(irc_client_t *irc) {}

void free_resources(void *res) { 
    free(res);
}
