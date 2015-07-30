#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#ifdef _WIN32

#include <windows.h>

#endif

#ifdef __linux__ 

#include <sys/stat.h>
#include <sys/types.h>

#endif

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "../faceRecognizerContrib/include/face.hpp"


#include "Constantes.h"

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <thread>

class Recognizer
{
public:
	Recognizer(int widht = 92, int heihgt = 112);

	~Recognizer();

	void addFrameToCurrentTraining(cv::Mat frame, int label, std::string faceName);
	void addFrameToCurrentTrainingAndSave(cv::Mat frame, int label, std::string faceName, std::string fileName, std::string folderName = "face/");
	void askForAddImageToCurrentTrainingAndSave(cv::Mat noRecognizedFace, std::string folderName = "face/");

	std::string Recognizer::recognize(cv::Mat faceToRecognize);
	void train();

	void readCsv(const std::string& filename, char separator = ';');
	void saveCsv(std::string pathToFile = CSV_FACE_RECO);
	void saveImg(std::string folderName, std::string nameOfFile, cv::Mat faceToSave);
	cv::Mat processFrame(cv::Mat frameToProcess);

	bool isTrained();
	bool askForAddImageInProcess();
	int getFrameWidth();
	int getFrameHeight();
	cv::Size getFrameSize();
	double getLastConfidence();
	int getLabelFrameSize();
	int getTrainingFramesSize();
	int getNumberOfFaceSauvegarded();
	void printConf();
	cv::Ptr<cv::face::FaceRecognizer> getModel();

	bool equalTest(std::vector<cv::Mat> vec1, std::vector<cv::Mat> vec2);

	bool imageExist(const std::string name, const std::string folderName = "face/");
	void createDirectory(std::string folderName);
private :
	std::vector<cv::Mat> _trainingFrames, _trainingFramesInTrain;
	std::vector<int> _labelsFrames, _labelsFramesInTrain;
	std::map<int, std::string> _pathToFrame;
	std::map<int, cv::String> _labelsInfo; // use labelsInfo.insert(std::make_pair(label, info));

	cv::Ptr<cv::face::FaceRecognizer> _model = cv::face::createEigenFaceRecognizer();//look like fisher face recognition algorithm doesn't work when two image have the same label

	cv::Size _frameSize;

	bool _newData = true, _isTrained = false, _askForAddImageInProcess = false;
	double _confidence = 0.0;
};

#endif