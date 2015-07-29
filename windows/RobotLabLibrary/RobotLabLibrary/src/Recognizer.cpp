#include "../include/Recognizer.h"


Recognizer::Recognizer()
{
	_frameSize.width = 92;
	_frameSize.height = 112;
}


void Recognizer::addFrameToCurrentTraining(cv::Mat frame,int label, std::string faceName)
{
	_trainingFrames.push_back(processFrame(frame));//Add frame to the vector of frame that are used for recognition
	_labelsFrames.push_back(label);//add the corresponding label
	_labelsInfo.insert(std::make_pair(label, faceName));//make correspondance between the label and the name of the person
	_model->setLabelInfo(label, faceName);// Do the same but in the model
	_newData = true; // the next recognize time we will train the new added data
}

void Recognizer::addFrameToCurrentTrainingAndSave(cv::Mat frame, int label, std::string faceName, std::string fileName, std::string folderName)
{
	_trainingFrames.push_back(processFrame(frame));//Add a normalized frame (size and color) to the vector of frame that are used for recognition
	_labelsFrames.push_back(label);//add the corresponding label
	_labelsInfo.insert(std::make_pair(label, faceName));//make correspondance between the label and the name of the person
	_model->setLabelInfo(label, faceName);// Do the same but in the model
	_pathToFrame.insert(std::make_pair(_trainingFrames.size() - 1, IMG_DIR + folderName + fileName));//associate the frame vector index to a path for the sauvegarde process
	_newData = true; // the next recognize time we will train the new added data
	saveImg(folderName, fileName, frame); //save the frame to a specified folder in IMG_DIR with a specified name
}



std::string Recognizer::recognize(cv::Mat faceToRecognize)
{
	cv::Mat faceToRecognizeGood = processFrame(faceToRecognize); //Ajust size and color of the frame for matching with the database
	if (_trainingFrames.size() <= 1) //Check if we have add image to the trainingFrame vector
	{
		std::string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		CV_Error(cv::Error::StsError, error_message);
		getchar();
	}

	if (_newData) //when we add a frame in a vector _newData is set to true.
	{
		//std::cout << "train new data" << std::endl;
		train();//Function that call model->train() in faceRecognizer contibution
		_newData = false;
	}

	int predictedLabel = -1;
	_confidence = 0.0;
	_model->predict(faceToRecognizeGood, predictedLabel, _confidence); // This function in faceRecognizer contibution will give us the label associated to the nearest face recognized and the approximation. More confidence is high less the recognition is sure.

	std::cout << "predictedLabel : " << predictedLabel << " confidence = " << _confidence << std::endl;

	std::cout << "The face detected belongs to : " << _labelsInfo[predictedLabel] << std::endl;

	return _labelsInfo[predictedLabel];
}

//load face and label for trainning a database
void Recognizer::readCsv(const std::string& filename, char separator)
{
	std::ifstream file(filename.c_str(), std::ifstream::in); //open a file in lecture mode
	if (!file) 
	{
		std::string error_message = "No valid input file was given, please check the given filename. ";
		CV_Error(CV_StsBadArg, error_message);
	}
	std::string line, path, classLabel, infoLabel;
	cv::Mat tmp;
	while (getline(file, line)) //get line by line what contains the file
	{
		std::stringstream liness(line);
		std::getline(liness, path, separator); //get the value until the separator and store in path variable
		std::getline(liness, classLabel, separator); //get the value until the separator and store in classLabel variable
		std::getline(liness, infoLabel); //get the value until the separator and store in path variable
		if (!path.empty() && !classLabel.empty()) 
		{
			tmp = cv::imread(path, 0); //We load the img associate to a line into tmp and check that it's not null (path wrong)
			if (tmp.data != NULL)
			{
				_trainingFrames.push_back(processFrame(tmp)); //add to the current vector for training frame and make sur that the image is good (size and color)
			}
			else
			{
				std::cerr << "error in the reading file process" << std::endl;
			}
			_pathToFrame.insert(std::make_pair(_trainingFrames.size()-1, path));//associate the frame vector index to a path for the sauvegarde process
			_labelsFrames.push_back(atoi(classLabel.c_str()));//add the corresponding label
			_labelsInfo.insert(std::make_pair(atoi(classLabel.c_str()), infoLabel));//make correspondance between the label and the name of the person
			_model->setLabelInfo(atoi(classLabel.c_str()), infoLabel);
		}
	}
}

void Recognizer::train()
{
	_isTrained = true; // trainning make few second to execute so it is executes ind a threads. We need to know when it is trainned for not use the predict function
	//std::cout << "before train" << std::endl; //keep these many error occurs in train functions if the images doesn't match. If you have a opencv assertion failed with something around the size the colRange or the row of an image uncomment this line for confirm that the error is created in train function
	_model->train(_trainingFrames, _labelsFrames); //load the frame and label into a model created in faceRecognizer contrib
	std::cout << "Recognizer model sucessfully trained !" << std::endl;
	_isTrained = false;
}

void Recognizer::saveCsv(std::string fileName)
{
	std::ofstream fichier(IMG_DIR + fileName, std::ios::out | std::ios::trunc);  //open file in write mode. If the file exist we remove all the contains

	if (!fichier)  // si l'ouverture a réussi
	{
		std::cerr << "Error in the openning process" << std::endl;
	}

	int element = 0; // just an indexer

	for (std::vector<cv::Mat>::iterator frame = _trainingFrames.begin(); frame != _trainingFrames.end(); frame++, element++) // for every frame using in the training model
	{
		if (!_pathToFrame[element].empty())
		{
			fichier << _pathToFrame[element] << ";" << _labelsFrames[element] << ";" << _labelsInfo[_labelsFrames[element]] << std::endl; // parse the data like we want : path;label;labelinfo(name of faces)
		}
		else
		{
			std::cout << "image of " << _labelsInfo[element] << " not sauved when trained" << std::endl; //if we have used faces imaged just for these time and we don't choose to save them when we added them to the training vector
		}
	}	
	fichier.close();
}

