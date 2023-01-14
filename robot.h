#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define WEBJOYSTICK_SERVER (char *) "http://tici:5000"
#define MAGNITUDE 0.1
#define ROTATE_MAGNITUDE 0.15
#define REALSENSE_FACES_TOWARDS_ROAD 0

void sendMoveCommand(char* url, float x, float y) {
    CURL *curl;
    CURLcode res;
    char request[200];

    sprintf(request, "%s/control/%.2f/%.2f", url, x, y);
    printf("%s\n", request);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, request);
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
}

void robotMoveForward() {
  #ifdef REALSENSE_FACES_TOWARDS_ROAD
    sendMoveCommand(WEBJOYSTICK_SERVER, 0, MAGNITUDE);
  #else
    sendMoveCommand(WEBJOYSTICK_SERVER, 0, -MAGNITUDE);
  #endif
}

void robotMoveBackward() {
  #ifdef REALSENSE_FACES_TOWARDS_ROAD
    sendMoveCommand(WEBJOYSTICK_SERVER, 0, -MAGNITUDE);
  #else
    sendMoveCommand(WEBJOYSTICK_SERVER, 0, MAGNITUDE);
  #endif
}

void robotRotateLeft() {
  sendMoveCommand(WEBJOYSTICK_SERVER, ROTATE_MAGNITUDE, 0);
}

void robotRotateRight() {
  sendMoveCommand(WEBJOYSTICK_SERVER, -ROTATE_MAGNITUDE, 0);
}