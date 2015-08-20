#################CMAKE FOR BUILD LIBRARY################

cmake_minimum_required(VERSION 2.8.8)

#project declaration
project(RobotLabLibrary)

option(BUILD_SAMPLE "if you want to build sample" ON)

#variable declaration
set(LIB_NAME RobotLabLibrary)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(POCKETSPHINX_LIB pocketsphinx sphinxbase)
set(OPENCV_LIB opencv_ts300 opencv_world300)

#add link for the lib
include_directories(3rdparty/opencv/include)
include_directories(3rdparty/pocketSphinx/include)
include_directories(3rdparty/sphinxbase/include)
link_directories(3rdparty/pocketSphinx/bin)
link_directories(3rdparty/opencv/bin)

#auto find file
file(GLOB_RECURSE source_files src/*)
file(GLOB_RECURSE include_files include/*)
file(GLOB_RECURSE faceRecognizerContrib_files 3rdparty/faceRecognizerContrib/*)


#lib declaration
add_library(${LIB_NAME} STATIC ${source_files} ${include_files} ${faceRecognizerContrib_files})

#message("opencv lib :" ${OPENCV_LIB})
target_link_libraries( ${LIB_NAME} ${OPENCV_LIB} ${POCKETSPHINX_LIB})


#build sample
if(BUILD_SAMPLE)
	set(ROBOTLAB_LIB RobotLabLibrary)

	include_directories(include)
	link_directories(bin)

	#executions config
	add_executable(

	        voiceRecognition

	        samples/voiceRecognition.cpp

	)

	add_executable(

	        createFaceDatabase

	        samples/createFaceDatabase.cpp

	)

	add_executable(

	        faceRecognition

	        samples/faceRecognition.cpp

	)

	add_executable(

	        globalSample

	        samples/globalSample.cpp

	)

	add_executable(

	        videoDetection

	        samples/videoDetection.cpp

	)

	add_executable(

	        voiceAndDetection

	        samples/voiceAndDetection.cpp

	)


	target_link_libraries( voiceRecognition ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})
	target_link_libraries( createFaceDatabase ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})
	target_link_libraries( faceRecognition ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})
	target_link_libraries( globalSample ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})
	target_link_libraries( videoDetection ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})
	target_link_libraries( voiceAndDetection ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})

endif(BUILD_SAMPLE)