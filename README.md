# webcam-capture-driver-native

Driver connecting the video-capture-library by roxlu with the "Webcam Capture" Java library by sarxos

## Why use this driver?

Benefits when used on Windows:

1. Uses Microsoft Media Foundation for capturing webcam images, thus being compatible with some Windows 8 tablets that do not work with the default implementation used by the sarxos webcam library
2. Ability to query and set webcam capture resolutions
3. Lower CPU load than default implementation used by sarxos webcam capture library (Note: Make sure to use the same resolution when benchmarking CPU usage. This driver allows also to capture high resolution webcam images and CPU load increases the higher the resolution that is captured)

## How to use with the sarxos webcam capture library for Java

1. Setup the sarxos webcam capture library for your project
  1. Download the library at https://github.com/sarxos/webcam-capture
  2. Include the webcam-capture-0.3.10.jar into your project
  3. Follow the examples given there to get the sarxos library up and running
2. Setup this driver
  1. Download bin/webcam-capture-driver-native.jar and include it into your project
  2. Download the native libraries required by your system (see bin folder):
    * Windows: webcam-capture-driver-native-32.dll and webcam-capture-driver-native-64.dll (you may also just download the version matching the bitness of your Java Runtime)
    * Mac: libwebcam-capture-driver-native-64.dylib
    * Linux: not yet supported
  3. Make sure that the libraries are found by your Java Runtime: Either put them into your projects working directory or set the library path with "java -Djava.library.path=[insert path to dll or dylib here]"
  4. Add the following code in your project to use this driver: "Webcam.setDriver(new NativeWebcamDriver());"

## How to build

1. Checkout the master branch (including the submodules src/video-capture-library and src/libyuv)
2. Use CMake to build:
  1. Download CMake at https://cmake.org/ (version 3.5.1 or above)
  2. Start CMake GUI
  3. Choose "src" as source folder
  4. Choose "build32" or "build64" as build folder (depending on the bitness your are going to build)
  5. Click "Configure"
  6. Specify the Generator. I use:
    * Windows: "Visual Studio 14 2015" (for 32 bit build) and "Visual Studio 14 2015 Win64" (for 64 bit build)
    * Mac: "Xcode"
  7. Click "Generate"
  8. Browse to the build folder and build with your native compiler, e.g. open WebcamCaptureDriverNative.sln on Windows and build "ALL_BUILD" and then build "INSTALL" to copy the required files to the "bin" directory
  



