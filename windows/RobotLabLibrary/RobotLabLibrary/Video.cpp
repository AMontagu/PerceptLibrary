#include "Video.h"
#include "Constantes.h"


Video::Video(int numberCam)
{
	_capture.open(numberCam); //Open the cam connection
	namedWindow(WINDOWSNAME, WINDOW_NORMAL);
	time = (double)cvGetTickCount();
}

int Video::start()
{
	Rect faceToTrack;
	Mat frame, frameCopy;
	_capture >> frame; //The cam frames are stored in frame variable

	if (frame.empty())
		return -1;
	//cout << (double)cvGetTickCount() - time << endl;
	if ((double)cvGetTickCount() - time > 150000)
	{
		frame.copyTo(frameCopy);

		if (_detectFaceOn)
		{
			_averageFacesRect.clear();
			_averageSmilesRect.clear();
			_averageEyesRect.clear();
			faceDetect(frameCopy); //launch face detection smile and eye detection are launched in facedetect()
		}
		if (_detectCustomOn)
		{
			_averageCustomRect.clear();
			customDetect(frameCopy); // launch custom detection
		}
		time = (double)cvGetTickCount();
	}
	
	
	
	for each (Rect r in _averageFacesRect)
	{
		draw(r, frame, CV_RGB(0, 255, 255));
		if (r.area() > faceToTrack.area())
		{
			faceToTrack = r;
		}
	}
	_tracking = faceTracking(faceToTrack, frame);
	for each (Rect r in _averageEyesRect)
	{
		draw(r, frame, CV_RGB(255, 0, 255));
	}
	for each (Rect r in _averageSmilesRect)
	{
		draw(r, frame, CV_RGB(255, 255, 0));
	}
	for each (Rect r in _averageCustomRect)
	{
		draw(r, frame, CV_RGB(255, 255, 0));
	}
	cv::imshow(WINDOWSNAME, frame);
	
	waitKey(1);
	return 0;
}



int Video::faceDetect(Mat& img)
{
	int faceNumberTemp = 0;
	//double t = 0;

	Mat gray, smallImg(cvRound(img.rows / _scale), cvRound(img.cols / _scale), CV_8UC1); //Iniation two matrice for modify cam frame

	//The follows three fonctions transform the cam frame into a comptuter friendly frame for detection. To see it uncomment cv::imshow("test", smallImg);
	cvtColor(img, gray, COLOR_BGR2GRAY);
	resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
	equalizeHist(smallImg, smallImg);
	//cv::imshow("test", smallImg);

	//t = (double)cvGetTickCount();
	//detectMultiScale() fund the matching objet with the cascadeClassifier (_faceCascade here) and add Rect type variable into the vector faces here.
	//for the options go to http://stackoverflow.com/questions/20801015/recommended-values-for-opencv-detectmultiscale-parameters
	_faceCascade.detectMultiScale(smallImg, _averageFacesRect, 1.1, 2, 1, Size(30, 30), Size(400, 400));

	//t = (double)cvGetTickCount() - t;
	//printf("detection time = %g ms\n", t / ((double)cvGetTickFrequency()*1000.));

	_faceAreSmiling.clear();

	//foreach object find with detectMultiScale() we will draw a circle or a rectangle for show what detectMultiScale() have find
	for (vector<Rect>::const_iterator r = _averageFacesRect.begin(); r != _averageFacesRect.end(); r++, faceNumberTemp++)
	{
		//cout << "find a face" << endl;
		Mat smallImgROI;

		//We transform r into a Mat variable for reuse it for detect eye and smile 
		smallImgROI = smallImg(*r);
		if (_detectSmileOn)
		{
			_smileNumber = smileDetect(smallImgROI, img, r->x, r->y);
			if (_smileNumber > 0)
			{
				_faceAreSmiling.push_back(true);
			}
			else
			{
				_faceAreSmiling.push_back(false);
			}
		}
		if (_detectEyeOn)
		{
			_eyeNumber = eyeDetect(smallImgROI, img, r->x, r->y);
		}
		//for see what image we send to te function you can uncomment this line
		//cv::imshow("test4", smallImgROI);
	}
	_faceNumber = faceNumberTemp;
	//In smile or eye detect we already print all the draw so for the optimisation I make this conditions
	return 0;
}

