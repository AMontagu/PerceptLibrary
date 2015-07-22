#include "../include/Recognizer.h"


Recognizer::Recognizer()
{
}


void Recognizer::addFrameToCurrentTraining(cv::Mat frame,int label, std::string faceName)
{
	_trainingFrames.push_back(frame);
	_labelsFrames.push_back(label);
	labelsInfo.insert(std::make_pair(label, faceName));
	_newData = true;
}



void Recognizer::recognize(cv::Mat faceToRecognize)
{
	if (_trainingFrames.size() <= 1) {
		std::string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		CV_Error(cv::Error::StsError, error_message);
		getchar();
	}

	int nlabels = (int)_labelsFrames.size();
	int testLabel = _labelsFrames[nlabels - 1];
	//images.pop_back();
	//labels.pop_back();

	for (int i = 0; i < nlabels; i++)
	{
		_model->setLabelInfo(i, labelsInfo[i]);
	}
	std::cout << "before train" << std::endl;

	if (_newData)
	{
		std::cout << "train new data" << std::endl;
		train();
	}
	std::cout << "after train" << std::endl;
	//saveModel("face-rec-model.txt");

	// The following line predicts the label of a given
	// test image:
	//int predictedLabel = model->predict(testSample, -1, 0.0);
	//
	// To get the confidence of a prediction call the model with:
	//
	int predictedLabel = -1;
	double confidence = 0.0;
	_model->predict(faceToRecognize, predictedLabel, confidence);

	std::cout << "predictedLabel : " << predictedLabel << " confidence = " << confidence << std::endl;

	std::string result_message = cv::format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
	std::cout << result_message << std::endl;
	if ((predictedLabel == testLabel) && !_model->getLabelInfo(predictedLabel).empty())
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

void Recognizer::read_csv(const std::string& filename, char separator)
{
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (!file) {
		std::string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	std::string line, path, classlabel;
	while (getline(file, line)) {
		std::stringstream liness(line);
		std::getline(liness, path, separator);
		std::getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()) {
			_trainingFrames.push_back(cv::imread(path, 0));
			_labelsFrames.push_back(atoi(classlabel.c_str()));
		}
	}
	_frameWidth = _trainingFrames[0].cols;
	_frameHeight = _trainingFrames[0].rows;
}

void Recognizer::saveModel(std::string fileNamePath)
{
	_model->save(fileNamePath);
}

void Recognizer::train()
{
	_isTrained = true;
	_model->train(_trainingFrames, _labelsFrames);
	std::cout << " Recognizer model sucessfully charged !" << std::endl;
	_newData = false;
	_isTrained = false;
}

void Recognizer::saveModel()
{
	_model->save("testSave");
}

void Recognizer::loadModel()
{
	_model->load("testSave");
}

void Recognizer::saveImg(std::string path, cv::Mat faceToSave)
{
	cv::imwrite(IMG_DIR + path, faceToSave);
}




bool Recognizer::isTrained()
{
	return _isTrained;
}

cv::Ptr<cv::face::FaceRecognizer> Recognizer::getModel()
{
	return _model;
}

int Recognizer::getFrameWidth()
{
	return _frameWidth;
}
int Recognizer::getFrameHeight()
{
	return _frameHeight;
}

Recognizer::~Recognizer()
{
}
