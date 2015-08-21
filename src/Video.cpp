#include "../include/Video.h"


Video::Video()
{
	_numberCam = -1;
}

Video::Video(int numberCam)
{
	_numberCam = numberCam;
}

int Video::start()
{
	if (!_capture.isOpened())
	{
		_capture.open(_numberCam); //Open the cam connection
		cv::namedWindow(WINDOWSNAME, cv::WINDOW_NORMAL);
		time = (double)cv::getTickCount(); //initialize time
	}
	cv::Rect faceToTrack;
	cv::Mat frame, frameCopy;
	int i = 0;

	std::vector<std::string> label;
	std::vector<int>  labelIndex;

	while (true)
	{
		_capture >> frame; //The cam frames are stored in frame variable

		if (frame.empty())
		{
			std::cout << "your camera is not found or is not functional" << std::endl;
			return -1;
		}
		//cout << (double)cvGetTickCount() - time << endl;
		if ((double)cv::getTickCount() - time > 400000)
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
			time = (double)cv::getTickCount();
		}
		if (_detectFaceOn)
		{
			for (std::vector<cv::Rect>::iterator r = _averageFacesRect.begin(); r != _averageFacesRect.end(); r++)
			{
				draw(*r, frame, cv::Scalar(0, 255, 255));
				if (r->area() > faceToTrack.area())
				{
					faceToTrack = *r;
				}
			}
		}
		else
		{
			_averageFacesRect.clear();
		}
		_tracking = faceTracking(faceToTrack, frame);
		if (_detectEyeOn)
		{
			for (std::vector<cv::Rect>::iterator r = _averageEyesRect.begin(); r != _averageEyesRect.end(); r++)
			{
				draw(*r, frame, cv::Scalar(255, 0, 255));
			}
		}
		else
		{
			_averageEyesRect.clear();
		}
		if (_detectSmileOn)
		{
			for (std::vector<cv::Rect>::iterator r = _averageSmilesRect.begin(); r != _averageSmilesRect.end(); r++)
			{
				draw(*r, frame, cv::Scalar(255, 255, 0));
			}
		}
		else
		{
			_averageSmilesRect.clear();
		}
		if (_detectCustomOn)
		{
			for (std::vector<cv::Rect>::iterator r = _averageCustomRect.begin(); r != _averageCustomRect.end(); r++)
			{
				draw(*r, frame, cv::Scalar(100, 100, 100));
			}
		}
		else
		{
			_averageCustomRect.clear();
		}
		if (_labelOn)
		{
			label = _label; //For avoid to use the value when the vector is clean in a other thread
			labelIndex = _labelIndex; //For avoid to use the value when the vector is clean in a other thread
			i = 0;
			for (std::vector<std::string>::iterator lab = label.begin(); lab != label.end(); lab++, i++)
			{
				if (labelIndex.empty())
				{
					std::cout << "a strange error appear please add a condition here !" << std::endl;
				}
				if ((int)_lastFacesDetected.size() > labelIndex[i])
				{
					cv::putText(frame, *lab, cv::Point(_posX[labelIndex[i]], _posY[labelIndex[i]]), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 255, 255), 2);
				}
			}
		}
		cv::imshow(WINDOWSNAME, frame);

		if (!_imgToPrint.empty())
		{
			cv::imshow("Face to save", _imgToPrint);
		}
		else
		{
			cv::destroyWindow("Face to save");
		}

		cv::waitKey(1);
	}
	
	return 0;
}



int Video::faceDetect(cv::Mat& img)
{
	int faceNumberTemp = 0;
	//double t = 0;

	cv::Mat gray, smallImg(cvRound(img.rows / _scale), cvRound(img.cols / _scale), CV_8UC1); //Iniation two matrice for modify cam frame

	//The follows three fonctions transform the cam frame into a comptuter friendly frame for detection. To see it uncomment cv::imshow("test", smallImg);
	cvtColor(img, gray, cv::COLOR_BGR2GRAY);
	resize(gray, smallImg, smallImg.size(), 0, 0, cv::INTER_LINEAR);
	equalizeHist(smallImg, smallImg);
	//cv::imshow("test", smallImg);

	//t = (double)cvGetTickCount();
	//detectMultiScale() fund the matching objet with the cascadeClassifier (_faceCascade here) and add Rect type variable into the vector faces here.
	//for the options go to http://stackoverflow.com/questions/20801015/recommended-values-for-opencv-detectmultiscale-parameters
	_faceCascade.detectMultiScale(smallImg, _averageFacesRect, 1.1, 2, 1, cv::Size(30, 30), cv::Size(400, 400));

	//t = (double)cvGetTickCount() - t;
	//printf("detection time = %g ms\n", t / ((double)cvGetTickFrequency()*1000.));

	_faceAreSmiling.clear();
	_lastFacesDetected.clear();
	_posX.clear();
	_posY.clear();

	//foreach object find with detectMultiScale() we will draw a circle or a rectangle for show what detectMultiScale() have find
	for (std::vector<cv::Rect>::const_iterator r = _averageFacesRect.begin(); r != _averageFacesRect.end(); r++, faceNumberTemp++)
	{
		//cout << "find a face" << endl;

		//We transform r into a Mat variable for reuse it for detect eye and smile 
		_lastFacesDetected.push_back(smallImg(*r));
		//we save the face postion for maybe add a label
		_posX.push_back(r->x);
		_posY.push_back(r->y);
		if (_detectSmileOn)
		{
			_smileNumber = smileDetect(_lastFacesDetected[faceNumberTemp], r->x, r->y);
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
			_eyeNumber = eyeDetect(_lastFacesDetected[faceNumberTemp], r->x, r->y);
		}
		//for see what image we send to te function you can uncomment this line
		//cv::imshow("test4", smallImgROI);
	}
	_faceNumber = faceNumberTemp;
	//In smile or eye detect we already print all the draw so for the optimisation I make this conditions
	return 0;
}

