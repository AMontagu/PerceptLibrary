#################CMAKE FOR BUILD EXEC################
cmake_minimum_required(VERSION 2.8.8)

project(sampleProject)

set(EXECUTABLE_OUTPUT_PATH ${EXEC_NAME})
set(POCKETSPHINX_LIB pocketsphinx sphinxbase)
set(OPENCV_LIB opencv_ts300d opencv_world300d)
set(ROBOTLAB_LIB RobotLabLibrary)

#add link for the lib
include_directories(../3rdparty/opencv/include)
include_directories(../3rdparty/pocketSphinx/include)
include_directories(../3rdparty/sphinxbase/include)
include_directories(../include)
link_directories(../3rdparty/pocketSphinx/bin)
link_directories(../3rdparty/opencv/bin)
link_directories(../bin/VC12)


#executions config
add_executable(

        voiceRecognition

        voiceRecognition.cpp

)

add_executable(

        createFaceDatabase

        createFaceDatabase.cpp

)

add_executable(

        faceRecognition

        faceRecognition.cpp

)

add_executable(

        globalSample

        globalSample.cpp

)

add_executable(

        videoDetection

        videoDetection.cpp

)

add_executable(

        voiceAndDetection

        voiceAndDetection.cpp

)


#link the lib to the executable file
target_link_libraries( voiceRecognition ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})
target_link_libraries( createFaceDatabase ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})
target_link_libraries( faceRecognition ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})
target_link_libraries( globalSample ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})
target_link_libraries( videoDetection ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})
target_link_libraries( voiceAndDetection ${OPENCV_LIB} ${POCKETSPHINX_LIB} ${ROBOTLAB_LIB})