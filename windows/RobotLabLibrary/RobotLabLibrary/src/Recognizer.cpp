#include "../include/Recognizer.h"


Recognizer::Recognizer()
{
}


void Recognizer::recognize()
{
	// Get the path to your CSV.
	std::string fn_csv = CSV_FACE_RECO;
	// These vectors hold the images and corresponding labels.
	std::vector<cv::Mat> images;
	std::vector<int> labels;
	std::map<int, std::string> labelsInfo;
	// Read in the data. This can fail if no valid
	// input filename is given.
	try {
		read_csv(fn_csv, images, labels, labelsInfo);
	}
	catch (cv::Exception& e) {
		std::cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << std::endl;
		// nothing more we can do
		getchar();
		exit(1);
	}

	// Quit if there are not enough images for this demo.
	if (images.size() <= 1) {
		std::string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		getchar();
		CV_Error(cv::Error::StsError, error_message);
	}
	// The following lines simply get the last images from
	// your dataset and remove it from the vector. This is
	// done, so that the training data (which we learn the
	// cv::FaceRecognizer on) and the test data we test
	// the model with, do not overlap.
	cv::Mat testSample = images[images.size() - 1];
	int nlabels = (int)labels.size();
	int testLabel = labels[nlabels - 1];
	images.pop_back();
	labels.pop_back();
	// The following lines create an Eigenfaces model for
	// face recognition and train it with the images and
	// labels read from the given CSV file.
	// This here is a full PCA, if you just want to keep
	// 10 principal components (read Eigenfaces), then call
	// the factory method like this:
	//
	//      cv::createEigenFaceRecognizer(10);
	//
	// If you want to create a FaceRecognizer with a
	// confidennce threshold, call it with:
	//
	//      cv::createEigenFaceRecognizer(10, 123.0);
	//
	cv::Ptr<cv::face::BasicFaceRecognizer> model = cv::face::createEigenFaceRecognizer();
	for (int i = 0; i < nlabels; i++)
		model->setLabelInfo(i, labelsInfo[i]);
	model->train(images, labels);
	std::string saveModelPath = "face-rec-model.txt";
	std::cout << "Saving the trained model to " << saveModelPath << std::endl;
	model->save(saveModelPath);

	// The following line predicts the label of a given
	// test image:
	int predictedLabel = model->predict(testSample);
	//
	// To get the confidence of a prediction call the model with:
	//
	//      int predictedLabel = -1;
	//      double confidence = 0.0;
	//      model->predict(testSample, predictedLabel, confidence);
	//
	std::string result_message = cv::format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
	std::cout << result_message << std::endl;
	if ((predictedLabel == testLabel) && !model->getLabelInfo(predictedLabel).empty())
		std::cout << cv::format("%d-th label's info: %s", predictedLabel, model->getLabelInfo(predictedLabel).c_str()) << std::endl;

	// advanced stuff
	if (5>2) {
		// Sometimes you'll need to get/set internal model data,
		// which isn't exposed by the public cv::FaceRecognizer.
		// Since each cv::FaceRecognizer is derived from a
		// cv::Algorithm, you can query the data.
		//
		// First we'll use it to set the threshold of the FaceRecognizer
		// to 0.0 without retraining the model. This can be useful if
		// you are evaluating the model:
		//
		model->setThreshold(0.0);
		// Now the threshold of this model is set to 0.0. A prediction
		// now returns -1, as it's impossible to have a distance below
		// it
		predictedLabel = model->predict(testSample);
		std::cout << "Predicted class = " << predictedLabel << std::endl;
		// Here is how to get the eigenvalues of this Eigenfaces model:
		cv::Mat eigenvalues = model->getEigenValues();
		// And we can do the same to display the Eigenvectors (read Eigenfaces):
		cv::Mat W = model->getEigenVectors();
		// From this we will display the (at most) first 10 Eigenfaces:
		for (int i = 0; i < cv::min(10, W.cols); i++) {
			std::string msg = cv::format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
			std::cout << msg << std::endl;
			// get eigenvector #i
			cv::Mat ev = W.col(i).clone();
			// Reshape to original size & normalize to [0...255] for imshow.
			cv::Mat grayscale;
			normalize(ev.reshape(1), grayscale, 0, 255, cv::NORM_MINMAX, CV_8UC1);
			// Show the image & apply a Jet colormap for better sensing.
			cv::Mat cgrayscale;
			applyColorMap(grayscale, cgrayscale, cv::COLORMAP_JET);
			imshow(cv::format("%d", i), cgrayscale);
		}
		cv::waitKey(1);
	}
}

