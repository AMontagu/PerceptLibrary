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
	Voice();
	Voice(const char* hmm, const char* lm, const char* dict, const char* samprate, const char* nfft);
	Voice(const char* hmm, const char* lm, const char* dict, const char* samprate, const char* nfft, const char* pathToDirForData = TMP_FOLDER);
	void recognizeFromMicrophone();//Look for some sentence in a infinity loop
	const char * recognizeFromMicrophoneWhileTime(int timeToWait); //Add a limit time and do only one time 
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