//For the comment see faceDetect() juste some parameter for precision change
int Video::smileDetect(cv::Mat& img, int width, int height)
{
	int smileNumberTemp = 0;
	std::vector<cv::Rect> averageSmilesRectTemp;

	_smileCascade.detectMultiScale(img, averageSmilesRectTemp, 1.3, 10, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(20, 20));

	for (std::vector<cv::Rect>::iterator r = averageSmilesRectTemp.begin(); r != averageSmilesRectTemp.end(); r++, smileNumberTemp++)
	{
		if (r->y > img.size().height / 2 && r->x + r->width / 2 > img.size().width / 2 - 25 && r->x + r->width / 2 < img.size().width / 2 + 25 && _averageSmilesRect.empty())
		{
			r->x += width;
			r->y += height;
			_averageSmilesRect.push_back(*r);
		}
	}
	return smileNumberTemp;
}

//For the comment see faceDetect() juste some parameter for precision change
int Video::eyeDetect(cv::Mat& img, int width, int height)
{
	int eyeNumberTemp = 0;
	std::vector<cv::Rect> averageEyesRectTemp;

	_eyeCascade.detectMultiScale(img, averageEyesRectTemp, 1.2, 10, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(5, 5)); // We only change the minNeighbors parameters

	for (std::vector<cv::Rect>::iterator r = averageEyesRectTemp.begin(); r != averageEyesRectTemp.end(); r++)
	{
		//cout << "find a eye !" << endl;
		// if the detected objet is under the half of the face
		if (eyeNumberTemp < 2)
		{
			if (r->y < img.size().height / 2)
			{
				r->x += width;
				r->y += height;
				_averageEyesRect.push_back(*r);
				eyeNumberTemp++;
			}
		}
	}
	return eyeNumberTemp;
}


//For detect object from a CascadeClassifier object. More precision is up more the 
int Video::customDetect(cv::Mat& img)
{
	int objectNumberTemp = 0;
	//double t = 0;
	std::vector<cv::Rect> averageCustomRectTemp;

	cv::Mat gray, smallImg(cvRound(img.rows / _scale), cvRound(img.cols / _scale), CV_8UC1);

	cvtColor(img, gray, cv::COLOR_BGR2GRAY);
	resize(gray, smallImg, smallImg.size(), 0, 0, cv::INTER_LINEAR);
	equalizeHist(smallImg, smallImg);

	//t = (double)cvGetTickCount();
	_customCascade.detectMultiScale(smallImg, averageCustomRectTemp, 1.1, _precisionForCustomObject, 1, cv::Size(30, 30)); //_customCascade is charged in startCustomDetect()

	//t = (double)cvGetTickCount() - t;
	//printf("detection time = %g ms\n", t / ((double)cvGetTickFrequency()*1000.));
	for (std::vector<cv::Rect>::const_iterator r = averageCustomRectTemp.begin(); r != averageCustomRectTemp.end(); r++, objectNumberTemp++)
	{
		//Your conditions and other things before add the detected objet for drawing them.
		_averageCustomRect.push_back(*r);
	}

	_objectNumber = objectNumberTemp;
	return 0;
}


//Launch all faces detections
void Video::startAllDetect(const char* cascadeFacePath, const char* cascadeSmilePath, const char* cascadeEyePath)
{
	startFaceDetect(cascadeFacePath);
	startSmileDetect(cascadeSmilePath);
	startEyeDetect(cascadeEyePath);
}

void Video::stopAllDetect()
{
	stopEyeDetect();
	stopSmileDetect();
	stopFaceDetect();
}

