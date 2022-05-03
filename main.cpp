#include <chrono>
#include <thread>

#include <librealsense2/rs.hpp>
#include <iostream>
#include <cmath>

struct Quaternion {
    double w, x, y, z;
};

struct EulerAngles {
    double roll, pitch, yaw;
};

EulerAngles ToEulerAngles(Quaternion q) {
    EulerAngles angles;

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.roll = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = 2 * (q.w * q.y - q.z * q.x);
    if (std::abs(sinp) >= 1)
        angles.pitch = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        angles.pitch = std::asin(sinp);

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.yaw = std::atan2(siny_cosp, cosy_cosp);

    return angles;
}


int main(int argc, char **argv)
{
  // Declare RealSense pipeline, encapsulating the actual device and sensors
  rs2::pipeline pipe;
  // Create a configuration for configuring the pipeline with a non default profile
  rs2::config cfg;
  // Add pose stream
  cfg.enable_stream(RS2_STREAM_POSE, RS2_FORMAT_6DOF);
  // Start pipeline with chosen configuration
  pipe.start(cfg);

  // Main loop
  while (true)
  {
    // Wait for the next set of frames from the camera
    auto frames = pipe.wait_for_frames();
    // Get a frame from the pose stream
    auto f = frames.first_or_default(RS2_STREAM_POSE);
    // Cast the frame to pose_frame and get its data
    auto pose = f.as<rs2::pose_frame>().get_pose_data();

    int mult = 1000;

    auto x = pose.translation.x * mult;
    auto y = pose.translation.y * mult;
    auto z = pose.translation.z * mult;


    // Print the x, y, z values of the translation, relative to initial position
		//std::cout << x << "," << y << "," << z << "," << pose.rotation.x << "," << pose.rotation.y << "," << pose.rotation.z << "," << pose.rotation.w << "\n";

    struct Quaternion q;
    q.w = pose.rotation.w;
    q.x = pose.rotation.x;
    q.y = pose.rotation.y;
    q.z = pose.rotation.z;
    struct EulerAngles euler = ToEulerAngles(q);
    std::cout << std::endl << euler.yaw << std::endl << euler.pitch << std::endl << euler.roll << std::endl;


    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}
