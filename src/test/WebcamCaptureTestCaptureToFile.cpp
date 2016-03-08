/*
  
  Capture image using the video-capture-library and write it to file
  
  Adapted from api_example.cpp located in video-capture-library

*/
#include <signal.h>

#if defined(__APPLE__) || defined(__linux)
#  include <unistd.h> /* usleep */
#elif defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <videocapture/Capture.h>
#include <libyuv.h>
#include <chrono>

bool wrote_frame = false;
std::fstream outfile;

using namespace ca;

bool must_run = true;

void fcallback(PixelBuffer& buffer); 
void sig_handler(int sig);
 
int main() {
  printf("\nVideoCapture\n");

  signal(SIGINT, sig_handler);

  outfile.open("captured.rgb", std::ios::binary | std::ios::out);
  if (!outfile.is_open()) {
    printf("Error: failed to open `generated.raw`.\n");
    exit(1);
  }

  int width = 640;
  int height = 480;

  Settings cfg;
  cfg.device = 0;
  cfg.capability = 0;
  cfg.format = CA_NONE;

  Capture cap(fcallback, NULL);
  cap.listDevices();

  printf("\nOutput formats:\n");
  cap.listCapabilities(cfg.device);

  printf("\n##########\n");

  std::vector<Capability> caps = cap.getCapabilities(0);

  if(cap.open(cfg) < 0) {
    printf("Error: cannot open the device.\n");
    ::exit(EXIT_FAILURE);
  }

  if(cap.start() < 0) {
    printf("Error: cannot start capture.\n");
    ::exit(EXIT_FAILURE);
  }

  while(must_run == true) {
    cap.update();
#if defined(_WIN32)
    Sleep(5);
#else
    usleep(5 * 1000);
#endif
  }

  if(cap.stop() < 0) {
    printf("Error: cannot stop.\n");
  }
  if(cap.close() < 0) {
    printf("Error: cannot close.\n");
  }

  outfile.flush();
  outfile.close();
  printf("Info: Wrote raw YUV file.\n");

  return EXIT_SUCCESS;
}

int count = 0;
uint8_t* argbImage = NULL;
int argbImageLength = -1;
std::chrono::high_resolution_clock::time_point time_last_frame = std::chrono::high_resolution_clock::now();

void fcallback(PixelBuffer& buffer) { 

  std::chrono::high_resolution_clock::time_point time_now = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> time_span_frames = std::chrono::duration_cast<std::chrono::duration<double>>(time_now - time_last_frame);
  printf("Frame callback: %lu bytes, stride: %lu, time since last frame: %f ms \n", buffer.nbytes, buffer.stride[0], time_span_frames *1000);
  time_last_frame = time_now;

  size_t width = buffer.width[0];
  size_t height = buffer.height[0];

  if (argbImageLength == -1 || argbImageLength != width * height * 4) {
	  argbImageLength = width * height * 4;
	  delete argbImage;
	  argbImage = new uint8_t[argbImageLength];
  }

  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

  if (buffer.pixel_format == CA_YUV420P) {
	  libyuv::I420ToARGB(buffer.plane[0], buffer.stride[0], buffer.plane[1], buffer.stride[1], buffer.plane[2], buffer.stride[2], argbImage, width * 4, width, height);
  }
  else if (buffer.pixel_format == CA_RGB24) {
	  libyuv::RGB24ToARGB(buffer.plane[0], buffer.stride[0], argbImage, width * 4, width, -1 * height); // -1 * flip vertically
  }
  else if (buffer.pixel_format == CA_YUYV422) {
	  libyuv::YUY2ToARGB(buffer.plane[0], buffer.stride[0], argbImage, width * 4, width, height);
  }
  else if (buffer.pixel_format == CA_YUV420BP) {
	  libyuv::NV12ToARGB(buffer.plane[0], buffer.stride[0], buffer.plane[1], buffer.stride[1], argbImage, width * 4, width, height);
  }

  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

  printf("Time for ARGB conversion: %f ms \n", time_span*1000);

  count++;

  if (false == wrote_frame & count > 40) { // wait for webcam to adapt to light conditions
    printf("write now");
	outfile.write((const char*)argbImage, width * height * 4);
    wrote_frame = true;
	must_run = false;
  }
}

void sig_handler(int sig) {
  printf("Handle signal.\n");
  must_run = false;
}
