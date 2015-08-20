#include "Video.h"
#include "Recognizer.h"
#include "Constantes.h"

#include <iostream>

/*###############################################################################################################################
#																																#
#												Sample for face recognition         											#
#																																#
#################################################################################################################################*/


int main(int argc, const char** argv)
{
	//----------------------------------- variables initialisation ---------------------------------------------------------
	std::thread t[2];
	// Video is the class for use video and detection
	Video myVideo(0); //0 is the defaut camera, use -1 for any cammera or a number between 1 to 99
	Recognizer myRecognizer;

	std::vector<cv::Mat> lastFacesDetected;
	bool done = false;
	int i = 0;
	std::string namePersonRecognized, goodName;
	time_t timer;
	time(&timer); //set timer to the current date in ms in

	cv::Mat imgNull;

	//Launch video in a thread
	t[0] = std::thread([&] {myVideo.start(); });
	myVideo.startFaceDetect();

	// ---------------------------------- first option load a model manually with files -----------------------------------

	//cv::Mat person1 = cv::imread("../data/dataForRec/face/person1.jpeg", 0);
	//cv::Mat person2 = cv::imread("../data/dataForRec/face/person2.jpeg", 0);
	//cv::Mat person3 = cv::imread("../data/dataForRec/face/person3.jpeg", 0);
	//cv::Mat person4 = cv::imread("../data/dataForRec/face/person4.jpeg", 0);
	//cv::Mat person5 = cv::imread("../data/dataForRec/face/person5.jpeg", 0);

	//myRecognizer.addFrameToCurrentTraining(person1,1,"person1");
	//myRecognizer.addFrameToCurrentTraining(person2, 2, "person2");
	//myRecognizer.addFrameToCurrentTraining(person3, 3, "person3");
	//myRecognizer.addFrameToCurrentTraining(person4, 4, "person4");
	//myRecognizer.addFrameToCurrentTraining(person5, 5, "person5");

	// ------------------------------- second option load a model automatically with csv file --------------------------------

	try {
		myRecognizer.readCsv(CSV_FACE_RECO); // see Constantes.h
	}
	catch (cv::Exception& e) {
		std::cerr << "Error opening file \"" << CSV_FACE_RECO << "\". Reason: " << e.msg << std::endl;
		getchar();
		exit(1);
	}


	//-------------------------------------- Train the loaded data for face recognition -------------------------------------

	t[1] = std::thread([&] {myRecognizer.train(); });

	//If you want informations on the model used, uncomment these lines
	//std::cout << "width = " << myRecognizer.getFrameWidth() << " height = " << myRecognizer.getFrameHeight() << std::endl;
	//std::cout << "trainingFrame size = " << myRecognizer.getTrainingFramesSize() << " labelFrameSize = " << myRecognizer.getLabelFrameSize() << std::endl;
	//myRecognizer.printConf();


	//------------------------------------------------Recognize face in video ------------------------------------------

	while (true)
	{
		//dont recognize if the model is not trained or if no face never get Recognize
		if (abs(time(NULL) - timer) > 1 && !myRecognizer.isTrained() && !myVideo.getLastFacesDetected().empty() && !myRecognizer.equalTest(myVideo.getLastFacesDetected(), lastFacesDetected))//Maybe add myVideo.getLastFacesDetected() != oldLastFacesDetected for not recognize the same image again and again
		{
			lastFacesDetected = myVideo.getLastFacesDetected(); // get all the current face in video stream
			i = 0;
			myVideo.clearLabel(); // remove the old name associed to a face
			for (std::vector<cv::Mat>::iterator face = lastFacesDetected.begin(); face != lastFacesDetected.end(); face++, i++)//for each face detected
			{
				namePersonRecognized = myRecognizer.recognize(*face);//launch the recognition
				myVideo.addLabel(namePersonRecognized, i); //display the name on the video stream. If you only want to display him if the reognition is sur move this line under, in the else statement.
				myVideo.setImgToPrint(*face); // display in an another windows in the video thread the region of interest corresponding to the face detected

				if (myRecognizer.getLastConfidence() > 3000 && !myRecognizer.askForAddImageInProcess())//If we are not sur of the recognition and we are not already asking for add image if we ask in an other thread
				{
					myRecognizer.askForAddImageToCurrentTrainingAndSave(*face, "customDatabase/");//Function ask the user if he want to add his face to the database and do it if the answer is positif
					//t[2] = std::thread([&] {myRecognizer.askForAddImageToCurrentTrainingAndSave(*face); }); // if you want to use multi threading but you need to use this differently to use when you implement your GUI (maybe a other windows specially created fot that or the question under the face displayed for informations)
				}
				else
				{
					if (!myRecognizer.askForAddImageInProcess()) //if we are not already asking for add image if we ask in an other thread
					{
						std::cout << "Good Name ? y/n" << std::endl; //Ask the user if the name given to the face detected is correct
						std::cin >> goodName;
						if (goodName == "n" || goodName == "N")//If the user said that the name isn't correct ask him if he want to add the correct name to the face in the database
						{
							myRecognizer.askForAddImageToCurrentTrainingAndSave(*face,"customDatabase/");
						}
					}
				}
				myVideo.setImgToPrint(imgNull);
			}
			//std::cout << "face analyzed = " << i << std::endl;
			time(&timer);
		}
		cv::waitKey(1);
	}
	return 0;
}