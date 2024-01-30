#include <iostream>
#include "mnist.h"

std::ostream& operator<<(std::ostream& out, MnistImageTable table) {
	out << "Images Table:" << std::endl;
	out << "Magic Number: " << table.magicNumber << std::endl;
	out << "Number of Images: " << table.numberOfImages << std::endl;
	out << "Number of Rows: " << table.numberOfRows << std::endl;
	out << "Number of Columns: " << table.numberOfColumns << std::endl;
	return out;
}

// Train model
void trainMnistModel() {
	///		1 - Reading datas
	MnistImageTable imagesTable("../train-images.idx3-ubyte");
	MnistLabelTable labelsTable("../train-labels.idx1-ubyte");
	std::cout << imagesTable;

	std::vector<cv::Mat> imagesData;
	std::vector<int> labelsData;

	for (auto i = 0; i < imagesTable.images.size(); i++) {
		cv::Mat tmp = cv::Mat::zeros(cv::Size(28, 28), CV_8UC1);
		int colCounter = 0, rowCounter = 0;
		for (auto j = 0; j < imagesTable.images[i].size(); j++) {

			if (rowCounter == 28) break;

			tmp.at<uint8_t>(cv::Point(colCounter++, rowCounter)) = (uint8_t)imagesTable.images[i][j];
			if (j % imagesTable.numberOfRows == 0) {
				rowCounter++;
				colCounter = 0;
			}
		}
		// Uncomment this part if you wanna see the images, it will be really slow because of 6000 images for MNIST
		//std::cout << (int)(labelsTable.items[i][0]) << std::endl;
		//cv::imshow("Image", tmp);
		//cv::waitKey(0);

		labelsData.emplace_back(labelsTable.items[i][0]);
		imagesData.emplace_back(tmp);
	}


	///		2 - MLP Init
	auto images = imagesTable.images;
	cv::Ptr<cv::ml::ANN_MLP> mlp = cv::ml::ANN_MLP::create();
	mlp->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM, 1, 1);

	int inputLayerSize = imagesData[0].total();
	int hiddenLayerSize = 100;
	int outputLayerSize = 10;

	cv::Mat layers = (cv::Mat_<int>(3, 1) << images[0].size(), hiddenLayerSize, outputLayerSize);
	mlp->setLayerSizes(layers);

	///		3 - Preparing training
	int numSamples = imagesData.size();
	cv::Mat trainingData(numSamples, inputLayerSize, CV_32F);
	cv::Mat labelData(numSamples, outputLayerSize, CV_32F);

	for (auto i = 0; i < numSamples; i++) {
		cv::Mat image = imagesData[i].reshape(1, 1);
		image.convertTo(trainingData.row(i), CV_32F);

		cv::Mat label = cv::Mat::zeros(1, outputLayerSize, CV_32F);
		label.at<float>(0, labelsData[i]) = 1.0;
		label.copyTo(labelData.row(i));
	}

	///		4 - Training
	std::cout << "Training in progress..." << std::endl;
	cv::TermCriteria termCrit(cv::TermCriteria::MAX_ITER * cv::TermCriteria::EPS, 10000, 0.001);

	mlp->setTermCriteria(termCrit);
	mlp->setTrainMethod(cv::ml::ROW_SAMPLE, 0.001, 0.1);
	mlp->train(trainingData, cv::ml::ROW_SAMPLE, labelData);
	mlp->save("trained_digit_model.xml");
}

// Test model
void testMnistModel(const std::string& modelPath, const std::string& filePath) {
	// Load model
	cv::Ptr<cv::ml::ANN_MLP> mlp = cv::ml::ANN_MLP::load(modelPath);
	if (!mlp) {
		std::cout << "Model not found..." << std::endl;
		exit(1);
	}

	// Load and flatten the image
	cv::Mat image = cv::imread(filePath, cv::IMREAD_GRAYSCALE);
	cv::resize(image, image, cv::Size(28, 28));
	cv::Mat flattenImage = image.reshape(1, 1);
	cv::Mat input;
	flattenImage.convertTo(input, CV_32F);

	// Predict
	cv::Mat output;
	mlp->predict(input, output);

	std::cout << output << std::endl;

	// Find highest class
	cv::Point classIdPoint;
	double confidence;
	cv::minMaxLoc(output, &confidence, nullptr, &classIdPoint);
	int predictedClass = classIdPoint.x;
	std::cout << "Predicted class: " << predictedClass << " with confidence " << confidence << std::endl;
}

int main()
{
	testMnistModel("trained_digit_model.xml", "3.jpg");
	return 0;
}