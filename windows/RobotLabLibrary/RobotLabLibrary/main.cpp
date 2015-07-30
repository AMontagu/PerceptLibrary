#include "include/Video.h"
#include "include/Voice.h"
#include "include/Recognizer.h"

/*###############################################################################################################################
#																																#
#												Sample for video detection  													#
#																																#
#################################################################################################################################*/


/*int main(int argc, const char** argv)
{
	std::thread t[1];
	// Video is the class for use video and detection
	Video myVideo(0); //0 is the defaut camera, use -1 for any cammera or a number between 1 to 99

	bool done = false;
	time_t timer;
	time(&timer); //set timer to the current date in ms in

	//CascadeClassifier customCascade;
	//customCascade.load("../../../data/haarcascades/haarcascade_frontalface_alt.xml"); //for test Custom detect use with face haar

	//Launch video in a thread
	t[0] = std::thread([&] {myVideo.start(); });

	while (true)
	{
		//We wait two seconds before to use the detection (the programs will wait the time that te detection begin) 
		if (abs(time(NULL) - timer) > 2 && !done)
		{
			//For use the detection program uncomment the follows Line.
			//Smile and Eye detect works only with face Detect
			//All detect is face + smile + eye
			myVideo.startAllDetect();
			//myVideo.startFaceDetect();
			//myVideo.startEyeDetect();
			//myVideo.startSmileDetect();
			//myVideo.startCustomDetect(customCascade,3);
			done = true;
		}
		//Show the number of the spcified object
		//cout << "nombre de visage detecter : " << myVideo.getFaceNumber() << endl;

		//Says how many faces are detected and if they are smiling or not
		//myVideo.printVectorSmilingData();

		//Says where the bigger face detected is
		//if (myVideo.getTracking() == 1)
		//{
			//cout << "the face is on the right" << endl;
		//}
		//else if (myVideo.getTracking() == 2)
		//{
			//cout << "the face is on the left" << endl;
		//}
		//else
		//{
			//cout << "the face is on the center" << endl;
		//}

		cv::waitKey(1);
	}

	return 0;
}



/*###############################################################################################################################
#																																#
#												Sample for voice recognition													#
#																																#
#################################################################################################################################*/


/*int main(int argc, const char** argv)
{
	std::string again = "a";
	Voice myVoice;
	std::string retour(myVoice.recognizeFromFile("../../../data/sound44100Hz/allDirection.wav"));
	std::cout << "finit all direction : " << retour << std::endl;
	std::string retour2(myVoice.recognizeFromFile("../../../data/sound44100Hz/avantDerrierre.wav"));
	std::cout << "finit avant derrierre : " << retour2 << std::endl;
	std::string retour3(myVoice.recognizeFromFile("../../../data/sound44100Hz/droiteGauche.wav"));
	std::cout << "finit droite gauche : " << retour3 << std::endl;
	std::string retour4(myVoice.recognizeFromFile("../../../data/sound44100Hz/detectSourires.wav"));
	std::cout << "finit detect sourires : " << retour4 << std::endl;
	std::string retour5(myVoice.recognizeFromFile("../../../data/sound44100Hz/detectFaciale.wav"));
	std::cout << "finit detect faciale : " << retour5 << std::endl;
	std::string retour6(myVoice.recognizeFromFile("../../../data/sound44100Hz/detectYeux.wav"));
	std::cout << "finit detect yeux : " << retour6 << std::endl;
	std::string retour7(myVoice.recognizeFromFile("../../../data/sound44100Hz/recoFacial.wav"));
	std::cout << "finit reco facial : " << retour7 << std::endl;
	std::string retour8(myVoice.recognizeFromFile("../../../data/sound44100Hz/stop.wav"));
	std::cout << "finit reco stop : " << retour8 << std::endl;
	getchar();

	//myVoice.recognizeFromMicrophone();

	while (again == "a")
	{
		std::string retour(myVoice.recognizeFromMicrophoneWhileTime(10));
		std::cout << retour << std::endl;
		std::cin >> again;
	}
	return 0;
}*/


/*###############################################################################################################################
#																																#
#												Sample for voice & video recognition											#		
#																																#
#################################################################################################################################*/

