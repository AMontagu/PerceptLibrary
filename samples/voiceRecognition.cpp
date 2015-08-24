#include "Voice.h"
#include "Video.h"
#include "Recognizer.h"

#include <iostream>
#include <stdio.h>

/*###############################################################################################################################
#																																#
#												Sample for voice recognition													#
#																																#
#################################################################################################################################*/


//std::string processOnRecognitionPersonnalized(std::string dataToProcess);

int main(int argc, const char** argv)
{
	std::string again = "y";
	Voice myVoice;
	std::string retour(myVoice.recognizeFromFile("../data/sound44100Hz/allDirection.wav"));
	std::cout << "finit all direction : " << retour << std::endl;
	std::string retour2(myVoice.recognizeFromFile("../data/sound44100Hz/avantDerrierre.wav"));
	std::cout << "finit avant derrierre : " << retour2 << std::endl;
	std::string retour3(myVoice.recognizeFromFile("../data/sound44100Hz/droiteGauche.wav"));
	std::cout << "finit droite gauche : " << retour3 << std::endl;
	std::string retour4(myVoice.recognizeFromFile("../data/sound44100Hz/detectSourires.wav"));
	std::cout << "finit detect sourires : " << retour4 << std::endl;
	std::string retour5(myVoice.recognizeFromFile("../data/sound44100Hz/detectFaciale.wav"));
	std::cout << "finit detect faciale : " << retour5 << std::endl;
	std::string retour6(myVoice.recognizeFromFile("../data/sound44100Hz/detectYeux.wav"));
	std::cout << "finit detect yeux : " << retour6 << std::endl;
	std::string retour7(myVoice.recognizeFromFile("../data/sound44100Hz/recoFacial.wav"));
	std::cout << "finit reco facial : " << retour7 << std::endl;
	std::string retour8(myVoice.recognizeFromFile("../data/sound44100Hz/stop.wav"));
	std::cout << "finit reco stop : " << retour8 << std::endl;
	getchar();

	//myVoice.recognizeFromMicrophone();

	while (again == "y" || again =="Y")
	{
		std::cout << "The world recognized are avant, derierre, droite, gauche, detection, faciale, sourires, yeux, reconnaissance and stop " << std::endl;
		std::string retour(myVoice.recognizeFromMicrophoneWhileTime(10));
		std::cout << "recognized : " << retour << std::endl;
		std::string retourProcess = myVoice.processOnRecognition(retour);
		std::cout << "after process : " << retourProcess << std::endl;
		std::cout << "test again ? y/n " << std::endl;
		std::cin >> again;
	}
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