//save the frame used for recognition into a specified folder into IMG_DIR (Constantes.h) with a specified name 
void Recognizer::saveImg(std::string folderName, std::string nameOfFile, cv::Mat faceToSave)
{
	cv::Mat tmp = processFrame(faceToSave); //make sur the image format is good
	bool save = cv::imwrite(IMG_DIR + folderName + nameOfFile, tmp); //save the image
	if (!save)
	{
		createDirectory(folderName);//if save doesn't work it's because the path is wrong so we try to create the specified folder
		save = cv::imwrite(IMG_DIR + folderName + nameOfFile, faceToSave);//save again
	}
	if (save)
	{
		std::cout << "file " << nameOfFile << " saved" << std::endl;
		saveCsv("customFaceCsv.txt");
	}
	else
	{
		std::cout << "error in saved process" << std::endl;
	}
}


cv::Mat Recognizer::processFrame(cv::Mat frameToProcess)
{
	cv::Mat rezizedFrame, grayRezizedFrame;

	cv::resize(frameToProcess, rezizedFrame, _frameSize, 1.0, 1.0, cv::INTER_CUBIC); //rezize

	if (rezizedFrame.channels() == 3)// 3 channel = BGR != gray
	{
		cv::cvtColor(rezizedFrame, grayRezizedFrame, cv::COLOR_BGR2GRAY); // change color to gray

		return grayRezizedFrame;
	}
	

	return rezizedFrame;
}


// in main.cpp if the confidence is too high we call this function because may the face detected is not good so we had a new people
void Recognizer::askForAddImageToCurrentTrainingAndSave(cv::Mat noRecognizedFace)
{
	_askForAddImageInProcess = true;
	std::string answer, fileName;
	int i;
	std::cout << "The confidence is high this is maybe because you are not in our database. Would you like to join our Database (the image to save is in the new windows) ? y/n" << std::endl;
	std::cin >> answer;
	if (answer == "y" || answer == "Y")
	{
		std::cout << "We are going to add your face to the database what is your name ? " << std::endl;
		std::cin >> answer;



		if (!imageExist(answer + ".jpeg"))//if someone have already the same name or we want more image of the same person we will have already a file named like "firstname.jpeg" so we check for no overrided 
		{
			fileName = answer + ".jpeg";
			addFrameToCurrentTrainingAndSave(noRecognizedFace, getLabelFrameSize() + 1, answer, fileName, "face/"); //add face to current training and save it
		}
		else //if there is already a face for this name we had a number for not overrided
		{
			i = 2;
			while (imageExist(answer + std::to_string(i) + ".jpeg"))
			{
				i++;
			}
			fileName = answer + std::to_string(i) + ".jpeg";
			std::vector<int> label;
			cv::String test(answer);
			label = _model->getLabelsByString("adrien");
			//printConf();
			/*for (int j = 0; j < label.size(); j++)
			{
				std::cout << "label[" <<j<<"] = " << label[j] << std::endl;
			}*/
			if (!label.empty())
			{
				//std::cout << "label = " << label[0] << std::endl;
				addFrameToCurrentTrainingAndSave(noRecognizedFace, label[0], answer, fileName, "face/");
			}
			else
			{
				addFrameToCurrentTrainingAndSave(noRecognizedFace, getLabelFrameSize() + 1, answer, fileName, "face/"); //add face to current training and save it
			}
		}
	}
	_askForAddImageInProcess = false;
}



bool Recognizer::isTrained()
{
	return _isTrained; //we can't use model->predict() when model is trained
}

bool Recognizer::askForAddImageInProcess()
{
	return _askForAddImageInProcess;
}

cv::Ptr<cv::face::FaceRecognizer> Recognizer::getModel()
{
	return _model;
}

double Recognizer::getLastConfidence()
{
	return _confidence; //get the precision of face recognition 
}

int Recognizer::getFrameWidth()
{
	return _frameSize.width; //If we need to know wich with size is used for the image
}
int Recognizer::getFrameHeight()
{
	return _frameSize.height; //If we need to know wich height size is used for the image
}

cv::Size Recognizer::getFrameSize()
{
	return _frameSize; //for rezize function is faster than call the both function above
}

int Recognizer::getLabelFrameSize()
{
	return _labelsFrames.size(); //Usefull for information on the size of current training 
}
int Recognizer::getTrainingFramesSize()
{
	return _trainingFrames.size(); //Usefull for information on the size of current training 
}

//TODO get the number of different person face sauvegarded with a code like contains or with if name != then thera are a new face
int Recognizer::getNumberOfFaceSauvegarded()
{
	return -1;
}

//return what we save : path of a frame, her label and her name, large information if big training data more usefull for debugging when learning 
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

//check if an image exist with her name and the folder inside IMG_DIR given
bool Recognizer::imageExist(const std::string name, const std::string folderName) 
{
	cv::Mat image = cv::imread(IMG_DIR + folderName + name, 0);

	if (image.data)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//function for create a directory depending on the operating system
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


bool Recognizer::equalTest(std::vector<cv::Mat> vec1, std::vector<cv::Mat> vec2)
{
	if (vec1.size() != vec2.size())
	{
		return false;
	}
	for (int i = 0; i < vec1.size(); i++)
	{
		if (vec1[i].data != vec2[i].data)
		{
			return false;
		}
	}
	return true;
}
