#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "../faceRecognizerContrib/include/face.hpp"


#include "Constantes.h"

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>

class Recognizer
{
public:
	Recognizer();

	~Recognizer();

	void recognize();
	void read_csv(const std::string& filename, std::vector<cv::Mat>& images, std::vector<int>& labels, std::map<int, std::string>& labelsInfo, char separator = ';');

	/*cv::Mat processImage(cv::Mat& image);
	bool processAndSaveImage(cv::Mat& image, const std::string& name);

	virtual bool addTrainingImage(const std::string &filename, const std::string &name) = 0;

	virtual bool train() = 0;
	virtual std::string recognize(cv::Mat &face) = 0;
	virtual std::size_t numTrainingImages() = 0;
	bool trained() { return _isTrained; }

	
private :
	std::string _directoryPath;
	cv::Size _goalSize;
	bool _keepAspectRatio;
	bool _isTrained;*/
};

#endif