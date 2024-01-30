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

int main()
{
	MnistImageTable imagesTable("../train-images.idx3-ubyte");
	MnistLabelTable labelsTable("../train-labels.idx1-ubyte");
	std::cout << imagesTable;

	cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
	for (auto i = 0; i < imagesTable.images.size(); i++) {
		cv::Mat tmp = cv::Mat::zeros(cv::Size(28, 28), CV_8UC1);
		int colCounter = 0, rowCounter = 0;
		for (auto j = 0; j < imagesTable.images[i].size(); j++) {
			tmp.at<uchar>(cv::Point(colCounter++, rowCounter)) = (int)imagesTable.images[i][j];
			if (j % imagesTable.numberOfRows == 0) {
				rowCounter++;
				colCounter = 0;
			}
		}
		std::cout << (int)(labelsTable.items[i][0]) << std::endl;
		cv::imshow("Image", tmp);
		cv::waitKey(0);
	}

	return 0;
}