/*int main(int argc, const char** argv)
{
	std::thread t[1];
	std::string speech;

	Voice myVoice;
	Video myVideo(0);

	//Launch video in a thread
	t[0] = std::thread([&] {myVideo.start(); });

	while (true)
	{
		std::cout << "You can say detection faciale, detection sourrire detection yeux and stop " << std::endl;
		speech = myVoice.recognizeFromMicrophoneWhileTime(10);
		std::cout << speech << std::endl;
		if (speech == "detection faciale")
		{
			std::cout << "Begin face detect" << std::endl;
			myVideo.startFaceDetect();
		}
		if (speech == "detection sourires")
		{
			std::cout << "Begin smile detect" << std::endl;
			myVideo.startSmileDetect();
		}
		if (speech == "detection yeux")
		{
			std::cout << "Begin eyes detect" << std::endl;
			myVideo.startEyeDetect();
		}
		if (speech == "stop")
		{
			std::cout << "stop all detect" << std::endl;
			myVideo.stopAllDetect();
		}
	}
	getchar();
	return 0;
}*/


/*###############################################################################################################################
#																																#
#												Sample for face recognition         											#
#																																#
#################################################################################################################################*/


//int main(int argc, const char** argv)
//{
//	//----------------------------------- variables initialisation ---------------------------------------------------------
//	std::thread t[2];
//	// Video is the class for use video and detection
//	Video myVideo(0); //0 is the defaut camera, use -1 for any cammera or a number between 1 to 99
//	Recognizer myRecognizer;
//
//	std::vector<cv::Mat> lastFacesDetected;
//	bool done = false;
//	int i = 0;
//	std::string namePersonRecognized, goodName;
//	time_t timer;
//	time(&timer); //set timer to the current date in ms in
//
//	cv::Mat imgNull;
//
//	//Launch video in a thread
//	t[0] = std::thread([&] {myVideo.start(); });
//	myVideo.startFaceDetect();
//
//	// ---------------------------------- first option load a model manually with files -----------------------------------
//
//	//cv::Mat person1 = cv::imread("C:/dev/git/RobotLabLibrary/data/dataForRec/face/person1.jpeg", 0);
//	//cv::Mat person2 = cv::imread("C:/dev/git/RobotLabLibrary/data/dataForRec/face/person2.jpeg", 0);
//	//cv::Mat person3 = cv::imread("C:/dev/git/RobotLabLibrary/data/dataForRec/face/person3.jpeg", 0);
//	//cv::Mat person4 = cv::imread("C:/dev/git/RobotLabLibrary/data/dataForRec/face/person4.jpeg", 0);
//	//cv::Mat person5 = cv::imread("C:/dev/git/RobotLabLibrary/data/dataForRec/face/person5.jpeg", 0);
//
//	//myRecognizer.addFrameToCurrentTraining(person1,1,"person1");
//	//myRecognizer.addFrameToCurrentTraining(person2, 2, "person2");
//	//myRecognizer.addFrameToCurrentTraining(person3, 3, "person3");
//	//myRecognizer.addFrameToCurrentTraining(person4, 4, "person4");
//	//myRecognizer.addFrameToCurrentTraining(person5, 5, "person5");
//
//	// ------------------------------- second option load a model automatically with csv file --------------------------------
//
//	try {
//		myRecognizer.readCsv(CSV_FACE_RECO); // see Constantes.h
//	}
//	catch (cv::Exception& e) {
//		std::cerr << "Error opening file \"" << CSV_FACE_RECO << "\". Reason: " << e.msg << std::endl;
//		getchar();
//		exit(1);
//	}
//
//
//	//-------------------------------------- Train the loaded data for face recognition -------------------------------------
//
//	t[1] = std::thread([&] {myRecognizer.train(); });
//
//	//If you want informations on the model used, uncomment these lines
//	//std::cout << "width = " << myRecognizer.getFrameWidth() << " height = " << myRecognizer.getFrameHeight() << std::endl;
//	//std::cout << "trainingFrame size = " << myRecognizer.getTrainingFramesSize() << " labelFrameSize = " << myRecognizer.getLabelFrameSize() << std::endl;
//	//myRecognizer.printConf();
//
//
//	//------------------------------------------------Recognize face in video ------------------------------------------
//
//	while (true)
//	{
//		//dont recognize if the model is not trained or if no face never get Recognize
//		if (abs(time(NULL) - timer) > 1 && !myRecognizer.isTrained() && !myVideo.getLastFacesDetected().empty() && !myRecognizer.equalTest(myVideo.getLastFacesDetected(), lastFacesDetected))//Maybe add myVideo.getLastFacesDetected() != oldLastFacesDetected for not recognize the same image again and again
//		{
//			lastFacesDetected = myVideo.getLastFacesDetected(); // get all the current face in video stream
//			i = 0;
//			myVideo.clearLabel(); // remove the old name associed to a face
//			for (std::vector<cv::Mat>::iterator face = lastFacesDetected.begin(); face != lastFacesDetected.end(); face++, i++)//for each face detected
//			{
//				namePersonRecognized = myRecognizer.recognize(*face);//launch the recognition
//				myVideo.addLabel(namePersonRecognized, i); //display the name on the video stream. If you only want to display him if the reognition is sur move this line under, in the else statement.
//				myVideo.setImgToPrint(*face); // display in an another windows in the video thread the region of interest corresponding to the face detected
//
//				if (myRecognizer.getLastConfidence() > 3000 && !myRecognizer.askForAddImageInProcess())//If we are not sur of the recognition and we are not already asking for add image if we ask in an other thread
//				{
//					myRecognizer.askForAddImageToCurrentTrainingAndSave(*face);//Function ask the user if he want to add his face to the database and do it if the answer is positif
//					//t[2] = std::thread([&] {myRecognizer.askForAddImageToCurrentTrainingAndSave(*face); }); // if you want to use multi threading but you need to use this differently to use when you implement your GUI (maybe a other windows specially created fot that or the question under the face displayed for informations)
//				}
//				else
//				{
//					if (!myRecognizer.askForAddImageInProcess()) //if we are not already asking for add image if we ask in an other thread
//					{
//						std::cout << "Good Name ? y/n" << std::endl; //Ask the user if the name given to the face detected is correct
//						std::cin >> goodName;
//						if (goodName == "n" || goodName == "N")//If the user said that the name isn't correct ask him if he want to add the correct name to the face in the database
//						{
//							myRecognizer.askForAddImageToCurrentTrainingAndSave(*face);
//						}
//					}
//				}
//				myVideo.setImgToPrint(imgNull);
//			}
//			//std::cout << "face analyzed = " << i << std::endl;
//			time(&timer);
//		}
//		cv::waitKey(1);
//	}
//	return 0;
//}


