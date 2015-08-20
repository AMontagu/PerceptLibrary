#################CMAKE FOR BUILD LIBRARY################

cmake_minimum_required(VERSION 2.8.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#project declaration
project(RobotLabLibrary)

option(BUILD_SAMPLE "if you want to build sample" ON)
find_package(OpenCV)
find_package(PocketSphinx)
find_package(SphinxBase)

#variable declaration
set(LIB_NAME RobotLabLibrary)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(POCKETSPHINX_LIB pocketsphinx sphinxbase)
set(OPENCV_LIB opencv_ts300 opencv_world300)

#add link for the lib
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PocketSphinx_INCLUDE_DIRS})
include_directories(${SphinxBase_INCLUDE_DIRS})

#auto find file
file(GLOB_RECURSE source_files src/*)
file(GLOB_RECURSE include_files include/*)
file(GLOB_RECURSE faceRecognizerContrib_files 3rdparty/faceRecognizerContrib/*)


#lib declaration
add_library(${LIB_NAME} STATIC ${source_files} ${include_files} ${faceRecognizerContrib_files})

#message("opencv lib :" ${OPENCV_LIB})
target_link_libraries( ${LIB_NAME} ${OpenCV_LIBRARIES} ${PocketSphinx_LIBRARIES} ${SphinxBase_LIBRARIES})


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