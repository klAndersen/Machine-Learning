#include "Stdafx.h"
#include "artificialnn.h"

namespace MachineLearning {

	/*
	Default constructor
	*/
	ArtificialNeuralNetwork::ArtificialNeuralNetwork() : NeuralNetwork() {
		
	} //constructor

	/*
	Default constructor
	*/
	ArtificialNeuralNetwork::ArtificialNeuralNetwork(List<List<double> ^>^ %inputList, List<double> ^%desiredOutputList)
		: NeuralNetwork(inputList, desiredOutputList) {

	} //constructor

	/*
	Destructor
	*/
	ArtificialNeuralNetwork::~ArtificialNeuralNetwork() {
		
	} //destructor
} //namespace