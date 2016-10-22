#include "Stdafx.h"
#include "fileoperations.h"
#include <fstream>
#include <sstream>
using namespace std;

namespace MachineLearning {

	/*
	Default constructor
	*/
	FileOperations::FileOperations() {

	} //constructor

	/*
	Destructor
	*/
	FileOperations::~FileOperations() {

	} //destructor

	/*
	Reads all the content from the file and adds each line into a list.
	----------------------------------------------------
	@param fileName: string - name of file to be read from
	@throw: ExceptionClass - throws error if f
	@return List<String>: Pointer to content of the file
	*/
	List<String^> ^ FileOperations::readStringFromFile(String^ fileName) {
		List<String^> ^ registeredData = gcnew List<String ^>();
		try {
			if(isFileReadableAndNotEmpty(fileName)) {
				string content = "",
					fName = "";
				ifstream openFile;
				Conversion::MarshalString(fileName, fName);
				//no need to check for file error, since if we get here we know the file 
				//was successfully opened and probably has content		
				openFile.open(fName.c_str());
				//read content from file
				while(!openFile.eof()) {
					getline(openFile, content, '\n');
					registeredData->Add(gcnew String(content.c_str()));
				} //while
				openFile.close();
			} //if
		} catch(ExceptionClass^) {
			throw;
		} //try/catch
		return registeredData;
	} //readStringFromFile

