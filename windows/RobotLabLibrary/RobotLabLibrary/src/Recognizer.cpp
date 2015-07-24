#include "../include/Recognizer.h"


Recognizer::Recognizer()
{
	_frameSize.width = 92;
	_frameSize.height = 112;
}


void Recognizer::addFrameToCurrentTraining(cv::Mat frame,int label, std::string faceName)
{
	_trainingFrames.push_back(frame);
	_labelsFrames.push_back(label);
	_labelsInfo.insert(std::make_pair(label, faceName));
	_model->setLabelInfo(label, faceName);
	_newData = true;
}

void Recognizer::addFrameToCurrentTrainingAndSave(cv::Mat frame, int label, std::string faceName, std::string fileName, std::string pathToFolder)
{
	_trainingFrames.push_back(processFrame(frame));
	_labelsFrames.push_back(label);
	_labelsInfo.insert(std::make_pair(label, faceName));
	_pathToFrame.insert(std::make_pair(_trainingFrames.size()-1, IMG_DIR+pathToFolder+fileName));
	_model->setLabelInfo(label, faceName);
	_newData = true;
	saveImg(pathToFolder, fileName, frame);
}



void Recognizer::recognize(cv::Mat faceToRecognize)
{
	cv::Mat faceToRecognizeGood = processFrame(faceToRecognize);
	if (_trainingFrames.size() <= 1) {
		std::string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		CV_Error(cv::Error::StsError, error_message);
		getchar();
	}

	if (_newData)
	{
		//std::cout << "train new data" << std::endl;
		train();
	}
	//saveModel("face-rec-model.txt");

	// The following line predicts the label of a given
	// test image:
	//int predictedLabel = model->predict(faceToRecognize, -1, 0.0);
	//
	// To get the confidence of a prediction call the model with:
	//
	int predictedLabel = -1;
	_confidence = 0.0;
	_model->predict(faceToRecognizeGood, predictedLabel, _confidence);

	std::cout << "predictedLabel : " << predictedLabel << " confidence = " << _confidence << std::endl;

	if (!_model->getLabelInfo(predictedLabel).empty())
	{
		std::cout << cv::format("%d-th label's info: %s", predictedLabel, _model->getLabelInfo(predictedLabel).c_str()) << std::endl;
	}

	/*if (5>2) {
		// Sometimes you'll need to get/set internal model data,
		// which isn't exposed by the public cv::FaceRecognizer.
		// Since each cv::FaceRecognizer is derived from a
		// cv::Algorithm, you can query the data.
		//
		// First we'll use it to set the threshold of the FaceRecognizer
		// to 0.0 without retraining the model. This can be useful if
		// you are evaluating the model:
		//
		model->setThreshold(0.0);
		// Now the threshold of this model is set to 0.0. A prediction
		// now returns -1, as it's impossible to have a distance below
		// it
		predictedLabel = model->predict(testSample);
		std::cout << "Predicted class = " << predictedLabel << std::endl;
		// Here is how to get the eigenvalues of this Eigenfaces model:
		cv::Mat eigenvalues = model->getEigenValues();
		// And we can do the same to display the Eigenvectors (read Eigenfaces):
		cv::Mat W = model->getEigenVectors();
		// From this we will display the (at most) first 10 Eigenfaces:
		for (int i = 0; i < cv::min(10, W.cols); i++) {
			std::string msg = cv::format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
			std::cout << msg << std::endl;
			// get eigenvector #i
			cv::Mat ev = W.col(i).clone();
			// Reshape to original size & normalize to [0...255] for imshow.
			cv::Mat grayscale;
			normalize(ev.reshape(1), grayscale, 0, 255, cv::NORM_MINMAX, CV_8UC1);
			// Show the image & apply a Jet colormap for better sensing.
			cv::Mat cgrayscale;
			applyColorMap(grayscale, cgrayscale, cv::COLORMAP_JET);
			imshow(cv::format("%d", i), cgrayscale);
		}
		cv::waitKey(1);
	}*/
}

void Recognizer::readCsv(const std::string& filename, char separator)
{
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (!file) 
	{
		std::string error_message = "No valid input file was given, please check the given filename. ";
		CV_Error(CV_StsBadArg, error_message);
	}
	std::string line, path, classlabel, infoLabel;
	cv::Mat tmp;
	while (getline(file, line)) 
	{
		std::stringstream liness(line);
		std::getline(liness, path, separator);
		std::getline(liness, classlabel, separator);
		std::getline(liness, infoLabel);
		if (!path.empty() && !classlabel.empty()) 
		{
			tmp = cv::imread(path, 0);
			if (tmp.data != NULL)
			{
				_trainingFrames.push_back(processFrame(tmp));
			}
			else
			{
				std::cerr << "error in the reading file process" << std::endl;
			}
			_pathToFrame.insert(std::make_pair(_trainingFrames.size()-1, path));
			_labelsFrames.push_back(atoi(classlabel.c_str()));
			_labelsInfo.insert(std::make_pair(atoi(classlabel.c_str()), infoLabel));
		}
	}
	_frameWidth = _trainingFrames[0].cols;
	_frameHeight = _trainingFrames[0].rows;
}

