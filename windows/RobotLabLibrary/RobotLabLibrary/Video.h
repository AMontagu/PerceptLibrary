#ifndef VIDEO_H
#define VIDEO_H

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include <time.h>

using namespace std;
using namespace cv;

class Video
{
public:
	Video(int numberCam);

	void startAllDetect();
	void startFaceDetect();
	void stopFaceDetect();
	void startSmileDetect();
	void stopSmileDetect();
	void startEyeDetect();
	void stopEyeDetect();
	void startCustomDetect(CascadeClassifier& cascade);
	void stopCustomDetect();

	int start();

	int faceDetect(Mat& img);
	int eyeDetect(Mat& img, Mat& principalFrame, int width, int height);
	int smileDetect(Mat& img, Mat& principalFrame, int width, int height);
	int customDetect(Mat& img, int precision);

	int getVisageNumber();

	~Video();


private:
	bool _detectFaceOn = false, _detectSmileOn = false, _detectEyeOn = false, _detectCustomOn = false;
	int _numberCam, _visageNumber = 0;
	bool _recording = true;
	Mat _frame, _frameCopy;
	double _scale = 1;
	CascadeClassifier _faceCascade, _smileCascade, _eyeCascade, _customCascade;
	VideoCapture _capture;

};

#endif
