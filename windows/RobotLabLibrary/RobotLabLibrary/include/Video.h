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
	Video(int numberCam);
	Video();

	int start();

	void startAllDetect();
	void stopAllDetect();
	void startFaceDetect();
	void stopFaceDetect();
	void startSmileDetect();
	void stopSmileDetect();
	void startEyeDetect();
	void stopEyeDetect();
	void startCustomDetect(cv::CascadeClassifier& cascade, int precision);
	void stopCustomDetect();
	void addLabel(std::string label, int index);
	void clearLabel();

	int faceDetect(cv::Mat& img);
	int eyeDetect(cv::Mat& img, cv::Mat& principalFrame, int width, int height);
	int smileDetect(cv::Mat& img, cv::Mat& principalFrame, int width, int height);
	int customDetect(cv::Mat& img);
	void draw(cv::Rect rectangle, cv::Mat &img, cv::Scalar color);

	//print the real number of specified object detected. Some smile or eye detected aren't draw for more accuracy but count in this functions
	int getFaceNumber();
	int getSmileNumber();
	int getEyeNumber();
	int getObjectNumber();
	std::vector<cv::Mat> getLastFacesDetected();
	std::vector<bool> getVectorSmiling();
	int faceTracking(cv::Rect faceToTrack, cv::Mat& frame);
	int getTracking();
	void printVectorSmilingData();

	void setImgToPrint(cv::Mat img);
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
