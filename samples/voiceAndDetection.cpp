#include "Video.h"
#include "Voice.h"

#include <iostream>

/*###############################################################################################################################
#																																#
#												Sample for voice & video detection												#		
#																																#
#################################################################################################################################*/

int main(int argc, const char** argv)
{
	std::thread t[1];
	std::string speech;

	Voice myVoice;
	Video myVideo(0);

	//Launch video in a thread
	t[0] = std::thread([&] {myVideo.start(); });

	while (true)
	{
		std::cout << "You can say detection faciale, detection sourire detection yeux and stop " << std::endl;
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
	}
	getchar();
	return 0;
}