//For the comment see faceDetect() juste some parameter for precision change
int Video::smileDetect(Mat& img, Mat& principalFrame, int width, int height)
{
	int smileNumberTemp = 0;
	vector<Rect> averageSmilesRectTemp;

	_smileCascade.detectMultiScale(img, averageSmilesRectTemp, 1.4, 30, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	for (vector<Rect>::iterator r = averageSmilesRectTemp.begin(); r != averageSmilesRectTemp.end(); r++, smileNumberTemp++)
	{
		if (r->y > img.size().height / 2)
		{
			r->x += width;
			r->y += height;
			_averageSmilesRect.push_back(*r);
		}
	}

	/*if (!_timeToDraw)
	{
		vector<Rect> smileDetected;
		for each (Rect rect in averageSmilesRectTemp)
		{
			smileDetected.push_back(rect);
		}
		_storeSmileDetected.push_back(smileDetected);
	}
	else
	{
		_averageSmilesRect.clear();
		vector<int> averageX, averageY, averageWidth, averageHeight, smileNumberInCaptation;
		int captationNumber = 0, smileCaptationNumber = 0, MaxNumberOfSmileDetecting = 0;
		for each (vector<Rect> vectorRect in _storeSmileDetected)
		{
			for each (Rect smile in vectorRect)
			{
				if (smileCaptationNumber == MaxNumberOfSmileDetecting)
				{
					averageHeight.push_back(smile.height);
					averageWidth.push_back(smile.width);
					averageX.push_back(smile.x);
					averageY.push_back(smile.y);
				}
				else
				{
					averageHeight[smileCaptationNumber] += smile.height;
					averageWidth[smileCaptationNumber] += smile.width;
					averageX[smileCaptationNumber] += smile.x;
					averageY[smileCaptationNumber] += smile.y;
				}
				smileCaptationNumber++;
			}
			smileNumberInCaptation.push_back(smileCaptationNumber);
			smileCaptationNumber = 0;
			captationNumber++;
			for (int i = 0; i < MaxNumberOfSmileDetecting; i++)
			{
				//Rect rectTemp(averageX[i] / captationNumber, averageY[i] / captationNumber, averageWidth[i] / captationNumber, averageHeight[i] / captationNumber);
				//cout << " le sourire numero " << MaxNumberOfSmileDetecting << " a pour coordonnees :" << endl;
				//cout << " X : " << rectTemp.x << "  Y : " << rectTemp.y << "  widht : " << rectTemp.width << "  height : " << rectTemp.height << endl;
				//cout << endl;
				//_averageSmilesRect.push_back(rectTemp);
			}
		}
		_timeToDraw = false;
	}*/
	
	return smileNumberTemp;
}

//For the comment see faceDetect() juste some parameter for precision change
int Video::eyeDetect(Mat& img, Mat& principalFrame, int width, int height)
{
	int eyeNumberTemp = 0;
	vector<Rect> averageEyesRectTemp;

	_eyeCascade.detectMultiScale(img, averageEyesRectTemp, 1.3, 12, 0 | CASCADE_SCALE_IMAGE, Size(20, 20)); // We only change the minNeighbors parameters

	for (vector<Rect>::iterator r = averageEyesRectTemp.begin(); r != averageEyesRectTemp.end(); r++, eyeNumberTemp++)
	{
		//cout << "find a eye !" << endl;
		// if the detected objet is under the half of the face
		if (r->y < img.size().height / 2)
		{
			r->x += width;
			r->y += height;
			_averageEyesRect.push_back(*r);
		}
	}
	return eyeNumberTemp;
}


//For detect object from a CascadeClassifier object. More precision is up more the 
int Video::customDetect(Mat& img)
{
	int objectNumberTemp = 0;
	//double t = 0;
	vector<Rect> averageCustomRectTemp;

	Mat gray, smallImg(cvRound(img.rows / _scale), cvRound(img.cols / _scale), CV_8UC1);

	cvtColor(img, gray, COLOR_BGR2GRAY);
	resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
	equalizeHist(smallImg, smallImg);

	//t = (double)cvGetTickCount();
	_customCascade.detectMultiScale(smallImg, averageCustomRectTemp, 1.1, _precisionForCustomObject, 1, Size(30, 30)); //_customCascade is charged in startCustomDetect()

	//t = (double)cvGetTickCount() - t;
	//printf("detection time = %g ms\n", t / ((double)cvGetTickFrequency()*1000.));
	for (vector<Rect>::const_iterator r = averageCustomRectTemp.begin(); r != averageCustomRectTemp.end(); r++, objectNumberTemp++)
	{
		//Your conditions and other things before add the detected objet for drawing them.
		_averageCustomRect.push_back(*r);
	}

	_objectNumber = objectNumberTemp;
	return 0;
}


//Launch all faces detections
void Video::startAllDetect()
{
	startFaceDetect();
	startSmileDetect();
	startEyeDetect();
}

//Load the cascade classifier and turn on the detection
void Video::startFaceDetect()
{
	if (_faceCascade.empty())
	{
		if (!_faceCascade.load(cascadeFacePath))
		{
			cerr << "ERROR: Could not load classifier Facecascade" << endl;
		}
		else
		{
			cout << "classifier cascade is okay" << endl;
		}
		// TODO add a condition for launch facedetect only if cascade is find
		_detectFaceOn = true;
	}
	else
	{
		cout << "The face cascade is already initialized" << endl;
	}

}

void Video::stopFaceDetect()
{
	_detectFaceOn = false;
}

void Video::startSmileDetect()
{
	if (_smileCascade.empty())
	{
		if (!_smileCascade.load(cascadeSmilePath))
		{
			cerr << "ERROR: Could not load classifier smileCascade" << endl;
		}
		else
		{
			cout << "classifier cascade smile is okay" << endl;
		}
		// TODO add a condition for launch facedetect only if cascade is find
		_detectSmileOn = true;
	}
	else
	{
		cout << "The smile cascade already initialized" << endl;
	}

}

void Video::stopSmileDetect()
{
	_detectSmileOn = false;
}

void Video::startEyeDetect()
{
	if (_eyeCascade.empty())
	{
		if (!_eyeCascade.load(cascadeEyePath))
		{
			cerr << "ERROR: Could not load classifier eyeCascade" << endl;
		}
		else
		{
			cout << "classifier cascade eye is okay" << endl;
		}
		_detectEyeOn = true;
	}
	else
	{
		cout << "The eye cascade already initialized" << endl;
	}

}

void Video::stopEyeDetect()
{
	_detectEyeOn = false;
}

//We assign the custom cascade and launch the detection
void Video::startCustomDetect(CascadeClassifier& cascade, int precision)
{
	_customCascade = cascade;
	_precisionForCustomObject = precision;
	_detectCustomOn = true;
}

void Video::stopCustomDetect()
{
	_detectCustomOn = false;
}


int Video::getFaceNumber()
{
	return _faceNumber;
}

int Video::getSmileNumber()
{
	return _smileNumber;
}

int Video::getEyeNumber()
{
	return _eyeNumber;
}

int Video::getObjectNumber()
{
	return _objectNumber;
}

vector<bool> Video::getVectorSmiling()
{
	return _faceAreSmiling;
}

int Video::getTracking()
{
	return _tracking;
}

void Video::printVectorSmilingData()
{
	cout << "There is " << _faceAreSmiling.size() << " face detected :" << endl;
	int count = 1;
	for each (bool smile in _faceAreSmiling)
	{
		if (smile)
		{
			cout << "The face " << count << " is smiling." << endl;
		}
		else
		{
			cout << "The face " << count << " is not smiling." << endl;
		}
	}
}

//return 0 if the face is in the center, 1 for right and 2 for left.
int Video::faceTracking(Rect faceToTrack, Mat& frame)
{
	cout << "faceToTrack = " << faceToTrack.x << endl;
	cout << "frame.size = " << frame.size().width << endl;
	if (faceToTrack.x + faceToTrack.width / 2 > frame.size().width * 2 / 3)
	{
		return 1;
	}
	else if (faceToTrack.x + faceToTrack.width / 2 < frame.size().width / 3)
	{
		return 2;
	}
	else
	{
		return 0;
	}
}

void Video::draw(Rect r, Mat& img, Scalar color)
{

	Point center;
	int radius;

	double aspect_ratio = (double)r.width / r.height;
	if (0.75 < aspect_ratio && aspect_ratio < 1.3)
	{
		center.x = cvRound((r.x + r.width*0.5)*_scale);
		center.y = cvRound((r.y + r.height*0.5)*_scale);
		radius = cvRound((r.width + r.height)*0.25*_scale);
		circle(img, center, radius, color, 3, 8, 0);
	}
	else
	{
		rectangle(img, cvPoint(cvRound(r.x*_scale), cvRound(r.y*_scale)),
			cvPoint(cvRound((r.x + r.width - 1)*_scale), cvRound((r.y + r.height - 1)*_scale)),
			color, 3, 8, 0);
	}
}


Video::~Video()
{
}
