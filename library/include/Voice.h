#ifndef VOICE_H
#define VOICE_H

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <pocketsphinx.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <ctime>
#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>

#include "Constantes.h"

#if defined(_WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#else
#include <sys/select.h>
#endif

class Voice
{
public:

	/* @brief Constructor that allow you to use your own hmm and language model for vocal recoginition.
	*
	* @param hmm: Path to the acoustic model folder.
	* @param lm: Path to the langage model.
	* @param dict: path to the dictionnary.
	* @param samprate: The samprate used by your microphone. If you don't know record some sentence and open the .raw data with audacity to find wich samprate your microphone use.
	* @param nfft: adapting the buffer based on the samprate.
	*/
	Voice(const char* hmm = HMM_PATH, const char* lm = LM_PATH, const char* dict = DICT_PATH, const char* samprate = "44100", const char* nfft = "2048");

	/* @brief Constructor that allow you to use your own hmm and language model for vocal recoginition and to store your vocal recording in a specified folder. usefull for check in which samprate your microphone work.
	*
	* @param useless: for make the difference between the both constructors. You can pass what you want.
	* @param hmm: Path to the acoustic model folder.
	* @param lm: Path to the langage model.
	* @param dict: path to the dictionnary.
	* @param samprate: The samprate used by your microphone. If you don't know record some sentence and open the .raw data with audacity to find wich samprate your microphone use.
	* @param nfft: adapting the buffer based on the samprate.
	* @param _pathToDirForData: Path to a directory where you could save your record for find some error in recorder.
	*/
	Voice(bool useless, const char* hmm = HMM_PATH, const char* lm = LM_PATH, const char* dict = DICT_PATH, const char* samprate = "44100", const char* nfft = "2048", const char* pathToDirForData = TMP_FOLDER);
	
	/* @brief The sample function of pocketSphinx renamed. Wait until you talk begin to recording until you stop and print what he recognized of what you said.
	*
	*/
	void recognizeFromMicrophone();

	/* @brief An upgraded function who do the same that recognizeFromMicrophone but with a timeout and only for one command and not in an infinity loop. Return the world recognized.
	*
	* @param timeToWait: Time in seconds for the listening process.
	*/
	const char * recognizeFromMicrophoneWhileTime(int timeToWait); 

	/* @brief Recognize world in a .wav file. Be careful to recording a file with the good settings, especially the samperate.
	*
	* @param fname: Name of the audio file we want to recognize.
	*/
	const char * recognizeFromFile(char *fname);

	~Voice();

private:
	ps_decoder_t *ps;
	cmd_ln_t *config;
	FILE *rawfd;
	void sleepMsec(int32 ms);
	int check_wav_header(char *header, int expected_sr);
};

#endif


