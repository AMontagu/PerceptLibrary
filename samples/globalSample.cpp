#include "Video.h"
#include "Voice.h"
#include "Recognizer.h"
#include "Constantes.h"

#include <iostream>

/*###############################################################################################################################
#																																#
#										Sample for all the function of the library     											#
#																																#
#################################################################################################################################*/

int main(int argc, const char** argv)
{
	std::thread t[1];
	std::string speech;
	std::string namePersonRecognized, goodName;
	std::vector<cv::Mat> lastFacesDetected;

	cv::Mat imgNull;

	int i = 0;

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
		std::cout << "You can say detection faciale, detection sourire detection yeux, reconnaissance faciale and stop " << std::endl;
		speech = myVoice.recognizeFromMicrophoneWhileTime(5);
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
			if (!myRecognizer.isTrained() && !myVideo.getLastFacesDetected().empty() && !myRecognizer.equalTest(myVideo.getLastFacesDetected(), lastFacesDetected))
			{
				lastFacesDetected = myVideo.getLastFacesDetected(); // get all the current face in video stream
				i = 0;
				myVideo.clearLabel(); // remove the old name associed to a face
				for (std::vector<cv::Mat>::iterator face = lastFacesDetected.begin(); face != lastFacesDetected.end(); face++, i++)//for each face detected
				{
					namePersonRecognized = myRecognizer.recognize(*face);//launch the recognition
					myVideo.setImgToPrint(*face);
					myVideo.addLabel(namePersonRecognized, i); //display the name on the video stream. If you only want to display him if the reognition is sur move this line under, in the else statement.
					if (myRecognizer.getLastConfidence() > 2500 && !myRecognizer.askForAddImageInProcess())
					{
						myRecognizer.askForAddImageToCurrentTrainingAndSave(*face,"customDatabase/");//Function ask the user if he want to add his face to the database and do it if the answer is positif
					}
					else
					{
						if (!myRecognizer.askForAddImageInProcess()) //if we are not already asking for add image if we ask in an other thread
						{
							std::cout << "Good Name ? y/n" << std::endl; //Ask the user if the name given to the face detected is correct
							std::cin >> goodName;
							if (goodName == "n" || goodName == "N")//If the user said that the name isn't correct ask him if he want to add the correct name to the face in the database
							{
								myRecognizer.askForAddImageToCurrentTrainingAndSave(*face, "customDatabase/");
							}
						}
					}
					myVideo.setImgToPrint(imgNull);
					myVideo.clearLabel();
				}
			}
			else if (!myVideo.getDetectFaceOn())
			{
				std::cout << "Face detection need to be on for face recognition" << std::endl;
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
}