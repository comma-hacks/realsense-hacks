#ifndef ROBOT_H
#define ROBOT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "moves.h"

#define WEBJOYSTICK_SERVER (char *) "http://tici:5000"
#define MAGNITUDE 0.1
#define ROTATE_MAGNITUDE 0.15
#define REALSENSE_FACES_TOWARDS_ROAD 0

#include <chrono>
auto _last_api_call_time = std::chrono::high_resolution_clock::now();
auto last_api_call_time() {
  return _last_api_call_time;
}

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
    _last_api_call_time = std::chrono::high_resolution_clock::now();
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


bool robotApplyMove(ROBOT_MOVE mov) {
  switch (mov)
    {
    case MOVE_FORWARD:
      robotMoveForward();
      return true;
    case MOVE_BACKWARD:
      robotMoveBackward();
      return true;
    case ROTATE_LEFT:
      robotRotateLeft();
      return true;
    case ROTATE_RIGHT:
      robotRotateRight();
      return true;
    }
    return false;
}
#endif