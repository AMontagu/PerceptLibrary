#################CMAKE FOR BUILD LIBRARY################

cmake_minimum_required(VERSION 2.8.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

#project declaration
project(RobotLabLibrary)

if(NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(NOT WIN32)

option(BUILD_SAMPLE "if you want to build sample" ON)
if(NOT WIN32)
	find_package(OpenCV)
	find_package(PocketSphinx)
	find_package(SphinxBase)
endif(NOT WIN32)

#variable declaration
set(LIB_NAME RobotLabLibrary)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if(WIN32)
	set(POCKETSPHINX_LIB pocketsphinx sphinxbase)
	set(OPENCV_LIB opencv_ts300 opencv_world300)
endif(WIN32)

#add link for the lib
if(WIN32)
	include_directories(3rdparty/opencv/include)
	include_directories(3rdparty/pocketSphinx/include)
	include_directories(3rdparty/sphinxbase/include)
	link_directories(3rdparty/pocketSphinx/bin)
	link_directories(3rdparty/opencv/bin)
else(WIN32)
	include_directories(${POCKETSPHINX_INCLUDE_DIRS})
	include_directories(${SPHINXBASE_INCLUDE_DIRS})
endif(WIN32)


#auto find file
file(GLOB_RECURSE source_files src/*)
file(GLOB_RECURSE include_files include/*)
file(GLOB_RECURSE faceRecognizerContrib_files 3rdparty/faceRecognizerContrib/*)


#lib declaration
add_library(${LIB_NAME} STATIC ${source_files} ${include_files} ${faceRecognizerContrib_files})

if(WIN32)
	target_link_libraries( ${LIB_NAME} ${OPENCV_LIB} ${POCKETSPHINX_LIB})
else(WIN32)
	target_link_libraries( ${LIB_NAME} ${OpenCV_LIBS} ${POCKETSPHINX_LIBRARIES} ${SPHINXBASE_LIBRARIES} ${SPHINXAD_LIBRARIES})
endif(WIN32)

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

	if(WIN32)
		target_link_libraries( voiceRecognition ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})
		target_link_libraries( createFaceDatabase ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})
		target_link_libraries( faceRecognition ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})
		target_link_libraries( globalSample ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})
		target_link_libraries( videoDetection ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})
		target_link_libraries( voiceAndDetection ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})
	else(WIN32)
		target_link_libraries( voiceRecognition ${OpenCV_LIBS} ${POCKETSPHINX_LIBRARIES} ${SPHINXBASE_LIBRARIES} ${SPHINXAD_LIBRARIES} ${ROBOTLAB_LIB})
		target_link_libraries( createFaceDatabase ${OpenCV_LIBS} ${POCKETSPHINX_LIBRARIES} ${SPHINXBASE_LIBRARIES} ${SPHINXAD_LIBRARIES} ${ROBOTLAB_LIB})
		target_link_libraries( faceRecognition ${OpenCV_LIBS} ${POCKETSPHINX_LIBRARIES} ${SPHINXBASE_LIBRARIES} ${SPHINXAD_LIBRARIES} ${ROBOTLAB_LIB})
		target_link_libraries( globalSample ${OpenCV_LIBS} ${POCKETSPHINX_LIBRARIES} ${SPHINXBASE_LIBRARIES} ${SPHINXAD_LIBRARIES} ${ROBOTLAB_LIB})
		target_link_libraries( videoDetection ${OpenCV_LIBS} ${POCKETSPHINX_LIBRARIES} ${SPHINXBASE_LIBRARIES} ${SPHINXAD_LIBRARIES} ${ROBOTLAB_LIB})
		target_link_libraries( voiceAndDetection ${OpenCV_LIBS} ${POCKETSPHINX_LIBRARIES} ${SPHINXBASE_LIBRARIES} ${SPHINXAD_LIBRARIES} ${ROBOTLAB_LIB})
	endif(WIN32)

endif(BUILD_SAMPLE)