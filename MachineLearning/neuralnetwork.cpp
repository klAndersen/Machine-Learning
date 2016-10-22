#include "Stdafx.h"
#include "neuralnetwork.h"
#include <random>
#include <ctime>

namespace MachineLearning {

	/*
	Default constructor
	*/
	NeuralNetwork::NeuralNetwork() {
		_weightedSum = 0;
		_learningRate = 0;
		_weightMatrix = gcnew Matrix();
		_outputMatrix = gcnew Matrix();
		_inputList = gcnew List<List<double> ^>();
		_desiredOutputList = gcnew List<double>();
	} //constructor

	/*
	Constructor creating a Neural network object.
	--------------------------------------------
	@param threshold: double - The threshold for this network
	@param inputList: List<List<double> ^>^ - List containing the input
	*/
	NeuralNetwork::NeuralNetwork(List<List<double> ^>^ %inputList, List<double> ^%desiredOutputList) : NeuralNetwork() {
		if(!inputList->Count == 0 && !desiredOutputList->Count == 0) {
			_inputList = inputList;
			//start randomgenerator 
			srand(static_cast<unsigned int>(time(0)));
			setLearningRate();
			_desiredOutputList = gcnew List<double>();
			_desiredOutputList->AddRange(desiredOutputList);
			//fill weight matrix with (random) values
			_outputMatrix = gcnew Matrix(inputList->Count, 1);
			_weightMatrix = gcnew Matrix(inputList[0]->Count, 1);
			List<double>^ weightList = fillWeightList(inputList[0]->Count);
			_weightMatrix->fillMatrixWithValues(0, weightList);
			/***CHEAT/DEBUG TEST***
			_weightMatrix->changeCellValueAt(0, 0, 1);
			_weightMatrix->changeCellValueAt(1, 0, -1);
			_weightMatrix->changeCellValueAt(2, 0, 0);
			_learningRate = 0.1;
			/***END CHEAT/DEBUG TEST***/
			_weightMatrix->transposeMatrix();
		} //if
	} //constructor

	/*
	Destructor
	*/
	NeuralNetwork::~NeuralNetwork() {

	} //destructor	

	/*
	Fills the passed matrix with the values from passed list.
	--------------------------------------------
	@param matrix: Matrix - Matrix to fill with values
	@param valueList: List<List<double> ^>^ - List with values to add to matrix
	*/
	void NeuralNetwork::fillMatrixWithValues(Matrix ^%matrix, List<List<double> ^>^ %valueList) {
		int counter = 0;
		for each (List<double> ^list in valueList) {
			matrix->fillMatrixWithValues(counter, list);
			counter++;
		} //for
	} //fillMatrixWithValues

	/*
	Fills a list with random values between 0.1 - 1.0, to use as weights
	*/
	List<double>^ NeuralNetwork::fillWeightList(int listCount) {
		int counter = 0;
		double randomNo = 0;
		List<double>^ weightList = gcnew List<double>();
		while(counter < listCount) {
			randomNo = 1 + (rand() % 100);
			randomNo = randomNo / 100;
			weightList->Add(randomNo);
			counter++;
		} //while
		return weightList;
	} //fillWeightList

	/*
	train the perceptron until weights give results matching criteria for desired output
	*/
	void NeuralNetwork::trainSingularPerceptron() {
		int counter = 0;
		double desOutput = 0,
			output = 0,
			runningError = 0;
		Matrix ^inputMatrix = gcnew Matrix();
		//loop through the input values
		for each(List<double> ^list in _inputList) {
			//get values for this round
			desOutput = _desiredOutputList[counter];
			inputMatrix = gcnew Matrix(list->Count, 1);
			inputMatrix->fillMatrixWithValues(0, list);
			//get the values for the multiplied matrices
			output = inputMatrix->matrixMultiplication(_weightMatrix, inputMatrix, false)->getCellValueAt(0, 0);
			_outputMatrix->changeCellValueAt(counter, 0, output);
			//is the resulting output equal to the desired output?
			if(desOutput != checkCalculatedPerceptron(output)) {
				adjustWeights(inputMatrix, desOutput, output);
				runningError += calculateCurrentError(desOutput, output);
				//if a numerical format error occured, just set error to 1/2
				if(Double::IsNaN(runningError)) {
					runningError = 0.5;
				} //if
			} //if
			counter++;
		} //foreach
		//the training cycle is completed, but did any errors occur?
		if(runningError > 0) {
			trainSingularPerceptron();
		} //if
	} //trainPerceptron

