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

    irc_client_t* irc = establish_connection(twitch_irc_server.ip, twitch_irc_server.port);
    irc->channel_name = argv[1];
    irc->nickname = "TestingTestingZai";
    irc->auth_token = getenv("TWITCH_AUTH_TOKEN"); 
    if(irc->auth_token == NULL) {
        fprintf(stderr, "TWITCH_AUTH_TOKEN env variable is not set!\n");
        return -1;
    }
   
    // for now this will ALWAYS return true
    if(!authenticate_bot(irc)) {
        fprintf(stderr, "Failed to authenticate bot\n");
        return -1;
    }
     
    printf("Twitch bot authenticated!\n");
    keep_alive(irc);
    return 0;
}

// TODO: Maintain connection with Twitch IRC Server
// TODO: Before maintaining a connection send the 'JOIN #<twitch_channel>' command
// TODO: Listen for the 'PING' command

void keep_alive(irc_client_t *client) {
    message_t join = create_join_msg(client->channel_name);
    send_irc_msg(client->fd, join.buffer, join.len);

    message_t join_resp;
    join_resp.len = TWITCH_IRC_BUFF_SIZE;
    recv_irc_msg(client->fd, join_resp.buffer, join_resp.len);
    
    printf("Listening to Twitch channel: %s\n", client->channel_name);
    for(;;) {
        char twitch_chat[TWITCH_IRC_BUFF_SIZE] = {0};
        size_t bytes = recv_irc_msg(client->fd, twitch_chat, sizeof(twitch_chat));
        
        // if Twitch responds with PING
        // respond with PONG

        printf("%s", twitch_chat);

    }
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

// NICK TestingTestingZai\r\n -> 24
bool authenticate_bot(irc_client_t *client) {
    printf("Authenticating Bot...\n");
    message_t pass = create_pass_msg(client->auth_token);
    message_t nick = create_nick_msg(client->nickname);
    
    message_t recv;
    recv.len = TWITCH_IRC_BUFF_SIZE;
    
    //printf("%s", pass.buffer);
    //printf("%s", nick.buffer);
   
    /*
        * Sending the following IRC Messages
        * PASS oauth:<access_token>
        * NICK <nickname of twitch bot>
    */
    

    ssize_t bytes_sent_p = send_irc_msg(client->fd, pass.buffer, pass.len);
    ssize_t bytes_sent_n = send_irc_msg(client->fd, nick.buffer, pass.len);

    printf("bytes_sent_p: %zu\n", bytes_sent_p);
    printf("bytes_sent_n: %zu\n", bytes_sent_n);

    /*
        * Since my access token expired, I should expect the following message:
        * if response buffer == :tmi.twitch.tv NOTICE * :Login authentication failed : failed to auth
        * otherwise: did not fail
        *
    */
    //      0          1   2         3           4       5        
    // :tmi.twitch.tv 421 you QPMLCJ8FHSTBLNME :Unknown command/r/n
    recv_irc_msg(client->fd, recv.buffer, recv.len);
    printf("%s\n", recv.buffer);

    /* Parsing Raw IRC Message */
    string_list_t* list = init_string();
    const char *delim = " ";
    char *token;
    token = strtok(recv.buffer, delim);
   
    while(token != NULL) {
        append_string(list, token);
        token = strtok(NULL, delim); 
    }

    // list now has the raw irc message
    // now we need to contruct the irc_raw_message_t struct
    raw_irc_message_t irc_msg;
    irc_msg.prefix = list->data[0];
    irc_msg.command = list->data[1];

    int trailing_index = trailing_param_index(list); 
    if(strcmp(irc_msg.command, "421") == 0) {
        twitch_response(trailing_index, list);
        return false;
    }
    
    free(list->data);
    return true;
}

message_t create_pass_msg(char *msg) {
    message_t irc_msg;
    int char_written = snprintf(irc_msg.buffer, irc_msg.len, "PASS oauth:%s\r\n", msg);
    irc_msg.len = char_written;
    return irc_msg;
}

message_t create_nick_msg(char *msg) {
    message_t irc_msg;
    int char_written = snprintf(irc_msg.buffer, irc_msg.len, "NICK %s\r\n", msg);
    irc_msg.len = char_written;
    return irc_msg;
}

message_t create_join_msg(char *msg) {
    message_t irc_msg;
    int char_written = snprintf(irc_msg.buffer, irc_msg.len, "JOIN #%s\r\n", msg);
    irc_msg.len = char_written;
    return irc_msg;
}

ssize_t send_irc_msg(int fd, void *buff, size_t n) {
    ssize_t bytes_sent = send(fd, buff, n, 0);
    return bytes_sent;
}

ssize_t recv_irc_msg(int fd, void *buff, size_t n) {
    ssize_t bytes_recv = recv(fd, buff, n, 0);
    return bytes_recv;
}

int trailing_param_index(string_list_t *list) {
    int i = 2;
    for(i = 2; i < list->size; i++) {
        if(list->data[i][0] == ':') break;
    }

    return i;
}

void twitch_response(int index, string_list_t *list) {
    printf("\n-------Twitch Response-------\n");
    for(int i = index; i < list->size; i++) {
        printf("%s ", list->data[i]);
    }
}

void print_irc_info(irc_client_t *irc) {}

void free_resources(void *res) { 
    free(res);
}
