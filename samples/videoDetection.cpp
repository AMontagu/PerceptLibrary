#include "Video.h"

#include <iostream>

/*###############################################################################################################################
#																																#
#												Sample for video detection  													#
#																																#
#################################################################################################################################*/


int main(int argc, const char** argv)
{
	std::thread t[1];
	// Video is the class for use video and detection
	Video myVideo(0); //0 is the defaut camera, use -1 for any cammera or a number between 1 to 99

	bool done = false;
	time_t timer;
	time(&timer); //set timer to the current date in ms in

	//cv::CascadeClassifier customCascade;
	//customCascade.load("../../../data/haarcascades/haarcascade_smile.xml"); //for test Custom detect use with face haar

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
			//myVideo.startAllDetect();
			myVideo.startFaceDetect();
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