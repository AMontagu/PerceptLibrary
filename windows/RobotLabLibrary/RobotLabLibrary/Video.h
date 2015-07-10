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
	void startCustomDetect(CascadeClassifier& cascade, int precision);
	void stopCustomDetect();

	int start();

	int faceDetect(Mat& img);
	int eyeDetect(Mat& img, Mat& principalFrame, int width, int height);
	int smileDetect(Mat& img, Mat& principalFrame, int width, int height);
	int customDetect(Mat& img);
	void draw(Rect rectangle, Mat &img, Scalar color);

	//print the real number of specified object detected. Some smile or eye detected aren't draw for more accuracy but count in this functions
	int getFaceNumber();
	int getSmileNumber();
	int getEyeNumber();
	int getObjectNumber();
	vector<bool> Video::getVectorSmiling();
	int faceTracking(Rect faceToTrack, Mat& frame);
	int getTracking();

	void Video::printVectorSmilingData();

	~Video();


private:
	bool _detectFaceOn = false, _detectSmileOn = false, _detectEyeOn = false, _detectCustomOn = false;
	int _numberCam, _faceNumber = 0, _smileNumber = 0, _eyeNumber = 0, _objectNumber = 0, _precisionForCustomObject = 3;
	int _tracking = 0;
	bool _recording = true, _timeToDraw = false;;
	vector<bool> _faceAreSmiling ;
	double _scale = 1;
	CascadeClassifier _faceCascade, _smileCascade, _eyeCascade, _customCascade;
	VideoCapture _capture;
	double time;
	vector<vector<Rect>> _storeSmileDetected;
	vector<Rect> _averageSmilesRect, _averageFacesRect, _averageEyesRect, _averageCustomRect;

};

#endif
