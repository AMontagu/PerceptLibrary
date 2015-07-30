

# Welcome to the RobotLabLibrary !

Here we are to talk more about the RobotLabLibrary and how to use it. For the moment only source are available and you will need to include directly the project or the file in your project. Soon a .lib will be available.

The library is constitued to 3 classes : [Video](https://github.com/AMontagu/RobotLabLibrary/wiki/Video), [Recognizer](https://github.com/AMontagu/RobotLabLibrary/wiki/Voice) and [Voice](https://github.com/AMontagu/RobotLabLibrary/wiki/Recognizer)

**For documentation please see the links above.**

# Sommaire : 

* [Video Presentation](#video-presentation)
* [How To Install it](#how-to-install-it)
    * [Windows](#windows)
    * [Linux](#linux)
* [Troubleshooting](#troubleshooting)
* [What Do The Sample](#what-do-the-sample)

#Video presentation

See our video at this adress : (coming soon)

# How to install it

First you need to install on your computer OpenCV and PocketSphinx\PocketBase.   

### Windows
To do that follow this instructions for openCV : 


* [http://docs.opencv.org/doc/tutorials/introduction/windows_install/windows_install.html](http://docs.opencv.org/doc/tutorials/introduction/windows_install/windows_install.html)
* [https://www.youtube.com/watch?v=EcFtefHEEII](https://www.youtube.com/watch?v=EcFtefHEEII) (if you prefere a simple tutorial)

And this instruction for pocketSphinx and pocketBase : 

* [http://cmusphinx.sourceforge.net/wiki/download](http://cmusphinx.sourceforge.net/wiki/download)(You just need to download the source and uncompressed them. For use them after I recommend you to save them near to a root folder)

The next and final step is to configure your visual project.
Open the robotLab project and add your path to opencv and pocketSphinx : 


**In C/C++ -> General -> Other include repertories :**

< pocketsphinx_dir >\include;

< sphinxbase_dir >\include\win32;

< sphinxbase_dir >\include;

< opencv_dir >\build\include;



**(In debug mode) In Link Editors -> General -> Additional library directories :**

< pocketsphinx_dir >\bin\Debug;

< opencv_dir >\build\x86\vc12\lib;



**(In release mode) In Link Editors -> General -> Additional library directories :**

< pocketsphinx_dir >\bin\Release;

< opencv_dir >\build\x86\vc12\lib;



**(In debug mode) In Link Editors -> Entry -> additional dependencies :**

opencv_world300d.lib;opencv_ts300d.lib;%(AdditionalDependencies);sphinxbase.lib;pocketsphinx.lib



**(In release mode ) In Link Editors -> Entry -> additional dependencies :**

opencv_world300.lib;opencv_ts300.lib;%(AdditionalDependencies);sphinxbase.lib;pocketsphinx.lib



Now uncomment the sample program in main.cpp that you want to launch and build !

If you have an error go to [troubleshooting](https://github.com/AMontagu/RobotLabLibrary/wiki/Home#troubleshooting) 

### Linux 


### Troubleshooting : 

**If you have this error : Can't launch the program because it miss *.dll on your computer.**

Be sur in In Link Editors -> Entry -> additional dependencies you have all the dependencies and in Debug and Release mode. And be sure that you have add in the PATH variable a path to your bin opencv folder. (You may be need to restart your computer). 

--------------------------------------------

**If you have this error : error LNK1104: can't open the file '*.lib'.**   	
This is because you don't have add the right path in In Link Editors -> General -> Additional library directories be careful of the two mode Debug and Release for pocketSphinx.

-------------------------------------------

**If you have this error : error C1083: can't open include file : '*.h' : No such file or directory.**

It's because you miss a include directory to add in C/C++ -> General -> Other include repertories please check again with the reportories I said to add and if you are all find in what include folder is the file in question. When you find it add it to the other include repertories with the others.


-----------------------------------------

**If no error appears but your camera doesn't return anything.**

Try to change the ip of your cam device. Try Video myVideo(1); or Video myVideo(-1); instead of Video myVideo(0);


-----------------------------------------

**Please tell me (adrienmontagu@gmail.com) if you meet an error or a problem in your installation process.**



# What do the sample

Above each sample you have an header like this : 

	/*###############################################################################################################################
	#																																#
	#												Sample for video detection  													#
	#																																#
	#################################################################################################################################*/

This is the name of the sample and I will use this name for describe them.


**Sample for video detection :**

This sample will show you the possibility of detecting something in a frame of one video stream. For the example and the usage I have already prepared 3 detections : faces, smiles and eyes. You can launch them in te same time or only few of them (for smiles and eyes detection you need to have face detection on).

For use the custom detection you have to create your haar_cascade xml (http://note.sonots.com/SciSoftware/haartraining.html and http://docs.opencv.org/modules/objdetect/doc/cascade_classification.html) and load it in a cv::CascadeClassifier variable that you will send in parameter wend you will start custom object detection.

Finally you can uncomment some features for print in the console the number of face detected or where the bigger face detected here compared to the camera.



**Sample for voice recognition :** 

This sample is shows us that the vocale recognition works with some enregistred sound and allow the user to test this with our voice and our recorder while we want. The other things you need to know are printed on the console.

The recognition stop when the recorder doesn't listen nothing (after have listenning somethings) or if the time is elapsed. You can change the time when you call the myVoice.recognizeFromMicrophoneWhileTime(10) function.


**Sample for voice & video detection :** 

A mix between voice and video sample. We launch every video function with the voice. 

**Sample for face recognition :**

Load a model manually or with a csv file and try to recognize face from video stream with this model.If the face is recognized the sample will display the name of the person next to her face. Otherwise it will ask to the user if he want to save his face to the model with his name.


**Sample for all the function of the library :**

A mix between the three classes. Is the same that the Sample for voice & video but with a vocal command for begin face recognition.

**Sample for create custom face database :**

I recommended you to launch this sample before any other sample using face recognition. It will allow you to use your own database and use the good size for the recognition. 


# Wiki Sommarie :

* [Home](https://github.com/AMontagu/RobotLabLibrary/wiki)
    * [Video Presentation](https://github.com/AMontagu/RobotLabLibrary/wiki#video-presentation)
    * [How To Install it](https://github.com/AMontagu/RobotLabLibrary/wiki#how-to-install-it)
        * [Windows](https://github.com/AMontagu/RobotLabLibrary/wiki#windows)
        * [Linux](https://github.com/AMontagu/RobotLabLibrary/wiki#linux)
    * [Troubleshooting](https://github.com/AMontagu/RobotLabLibrary/wiki#troubleshooting)
    * [What Do The Sample](https://github.com/AMontagu/RobotLabLibrary/wiki#what-do-the-sample) 

* [Recognizer](https://github.com/AMontagu/RobotLabLibrary/wiki/Recognizer)
    * [Class Overview](https://github.com/AMontagu/RobotLabLibrary/wiki/Recognizer#class-overview)
    * [Public Constructors](https://github.com/AMontagu/RobotLabLibrary/wiki/Recognizer#public-constructors)
    * [Public Methods](https://github.com/AMontagu/RobotLabLibrary/wiki/Recognizer#public-methods)
    * [Sample](https://github.com/AMontagu/RobotLabLibrary/wiki/Recognizer#sample)
    * [How To Personnalize](https://github.com/AMontagu/RobotLabLibrary/wiki/Recognizer#how-to-personnalize)

* [Video](https://github.com/AMontagu/RobotLabLibrary/wiki/Video)
    * [Class Overview](https://github.com/AMontagu/RobotLabLibrary/wiki/Video#class-overview)
    * [Public Constructors](https://github.com/AMontagu/RobotLabLibrary/wiki/Video#public-constructors)
    * [Public Methods](https://github.com/AMontagu/RobotLabLibrary/wiki/Video#public-methods)
    * [Sample](https://github.com/AMontagu/RobotLabLibrary/wiki/Video#sample)
    * [How To Personnalize](https://github.com/AMontagu/RobotLabLibrary/wiki/Video#how-to-personnalize)

* [Voice](https://github.com/AMontagu/RobotLabLibrary/wiki/Voice)
    * [Class Overview](https://github.com/AMontagu/RobotLabLibrary/wiki/Voice#class-overview)
    * [Public Constructors](https://github.com/AMontagu/RobotLabLibrary/wiki/Voice#public-constructors)
    * [Public Methods](https://github.com/AMontagu/RobotLabLibrary/wiki/Voice#public-methods)
    * [Sample](https://github.com/AMontagu/RobotLabLibrary/wiki/Voice#sample)
    * [How To Personnalize](https://github.com/AMontagu/RobotLabLibrary/wiki/Voice#how-to-personnalize)





