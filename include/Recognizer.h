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
#include "../3rdparty/faceRecognizerContrib/include/face.hpp"


#include "Constantes.h"

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <thread>

class Recognizer
{
public:
	/* @brief Initialize a new instance of Recognizer class. Set the heigh and the width that the image in the current trainning will use. 92 and 112 are the value used by the ATDatabase.
	*
	* @param width: Width of the image used in the training espace.
	* @param height: Height of the image used in the current training.
	*/
	Recognizer(int widht = 92, int heihgt = 112);

	~Recognizer();

	/* @brief This function is the first step for recognition and for building a important database of face. It will add the frame passed in parameter to the current training space for adding it for the next model training and the next recognition and it will save the frame like an image in the specified folder with the specified name in the parameters.
	*
	* @param frame: Frame that contain a face that you want to add to the current training.
	* @param label: Label associated to the name and the face.
	* @param faceName: Name of the person who owns the face.
	*/
	void addFrameToCurrentTraining(cv::Mat frame, int label, std::string faceName);

	/* @brief This function is the first step for recognition and for building a important database of face. It will add the frame passed in parameter to the current training space for adding it for the next model training and the next recognition and it will save the frame like an image in the specified folder with the specified name in the parameters.
	*
	* @param frame: Frame that contain a face that you want to add to the current training.
	* @param label: Label associated to the name and the face.
	* @param faceName: Name of the person who owns the face.
	* @param fileName: Name given to the registered image.
	* @param folderName: Name of the folder in wich you want to save your face database.
	*/
	void addFrameToCurrentTrainingAndSave(cv::Mat frame, int label, std::string faceName, std::string fileName, std::string folderName = "face/");

	/* @brief Usefull function that ask the user if he want to add the frame in parameter to the current training and the database and if the answer is yes it will call addFrameToCurrentTrainingAndSave().
	*
	* @param noRecognizedFace: Image of a face that receive a too hight accuracy in the recognition process and that you want to ask the user if he want to add it in the database and the current training.
	* @param folderName: Name of the folder in wich you want to save your face if the user is okay.
	*/
	void askForAddImageToCurrentTrainingAndSave(cv::Mat noRecognizedFace, std::string folderName = "face/");


	/* @brief Main function of the class. Recognize() will print the name and the label of the person who looks the most like the picture passed in parameter. The function return the confidence of the recognize process. If he is high you can call askForAddImageToCurrentTrainingAndSave() for add a new people to the database.
	*
	* @param faceToRecognize: Frame that contains a detected face we want to try to recognize.
	*/
	std::string recognize(cv::Mat faceToRecognize);

	/* @brief Train the current model. That is used for transform image in data associate to a label for the next comparaison with an image.
	*
	*/
	void train();


	/* @brief Load the data into a CSV into the vector of the current training space. Image are stocked like path. When we read the CSV file we will read line by line the data. Anf for each line take all the character until ";". For the first string it's a path so we load the image at this path and add it to the current vector for training. We do the same for the second (label) and the third (name) but directly.
	*
	* @param fileName: The name of the file we want to read.
	* @param separator: The special character used like separator of data in the csv.
	*/
	void readCsv(const std::string& filename, char separator = ';');

	/* @brief Take the value of the path of the frame, the label and the name associated to a frame stored in the current training vector and save them in a single line separated with ";".
	*
	* @param pathToFile: The name of the file in wich we want to save the data.
	*/
	void saveCsv(std::string pathToFile = CSV_FACE_RECO);

	/* @brief Save a frame passed in parameter to the specified folder in the IMG_DIR defined in Constantes.h with the name passed in parameter. If the folder doesn't exist it will be created.
	*
	* @param folderName: Name of the folder in wich we want to save the image.
	* @param nameOfImage: Name given to the registered image.
	* @param faceToSave: Image to save.
	* @param imgDir: Path to the image directory
	* @param csvPath: Path to the csv file
	*/
	void saveImg(std::string folderName, std::string nameOfImage, cv::Mat faceToSave, const char* imgDir = IMG_DIR, const char* csvPath = CSV_FACE_RECO);

	/* @brief Rezize and change the color of a frame if needed for use regularized variables for the comparaison of two images.
	*
	* @param frameToProcess: The original image that we want to regularize.
	*/
	cv::Mat processFrame(cv::Mat frameToProcess);


	/* @brief Return true if the faceRecognizer model is training. While faceRecognizer model train we can't use others function of the model like predict().
	*
	*/
	bool isTrained();

	/* @brief Return true if we are asking to the user if he want to add an image to the database. Usefull only if you want to execute askForAddImageToCurrentTrainingAndSave() in a separated thread.
	*
	*/
	bool askForAddImageInProcess();

	/* @brief Return the width of the image used in our training database. Usefull if you want to check the compatibility between two image.
	*
	*/
	int getFrameWidth();

	/* @brief Return the height of the image used in our training database. Usefull if you want to check the compatibility between two image.
	*
	*/
	int getFrameHeight();

	/* @brief Return the Size class of the image used in our training database. Usefull for cv::rezize() function. Remplace getFrameWidth() + getFrameHeight() in one function.
	*
	*/
	cv::Size getFrameSize();

	/* @brief Return the last confidence of _model->predict() function. Used to know if the name returned by recognize() function is the good one or if maybe there are an error.
	*
	*/
	double getLastConfidence();

	/* @brief Return the size of the _labelFrame vector. Usefull to know if the adding frame to training space process worked fine.
	*
	*/
	int getLabelFrameSize();

	/* @brief Return the size of the _trainingFrame vector. Usefull to know if the adding frame to training space process worked fine.
	*
	*/
	int getTrainingFramesSize();

	/* @brief Need to be done. Get the number of different label in _labelFrame
	*
	*/
	int getNumberOfFaceSauvegarded();

	/* @brief Print in the console all the data in the different vector used in the training space. Usefull to debug small database.
	*
	*/
	void printConf();

	/* @brief Return the model used in faceRecognizer class. Usefull if you need to call directly some function like train or predict.
	*
	*/
	cv::Ptr<cv::face::FaceRecognizer> getModel();


	/* @brief some test no used
	*
	*/
	bool equalTest(std::vector<cv::Mat> vec1, std::vector<cv::Mat> vec2);


	/* @brief Return true if an image exist to the specifed path into IMG_DIR (Constantes.h) false otherwise.
	*
	* @param name: Name of the image we want to check the existence.
	* @param folderName: The name of the folder that contains the image.
	* @param imgDir: Path to the image directory
	*/
	bool imageExist(const std::string name, const std::string folderName = "face/", const char* imgDir = IMG_DIR);

	/* @brief Create a directory in IMG_DIR (Constantes.h) for create a new database. Function used in windows and linux.
	*
	* @param folderName: The name of the folder we want to create.
	* @param imgDir: Path to the image directory
	*/
	void createDirectory(std::string folderName, const char* imgDir = IMG_DIR);

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