#include "Video.h"
#include "Voice.h"

#include <iostream>

/*###############################################################################################################################
#																																#
#												Sample for voice & video detection												#		
#																																#
#################################################################################################################################*/

//std::string processOnRecognitionPersonnalized(std::string dataToProcess);

int main(int argc, const char** argv)
{
	std::thread t[1];
	std::string speech, wordRecognized;

	//Voice myVoice;
	/* -------------------------------- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!--------------------------------------
	You may have problem here if you don't use the right settings for pocketsphinx. Please be sur of your frequency recorder.
	In linux is often 16000 and in Windows 44100.
	If you set frequency to 16000 set nfft to 542 else for 44100 set to 2048.
	*/
	Voice myVoice("../data/modelPocketSphinx/roboticModel/fr-fr", "../data/modelPocketSphinx/roboticModel/roboticOrder.lm", "../data/modelPocketSphinx/roboticModel/roboticOrder.dic", "16000", "542");
	Video myVideo(0);

	//Launch video in a thread
	t[0] = std::thread([&] {myVideo.start(); });

	while (true)
	{
		std::cout << "You can say robotlab followed by: detection faciale, detection sourires, detection yeux and stop " << std::endl;
		wordRecognized = myVoice.recognizeFromMicrophoneWhileTime(7); //get all the world recognized in wordRecognized variable
		speech = myVoice.processOnRecognition(wordRecognized); //get only a valide commande in all the words says. A valide commande is for example : robotlab detection faciale
		std::cout << speech << std::endl; //print what are the valice command found
		if (speech == "robotlab detection faciale")
		{
			std::cout << "Begin face detect" << std::endl;
			myVideo.startFaceDetect(); //launch the face detection
		}
		if (speech == "robotlab detection sourires")
		{
			std::cout << "Begin smile detect" << std::endl;
			myVideo.startSmileDetect(); //launch the smile detection
		}
		if (speech == "robotlab detection yeux")
		{
			std::cout << "Begin eyes detect" << std::endl;
			myVideo.startEyeDetect(); //launch the eye detection
		}
		if (speech == "robotlab stop")
		{
			std::cout << "stop all detect" << std::endl;
			myVideo.stopAllDetect(); //stop all detection
		}
	}
	getchar();
	return 0;
}

//If you use an other language model that mine you can change the process on recognition function for adapt it to your need.
/*std::string processOnRecognitionPersonnalized(std::string dataToProcess)
{
	std::vector<std::string> vectorString;
	std::string stringRetour;
	bool robotlabSays = false, detectionSays = false, reconnaissanceSays = false;

	std::string::size_type stTemp = dataToProcess.find(" ");

	while (stTemp != std::string::npos)
	{
		vectorString.push_back(dataToProcess.substr(0, stTemp));
		dataToProcess = dataToProcess.substr(stTemp + 1);
		stTemp = dataToProcess.find(" ");
	}
	vectorString.push_back(dataToProcess);

	//std::cout << "vector size: " << vectorString.size() << std::endl;

	for (int i = 0; i < vectorString.size(); i++)
	{
		//std::cout << "value in treatment: " << vectorString[i].data() << std::endl;
		if (vectorString[i] == "robotlab")
		{
			robotlabSays = true;
			if (!stringRetour.empty())
			{
				stringRetour.clear();
			}
			stringRetour += vectorString[i];
		}
		else if (robotlabSays)
		{
			if (detectionSays)
			{
				if (vectorString[i] == "faciale" || vectorString[i] == "sourires" || vectorString[i] == "yeux")
				{
					stringRetour += " " + vectorString[i];
				}
				detectionSays = false;
				robotlabSays = false;
			}
			else if (reconnaissanceSays)
			{
				if (vectorString[i] == "faciale")
				{
					stringRetour += " " + vectorString[i];
				}
				reconnaissanceSays = false;
				robotlabSays = false;
			}
			else if (vectorString[i] == "reconnaissance")
			{
				stringRetour += " " + vectorString[i];
				reconnaissanceSays = true;
			}
			else if (vectorString[i] == "detection")
			{
				stringRetour += " " + vectorString[i];
				detectionSays = true;
			}
			else
			{
				stringRetour += " " + vectorString[i];
				robotlabSays = false;
			}
		}
	}

	if (!stringRetour.empty())
	{
		return stringRetour;
	}
	else
	{
		return "invalide";
	}
}*/