/*###############################################################################################################################
#																																#
#										Sample for all the function of the library     											#
#																																#
#################################################################################################################################*/

/*int main(int argc, const char** argv)
{
	std::thread t[1];
	std::string speech;
	cv::Mat lastFaceDetected;
	double confidence;

	Voice myVoice;
	Video myVideo(0);
	Recognizer myRecognizer;

	//Launch video in a thread
	t[0] = std::thread([&] {myVideo.start(); });

	try {
		myRecognizer.readCsv(CSV_FACE_RECO); // see Constantes.h
	}
	catch (cv::Exception& e) {
		std::cerr << "Error opening file \"" << CSV_FACE_RECO << "\". Reason: " << e.msg << std::endl;
		getchar();
		exit(1);
	}

	while (true)
	{
		std::cout << "You can say detection faciale, detection sourrire detection yeux, reconnaissance faciale and stop " << std::endl;
		speech = myVoice.recognizeFromMicrophoneWhileTime(10);
		std::cout << speech << std::endl;
		if (speech == "detection faciale")
		{
			std::cout << "Begin face detect" << std::endl;
			myVideo.startFaceDetect();
		}
		if (speech == "detection sourires")
		{
			std::cout << "Begin smile detect" << std::endl;
			myVideo.startSmileDetect();
		}
		if (speech == "detection yeux")
		{
			std::cout << "Begin eyes detect" << std::endl;
			myVideo.startEyeDetect();
		}
		if (speech == "stop")
		{
			std::cout << "stop all detect" << std::endl;
			myVideo.stopAllDetect();
		}
		if (speech == "reconnaissance faciale")
		{
			if (!myRecognizer.isTrained() && !myVideo.getLastFaceDetected().empty() && myVideo.getLastFaceDetected().data != lastFaceDetected.data)
			{
				std::cout << "face recognition..." << std::endl;
				lastFaceDetected = myVideo.getLastFaceDetected();
				confidence = myRecognizer.recognize(lastFaceDetected);//launch the recognition
				if (confidence > 2500)
				{
					myRecognizer.askForAddImageToCurrentTrainingAndSave(lastFaceDetected);//Function ask the user if he want to add his face to the database and do it if the answer is positif
				}
			}
			else if (!myRecognizer.isTrained())
			{
				std::cout << "data is trained wait a moment and retry" << std::endl;
			}
			else
			{
				std::cout << "You need for face recognition to wait that a circle appears around your face to show that the program have detected your face" << std::endl;
			}
		}
	}

	getchar();
	return 0;
}*/


