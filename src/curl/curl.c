#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

#define POST_REQUEST_BUF_SIZE 256

typedef struct {
    char *url;
    char *client_id;
    char *client_secret;
    char *code;
    char *grant_type;
    char *redirect_uri;
} TwitchAuth;

int main() {
    TwitchAuth tauth;
    tauth.url = "https://id.twitch.tv/oauth2/token";
    tauth.client_id = "client_id=dotr0r7hctn968f1nedt1b1gvo8lgy";

    tauth.client_secret = getenv("TWITCH_CLIENT_SECRET");
    tauth.code = getenv("TWITCH_CODE");

    tauth.grant_type = "&grant_type=authorization_code";
    tauth.redirect_uri = "&redirect_uri=http://localhost:3000";

    char post_buf[POST_REQUEST_BUF_SIZE];
    snprintf(post_buf, sizeof(post_buf), "%s%s%s%s%s", tauth.client_id, tauth.client_secret, tauth.code, tauth.grant_type, tauth.redirect_uri);
    printf("Post Request: %s\n", post_buf);

    #if 1
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, tauth.url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_buf); 
        
        // Perform the request
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
    #endif
    
    return 0;
}
