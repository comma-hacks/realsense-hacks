#ifndef PID_H
#define PID_H
#include <iostream>

class PID {
public:
    PID(float kp, float ki, float kd) : Kp(kp), Ki(ki), Kd(kd) {}

    float update(float error) {
        integral += error;
        float derivative = error - previous_error;
        float output = Kp * error + Ki * integral + Kd * derivative;
        previous_error = error;
        return output;
    }

private:
    float Kp, Ki, Kd;
    float integral = 0;
    float previous_error = 0;
};
#endif