#ifndef VIDEO_H
#define VIDEO_H

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include <time.h>

#include "Constantes.h"
#include "Recognizer.h"

class Video
{
public:
	/** @brief Create a new Video object with the specified cam id.
	*
	* @param numberCam: Id of the camera you want to use.
	*/
	Video(int numberCam);
	/** @brief Create a new Video object with the any camera found openCV option.*/
	Video();

	/** @brief Start to get frame from the camera and print them in a windows. If some detection is activated start() will print the circle or the rectangle around the detected object. It need to be launched in threads.	*/
	int start();

	/** @brief Start the detection of faces, eyes and smiles in the video stream. start() function need to be in execution.*/
	void startAllDetect();

	/** @brief Stop all the detection running in the video stream.*/
	void stopAllDetect();

	/** @brief Start the detection of faces in the video stream. start() function need to be in execution.*/
	void startFaceDetect();

	/** @brief Stop the face detection running in the video stream.*/
	void stopFaceDetect();

	/** @brief Start the detection of smiles in the video stream. start() function need to be in execution.*/
	void startSmileDetect();

	/** @brief Stop the smile detection running in the video stream.*/
	void stopSmileDetect();

	/** @brief Start the detection of eyes in the video stream. start() function need to be in execution.*/
	void startEyeDetect();

	/** @brief Stop the Eye detection running in the video stream.*/
	void stopEyeDetect();

	/** @brief Start the detection of custom object in the video stream. start() function need to be in execution.
	*
	* @param cascade: CascadeClassifier object of the object you want to detect on the frame. 
	* @param precision: Specify how much neighbors each rectangle retain should have.If the detection is accurate there will be a lot of neighbors.
	*/
	void startCustomDetect(cv::CascadeClassifier& cascade, int precision);

	/** @brief Stop the custom object detection running in the video stream.*/
	void stopCustomDetect();

	/** @brief Analize a frame and try to find a face on it. If facedetect() find a face it will store the face into a classe variable that start() function will use for print circle or rectangle around.
	*
	* @param img: Frame in which research faces.
	*/
	int faceDetect(cv::Mat& img);

	/** @brief Analize a face frame and try to find eyes on it. If eyeDetect() find eyes it will store the face into a classe variable that start() function will use for print circle or rectangle around.
	*
	* @param img: Frame in which research eyes. Best used with a face image.
	* @param width: Width position of the img parameter. Used for find the right position in the principal frame captured by the cam.
	* @param height: Height position of the img parameter. Used for find the right position in the principal frame captured by the cam.
	*/
	int eyeDetect(cv::Mat& img, int width, int height);

	/** @brief Analize a face frame and try to find smile on it. If eyeDetect() find a smile it will store the face into a classe variable that start() function will use for print circle or rectangle around.
	*
	* @param img: Frame in which research smile. Best used with a face image.
	* @param width: Width position of the img parameter. Used for find the right position in the principal frame captured by the cam.
	* @param height: Height position of the img parameter. Used for find the right position in the principal frame captured by the cam.
	*/
	int smileDetect(cv::Mat& img, int width, int height);

	/** @brief Analize a frame and try to find an object like the haarcascade that the user give to compare in startCustomdetect() funtcion. If customDetect() find a face it will store the custom obbject into a classe variable that start() function will use for print circle or rectangle around.
	*
	* @param img: Frame in which research the custom object.
	*/
	int customDetect(cv::Mat& img);

	/** @brief Draw a circle or a rectangle at the rectangle variable position with the color variable on the img frame.
	*
	* @param rectangle: Rect object that defines the size and the position of the draw.
	* @param img: Specify on which image we are drawing.
	* @param color: Specify the color of the draw.
	*/
	void draw(cv::Rect rectangle, cv::Mat &img, cv::Scalar color);

	/** @brief Add text in a vector and the vector index of the face associated to the text. If the vector is not empty sthe start() function will print all the label.
	*
	* @param label: Name of the face recognized.
	* @param index: Vector index of the face recognized.
	*/
	void addLabel(std::string label, int index);

	/** @brief Remove all the text in the vector for stopping printing name.*/
	void clearLabel();

	/** @brief Return the number of face detected in the current frame.*/
	int getFaceNumber();

	/** @brief Return the number of smile detected in the current frame.*/
	int getSmileNumber();

	/** @brief Return the number of eyes detected in the current frame.*/
	int getEyeNumber();

	/** @brief Return the number of custom object detected in the current frame.*/
	int getObjectNumber();

	/** @brief Return true if the face detection process is running.*/
	bool getDetectFaceOn();

	/** @brief Return all the image of the faces detected on the last camera frame before the call of the function.*/
	std::vector<cv::Mat> getLastFacesDetected();

	/** @brief Return a booleen vector. The size of a vector is the number of detected face and the value is true is the face is smiling and false otherwise.*/
	std::vector<bool> getVectorSmiling();

	/** @brief Print to the console whiche face are smiling or not.*/
	void printVectorSmilingData();

	/** @brief Return 1 if the face is on the right side of the windows, 2 if it is on the left and 0 if it is on the cente.
	*
	* @param faceToTrack: Position and size of the face you want to track.
	* @param frame: The frame in which there is the face to track.
	*/
	int faceTracking(cv::Rect faceToTrack, cv::Mat& frame);

	/** @brief Get the value returned by faceTracking.*/
	int getTracking();

	/** @brief Set an image in _imgToPrint variable for display her in the video threads. Allow the user to see which face is detected to know if the recognition is good and if he need to add the image to the database.
	*
	* @param img: Give the image to display in a other openCv windows.
	*/
	void setImgToPrint(cv::Mat img);

	/** @brief Get the image displayed in parallele of the video stream.*/
	cv::Mat getImgToPrint();

	~Video();

private:
	bool _detectFaceOn = false, _detectSmileOn = false, _detectEyeOn = false, _detectCustomOn = false, _labelOn = false;
	int _numberCam, _faceNumber = 0, _smileNumber = 0, _eyeNumber = 0, _objectNumber = 0, _precisionForCustomObject = 3;
	int _tracking = 0;
	bool _recording = true, _timeToDraw = false;
	std::vector<bool> _faceAreSmiling;
	double _scale = 1;
	cv::CascadeClassifier _faceCascade, _smileCascade, _eyeCascade, _customCascade;
	cv::VideoCapture _capture;
	double time;
	std::vector<std::vector<cv::Rect>> _storeSmileDetected;
	std::vector<cv::Mat> _lastFacesDetected;
	std::vector<cv::Rect> _averageSmilesRect, _averageFacesRect, _averageEyesRect, _averageCustomRect;

	std::vector<std::string> _label;
	std::vector<int> _posX, _posY, _labelIndex;

	cv::Mat _imgToPrint;
};

#endif
