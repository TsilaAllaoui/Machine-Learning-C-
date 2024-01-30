#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

struct MnistImageTable {
	int magicNumber;
	int numberOfImages;
	int numberOfRows;
	int numberOfColumns;
	std::vector<std::vector<uint8_t>> images;
};

std::ostream& operator<<(std::ostream& out, MnistImageTable table) {
	out << "Images Table:" << std::endl;
	out << "Magic Number: " << table.magicNumber << std::endl;
	out << "Number of Images: " << table.numberOfImages << std::endl;
	out << "Number of Rows: " << table.numberOfRows << std::endl;
	out << "Number of Columns: " << table.numberOfColumns << std::endl;
	return out;
}

MnistImageTable readImages() {
	std::ifstream file("../train-images.idx3-ubyte", std::ios::binary);
	if (!file) {
		std::cout << "Can't open file..." << std::endl;
		std::exit(1);
	}

	MnistImageTable table;

	char* content = new char[4];
	file.read(content, 4);
	table.magicNumber = ((uint8_t)content[0] << 24 | (uint8_t)content[1] << 16 | (uint8_t)content[2] << 8 | (uint8_t)content[3]);

	file.read(content, 4);
	table.numberOfImages = ((uint8_t)content[0] << 24 | (uint8_t)content[1] << 16 | (uint8_t)content[2] << 8 | (uint8_t)content[3]);

	file.read(content, 4);
	table.numberOfRows = ((uint8_t)content[0] << 24 | (uint8_t)content[1] << 16 | (uint8_t)content[2] << 8 | (uint8_t)content[3]);

	file.read(content, 4);
	table.numberOfColumns = ((uint8_t)content[0] << 24 | (uint8_t)content[1] << 16 | (uint8_t)content[2] << 8 | (uint8_t)content[3]);

	for (auto i = 0; i < table.numberOfImages; i++) {
		std::vector<uchar> image(table.numberOfRows * table.numberOfColumns);
		file.read((char*)image.data(), table.numberOfRows * table.numberOfColumns);
		table.images.emplace_back(image);
	}

	file.close();
	return table;
}

int main()
{
	auto table = readImages();
	std::cout << table;

	cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
	for (auto i = 0; i < table.images.size(); i++) {
		cv::Mat tmp = cv::Mat::zeros(cv::Size(28, 28), CV_8UC1);
		int colCounter = 0, rowCounter = 0;
		for (auto j = 0; j < table.images[i].size(); j++) {
			tmp.at<uchar>(cv::Point(colCounter++, rowCounter)) = (int)table.images[i][j];
			if (j % table.numberOfRows == 0) {
				rowCounter++;
				colCounter = 0;
			}
		}
		cv::imshow("Image", tmp);
		cv::waitKey(0);
	}


	return 0;
}