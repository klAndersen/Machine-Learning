#pragma once

#ifndef ARTIFICIALNN_H

#define ARTIFICIALNN_H	

#include "Stdafx.h"
#include "neuralnetwork.h"

namespace MachineLearning {
	
	/*
	Artificial neural network class inheriting the setup 
	from the NeuralNetwork class.
	-----------------------------------------
	@Author: Knut Lucas Andersen
	@see: NeuralNetwork
	*/
	public ref class ArtificialNeuralNetwork sealed : public NeuralNetwork {
	public:
		//constructor
		ArtificialNeuralNetwork();
		//constructor initalizing objects
		ArtificialNeuralNetwork(List<List<double> ^>^ %inputList, List<double> ^%desiredOutputList);
		//destructor
		~ArtificialNeuralNetwork();
	private:

	}; //ArtificialNeuralNetwork
} //namespace
#endif