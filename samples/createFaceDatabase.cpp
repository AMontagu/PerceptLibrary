#include "Video.h"
#include "Recognizer.h"
#include "Constantes.h"

#include <iostream>

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

				myRecognizer.askForAddImageToCurrentTrainingAndSave(*face, "customDatabase/"); //Ask if you want to add the face

				myVideo.setImgToPrint(imgNull); //remove the second windows
			}
			//std::cout << "face analyzed = " << i << std::endl;
			time(&timer);
		}
		cv::waitKey(1);
	}
	return 0;
}