	/*
	Reads cluster vaulues from file. The number of attributes assigned to
	each sample is based on the passed noAttributes. If the file cannot be
	opened or if the file is empty an error is thrown.
	----------------------------------------------------
	@param fileName: string - name of file to be read from
	@param noAttributes: int - number of attributes belonging to each sample
	@throw: ExceptionClass - throws error if file cannot be opened or if file is empty
	@return: List<List<double>>: Pointer to content of the file
	*/
	void FileOperations::readInfiniteClusterAttributesFromFile(String^ fileName, List<List<double>^>^ %registeredData, const int noAttributes) {
		List<double>^ sampleAttributes = gcnew List<double>();
		try {
			if(isFileReadableAndNotEmpty(fileName)) {
				string content = "",
					fName = "";
				Conversion::MarshalString(fileName, fName);
				ifstream openFile;
				double value = 0;
				int counter = 0;
				//no need to check for file error, since if we get here we know the file 
				//was successfully opened and probably has content		
				openFile.open(fName.c_str());
				//read content from file
				while(!openFile.eof()) {
					//read all the space seperated values
					getline(openFile, content, ' ');
					value = Conversion::convertStringToDouble(gcnew String(content.c_str()));
					sampleAttributes->Add(value);
					counter++;
					//is this the last element?
					if(counter == (noAttributes - 1)) {
						//get value before newline and add it to list
						getline(openFile, content, '\n');
						value = Conversion::convertStringToDouble(gcnew String(content.c_str()));
						sampleAttributes->Add(value);
						//add this samples attributes to the list
						registeredData->Add(sampleAttributes);
						//empty the sample list of previous content
						//and reset the counter
						sampleAttributes->TrimExcess();
						sampleAttributes = gcnew List<double>();
						counter = 0;
					} //if
				} //while
				openFile.close();
				registeredData->TrimExcess();
				//were there any data in the file?
				if(registeredData->Count > 0) {
					//get size of list
					unsigned int listSize = 0;
					listSize = registeredData->Count - 1;
					List<double> ^tempList = gcnew List<double>();
					tempList = registeredData[listSize];
					unsigned int lastAttributeSize = 0;
					lastAttributeSize = tempList->Count - 1;
					//check if the last element is -9999 (failed conversion due to e.g. newline)
					if(tempList[lastAttributeSize] == Conversion::INPUT_NOT_A_NUMBER) {
						//last element is a failed conversion, remove it
						registeredData->RemoveAt(listSize);
					} //if
					delete tempList;
				} //if
			} //if
			delete sampleAttributes;
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
	} //readInfiniteClusterAttributesFromFile

	/*
	Reads all the content from the file and adds the file content to a list.
	If the file doesn't exist, it will be created. If the file couldn't be opened
	or the file is empty, an error is thrown. Depending on the value passed in
	fileToReadFrom (use constants from class; else error is thrown).
	----------------------------------------------------
	@param fileName: String^ - name of file to be read from
	@param fileToReadFrom: int - Select one of the constants belonging to this class
	@param isTestData: bool - is it testdata that is going to be read from file?
	@param noAttributes - int: The number of attributes (set to zero if reading from space-seperated file).
	@throw: ExceptionClass - throws error if file cannot be opened or if file is empty
	@return: List<Training>: Pointer to content of the file
	*/
	List<Training^> ^FileOperations::readTrainingContentFromFile(String^ fileName, const int fileToReadFrom, const bool isTestData, int noAttributes) {
		List<Training^> ^registeredData = gcnew List<Training ^>();
		try {
			//check if the file is empty, if it isn't read selected content from file
			if(isFileReadableAndNotEmpty(fileName)) {
				switch(fileToReadFrom) {
					case READ_SPACE_SEPERATED_TRAINING_FROM_FILE:
						registeredData = readSpaceSeperatedContentFromFile(fileName, isTestData, registeredData);
						break;
					case READ_CSV_TRAINING_FROM_FILE:
						registeredData = readCSVContentFromFile(fileName, registeredData, noAttributes);
						break;
					default:
						throw gcnew ExceptionClass("No valid option selected for file reading!");
						break;
				} //switch
			} //if
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
		return registeredData;
	} //readTrainingContentFromFile

	/*
	Reads from files the two attribute values seperated by blanks (space) and
	then the classID, adding these into the list, which then is returned.
	When reading from file, it's checked whether it's reading training or
	test data (the difference being test data doesn't have class labels).
	----------------------------------------------------
	@param fileName: string - name of file to be read from
	@param testData: bool - is the file data to be read test samples?
	@param registeredData: List<Training> - the list to fill with content
	@return: List<Training>: Pointer to content of the file
	*/
	List<Training^> ^FileOperations::readSpaceSeperatedContentFromFile(String^ fileName, bool testData, List<Training^> ^registeredData) {
		ifstream openFile;
		string fName = "";
		Conversion::MarshalString(fileName, fName);
		//no need to check for file error, since if we get here we know the file 
		//was successfully opened and probably has content
		openFile.open(fName.c_str());
		double attr1 = -1,
			attr2 = -1;
		int classID = -1;
		string content = "";
		while(!openFile.eof()) {
			//get the first attribute
			getline(openFile, content, ' ');
			attr1 = Conversion::convertStringToDouble(gcnew String(content.c_str()));
			//is it testdata or Training-samples being read?
			if(testData) {
				getline(openFile, content, '\n');
				attr2 = Conversion::convertStringToDouble(gcnew String(content.c_str()));
				//add the value to the list
				registeredData->Add(gcnew Training(attr1, attr2));
			} else {
				//read the 2. and 3. attribute
				getline(openFile, content, ' ');
				attr2 = Conversion::convertStringToDouble(gcnew String(content.c_str()));
				getline(openFile, content, '\n');
				classID = Conversion::convertStringToInteger(gcnew String(content.c_str()));
				//add the value to the list
				registeredData->Add(gcnew Training(attr1, attr2, classID));
			} //if
		} //while
		openFile.close();
		//were there any data in the file?
		removeLastEntryIfNewline(registeredData);
		return registeredData;
	} //readSpaceSeperatedContentFromFile

	/*
	Reads multiple attribute values (no. of attributevalues > 2) and
	adds these values into a list together with the classID.
	The function is based on the following sequence;
	classID, attribute1, ..., attributeN. A loop is used to retrieve all
	the values (by using noAttributes as a limit for iteration) that are added
	to a list. This list and the classID is then passed to a constructor
	for the Training class, and finally the Training object is added to the list
	registeredData which is returned.
	----------------------------------------------------
	@param fileName: string - name of file to be read from
	@param registeredData: List<Training> - the list to fill with content
	@param noAttributes: int - the total number of attributes to read from file
	@return: List<Training>: Pointer to content of the file
	*/
	List<Training^> ^FileOperations::readCSVContentFromFile(String^ fileName, List<Training^> ^registeredData, const int noAttributes) {
		ifstream openFile;
		string fName = "";
		Conversion::MarshalString(fileName, fName);
		//no need to check for file error, since if we get here we know the file 
		//was successfully opened and probably has content
		openFile.open(fName.c_str());
		double attribute = -1;
		int classID = -1,
			counter = 1;
		string content = "";
		//container for current class attribute values
		List<double>^ attributeValues = gcnew List<double>();
		bool newClass = true;
		//get the classID for the first class
		getline(openFile, content, ',');
		classID = Conversion::convertStringToInteger(gcnew String(content.c_str()));
		while(!openFile.eof()) {
			//is attribute value to be read?
			if(counter <= noAttributes) {
				//get the attribute, convert it to double and add it to list
				getline(openFile, content, ',');
				attribute = Conversion::convertStringToDouble(gcnew String(content.c_str()));
				attributeValues->Add(attribute);
				counter++;
				//is this the last element?
				if(counter == noAttributes) {
					//read until newline
					getline(openFile, content, '\n');
					attribute = Conversion::convertStringToDouble(gcnew String(content.c_str()));
					attributeValues->Add(attribute);
					counter++;
				} //if
				if(counter > noAttributes) {
					//add the class and values into the list
					registeredData->Add(gcnew Training(attributeValues, classID));
					//empty the list of previos content
					attributeValues = gcnew List<double>();
					//reset counter
					counter = 1;
					//get the next classID
					getline(openFile, content, ',');
					classID = Conversion::convertStringToInteger(gcnew String(content.c_str()));
				} //if
			} //if
		} //while
		openFile.close();
		//were there any data in the file?
		removeLastEntryIfNewline(registeredData);
		return registeredData;
	} //readCSVContentFromFile	

	/*
	{Not working properly}
	Reads the content from a textfile and seperates each string value based
	on the passed symbol until it reaches newline. If you want all string
	values from each line, use readStringFromFile(String^) instead.
	----------------------------------------------------
	@param fileName: string - name of file to be read from
	@param registeredData: List<String> - the list to fill with content
	@param symbol: wchar_t - the symbol seperating the text
	@return: List<String> - Pointer to content of the file
	@see readStringFromFile(String ^fileName)
	*/
	List<List<String ^> ^> ^FileOperations::readSymbolSeperatedStringFromFile(String^ fileName, wchar_t symbol) {
		List<String ^> ^oneLineContent = gcnew List<String ^>();
		List<List<String ^> ^> ^registeredData = gcnew List<List<String ^> ^>();
		try {
			if(isFileReadableAndNotEmpty(fileName)) {
				ifstream openFile;
				string fName = "";
				string content = "";
				Conversion::MarshalString(fileName, fName);
				//no need to check for file error, since if we get here we know the file 
				//was successfully opened and probably has content
				openFile.open(fName.c_str());
				char seperator = static_cast<char>(symbol);
				while(!openFile.eof()) {
					//clear previous content
					oneLineContent = gcnew List<String ^>();
					getline(openFile, content);
					istringstream iStringStream(content);
					while(iStringStream) {
						getline(iStringStream, content, seperator);
						oneLineContent->Add(gcnew String(content.c_str()));
					} //while
					//for some reason, the last element gets added twice in 
					//istringstream, so remove it
					oneLineContent->RemoveAt(oneLineContent->Count - 1);
					registeredData->Add(oneLineContent);
				} //while
				openFile.close();
				//were there any data in the file?
				removeLastEntryIfNewline(registeredData);
			} //if
		} catch(MachineLearning::ExceptionClass^) {
			throw;
		} //try/catch
		return registeredData;
	} //readSymbolSeperatedStringFromFile

	/*

	--------------------------------------------------
	@param fileName: String^ - name of file to write to
	@param fileContent: List<String ^>^ - List containing content to write to file
	*/
	void FileOperations::writeStringToFile(String ^fileName, List<String ^>^ contentList) {
		ofstream outFile;
		string fName = "",
			cString = "",
			fileContent = "";
		Conversion::MarshalString(fileName, fName);
		//loop through content and add newline at the end
		for(int i = 0; i < contentList->Count; i++) {
			Conversion::MarshalString(contentList[i], cString);
			fileContent += cString + "\n";
		} //for
		//open, write and close the file
		outFile.open(fName.c_str());
		outFile << fileContent;
		outFile.close();
	} //writeStringToFile

	/*
	Check if the last element in the list is a newline/not a valid number
	(based on the constant Conversion::INPUT_NOT_A_NUMBER).
	----------------------------------------------------
	@param registeredData: List<Training> - list filled with content from file
	*/
	void FileOperations::removeLastEntryIfNewline(List<Training^> ^registeredData) {
		//were there any data in the file?
		if(registeredData->Count > 0) {
			//get size of list
			unsigned int listSize = registeredData->Count - 1;
			//check if the last element is -9999 (failed conversion due to e.g. newline)
			if(registeredData[listSize]->getAttributeValue1() == Conversion::INPUT_NOT_A_NUMBER) {
				//last element is a failed conversion, remove it
				registeredData->RemoveAt(listSize);
			} //if
		} //if
	} //removeLastEntryIfNewline

	/*
	Check if the last element in the list is newline.
	----------------------------------------------------
	@param registeredData: List<String> - list filled with content from file
	*/
	void FileOperations::removeLastEntryIfNewline(List<List<String ^> ^> ^registeredData) {
		//were there any data in the file?
		if(registeredData->Count > 0) {
			//get size of list
			unsigned int listSize = registeredData->Count - 1;
			unsigned int lastListSize = registeredData[listSize]->Count - 1;
			if(registeredData[listSize]->ToArray()[lastListSize] == "\n") {
				//last element is a failed ConvertString, remove it
				registeredData->RemoveAt(listSize);
			} //if
		} //if
	} //removeLastEntryIfNewline

	/*
	Check if the file can be opened and if the file is empty.
	If file cannot be opened or if the file is empty, an ExceptionClass error
	is thrown.
	----------------------------------------------------
	@param fileName: String^ - name of file to be read from
	@throw: ExceptionClass - throws error if file cannot be opened or is empty
	@return: bool - returns true if file is accessible and not empty
	*/
	bool FileOperations::isFileReadableAndNotEmpty(String^ fileName) {
		string fName = "";
		ifstream openFile;
		Conversion::MarshalString(fileName, fName);
		//create file if file doesnt exist
		createFileIfFileNotExists(fName);
		openFile.open(fName.c_str());
		//was the file opened?
		if(openFile.fail()) {
			throw gcnew ExceptionClass("Couldn't open file: " + fileName + "!");
		} else { //file was opened
			int counter = numLinesInTextfile(openFile);
			//does the file have any content?
			if(counter == 0) {
				throw gcnew ExceptionClass("File \"" + fileName + "\" is empty.");
			} //if
		} //if
		return true;
	} //isFileReadableAndNotEmpty

	/*
	Create file if file doesn"t exist.
	To avoid deletion of existing data, ios:app is used.
	--------------------------------------------------
	@param fileName: std::string - name of file to be created
	*/
	void FileOperations::createFileIfFileNotExists(const string fileName) {
		ofstream createFile(fileName, ios::app);
		createFile.close();
	} //createFileIfNotExists

	/*
	Loops through the content of the file and counts up the number of lines.
	The only task of this function is to check if file has content.
	------------------------------------------------------------------------------
	@param openFile: reference to object of ifstream; object used to open file in question
	@return: integer (number of lines in file, if empty, 0 is returned)
	*/
	int FileOperations::numLinesInTextfile(ifstream &openFile) {
		string fileContent = "";
		//initialize a counter that counts the number of lines in the textfile
		//starts at -1 so that the loop is run at least one time and that the counter 
		//properly counts number of lines to check if file is empty
		int counter = -1;
		//loop through the file to see if it has content
		while(!openFile.eof()) {
			//read line by line, seperated by newline
			getline(openFile, fileContent, '\n');
			//increase counter for each line
			counter++;
		} //while
		//close file and return the result
		openFile.close();
		return counter;
	} //numLinesInTextfile
} //namespace