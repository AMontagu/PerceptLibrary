#include "../include/Voice.h"
#include <pocketsphinx.h>


//Personnalizable constructor
Voice::Voice(const char* hmm, const char* lm, const char* dict, const char* samprate, const char* nfft)
{
	err_set_logfp(NULL);
	config = cmd_ln_init(NULL, ps_args(), TRUE,
		"-hmm", hmm,
		"-lm", lm,
		"-dict", dict,
		"-samprate", samprate, "-nfft", nfft,
		NULL);

	if (config == NULL)
	{
		std::cout << "config is null" << std::endl;
	}
	ps = ps_init(config);
	if (ps == NULL)
	{
		std::cout << "ps is null" << std::endl;
		getchar();
	}
}

//Personnalizable constructor with save data
Voice::Voice(bool useless, const char* hmm, const char* lm, const char* dict, const char* samprate, const char* nfft, const char* pathToDirForData)
{
	err_set_logfp(NULL);
	config = cmd_ln_init(NULL, ps_args(), TRUE,
		"-hmm", hmm,
		"-lm", lm,
		"-dict", dict,
		"-samprate", samprate, "-nfft", nfft,
		"-rawlogdir", pathToDirForData,
		NULL);

	if (config == NULL)
	{
		std::cout << "config is null" << std::endl;
	}
	ps = ps_init(config);
	if (ps == NULL)
	{
		std::cout << "ps is null" << std::endl;
		getchar();
	}
}

const char * Voice::recognizeFromMicrophoneWhileTime(int timeToWait)
{
	ad_rec_t *ad; //pointer to the audio device
	int16 adbuf[65536]; //buffer for stock data before convertion
	uint8 utt_started, in_speech; //utt_started go to true when speaker begin to speek. in_speech is true while the speaker talk.
	int32 k;
	const char *hyp; // is the result of vocal recognition in const char* format
	time_t timer; //For stop the listening 

	//Some secutity check when we open the audio device:
	if ((ad = ad_open_dev(cmd_ln_str_r(config, "-adcdev"), (int)cmd_ln_float32_r(config, "-samprate"))) == NULL)
		E_FATAL("Failed to open audio device\n");
	if (ad_start_rec(ad) < 0)
		E_FATAL("Failed to start recording\n");

	if (ps_start_utt(ps) < 0)
		E_FATAL("Failed to start utterance\n");

	std::cout << "LISTENING...." << std::endl;

	time(&timer);
	in_speech = FALSE;
	utt_started = FALSE;
	while (time(NULL) - timer < timeToWait && (in_speech || !utt_started))//While time is okay and the speeker had say somethings and he didn't stop
	{
		if ((k = ad_read(ad, adbuf, 65536)) < 0) //read sound into buffer
			E_FATAL("Failed to read audio\n");
		ps_process_raw(ps, adbuf, k, FALSE, FALSE);
		in_speech = ps_get_in_speech(ps);
		if (in_speech && !utt_started)
		{
			utt_started = TRUE;
		}
	}
	std::cout << "FINISH...." << std::endl;
	ps_end_utt(ps);
	hyp = ps_get_hyp(ps, NULL); //We get the result of the recorder
	ad_close(ad);
	if (hyp != NULL)
	{
		return hyp; //return the result
	}
	else
	{
		return "Error";
	}
}

void Voice::recognizeFromMicrophone()
{
	ad_rec_t *ad;
	int16 adbuf[65536];
	uint8 utt_started, in_speech;
	int32 k;
	char const *hyp;

	//some security check when we open the audio device:
	if ((ad = ad_open_dev(cmd_ln_str_r(config, "-adcdev"), (int)cmd_ln_float32_r(config, "-samprate"))) == NULL)
		E_FATAL("Failed to open audio device\n");
	if (ad_start_rec(ad) < 0)
		E_FATAL("Failed to start recording\n");

	if (ps_start_utt(ps) < 0)
		E_FATAL("Failed to start utterance\n");

	utt_started = FALSE;
	std::cout << "READY...." << std::endl;

	while (true) {
		if ((k = ad_read(ad, adbuf, 65536)) < 0)//Get the sound in buffer
			E_FATAL("Failed to read audio\n");
		ps_process_raw(ps, adbuf, k, FALSE, FALSE);
		in_speech = ps_get_in_speech(ps);
		if (in_speech && !utt_started) {
			utt_started = TRUE;
			std::cout << "Listening...." << std::endl;
		}
		if (!in_speech && utt_started) {
			std::cout << "finish to listening" << std::endl;
			/* speech -> silence transition, time to start new utterance  */
			ps_end_utt(ps);
			hyp = ps_get_hyp(ps, NULL); //Get value
			if (hyp != NULL)
				std::cout << hyp << std::endl;

			if (ps_start_utt(ps) < 0)
				E_FATAL("Failed to start utterance\n");
			utt_started = FALSE;
			std::cout << "READY...." << std::endl;
		}
		sleepMsec(100);
	}
	ad_close(ad);

}


