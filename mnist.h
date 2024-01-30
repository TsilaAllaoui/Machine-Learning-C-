#include <iostream>
#include <vector>
#include <fstream>
#include <opencv2/opencv.hpp>

struct MnistImageTable {
	int magicNumber = 0;
	int numberOfImages = 0;
	int numberOfRows = 0;
	int numberOfColumns = 0;
	std::vector<std::vector<uint8_t>> images;

	MnistImageTable(const std::string& filePath) {
		std::ifstream file(filePath, std::ios::binary);
		if (!file) {
			std::cout << "Can't open file..." << std::endl;
			std::exit(1);
		}

		char* content = new char[4];
		file.read(content, 4);
		magicNumber = ((uint8_t)content[0] << 24 | (uint8_t)content[1] << 16 | (uint8_t)content[2] << 8 | (uint8_t)content[3]);

		file.read(content, 4);
		numberOfImages = ((uint8_t)content[0] << 24 | (uint8_t)content[1] << 16 | (uint8_t)content[2] << 8 | (uint8_t)content[3]);

		file.read(content, 4);
		numberOfRows = ((uint8_t)content[0] << 24 | (uint8_t)content[1] << 16 | (uint8_t)content[2] << 8 | (uint8_t)content[3]);

		file.read(content, 4);
		numberOfColumns = ((uint8_t)content[0] << 24 | (uint8_t)content[1] << 16 | (uint8_t)content[2] << 8 | (uint8_t)content[3]);

		for (auto i = 0; i < numberOfImages; i++) {
			std::vector<uchar> image(numberOfRows * numberOfColumns);
			file.read((char*)image.data(), numberOfRows * numberOfColumns);
			images.emplace_back(image);
		}

		file.close();
	}
};

struct MnistLabelTable {
	int magicNumber = 0;
	int numberOfItems = 0;
	std::vector<std::vector<uint8_t>> items;

	MnistLabelTable(const std::string& filePath) {
		std::ifstream file(filePath, std::ios::binary);
		if (!file) {
			std::cout << "Can't open file..." << std::endl;
			std::exit(1);
		}

		char* content = new char[4];
		file.read(content, 4);
		magicNumber = ((uint8_t)content[0] << 24 | (uint8_t)content[1] << 16 | (uint8_t)content[2] << 8 | (uint8_t)content[3]);

		file.read(content, 4);
		numberOfItems = ((uint8_t)content[0] << 24 | (uint8_t)content[1] << 16 | (uint8_t)content[2] << 8 | (uint8_t)content[3]);

		for (auto i = 0; i < numberOfItems; i++) {
			std::vector<uint8_t> image(1);
			file.read((char*)image.data(), 1);
			items.emplace_back(image);
		}

		file.close();
	}
};