/*###############################################################################################################################
#																																#
#											Sample for create custom face database     											#
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
	int i = 0;
	std::string namePersonRecognized, goodName;
	time_t timer;
	time(&timer); //set timer to the current date in ms in

	cv::Mat imgNull;

	//Launch video in a thread
	t[0] = std::thread([&] {myVideo.start(); });
	myVideo.startFaceDetect();

	// ---------------------------------- first option load a model manually with files -----------------------------------

	//cv::Mat person1 = cv::imread("C:/dev/git/RobotLabLibrary/data/dataForRec/face/person1.jpeg", 0);
	//cv::Mat person2 = cv::imread("C:/dev/git/RobotLabLibrary/data/dataForRec/face/person2.jpeg", 0);
	//cv::Mat person3 = cv::imread("C:/dev/git/RobotLabLibrary/data/dataForRec/face/person3.jpeg", 0);
	//cv::Mat person4 = cv::imread("C:/dev/git/RobotLabLibrary/data/dataForRec/face/person4.jpeg", 0);
	//cv::Mat person5 = cv::imread("C:/dev/git/RobotLabLibrary/data/dataForRec/face/person5.jpeg", 0);

	//myRecognizer.addFrameToCurrentTrainingAndSave(person1,1,"person1", "person1", "customDatabase");
	//myRecognizer.addFrameToCurrentTrainingAndSave(person2, 2, "person2", "person1", "customDatabase");
	//myRecognizer.addFrameToCurrentTrainingAndSave(person3, 3, "person3", "person1", "customDatabase");
	//myRecognizer.addFrameToCurrentTrainingAndSave(person4, 4, "person4", "person1", "customDatabase");
	//myRecognizer.addFrameToCurrentTrainingAndSave(person5, 5, "person5", "person1", "customDatabase");


	// ------------------------------- second option load a model automatically with csv file --------------------------------

	while (true)
	{
		//dont recognize if the model is not trained or if no face never get Recognize
		if (abs(time(NULL) - timer) > 1 && !myVideo.getLastFacesDetected().empty() && !myRecognizer.equalTest(myVideo.getLastFacesDetected(), lastFacesDetected))//Maybe add myVideo.getLastFacesDetected() != oldLastFacesDetected for not recognize the same image again and again
		{
			lastFacesDetected = myVideo.getLastFacesDetected(); // get all the current face in video stream
			i = 0;
			for (std::vector<cv::Mat>::iterator face = lastFacesDetected.begin(); face != lastFacesDetected.end(); face++, i++)//for each face detected
			{
				myVideo.setImgToPrint(*face); // display in another windows in the video thread the region of interest corresponding to the face detected

				myRecognizer.askForAddImageToCurrentTrainingAndSave(*face, "customDatabase/");

				myVideo.setImgToPrint(imgNull);
			}
			//std::cout << "face analyzed = " << i << std::endl;
			time(&timer);
		}
		cv::waitKey(1);
	}
	return 0;
}

