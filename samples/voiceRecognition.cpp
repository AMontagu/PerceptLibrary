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
		std::cout << "The world recognized are avant, derierre, droite, gauche, detection, faciale, sourire, yeux, reconnaissance and stop " << std::endl;
		std::string retour(myVoice.recognizeFromMicrophoneWhileTime(5));
		std::cout << "reconnu : " << retour << std::endl;
		std::cout << "test again ? y/n " << std::endl;
		std::cin >> again;
	}
	return 0;
}

