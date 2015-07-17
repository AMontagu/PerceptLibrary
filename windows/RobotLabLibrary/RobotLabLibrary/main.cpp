#include "Video.h"
#include "Voice.h"
#include <thread>

/*###############################################################################################################################
#																																#
#												Sample for video recognition													#
#																																#
#################################################################################################################################*/

/*
int main(int argc, const char** argv)
{
	// Video is the class for use video and detection
	Video myVideo(0); //0 is the defaut camera, use -1 for any cammera or a number between 1 to 99

	bool done = false;
	time_t timer;
	time(&timer); //set timer to the current date in ms in

	//CascadeClassifier customCascade;
	//customCascade.load("../../../data/haarcascades/haarcascade_frontalface_alt.xml"); //for test Custom detect use with face haar

	while (true)
	{
		myVideo.start(); // Launch the windows for display the video stream

		//We wait two seconds before to use the detection (the programs will wait the time that te detection begin) 
		if (abs(time(NULL) - timer) > 0 && !done)
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
}*/



/*###############################################################################################################################
#																																#
#												Sample for voice recognition													#
#																																#
#################################################################################################################################*/

/*
int main(int argc, const char** argv)
{
	std::string again = "a";
	Voice myVoice;
	std::string retour(myVoice.recognizeFromFile("../../../data/sound44100Hz/direction.wav"));
	std::cout << "finit direction : " << retour << std::endl;
	std::string retour2(myVoice.recognizeFromFile("../../../data/sound44100Hz/recoSourires.wav"));
	std::cout << "finit recoSmile : " << retour2 << std::endl;
	std::string retour3(myVoice.recognizeFromFile("../../../data/sound44100Hz/recoYeux.wav"));
	std::cout << "finit reco eyes : " << retour3 << std::endl;
	std::string retour4(myVoice.recognizeFromFile("../../../data/sound44100Hz/recoFacial.wav"));
	std::cout << "finit reco facial : " << retour4 << std::endl;
	std::string retour5(myVoice.recognizeFromFile("../../../data/sound44100Hz/stop.wav"));
	std::cout << "finit reco stop : " << retour5 << std::endl;
	getchar();

	//myVoice.recognizeFromMicrophone();

	while (again == "a")
	{
		std::string retour(myVoice.recognizeFromMicrophoneWhileTime(10));
		std::cout << retour << std::endl;
		std::cin >> again;
	}
	return 0;
	//test for git
}*/


/*###############################################################################################################################
#																																#
#												Sample for voice & video recognition											#		
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
		//myVideo.start();
		std::cout << "You can say reconnaissance facial, reconnaissance sourrire reconnaissance yeux and stop " << std::endl;
		speech = myVoice.recognizeFromMicrophoneWhileTime(10);
		std::cout << speech << std::endl;
		if (speech == "reconnaissance facial")
		{
			std::cout << "Begin face detect" << std::endl;
			myVideo.startFaceDetect();
		}
		if (speech == "reconnaissance sourires")
		{
			std::cout << "Begin smile detect" << std::endl;
			myVideo.startSmileDetect();
		}
		if (speech == "reconnaissance yeux")
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