#pragma once

#ifndef FILEOPERATIONS_H

#define FILEOPERATIONS_H	

#include "Stdafx.h"
#include "training.h"

namespace MachineLearning {
	
	/*
	Class reading and writing (not implemented) content from file.
	It also creates file if file doesn't exist and checks if file
	has content when reading.
	-----------------------------------------
	@Author: Knut Lucas Andersen
	*/
	public ref class FileOperations sealed {
	public:
		//constructor
		FileOperations();
		//destructor for cleanup
		~FileOperations();
		//read values seperated by blanks (space) (for Training class)
		static const int READ_SPACE_SEPERATED_TRAINING_FROM_FILE = 0;
		//read values seperated by comma (for the Training class)
		static const int READ_CSV_TRAINING_FROM_FILE = 1;
		//read string content from file
		List<String ^>^ readStringFromFile(String ^fileName);
		//writes the content in the passed list to file
		void writeStringToFile(String ^fileName, List<String ^>^ contentList);
		//read symbol-seperated values from file
		List<List<String ^> ^> ^readSymbolSeperatedStringFromFile(String ^fileName, wchar_t symbol);
		//read Training data content from file
		List<Training^> ^readTrainingContentFromFile(String ^fileName, const int fileToReadFrom, const bool isTestData, int noAttributes);
		//read cluster attributes from file (the number of attributes read is based on the passed noAttributes)
		void readInfiniteClusterAttributesFromFile(String ^fileName, List<List<double>^>^ %registeredData, const int noAttributes);
	private:
		//count number of lines in textfile to check if file is empty
		int numLinesInTextfile(std::ifstream &openFile);
		//check if the file is able to be opened and that it isn't empty
		bool isFileReadableAndNotEmpty(String ^ fileName);
		//create the file if it doesn't exist
		void createFileIfFileNotExists(const std::string filename);
		//remove newline if that's the last element in the TrainingClass List
		void removeLastEntryIfNewline(List<Training^> ^registeredData);
		//remove newline if that's the last element in the String List
		void removeLastEntryIfNewline(List<List<String ^> ^> ^registeredData);
		//read content from file that is seperated by comma (csv-format)
		List<Training^> ^readCSVContentFromFile(String ^fileName, List<Training^> ^registeredData, const int noAttributes);
		//read content from file with Training data seperated by blanks
		List<Training^> ^readSpaceSeperatedContentFromFile(String ^fileName, const bool testData, List<Training^> ^registeredData);
	}; //FileOperations
} //namespace
#endif