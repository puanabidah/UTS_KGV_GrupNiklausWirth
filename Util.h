#pragma once

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

string readFile(string fileName)
{
	std::cout << "Reading file " << fileName << std::endl;
	string fileText;

	ifstream myFile(fileName);

	string temptText;

	while (getline(myFile, temptText))
	{
		std::cout << temptText << std::endl;
		fileText += temptText + "\n";
	}

	std::cout << fileText << std::endl;

	return fileText;
}

