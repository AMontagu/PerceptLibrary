#include "Video.h"
#include "Constantes.h"


Video::Video(int numberCam)
{
	_capture.open(numberCam); //Open the cam connection
	namedWindow(WINDOWSNAME, WINDOW_NORMAL);
	time(&_timer);
}

int Video::start()
{
	
	_capture >> _frame; //The cam frames are stored in frame variable
	if (abs(time(NULL) - _timer) > 1)
	{
		_timeToDraw = true;
		time(&_timer);
	}
	else
	{
		_timeToDraw = false;
	}

	if (!_detectFaceOn && !_detectCustomOn)
	{
		imshow(WINDOWSNAME, _frame); // Print the frames in a windows. The windows name is in Constantes.h
	}

	if (_frame.empty())
		return -1;

	_frame.copyTo(_frameCopy);

	if (_detectFaceOn)
	{
		faceDetect(_frameCopy); //launch face detection smile and eye detection are launched in facedetect()
	}
	if (_detectCustomOn)
	{
		customDetect(_frameCopy); // launch custom detection
	}
	waitKey(1);
	return 0;
}



int Video::faceDetect(Mat& img)
{
	int faceNumberTemp = 0;
	double t = 0;
	vector<Rect> faces;
	//The differente color use for show face
	const static Scalar colors[] = { CV_RGB(0, 0, 255),
		CV_RGB(0, 128, 255),
		CV_RGB(0, 255, 255),
		CV_RGB(0, 255, 0),
		CV_RGB(255, 128, 0),
		CV_RGB(255, 255, 0),
		CV_RGB(255, 0, 0),
		CV_RGB(255, 0, 255) };

	Mat gray, smallImg(cvRound(img.rows / _scale), cvRound(img.cols / _scale), CV_8UC1); //Iniation two matrice for modify cam frame

	//The follows three fonctions transform the cam frame into a comptuter friendly frame for detection. To see it uncomment cv::imshow("test", smallImg);
	cvtColor(img, gray, COLOR_BGR2GRAY);
	resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
	equalizeHist(smallImg, smallImg);
	//cv::imshow("test", smallImg);

	t = (double)cvGetTickCount();
	//detectMultiScale() fund the matching objet with the cascadeClassifier (_faceCascade here) and add Rect type variable into the vector faces here.
	//for the options go to http://stackoverflow.com/questions/20801015/recommended-values-for-opencv-detectmultiscale-parameters
	_faceCascade.detectMultiScale(smallImg, faces, 1.1, 1, 1, Size(30, 30), Size(400, 400));

	//cout << "ici3" << endl;

	t = (double)cvGetTickCount() - t;
	//printf("detection time = %g ms\n", t / ((double)cvGetTickFrequency()*1000.));

	//foreach object find with detectMultiScale() we will draw a circle or a rectangle for show what detectMultiScale() have find
	for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, faceNumberTemp++)
	{
		//cout << "dans le for on dessine" << endl;
		Mat smallImgROI;
		Point center;
		Scalar color = colors[faceNumberTemp % 8];
		int radius;

		double aspect_ratio = (double)r->width / r->height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r->x + r->width*0.5)*_scale);
			center.y = cvRound((r->y + r->height*0.5)*_scale);
			radius = cvRound((r->width + r->height)*0.25*_scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
		else
		{
			rectangle(img, cvPoint(cvRound(r->x*_scale), cvRound(r->y*_scale)),
				cvPoint(cvRound((r->x + r->width - 1)*_scale), cvRound((r->y + r->height - 1)*_scale)),
				color, 3, 8, 0);
		}
		/*rectangle(img, cvPoint(cvRound(r->x*_scale), cvRound(r->y*_scale)),
		cvPoint(cvRound((r->x + r->width - 1)*_scale), cvRound((r->y + r->height - 1)*_scale)),
		color, 3, 8, 0); // this is if you prefere rectangle instead of circle*/

		//We transform r into a Mat variable for reuse it for detect eye and smile 
		smallImgROI = smallImg(*r);
		if (_detectSmileOn)
		{
			_faceAreSmiling.clear();
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
	if (!_detectSmileOn && !_detectEyeOn)
	{
		cv::imshow(WINDOWSNAME, img);
	}
	return 0;
}

//For the comment see faceDetect() juste some parameter for precision change
int Video::smileDetect(Mat& img, Mat& principalFrame, int width, int height)
{
	int smileNumberTemp = 0;
	vector<Rect> faces;
	//The differente color use for show smile
	const static Scalar colors[] = { CV_RGB(0, 0, 255),
		CV_RGB(0, 128, 255),
		CV_RGB(0, 255, 255),
		CV_RGB(0, 255, 0),
		CV_RGB(255, 128, 0),
		CV_RGB(255, 255, 0),
		CV_RGB(255, 0, 0),
		CV_RGB(255, 0, 255) };

	//cv::imshow("test5", img);


	_smileCascade.detectMultiScale(img, faces, 1.1, 30, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	if (!_timeToDraw)
	{

	}
	else
	{

	}
	for (vector<Rect>::iterator r = faces.begin(); r != faces.end(); r++, smileNumberTemp++)
	{
		if (r->y > img.size().height / 2)
		{
			//cout << "find a smile !" << endl;
			Mat smallImgROI;
			Point center;
			Scalar color = colors[smileNumberTemp % 8];

			img.size().height;

			rectangle(principalFrame, cvPoint(cvRound(r->x*_scale) + width, cvRound(r->y*_scale) + height),
				cvPoint(cvRound((r->x + r->width - 1)*_scale) + width, cvRound((r->y + r->height - 1)*_scale) + height),
				color, 3, 8, 0); // we draw on the principal frame because the img is only the face detected

			//cv::imshow("test2", smallImgROI);
		}
	}

	if (!_detectEyeOn)
	{
		cv::imshow(WINDOWSNAME, principalFrame);
	}
	return smileNumberTemp;
}

//For the comment see faceDetect() juste some parameter for precision change
int Video::eyeDetect(Mat& img, Mat& principalFrame, int width, int height)
{
	int eyeNumberTemp = 0;
	vector<Rect> faces;
	//The differente color use for show eye
	const static Scalar colors[] = { CV_RGB(0, 0, 255),
		CV_RGB(0, 128, 255),
		CV_RGB(0, 255, 255),
		CV_RGB(0, 255, 0),
		CV_RGB(255, 128, 0),
		CV_RGB(255, 255, 0),
		CV_RGB(255, 0, 0),
		CV_RGB(255, 0, 255) };

	_eyeCascade.detectMultiScale(img, faces, 1.3, 12, 0 | CASCADE_SCALE_IMAGE, Size(20, 20)); // We only change the minNeighbors parameters

	for (vector<Rect>::iterator r = faces.begin(); r != faces.end(); r++, eyeNumberTemp++)
	{
		//cout << "find a eye !" << endl;
		// if the detected objet is under the half of the face
		if (r->y < img.size().height / 2)
		{
			Mat smallImgROI;
			Point center;
			Scalar color = colors[eyeNumberTemp % 8];
			int radius;

			double aspect_ratio = (double)r->width / r->height;
			if (0.75 < aspect_ratio && aspect_ratio < 1.3)
			{
				center.x = cvRound((r->x + r->width*0.5)*_scale) + width;
				center.y = cvRound((r->y + r->height*0.5)*_scale) + height;
				radius = cvRound((r->width + r->height)*0.25*_scale);
				circle(principalFrame, center, radius, color, 3, 8, 0);
			}
			else
			{
				rectangle(principalFrame, cvPoint(cvRound(r->x*_scale) + width, cvRound(r->y*_scale) + height),
					cvPoint(cvRound((r->x + r->width - 1)*_scale) + width, cvRound((r->y + r->height - 1)*_scale) + height),
					color, 3, 8, 0);
			}

		}
	}

	cv::imshow(WINDOWSNAME, principalFrame);
	return eyeNumberTemp;
}


//For detect object from a CascadeClassifier object. More precision is up more the 
int Video::customDetect(Mat& img)
{
	int objectNumberTemp = 0;
	double t = 0;
	vector<Rect> faces;
	const static Scalar colors[] = { CV_RGB(0, 0, 255),
		CV_RGB(0, 128, 255),
		CV_RGB(0, 255, 255),
		CV_RGB(0, 255, 0),
		CV_RGB(255, 128, 0),
		CV_RGB(255, 255, 0),
		CV_RGB(255, 0, 0),
		CV_RGB(255, 0, 255) };

	Mat gray, smallImg(cvRound(img.rows / _scale), cvRound(img.cols / _scale), CV_8UC1);

	cvtColor(img, gray, COLOR_BGR2GRAY);
	resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
	equalizeHist(smallImg, smallImg);

	t = (double)cvGetTickCount();
	_customCascade.detectMultiScale(smallImg, faces, 1.1, _precisionForCustomObject, 1, Size(30, 30)); //_customCascade is charged in startCustomDetect()

	t = (double)cvGetTickCount() - t;
	//printf("detection time = %g ms\n", t / ((double)cvGetTickFrequency()*1000.));
	for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, objectNumberTemp++)
	{
		//cout << "find object" << endl;
		Mat smallImgROI;
		Point center;
		Scalar color = colors[objectNumberTemp % 8];

		rectangle(img, cvPoint(cvRound(r->x*_scale), cvRound(r->y*_scale)),
			cvPoint(cvRound((r->x + r->width - 1)*_scale), cvRound((r->y + r->height - 1)*_scale)),
			color, 3, 8, 0);
	}

	_objectNumber = objectNumberTemp;

	cv::imshow(WINDOWSNAME, img);
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


Video::~Video()
{
}
