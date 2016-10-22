#pragma once

#ifndef NEURALNETWORK_H

#define NEURALNETWORK_H	

#include "Stdafx.h"

namespace MachineLearning {
	
	/*
	Abstract class for Neural networks, to ease creation
	of different neural network types and classes.
	-----------------------------------------
	@Author: Knut Lucas Andersen
	*/
	public ref class NeuralNetwork abstract {
	public:
		//constructor
		explicit NeuralNetwork();
		//constructor creating object of a Neural Network
		explicit NeuralNetwork(List<List<double> ^>^ %inputList, List<double> ^%desiredOutputList);
		//destructor
		virtual ~NeuralNetwork();
		//train the perceptron until weights give results matching criteria for desired output
		virtual void trainSingularPerceptron();
		//return the weighted sum
		virtual double getWeightedSum();
		//return the weighted matrix after training cycle is done
		virtual Matrix^ getWeightedMatrix();
		//return the output matrix (filled after completing training perceptron)
		virtual Matrix^ getResultingOutputMatrix();
		//get the learning rate value
		virtual double getLearningRate();
		//Note: all functions need to be declared 'virtual'
		//an example of virtual function making this class abstract
		//virtual void MakeClassAbstract() = 0;
	private:
		//the sum of the weights
		double _weightedSum;
		//matrix containing all the weights
		Matrix ^_weightMatrix;
		//matrix containing the calculated output
		Matrix ^_outputMatrix;
		//learning rate for the perception
		double _learningRate;
		//list containing input values 
		List<List<double> ^>^ _inputList;
		//list containing desired output values
		List<double>^ _desiredOutputList;
	protected:
		//
		virtual void setLearningRate();
		//calculate the weighted sum
		virtual void calculateWeightedSum(Matrix ^inputMatrix);
		//calculate the perceptron
		virtual double checkCalculatedPerceptron(double output);
		//fill list of weights with random values
		virtual List<double> ^fillWeightList(int listCount);
		//calculate error for this round
		virtual double calculateCurrentError(double desOutput, double output);
		//adjust weights based on formula: _weightMatrix = _weightMatrix + (desOutput - output) * inputMatrix; i = 1, ..., m
		virtual void adjustWeights(Matrix^ %inputMatrix, double desOutput, double output);
		//fill matrix with values
		virtual void fillMatrixWithValues(Matrix ^%matrix, List<List<double> ^>^ %valueList);
	}; //NeuralNetwork
} //namespace
#endif