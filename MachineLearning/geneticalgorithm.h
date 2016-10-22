#pragma once

#ifndef GENETIC_ALGORITHM_H

#define GENETIC_ALGORITHM_H

#include "Stdafx.h"

namespace MachineLearning {

	/*
	Class that creates objects to be used when attempting
	to create and retrieve the results from the genetic
	algorithm
	-----------------------------------------
	@Author: Knut Lucas Andersen
	*/
	public ref class GeneticAlgorithm {
	public:
		//default value used under initialization
		static signed const int IS_DEFAULT_VALUE = -9999;
		//default constructor
		GeneticAlgorithm();
		//destructor 
		~GeneticAlgorithm();
		//get the current generation
		int getGeneration();
		//get the fitness sum for this generation
		double getSumFitness();
		//get the average for this generation
		double getAvgFit();
		//get this generations binary values
		List<String^>^ getBinaryValues();
		//get this generations converted from binary values
		List<double>^ getFitnessValues();
		//get this generations probability percentage per value
		List<double>^ getProbabilityPercentage();
		//get this generations selected parents
		List<String^>^ getSelectedParents();
		//return the calculated probability for each value
		List<double>^ returnCalculatedProbability(List<double> ^%fitnessValues, double sumFitness);
		//initialize the first generation of the genetic algorithm with integer or double
		GeneticAlgorithm ^initializeGA(int populationSize, bool useInteger, int decimalLength, double lowest, double highest);
		//calculate fitness values based on integer values
		List<double>^ returnCalculatedFitness(List<String^> ^%binaryValues, double &sumFitness);
		//calculate fitness values based on double values
		List<double>^ returnCalculatedFitness(List<String^> ^%binaryValues, double &sumFitness, int decimalLength);
		//create the children belonging to this generation
		GeneticAlgorithm ^createNewGeneration(double crossover, double mutation, int populationSize);
	private:
		//the average fit for this generation
		double _avgFit;
		//value identifying this generation
		int _generation;
		//value telling if the values used are integer or double
		bool _useInteger;
		//the fitness sum for this generation
		double _sumFitness;
		//the decimal length (number of decimals)
		int _decimalLength;
		//list containing the selected parents
		List<String^>^ _selectedParents;
		//list containing the fitness values
		List<double>^ _fitnessValues;
		//list containing the binary values
		List<String^>^ _binaryValues;
		//list containing the probability percentage
		List<double>^ _probPercentage;
		//default value for first generation
		static const int FIRST_GENERATION = 1;
		//set the selected parents ffrom this generation
		void setSelectedParents(List<String^>^ selectedParents);
		//set the probability percentage for this generations values
		void setProbabilityPercentage(List<double>^ probPercentage);
		//compute crossover (if it happens)
		String^ computeCrossover(List<String^> ^%selectedParents, int length, int noElements);
		//mutate the child (if it happens)
		String^ mutateChild(String^ newChild, int crossover, int length, double mutationChance, double mutation);
		//selects parents for the next generation
		List<String^>^ selectParents(int populationSize);
		//select random values for start/first generation (integer)
		List<String^>^ selectRandomValues(int populationSize, int min, int max);
		//select random values for start/first generation (double)
		List<String^>^ selectRandomValues(int populationSize, double min, double max, int decimalLength);
		//check if range is from negative to negative, negative to positive, or just positive
		void checkIfSelectedRangeIsNegative(double &min, double &max);
		//constructor for usage when creating new generations
		GeneticAlgorithm(int generation, double avgFit, double sumFitness, List<String^>^ binaryValues,
			List<double>^ convertedValues, List<double>^ probPercentage);
		//returns a boolean value for if the values used in the genetic algorithmn 
		//is integer or double
		bool getUsingIntAsNumeric();
		//sets a boolean value for if the values used in the genetic algorithmn 
		//is integer or double
		void setUsingIntAsNumeric(bool useInteger);
		//returns the decimal lenght (the number of decimals)
		int getDecimalLength();
		//set the decimal length (the number of decimals
		void setDecimalLength(int decimalLength);
	}; //GeneticAlgorithm
} //namespace
#endif