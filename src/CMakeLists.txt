cmake_minimum_required(VERSION 3.5)

project (WebcamCaptureDriverNative)

find_package(JNI REQUIRED)
find_package(Java 1.7 COMPONENTS Development REQUIRED)
include(UseJava)

set(libyuv-path  "libyuv")
set(video-capture-library-path  "video-capture-library")
set(webcam-capture-jar "${CMAKE_CURRENT_SOURCE_DIR}/java/extern/webcam-capture-0.3.10.jar")
set(slf4j-jar "${CMAKE_CURRENT_SOURCE_DIR}/java/extern/slf4j-api-1.7.2.jar")

enable_testing()

set(USE_GENERATE_X86 ON)
add_subdirectory(${video-capture-library-path}/build)

add_subdirectory(${libyuv-path})


add_jar(webcam-capture-driver-native-jar
			SOURCES
				java/driver/com/github/sarxos/webcam/ds/nativeapi/NativeWebcamDevice.java
				java/driver/com/github/sarxos/webcam/ds/nativeapi/NativeWebcamDriver.java
			INCLUDE_JARS
				${webcam-capture-jar}
			OUTPUT_NAME
				webcam-capture-driver-native
)

create_javah(TARGET webcam-capture-driver-native-javah
				CLASSES
					com.github.sarxos.webcam.ds.nativeapi.NativeWebcamDevice
					com.github.sarxos.webcam.ds.nativeapi.NativeWebcamDriver
				CLASSPATH
					webcam-capture-driver-native-jar;${webcam-capture-jar}
)

add_library(webcam-capture-driver-native SHARED
				jni/com_github_sarxos_webcam_ds_nativeapi_NativeWebcamDevice.cpp
				jni/com_github_sarxos_webcam_ds_nativeapi_NativeWebcamDriver.cpp
)
target_include_directories(webcam-capture-driver-native PRIVATE ${JNI_INCLUDE_DIRS} ${CMAKE_BINARY_DIR} ${video-capture-library-path}/include ${libyuv-path}/include)
add_dependencies(webcam-capture-driver-native webcam-capture-driver-native-javah)
target_link_libraries(webcam-capture-driver-native videocapture yuv)






add_jar(WebcamCaptureTestUseNativeDriver
			SOURCES
				test/WebcamCaptureTestUseNativeDriver.java
			INCLUDE_JARS
				${webcam-capture-jar}
				webcam-capture-driver-native-jar
)
add_dependencies(WebcamCaptureTestUseNativeDriver webcam-capture-driver-native)
add_test(NAME TestWebcamCaptureTestUseNativeDriver COMMAND ${Java_JAVA_EXECUTABLE} -Djava.library.path=$<CONFIGURATION> -cp "WebcamCaptureTestUseNativeDriver.jar;webcam-capture-driver-native.jar;${webcam-capture-jar};${slf4j-jar}" WebcamCaptureTestUseNativeDriver)

add_executable(WebcamCaptureTestCaptureToFile test/WebcamCaptureTestCaptureToFile.cpp) 
target_include_directories(WebcamCaptureTestCaptureToFile PRIVATE ${JNI_INCLUDE_DIRS} ${CMAKE_BINARY_DIR} ${video-capture-library-path}/include ${libyuv-path}/include)
target_link_libraries(WebcamCaptureTestCaptureToFile videocapture yuv)
add_test(NAME TestWebcamCaptureTestCaptureToFile COMMAND WebcamCaptureTestCaptureToFile)

add_jar(WebcamCaptureTestShowCapturedFile
			SOURCES
				test/WebcamCaptureTestShowCapturedFile.java
)
add_dependencies(WebcamCaptureTestShowCapturedFile WebcamCaptureTestCaptureToFile)
add_test(NAME TestWebcamCaptureTestShowCapturedFile COMMAND ${Java_JAVA_EXECUTABLE} -cp "WebcamCaptureTestShowCapturedFile.jar" WebcamCaptureTestShowCapturedFile "captured.rgb")


# Build with /MT instead of /MD on Windows
if(MSVC)
	foreach(flag_var
			CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
			CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
	   if(${flag_var} MATCHES "/MD")
		  string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
	   endif(${flag_var} MATCHES "/MD")
	endforeach(flag_var)
endif()