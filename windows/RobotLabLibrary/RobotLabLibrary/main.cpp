#include "Video.h"

int main(int argc, const char** argv)
{
	Video myVideo(0);

	time_t timer;

	bool done = false;

	time(&timer);

	while (true)
	{
		myVideo.start();

		//cout << timer - time(NULL) << endl;

		if (abs(time(NULL) - timer) > 2 && !done)
		{
			myVideo.startAllDetect();
			//myVideo.startSmileDetect();
			done = true;
		}
		//cout << myVideo.getVisageNumber() << endl;
		waitKey(1);
	}

	return 0;
}