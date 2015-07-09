#include "Video.h"
#include "Constantes.h"


Video::Video(int numberCam)
{
	_capture.open(numberCam); //Open the cam connection
}

int Video::start()
{
	_capture >> _frame; //The cam frames are stored in frame variable
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
		customDetect(_frameCopy, 3); // launch custom detection with a precision of 3
	}
	waitKey(1);
	return 0;
}



int Video::faceDetect(Mat& img)
{
	int i = 0, visageNumberTemp = 0;
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
	for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++)
	{
		visageNumberTemp++;
		//cout << "dans le for on dessine" << endl;
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		Scalar color = colors[i % 8];
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
		color, 3, 8, 0);*/

		//We transform r into a Mat variable for reuse it for detect eye and smile 
		smallImgROI = smallImg(*r);
		if (_detectSmileOn)
		{
			smileDetect(smallImgROI, img, r->x, r->y);
		}
		if (_detectEyeOn)
		{
			eyeDetect(smallImgROI, img, r->x, r->y);
		}

		//for see what image we send to te function you can uncomment this line
		//cv::imshow("test4", smallImgROI);
	}
	_visageNumber = visageNumberTemp;
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
	int i = 0;
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


	_smileCascade.detectMultiScale(img, faces,
		1.1, 30, 0
		//|CASCADE_FIND_BIGGEST_OBJECT
		//|CASCADE_DO_ROUGH_SEARCH
		| CASCADE_SCALE_IMAGE
		,
		Size(30, 30));

	for (vector<Rect>::iterator r = faces.begin(); r != faces.end(); r++, i++)
	{
		//cout << r->y - img.size().height / 2 << endl;
		if (r->y > img.size().height / 2)
		{
			//cout << "find a smile !" << endl;
			Mat smallImgROI;
			vector<Rect> nestedObjects;
			Point center;
			Scalar color = colors[i % 8];

			img.size().height;

			rectangle(principalFrame, cvPoint(cvRound(r->x*_scale) + width, cvRound(r->y*_scale) + height),
				cvPoint(cvRound((r->x + r->width - 1)*_scale) + width, cvRound((r->y + r->height - 1)*_scale) + height),
				color, 3, 8, 0);

			//cv::imshow("test2", smallImgROI);
		}
	}

	if (!_detectEyeOn)
	{
		cv::imshow(WINDOWSNAME, principalFrame);
	}
	return 0;
}

//For the comment see faceDetect() juste some parameter for precision change
int Video::eyeDetect(Mat& img, Mat& principalFrame, int width, int height)
{
	int i = 0;
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

	//cv::imshow("test5", img);


	_eyeCascade.detectMultiScale(img, faces,
		1.1, 12, 0
		//|CASCADE_FIND_BIGGEST_OBJECT
		//|CASCADE_DO_ROUGH_SEARCH
		| CASCADE_SCALE_IMAGE
		,
		Size(20, 20));

	for (vector<Rect>::iterator r = faces.begin(); r != faces.end(); r++, i++)
	{
		//cout << "find a eye !" << endl;
		if (r->y < img.size().height / 2)
		{
			Mat smallImgROI;
			vector<Rect> nestedObjects;
			Point center;
			Scalar color = colors[i % 8];
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

		//cv::imshow("test2", smallImgROI);
	}

	cv::imshow(WINDOWSNAME, principalFrame);
	return 0;
}


//For detect object from a CascadeClassifier object. More precision is up more the 
int Video::customDetect(Mat& img, int precision)
{
	int i = 0, visageNumberTemp = 0;
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
	//cout << img << endl;
	Mat gray, smallImg(cvRound(img.rows / _scale), cvRound(img.cols / _scale), CV_8UC1);

	cvtColor(img, gray, COLOR_BGR2GRAY);
	resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
	equalizeHist(smallImg, smallImg);

	t = (double)cvGetTickCount();
	_customCascade.detectMultiScale(smallImg, faces, 1.1, precision, 1, Size(30, 30));

	t = (double)cvGetTickCount() - t;
	printf("detection time = %g ms\n", t / ((double)cvGetTickFrequency()*1000.));
	for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++)
	{
		//cout << "dans le for on dessine" << endl;
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		Scalar color = colors[i % 8];

		rectangle(img, cvPoint(cvRound(r->x*_scale), cvRound(r->y*_scale)),
			cvPoint(cvRound((r->x + r->width - 1)*_scale), cvRound((r->y + r->height - 1)*_scale)),
			color, 3, 8, 0);
	}
	cv::imshow(WINDOWSNAME, img);
	return 0;
}


void Video::startAllDetect()
{
	startFaceDetect();
	startSmileDetect();
	startEyeDetect();
}

void Video::startFaceDetect()
{
	cout << "dans start" << endl;
	if (_faceCascade.empty())
	{
		if (!_faceCascade.load(cascadeFacePath))
		{
			cerr << "ERROR: Could not load classifier Facecascade" << endl;
			//return -1;
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
	//cout << "dans start smile" << endl;
	if (_smileCascade.empty())
	{
		if (!_smileCascade.load(cascadeSmilePath))
		{
			cerr << "ERROR: Could not load classifier smileCascade" << endl;
			//return -1;
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
	//cout << "dans start eye" << endl;
	if (_eyeCascade.empty())
	{
		if (!_eyeCascade.load(cascadeEyePath))
		{
			cerr << "ERROR: Could not load classifier smileCascade" << endl;
			//return -1;
		}
		else
		{
			cout << "classifier cascade smile is okay" << endl;
		}
		_detectEyeOn = true;
	}
	else
	{
		cout << "The smile cascade already initialized" << endl;
	}

}

void Video::stopEyeDetect()
{
	_detectEyeOn = false;
}

void Video::startCustomDetect(CascadeClassifier& cascade)
{
	_customCascade = cascade;
	_detectCustomOn = true;
}

void Video::stopCustomDetect()
{
	_detectCustomOn = false;
}


int Video::getVisageNumber()
{
	return _visageNumber;
}



Video::~Video()
{
}
