#include "Video.h"

int main(int argc, const char** argv)
{
	// Video is the class for use video and detection
	Video myVideo(0); //0 is the defaut camera, use -1 for any cammera or a number between 1 to 99

	bool done = false;
	time_t timer;
	time(&timer); //set timer to the current date in ms in

	/*CascadeClassifier customCascade;
	customCascade.load("../../../data/haarcascades/haarcascade_frontalface_alt.xml"); //for test Custom detect use with face haar*/

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
			//myVideo.startCustomDetect(customCascade,3);
			done = true;
		}
		//Show the number of the spcified object
		//cout << "nombre de visage detecter : " << myVideo.getFaceNumber() << endl;

		//Says how many faces are detected and if they are smiling or not
		//myVideo.printVectorSmilingData();

		//Says where the bigger face detected is
		/*if (myVideo.getTracking() == 1)
		{
			cout << "the face is on the right" << endl;
		}
		else if (myVideo.getTracking() == 2)
		{
			cout << "the face is on the left" << endl;
		}
		else
		{
			cout << "the face is on the center" << endl;
		}*/

		waitKey(1);
	}

	return 0;
}