//Load the cascade classifier and turn on the detection
void Video::startFaceDetect(const char* cascadeFacePath)
{
	if (_faceCascade.empty())
	{
		if (!_faceCascade.load(cascadeFacePath))//We load the Haar model here. Other is juste for form
		{
			std::cerr << "ERROR: Could not load classifier Facecascade" << std::endl;
		}
		else
		{
			std::cout << "classifier cascade face is okay" << std::endl;
			_detectFaceOn = true;//need this for call cognition function and draw.
		}
	}
	else
	{
		std::cout << "The face cascade is already initialized" << std::endl;
		_detectFaceOn = true;//need this for call cognition function and draw.
	}

}

void Video::stopFaceDetect()
{
	_detectFaceOn = false;
}

void Video::startSmileDetect(const char* cascadeSmilePath)
{
	if (_smileCascade.empty())
	{
		if (!_smileCascade.load(cascadeSmilePath))
		{
			std::cerr << "ERROR: Could not load classifier smileCascade" << std::endl;
		}
		else
		{
			std::cout << "classifier cascade smile is okay" << std::endl;
			_detectSmileOn = true;
		}
	}
	else
	{
		std::cout << "The smile cascade already initialized" << std::endl;
		_detectSmileOn = true;
	}

}

void Video::stopSmileDetect()
{
	_detectSmileOn = false;
}

void Video::startEyeDetect(const char* cascadeEyePath)
{
	if (_eyeCascade.empty())
	{
		if (!_eyeCascade.load(cascadeEyePath))
		{
			std::cerr << "ERROR: Could not load classifier eyeCascade" << std::endl;
		}
		else
		{
			std::cout << "classifier cascade eye is okay" << std::endl;
			_detectEyeOn = true;
		}
	}
	else
	{
		std::cout << "The eye cascade already initialized" << std::endl;
		_detectEyeOn = true;
	}

}

void Video::stopEyeDetect()
{
	_detectEyeOn = false;
}

//We assign the custom cascade and launch the detection
void Video::startCustomDetect(cv::CascadeClassifier& cascade, int precision)
{
	_customCascade = cascade;
	_precisionForCustomObject = precision;
	_detectCustomOn = true;
}

void Video::stopCustomDetect()
{
	_detectCustomOn = false;
}

void Video::addLabel(std::string label, int index)
{
	_label.push_back(label);
	_labelIndex.push_back(index);
	_labelOn = true;
}

void Video::clearLabel()
{
	_label.clear();
	_labelIndex.clear();
	_labelOn = false;
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

bool Video:: getDetectFaceOn()
{
	return _detectFaceOn;
}

std::vector<bool> Video::getVectorSmiling()
{
	return _faceAreSmiling;
}

int Video::getTracking()
{
	return _tracking;
}

std::vector<cv::Mat> Video::getLastFacesDetected()
{
	return _lastFacesDetected;
}

//Simple function for print who smile
void Video::printVectorSmilingData()
{
	std::cout << "There is " << _faceAreSmiling.size() << " face detected :" << std::endl;
	int count = 1;
	for (std::vector<bool>::const_iterator smile = _faceAreSmiling.begin(); smile != _faceAreSmiling.end(); smile++)
	{
		if (*smile)
		{
			std::cout << "The face " << count << " is smiling." << std::endl;
		}
		else
		{
			std::cout << "The face " << count << " is not smiling." << std::endl;
		}
	}
}

//return 0 if the face is in the center, 1 for right and 2 for left.
int Video::faceTracking(cv::Rect faceToTrack, cv::Mat& frame)
{
	//Simple calcul for know with the rect of an object on where side of the frame is it.
	if (faceToTrack.x + faceToTrack.width / 2 > frame.size().width * 2 / 3)
	{
		return 1; //right
	}
	else if (faceToTrack.x + faceToTrack.width / 2 < frame.size().width / 3)
	{
		return 2; //left
	}
	else
	{
		return 0; //center
	}
}

void Video::draw(cv::Rect r, cv::Mat& img, cv::Scalar color)
{

	cv::Point center;
	int radius;

	double aspect_ratio = (double)r.width / r.height;
	if (0.75 < aspect_ratio && aspect_ratio < 1.3) // if the ratio said that the object is more round
	{
		center.x = std::round((r.x + r.width*0.5)*_scale);
		center.y = std::round((r.y + r.height*0.5)*_scale);
		radius = std::round((r.width + r.height)*0.25*_scale);
		circle(img, center, radius, color, 3, 8, 0); // draw circle
	}
	else // if the object is more rectangle
	{
		rectangle(img, cv::Point(std::round(r.x*_scale), std::round(r.y*_scale)), cv::Point(std::round((r.x + r.width - 1)*_scale), std::round((r.y + r.height - 1)*_scale)), color, 3, 8, 0); //Draw circle
	}
}


void Video::setImgToPrint(cv::Mat img)
{
	_imgToPrint = img;
}

cv::Mat Video::getImgToPrint()
{
	return _imgToPrint;
}


Video::~Video()
{
}
