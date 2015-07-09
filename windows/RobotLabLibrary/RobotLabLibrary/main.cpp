#include "Video.h"

int main(int argc, const char** argv)
{
	// Video is the class for use video and detection
	Video myVideo(0); //0 is the defaut camera, use -1 for any cammera or a number between 1 to 99

	bool done = false;
	time_t timer;
	time(&timer); //set timer to the current date in ms in

	while (true)
	{
		myVideo.start(); // Launch the windows for display the video stream

		//We wait two seconds before to use the detection (the programs will bug the time that te detection begin) 
		if (abs(time(NULL) - timer) > 2 && !done)
		{
			//For use the detection program uncomment the follows Line.
			//Smile and Eye detect works only with face Detect
			//All detect is face + smile + eye
			myVideo.startAllDetect();
			//myVideo.startFaceDetect();
			//myVideo.startEyeDetect();
			//myVideo.startSmileDetect();
			//myVideo.startCustomDetect(MyOwnCascadeClassifierNeedToBeCreateBeforeWhile);
			done = true;
		}
		cout << "nombre de visage detecter : " << myVideo.getVisageNumber() << endl;
		waitKey(1);
	}

	return 0;
}