void Recognizer::train()
{
	_isTrained = true;
	_model->train(_trainingFrames, _labelsFrames);
	std::cout << "Recognizer model sucessfully charged !" << std::endl;
	_newData = false;
	_isTrained = false;
}

void Recognizer::saveCsv(std::string fileName)
{
	std::ofstream fichier(fileName, std::ios::out | std::ios::trunc);  //déclaration du flux et ouverture du fichier

	if (!fichier)  // si l'ouverture a réussi
	{
		std::cerr << "Error in the openning process" << std::endl;
	}

	int element = 0;

	for (std::vector<cv::Mat>::iterator frame = _trainingFrames.begin(); frame != _trainingFrames.end(); frame++, element++)
	{
		if (!_pathToFrame[element].empty())
		{
			fichier << _pathToFrame[element] << ";" << _labelsFrames[element] << ";" << _labelsInfo[_labelsFrames[element]] << std::endl;
		}
		else
		{
			std::cout << "image of " << _labelsInfo[element] << " not sauved when trained" << std::endl;
		}
	}

		
	fichier.close();
}

void Recognizer::saveImg(std::string pathToDir, std::string nameOfFile, cv::Mat faceToSave)
{
	cv::Mat tmp = processFrame(faceToSave);
	bool save = cv::imwrite(IMG_DIR + pathToDir + nameOfFile, faceToSave);
	if (!save)
	{
		createDirectory(pathToDir);
		save = cv::imwrite(IMG_DIR + pathToDir + nameOfFile, faceToSave);
	}
	if (save)
	{
		std::cout << "file " << nameOfFile << " saved" << std::endl;
	}
	else
	{
		std::cout << "error in saved process" << std::endl;
	}
}


// TODO
cv::Mat Recognizer::processFrame(cv::Mat frameToProcess)
{
	cv::Mat rezizedFrame, grayRezizedFrame;

	cv::resize(frameToProcess, rezizedFrame, _frameSize, 1.0, 1.0, cv::INTER_CUBIC);

	if (rezizedFrame.channels() == 3)
	{
		cv::cvtColor(rezizedFrame, grayRezizedFrame, cv::COLOR_BGR2GRAY);
		return grayRezizedFrame;
	}

	return rezizedFrame;
}



bool Recognizer::isTrained()
{
	return _isTrained;
}

cv::Ptr<cv::face::FaceRecognizer> Recognizer::getModel()
{
	return _model;
}

double Recognizer::getLastConfidence()
{
	return _confidence;
}

int Recognizer::getFrameWidth()
{
	return _frameSize.width;
}
int Recognizer::getFrameHeight()
{
	return _frameSize.height;
}

cv::Size Recognizer::getFrameSize()
{
	return _frameSize;
}

int Recognizer::getLabelFrameSize()
{
	return _labelsFrames.size();
}
int Recognizer::getTrainingFramesSize()
{
	return _trainingFrames.size();
}

int Recognizer::getNumberOfFacePerLabel()
{
	return -1;
}
int Recognizer::getNumberOfFaceSauvegarded()
{
	return -1;
}

void Recognizer::printConf()
{
	int element = 0;
	for (std::vector<cv::Mat>::iterator frame = _trainingFrames.begin(); frame != _trainingFrames.end(); frame++, element++)
	{
		std::cout << "path : " << _pathToFrame[element] << std::endl;
		std::cout << "label : " << _labelsFrames[element] << std::endl;
		std::cout << "label info : " << _labelsInfo[_labelsFrames[element]] << std::endl;
	}	
}



#ifdef _WIN32
void Recognizer::createDirectory(std::string folderName)
{
	std::string tmp = IMG_DIR + folderName;
	int len;
	int slength = (int)tmp.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, tmp.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, tmp.c_str(), slength, buf, len);
	std::wstring stemp(buf);
	LPCWSTR result = stemp.c_str();
	CreateDirectory(result, NULL);
	delete[] buf;
}
#endif

#ifdef __linux__ 
void Recognizer::createDirectory(std::string pathName)
{
	mkdir(IMG_DIR + pathName, 01777);
}
#endif

Recognizer::~Recognizer()
{
}