void Recognizer::read_csv(const std::string& filename, std::vector<cv::Mat>& images, std::vector<int>& labels, std::map<int, std::string>& labelsInfo, char separator)
{
	std::ifstream csv(filename.c_str());
	if (!csv) CV_Error(cv::Error::StsBadArg, "No valid input file was given, please check the given filename.");
	std::string line, path, classlabel, info;
	while (getline(csv, line)) {
		std::stringstream liness(line);
		path.clear(); classlabel.clear(); info.clear();
		getline(liness, path, separator);
		getline(liness, classlabel, separator);
		getline(liness, info, separator);
		if (!path.empty() && !classlabel.empty()) {
			std::cout << "Processing " << path << std::endl;
			int label = atoi(classlabel.c_str());
			if (!info.empty())
				labelsInfo.insert(std::make_pair(label, info));
			// 'path' can be file, dir or wildcard path
			cv::String root(path.c_str());
			std::vector<cv::String> files;
			cv::glob(root, files, true);
			for (std::vector<cv::String>::const_iterator f = files.begin(); f != files.end(); ++f) {
				std::cout << "\t" << *f << std::endl;
				cv::Mat img = imread(*f, cv::IMREAD_GRAYSCALE);
				static int w = -1, h = -1;
				static bool showSmallSizeWarning = true;
				if (w>0 && h>0 && (w != img.cols || h != img.rows)) std::cout << "\t* Warning: images should be of the same size!" << std::endl;
				if (showSmallSizeWarning && (img.cols<50 || img.rows<50)) {
					std::cout << "* Warning: for better results images should be not smaller than 50x50!" << std::endl;
					showSmallSizeWarning = false;
				}
				images.push_back(img);
				labels.push_back(label);
			}
		}
	}
}


/*cv::Mat Recognizer::processImage(cv::Mat& image)
{
	cv::Mat resized;
	cv::Size originalSize = image.size();

	if (_keepAspectRatio)
	{
		float ratio = static_cast<float>(_goalSize.height) / originalSize.height;
		cv::Size newSize((int)(originalSize.width * ratio), (int)(originalSize.height * ratio));

		//fix possible rounding error by float
		if (newSize.height != _goalSize.height) newSize.height = _goalSize.height;

		cv::resize(image, resized, newSize);

		if (resized.size().width != _goalSize.width)
		{
			if (_keepAspectRatio)
			{
				int delta = _goalSize.width - resized.size().width;

				if (delta < 0)
				{
					cv::Rect clipRect(std::abs(delta) / 2, 0, _goalSize.width, resized.size().height);
					resized = resized(clipRect);
				}
				else if (delta > 0)
				{
					//width needs to be widened, create bigger mat, get region of 
					//interest at the center that matches the size of the resized   
					//image, and copy the resized image into that ROI

					cv::Mat widened(_goalSize, resized.type());
					cv::Rect widenRect(delta / 2, 0, _goalSize.width, _goalSize.height);
					cv::Mat widenedCenter = widened(widenRect);
					resized.copyTo(widenedCenter);
					resized = widened; //we return resized, so set widened to resized
				}
			}
		}
	}
	else
		cv::resize(image, resized, _goalSize);

	cv::Mat grayFrame;
	cv::cvtColor(resized, grayFrame, CV_BGR2GRAY);

	return grayFrame;
}

bool Recognizer::processAndSaveImage(cv::Mat& image, const std::string& name)
{
	cv::Mat processed = processImage(image);

	return cv::imwrite(_directoryPath + name, processed);
}*/

Recognizer::~Recognizer()
{
}