	/*
	Based on the formula runningError = E; 
	E = E + 1/2 * (y - o)^2
	*/
	double NeuralNetwork::calculateCurrentError(double desOutput, double output) {
		double half = 0.5,
			error = 0,
			outputPow2 = 0;
		//	outputPow2 = || y - o ||^2 
		outputPow2 = desOutput - output;
		outputPow2 = pow(outputPow2, 2);
		error = (half*outputPow2);
		return error;
	} //calculateCurrentError

	/*
	Adjusts the weight matrix by use of the following formula:
	_weightMatrix = _weightMatrix + (desOutput - output) * inputMatrix; i = 1, ..., m
	*/
	void NeuralNetwork::adjustWeights(Matrix^ %inputMatrix, double desOutput, double output) {
		double value = 0,
			cellValue = 0,
			nTimesOutput = 0;
		//multiply learning rate with the desired and result output
		nTimesOutput = _learningRate * (desOutput - output);
		//create a new matrix with the same values as the passed input matrix
		Matrix ^tempMatrix = gcnew Matrix(inputMatrix->getNumRows(), inputMatrix->getNumColumns());
		//fill tempMatrix with inputMatrix values
		for(int rowNo = 0; rowNo < inputMatrix->getNumRows(); rowNo++) {
			for(int columnNo = 0; columnNo < inputMatrix->getNumColumns(); columnNo++) {
				cellValue = inputMatrix->getCellValueAt(rowNo, columnNo);
				tempMatrix->changeCellValueAt(rowNo, columnNo, cellValue);
			} //for
		} //for
		//multiply the cells in the tempmatrix with the nTimesOutput result, 
		//transpose tempMatrix to match _weightMatrix and then and update the 
		//_weightMatrix by adding this with the tempMatrix
		tempMatrix->multiplyCellsWithValue(nTimesOutput);
		tempMatrix->transposeMatrix();
		_weightMatrix = tempMatrix->matrixAddition(_weightMatrix, tempMatrix);
	} //adjustWeights

	/*
	Returns a value for the mathematical function for the calculated 
	output. 
	f = { 1 IF output > 0, -1 otherwise
	----------------------------------------------------------------
	@param output: double - the currently calculated output
	@return double:  (output > 0) ? 1 : -1;
	*/
	double NeuralNetwork::checkCalculatedPerceptron(double output) {
		return (output > 0) ? 1 : -1;
	} //calculatePerceptron

	/*
	Calculates the weighted sum based on the content in the matrix inputMatrix
	and _weightMatrix.
	*/
	void NeuralNetwork::calculateWeightedSum(Matrix ^inputMatrix) {
		Matrix ^multpliedMatrix = gcnew Matrix();
		_weightMatrix->transposeMatrix();
		//multiply the matrices to get the weighted sum
		multpliedMatrix->matrixMultiplication(inputMatrix, _weightMatrix, false);
		//loop through values to get the weighted sum
		for(int rowNo = 0; rowNo < multpliedMatrix->getNumRows(); rowNo++) {
			for(int columnNo = 0; columnNo < multpliedMatrix->getNumColumns(); columnNo++) {
				_weightedSum += multpliedMatrix->getCellValueAt(rowNo, columnNo);
			} //for
		} //for
	} //calculateWeightedSum

	void NeuralNetwork::setLearningRate() {
		double randomNo = 0;
		//set learning rate
		do {
			randomNo = 1 + (rand() % 7);
			randomNo = randomNo / 10;
			_learningRate = randomNo;
		} while(_learningRate <= 0);
	} //setLearningRate

	Matrix ^NeuralNetwork::getWeightedMatrix() {
		return _weightMatrix;
	} //getWeightedMatrix

	Matrix^ NeuralNetwork::getResultingOutputMatrix() {
		return _outputMatrix;
	} //getOutputMatrix

	double NeuralNetwork::getLearningRate() {
		return _learningRate;
	} //getLearningRate

	/*
	Returns the weighted sum
	*/
	double NeuralNetwork::getWeightedSum() {
		return _weightedSum;
	} //getWeightedSum
} //namespace