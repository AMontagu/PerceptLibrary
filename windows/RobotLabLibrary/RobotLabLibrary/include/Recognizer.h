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

	void addFrameToCurrentTraining(cv::Mat frame, int label, std::string faceName);
	cv::Ptr<cv::face::FaceRecognizer> getModel();

	
	void recognize(cv::Mat faceToRecognize);
	void read_csv(const std::string& filename, char separator = ';');

	void saveModel(std::string fileName = "face-rec-model.txt");
	void train();
	void saveModel();
	void loadModel();
	void saveImg(std::string path, cv::Mat faceToSave);

	bool isTrained();
	int getFrameWidth();
	int getFrameHeight();

private :
	std::vector<cv::Mat> _trainingFrames, _trainingFramesInTrain;
	std::vector<int> _labelsFrames, _labelsFramesInTrain;
	std::map<int, std::string> labelsInfo; // use labelsInfo.insert(std::make_pair(label, info));

	cv::Ptr<cv::face::FaceRecognizer> _model = cv::face::createEigenFaceRecognizer();

	bool _newData = true, _isTrained = false;
	int _frameWidth, _frameHeight;
};

#endif