const char * Voice::recognizeFromFile(char *fname)
{
	int16 adbuf[65536];
	const char *hyp;
	int32 k;
	uint8 utt_started, in_speech;

	if ((rawfd = fopen(fname, "rb")) == NULL)//open file
	{
		std::cout << "Failed to open file " << fname << " for reading" << std::endl;
		//E_FATAL_SYSTEM("Failed to open file '%s' for reading", fname);
		getchar();//Permit to read the error message
	}

	//Check for fille format
	if (strlen(fname) > 4 && strcmp(fname + strlen(fname) - 4, ".wav") == 0)
	{
		char waveheader[44];
		fread(waveheader, 1, 44, rawfd);
		if (!check_wav_header(waveheader, (int)cmd_ln_float32_r(config, "-samprate")))
		{
			std::cout << "Failed to process file " << fname << " due to format mismatch." << std::endl;
			//E_FATAL("Failed to process file '%s' due to format mismatch.\n", fname);
			getchar();
		}
	}

	//Check for file format
	if (strlen(fname) > 4 && strcmp(fname + strlen(fname) - 4, ".mp3") == 0)
	{
		std::cout << "Can not decode mp3 files, convert input file to WAV 16kHz 16-bit mono before decoding." << std::endl;
		//E_FATAL("Can not decode mp3 files, convert input file to WAV 16kHz 16-bit mono before decoding.\n");
		getchar();
	}

	ps_start_utt(ps);
	utt_started = FALSE;

	while ((k = fread(adbuf, sizeof(int16), 65536, rawfd)) > 0) //While there is data in file
	{
		ps_process_raw(ps, adbuf, k, FALSE, FALSE);//Stock data in buffer
		in_speech = ps_get_in_speech(ps);//Says if the person talks or not
		if (in_speech && !utt_started)
		{
			utt_started = TRUE;
		}
		if (!in_speech && utt_started)
		{
			ps_end_utt(ps);
			hyp = ps_get_hyp(ps, NULL);
			if (hyp != NULL)
			{
				return hyp;
			}
			else
			{
				return "Error";
			}
			ps_start_utt(ps);
			utt_started = FALSE;
		}
	}
	ps_end_utt(ps);
	if (utt_started) {
		hyp = ps_get_hyp(ps, NULL);
		if (hyp != NULL)
		{
			fclose(rawfd);
			return hyp;
		}
		else
		{
			fclose(rawfd);
			return "Error";
		}
	}
	return "Error";
}

//Function for check wav header
int Voice::check_wav_header(char *header, int expected_sr)
{
	int sr;

	if (header[34] != 0x10) {
		E_ERROR("Input audio file has [%d] bits per sample instead of 16\n", header[34]);
		return 0;
	}
	if (header[20] != 0x1) {
		E_ERROR("Input audio file has compression [%d] and not required PCM\n", header[20]);
		return 0;
	}
	if (header[22] != 0x1) {
		E_ERROR("Input audio file has [%d] channels, expected single channel mono\n", header[22]);
		return 0;
	}
	sr = ((header[24] & 0xFF) | ((header[25] & 0xFF) << 8) | ((header[26] & 0xFF) << 16) | ((header[27] & 0xFF) << 24));
	if (sr != expected_sr) {
		E_ERROR("Input audio file has sample rate [%d], but decoder expects [%d]\n", sr, expected_sr);
		return 0;
	}
	return 1;
}

std::string Voice::processOnRecognition(std::string dataToProcess)
{
	std::vector<std::string> vectorString;
	std::string stringRetour;
	bool robotlabSays = false, detectionSays = false, reconnaissanceSays = false;

	std::string::size_type stTemp = dataToProcess.find(" ");

	while (stTemp != std::string::npos)
	{
		vectorString.push_back(dataToProcess.substr(0, stTemp));
		dataToProcess = dataToProcess.substr(stTemp + 1);
		stTemp = dataToProcess.find(" ");
	}
	vectorString.push_back(dataToProcess);

	//std::cout << "vector size: " << vectorString.size() << std::endl;

	for (int i = 0; i < vectorString.size(); i++)
	{
		//std::cout << "value in treatment: " << vectorString[i].data() << std::endl;
		if (vectorString[i] == "robotlab")
		{
			robotlabSays = true;
			if (!stringRetour.empty())
			{
				stringRetour.clear();
			}
			stringRetour += vectorString[i];
		}
		else if (robotlabSays)
		{
			if (detectionSays)
			{
				if (vectorString[i] == "faciale" || vectorString[i] == "sourires" || vectorString[i] == "yeux")
				{
					stringRetour += " " + vectorString[i];
				}
				detectionSays = false;
				robotlabSays = false;
			}
			else if (reconnaissanceSays)
			{
				if (vectorString[i] == "faciale")
				{
					stringRetour += " " + vectorString[i];
				}
				reconnaissanceSays = false;
				robotlabSays = false;
			}
			else if (vectorString[i] == "reconnaissance")
			{
				stringRetour += " " + vectorString[i];
				reconnaissanceSays = true;
			}
			else if (vectorString[i] == "detection")
			{
				stringRetour += " " + vectorString[i];
				detectionSays = true;
			}
			else
			{
				stringRetour += " " + vectorString[i];
				robotlabSays = false;
			}
		}
	}

	if (!stringRetour.empty())
	{
		return stringRetour;
	}
	else
	{
		return "invalide";
	}
}

//Function for wait the programm in Ms
void Voice::sleepMsec(int32 ms)
{
#if (defined(_WIN32) && !defined(GNUWINCE)) || defined(_WIN32_WCE)
	Sleep(ms);
#else
	/* ------------------- Unix ------------------ */
	struct timeval tmo;

	tmo.tv_sec = 0;
	tmo.tv_usec = ms * 1000;

	select(0, NULL, NULL, NULL, &tmo);
#endif
}


Voice::~Voice()
{
}