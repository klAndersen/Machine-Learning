#include "Stdafx.h"
#include "geneticalgorithm.h"

namespace MachineLearning {

	/*
	Default constructor
	*/
	GeneticAlgorithm::GeneticAlgorithm() {
		//default initialization
		_generation = 0;
		_avgFit = 0;
		_sumFitness = 0;
		_selectedParents = gcnew List<String ^>();
		_binaryValues = gcnew List<String ^>();
		_fitnessValues = gcnew List<double>();
		_probPercentage = gcnew List<double>();
		setUsingIntAsNumeric(true);
		setDecimalLength(0);
	} //constructor

	/*
	Destructor
	*/
	GeneticAlgorithm::~GeneticAlgorithm() {

	} //destructor

	/*
	Constructor initializing the members of the class.
	---------------------------------------------------------
	@param generation - int: The generation for this genetic object
	@param avgFit - double: The average fit
	@param sumFitness - double: The fitness sum
	@param binaryValues - List<String>: List of the binary values
	@param fitnessValues - List<double>: List of the fitness values
	@param probPercentage - List<double>: List of the probability percentage
	*/
	GeneticAlgorithm::GeneticAlgorithm(int generation, double avgFit, double sumFitness,
		List<String ^>^ binaryValues, List<double>^ fitnessValues, List<double>^ probPercentage) {
		_generation = generation;
		_avgFit = avgFit;
		_sumFitness = sumFitness;
		_fitnessValues = gcnew List<double>();
		_binaryValues = gcnew List<String ^>();
		//fill with new content
		_fitnessValues->AddRange(fitnessValues);
		_binaryValues->AddRange(binaryValues);
		setProbabilityPercentage(probPercentage);
	} //constructor

	/*
	Initializes the genetic algorithm with a generation of either integer or
	double values. Fills up the generation based on population size,
	followed by calculating the fitness sum, converting the values from binary
	and selecting parents for breeding the next generation.
	--------------------------------------------------------
	@param populationSize: int - size of population
	@param useInteger: bool - shall the values to be used be based on integer or double?
	@param decimallength: int - only needed if value is double, else just pass zero (0)
	if value is double, decimal lenght is the number of decimals, including the seperator ('.')
	@param lowest: double - the lowest start value for the initial population
	@param highest: double - the higest start value for the initial population
	@return GeneticAlgorithm: returns an object containing the first initialized generation
	*/
	GeneticAlgorithm ^GeneticAlgorithm::initializeGA(int populationSize, bool useInteger, int decimalLength, double lowest, double highest) {
		//Lists for keeping the values
		List<String ^>^ binaryValues = gcnew List<String ^>();
		List<double>^ fitnessValues = gcnew List<double>();
		List<double>^ probPercentage = gcnew List<double>();
		double avgFitness = 0,
			sumFitness = 0;
		int maxSize = 0,
			current = 0,
			noElements = 0;
		//is the binary value based on integer?
		if(useInteger) {
			//get the random, binary values
			binaryValues = selectRandomValues(populationSize, static_cast<int>(lowest), static_cast<int>(highest));
			//get fitness values for integer
			noElements = static_cast<int>(binaryValues->Count);
			fitnessValues = returnCalculatedFitness(binaryValues, sumFitness);
		} else {
			//get the random, binary values
			binaryValues = selectRandomValues(populationSize, lowest, highest, decimalLength);
			//binary value is double, get it's fitness values
			noElements = static_cast<int>(binaryValues->Count);
			fitnessValues = returnCalculatedFitness(binaryValues, sumFitness, decimalLength);
		} //if
		//determine population's average fitness
		avgFitness = sumFitness / noElements;
		//calculate the probability for each value
		probPercentage = returnCalculatedProbability(fitnessValues, sumFitness);
		//create an object of genetic algorithm and return it
		GeneticAlgorithm ^gaObject = gcnew GeneticAlgorithm(FIRST_GENERATION, avgFitness, sumFitness, binaryValues, fitnessValues, probPercentage);
		//select parents from the first generation
		gaObject->selectParents(noElements);
		gaObject->setUsingIntAsNumeric(useInteger);
		gaObject->setDecimalLength(decimalLength);
		//clean up
		delete binaryValues;
		delete fitnessValues;
		delete probPercentage;
		return gaObject;
	} //initializeGA

	/*
	Note!
	This is only a test function made for an example based on finding the maximum for a function
	f(x) = x^2 for the interval 0 - 31. The link contains the example, but no actual code, added for reference.
	Source: http://www.cs.rochester.edu/~nelson/courses/csc_173/genetic-algs/example.html
	For future implementations, this could be re-done to fit an optimization for integers.
	--------------------------------------------------------------------------------------
	@param binaryValues: List<String> - List containing the binary values
	@param sumFitness: double - the variable to hold the fitness sum
	@return List<double>: a List containing the calculated fitness (converted binary values)
	*/
	List<double>^ GeneticAlgorithm::returnCalculatedFitness(List<String ^> ^%binaryValues, double &sumFitness) {
		int noElements = 0,
			currentElement = 0;
		noElements = static_cast<int>(binaryValues->Count);
		List<double>^ fitnessValues = gcnew List<double>();
		//evaluate each individual's fitness
		for(int i = 0; i < noElements; i++) {
			//get the converted value and raise it (value^2)
			currentElement = Conversion::convertBinaryStringToInteger(binaryValues[i], false);
			currentElement = static_cast<int>(pow(currentElement, 2.0));
			fitnessValues->Add(currentElement);
			sumFitness += currentElement;
		} //for
		return fitnessValues;
	} //returnCalculatedFitness

	/**
	Returns calculated fitness for doubles based on the given function
	f(x; y) = 75 * (y + x^2)^2 + (1 + x)^2 (not implemented yet!)
	TODO: How to treat x and y values???
	--------------------------------------------------------------------------------------
	@param binaryValues: List<String> - List containing the binary values
	@param sumFitness: double - the variable to hold the fitness sum
	@param decimalLength: int - the number of decimals, including the seperator ('.')
	@return List<double>: a List containing the calculated fitness (converted binary values)
	*/
	List<double>^ GeneticAlgorithm::returnCalculatedFitness(List<String ^> ^%binaryValues, double &sumFitness, int decimalLength) {
		int noElements = 0;
		double currentElement = 0;
		List<double>^ fitnessValues = gcnew List<double>();
		noElements = static_cast<int>(binaryValues->Count);
		/*
		x = static_cast<double>^(i);
		y = x;
		xPow2 = pow(x, 2);
		powXplus1 = pow((1 + x), 2);
		powYplusX = pow((y + xPow2), 2);
		z = 75 * (powYplusX) + (powXplus1);
		if (z < lowestZ) {
		lowestZ = z;
		currentXY = static_cast<double>^(i);
		*/
		//evaluate each individual's fitness
		for(int i = 0; i < noElements; i++) {
			//get the converted value
			currentElement = Conversion::convertBinaryStringToDouble(binaryValues[i], decimalLength);
			fitnessValues->Add(currentElement);
			sumFitness += currentElement;
		} //for
		return fitnessValues;
	} //returnCalculatedFitness

	/**
	Return the calculated probability for all the fitness values.
	The probability = sum of probabilities + (current probability / total fitness sum).
	--------------------------------------------------------------------------------------
	@param fitnessValues: List<double> - List containing all the fitness values
	@param sumFitness: double - the sum of all the fitnesses
	@return List<double> - a List containing the calculated probability for each value
	*/
	List<double>^ GeneticAlgorithm::returnCalculatedProbability(List<double> ^%fitnessValues, double sumFitness) {
		int noElements = 0;
		double currentProbability = 0,
			sumOfProbabilities = 0;
		List<double>^ probPercentage = gcnew List<double>();
		noElements = static_cast<int>(fitnessValues->Count);
		//calculate the probability percentage for each value
		for(int i = 0; i < noElements; i++) {
			currentProbability = fitnessValues[i];
			currentProbability = sumOfProbabilities + (currentProbability / sumFitness);
			sumOfProbabilities = currentProbability;
			probPercentage->Add(currentProbability);
		} //for
		return probPercentage;
	} //returnCalculatedProbability

	/**
	Returns a List filled with the parents used for breeding the next generation.
	The selection is based on a random roulette wheel, where the parent with the highest
	probability gets selected. The selection is surrounded by a while loop that runs until
	the population has been filled, inside the while loop is two for loops, in the outer for
	is a call to rand() and in the inner it's selection based on highest probability.
	--------------------------------------------------------------------------------------
	@param populationSize: int - the size of the population
	@return List<String>- a List containing the selected parents for the next generation
	*/
	List<String ^>^ GeneticAlgorithm::selectParents(int populationSize) {
		List<String ^>^ binaryParents = gcnew List<String ^>();
		int position = 0,
			sizeOfParents = 0,
			currentChildrenSize = 0;
		double random = 0;
		String ^ selectedParent = "";
		//loop until all the children has been created
		while(currentChildrenSize < populationSize) {
			selectedParent = "";
			//roulette wheel selection
			for(int i = 0; i < 2; i++) {
				random = (double)rand() / RAND_MAX;
				for(int j = 0; j < populationSize; j++) {
					//select the parent if the random number is greater 
					//then current probability, but lesser then the next
					if(random < getProbabilityPercentage()[j]) {
						selectedParent = getBinaryValues()[j];
					} //if
				} //for
				binaryParents->Add(selectedParent);
			} //for
			//get the size of the children
			currentChildrenSize = static_cast<int>(binaryParents->Count);
		} //while
		setSelectedParents(binaryParents);
		return binaryParents;
	} //selectParents

	/**
	Creates a new generation based on the selected parents, crossover value and probability
	of mutation. The setup is set so that for a crossover to happen, the randomvalue must be
	equal to or greater the set crossover value. Whereas for mutation, the chance for this is
	set so that it has to be lower then set parameter for mutation. If mutation is set to 5%,
	then theres up to a 4% chance of a mutation happening
	--------------------------------------------------------------------------------------
	@param crossover: double - The set value for a crossover to happen
	@param mutation: double - The set value for a mutation to happen
	@param populationSize: int - The set value for the population size
	@return GeneticAlgorithm: the new generation that were created with set parameters based on parents
	*/
	GeneticAlgorithm ^GeneticAlgorithm::createNewGeneration(double crossover, double mutation, int populationSize) {
		//Lists for keeping the values
		List<double>^ fitnessValues = gcnew List<double>();
		List<double>^ probPercentage = gcnew List<double>();
		List<String ^>^ newGeneration = gcnew List<String ^>();
		String ^ newChild = "";
		String ^ crossoverPart = "";
		double avgFitness = 0,
			sumFitness = 0,
			mutationChance = 0;
		int counter = 0,
			length = 0,
			noElements = 0,
			crossoverPosition = 0,
			currentGeneration = 0;
		//get the parents selected for breeding
		List<String ^>^ selectedParents = getSelectedParents();
		//get the length of the binary String ^
		length = selectedParents[0]->Length;
		//get the number of parents	
		noElements = static_cast<int>(selectedParents->Count);
		//create children
		while(counter < populationSize) {
			mutationChance = (double)rand() / RAND_MAX;
			if(mutationChance >= crossover) {
				newChild = computeCrossover(selectedParents, length, noElements);
			} else {
				crossoverPosition = rand() % noElements;
				newChild = selectedParents[crossoverPosition];
			} //if
			//0 - 1 (100%) - check if child mutated
			mutationChance = (double)rand() / RAND_MAX;
			newChild = mutateChild(newChild, crossoverPosition, length, mutationChance, mutation);
			//add the new child to the List and increase the counter
			newGeneration->Add(newChild);
			counter++;
		} //while
		//get the current generation and increase it
		currentGeneration = getGeneration();
		currentGeneration++;
		//get the number of created children
		noElements = static_cast<int>(newGeneration->Count);
		if(getUsingIntAsNumeric()) {
			//get fitness values for integer
			fitnessValues = returnCalculatedFitness(newGeneration, sumFitness);
		} else {
			//get fitness for double
			fitnessValues = returnCalculatedFitness(newGeneration, sumFitness, getDecimalLength());
		} //if
		//determine population's average fitness
		avgFitness = sumFitness / noElements;
		//calculate the probability for each value
		probPercentage = returnCalculatedProbability(fitnessValues, sumFitness);
		//create an object of genetic algorithm and return it
		GeneticAlgorithm ^gaChild = gcnew GeneticAlgorithm(currentGeneration, avgFitness, sumFitness, newGeneration, fitnessValues, probPercentage);
		gaChild->selectParents(noElements);
		gaChild->setUsingIntAsNumeric(getUsingIntAsNumeric());
		gaChild->setDecimalLength(getDecimalLength());
		//clean up
		delete fitnessValues;
		delete probPercentage;
		delete newGeneration;
		return gaChild;
	} //createNewGeneration

	/**
	Returns the child/result of a crossover.
	This function currently only crosses one bit at a random position.
	--------------------------------------------------------------------------------------
	@param selectedParents: List<String> - The List containing the selected parents for this
	generation
	@param length: int - The length of the string
	@param noElements: noElements - Number of elements in the List
	@return String ^: the created child
	*/
	String ^ GeneticAlgorithm::computeCrossover(List<String ^> ^%selectedParents, int length, int noElements) {
		int randParent1 = 0,
			randParent2 = 0,
			crossoverPosition = 0;
		String ^ newChild = "";
		String ^ crossoverPart = "";
		//crossover is currently here only taking one bit of the String ^
		crossoverPosition = rand() % length;
		//select two parents randomly
		randParent1 = rand() % noElements;
		randParent2 = rand() % noElements;
		//create the new child by taking most genes from first parent
		newChild = selectedParents[randParent1]->Substring(0, crossoverPosition);
		crossoverPart = selectedParents[randParent2]->Substring(crossoverPosition, 1);
		newChild += crossoverPart + selectedParents[randParent1]->Substring(crossoverPosition + 1);
		return newChild;
	} //computeCrossover

	/**
	Returns the child/result of a mutation.
	This function currently only mutates one bit at a random position.
	--------------------------------------------------------------------------------------
	@param newChild: String ^ - The List containing the selected parents for this
	@param crossover: int - Just an int to use as placeholder to save time (less resource usage)
	@param length: int - The length of the String ^
	@param mutationChance: double - chance for that a mutation can happen (random value)
	@param mutation: double - The set parameter for that a mutation can happen
	@return String ^: the mutated child
	*/
	String ^ GeneticAlgorithm::mutateChild(String ^ newChild, int crossover, int length, double mutationChance, double mutation) {
		int mutationValue = 0;
		//has mutation occured?
		if(mutationChance < mutation) {
			//find a random position, get the value and convert it to boolean
			crossover = rand() % length;
			mutationValue = Conversion::convertStringToInteger(newChild->Substring(crossover, 1));
			//since boolean is either 0 or 1, it's easy to use, so flip the value
			(mutationValue) ? mutationValue = 0 : mutationValue = 1;
			if(crossover > 0 && crossover < length) {
				newChild = newChild->Substring(0, crossover - 1)
					+ Conversion::convertIntegerToString(mutationValue)
					+ newChild->Substring(crossover + 1);
			} else if(crossover == length) {
				newChild = newChild->Substring(crossover - 1)
					+ Conversion::convertIntegerToString(mutationValue);
			} else if(crossover == 0) {
				newChild = Conversion::convertIntegerToString(mutationValue)
					+ newChild->Substring(crossover + 1);
			} //if
		} //if
		return newChild;
	} //mutateChild

	/**
	Select random binary values to use for the first generation. The function generates
	random integer values that are then encoded as binary String ^s. These binary String ^s
	are then added to the List which is returned. To avoid range problems if one or both
	range values are negative, a call to the function checkIfSelectedRangeIsNegative(...)
	is done to get proper range settings.
	--------------------------------------------------------------------------------------
	@param populationSize: int - The size of the population
	@param min: int - The lowest value range
	@param max: int - The highest value range
	@return List<String>: List containing the random binary values for the first generation
	*/
	List<String ^>^ GeneticAlgorithm::selectRandomValues(int populationSize, int min, int max) {
		int random = 0;
		double tempMin = 0,
			tempMax = 0;
		List<int>^ randomValues = gcnew List<int>();
		tempMin = static_cast<double>(min);
		tempMax = static_cast<double>(max);
		checkIfSelectedRangeIsNegative(tempMin, tempMax);
		min = static_cast<int>(tempMin);
		max = static_cast<int>(tempMax);
		//loop and fill List with random values
		for(int i = 0; i < populationSize; i++) {
			random = rand() % max + (min);
			randomValues->Add(random);
		} //for
		//convert values to binary and return it
		return Conversion::binaryConverter(randomValues);
	} //selectRandomValues

	/**
	Select random binary values to use for the first generation. The function generates
	random double values that are then encoded as binary String ^s. These binary String ^s
	are then added to the List which is returned. To avoid range problems if one or both
	range values are negative, a call to the function checkIfSelectedRangeIsNegative(...)
	is done to get proper range settings.
	--------------------------------------------------------------------------------------
	@param populationSize: int - The size of the population
	@param min: int - The lowest value range
	@param max: int - The highest value range
	@param decimalLength: int - The number of decimals, including the seperator ('.')
	@return List<String>: List containing the random binary values for the first generation
	*/
	List<String ^>^ GeneticAlgorithm::selectRandomValues(int populationSize, double min, double max, int decimalLength) {
		double random = 0;
		List<double> ^randomValues = gcnew List<double>();
		checkIfSelectedRangeIsNegative(min, max);
		//loop and fill List with random values
		for(int i = 0; i < populationSize; i++) {
			random = fmod(rand(), max) + (min);
			randomValues->Add(random);
		} //for
		//convert values to binary and return it
		return Conversion::binaryConverter(randomValues, decimalLength);
	} //selectRandomValues

	/*
	Controls that max is greater then min, and if min, or both min and
	max is negative, then the range is calculated to get correct random
	range.
	--------------------------------------------------------
	@param min: double - the lowest value range for random start values
	@param max: double - the highest value range for random start values
	*/
	void GeneticAlgorithm::checkIfSelectedRangeIsNegative(double &min, double &max) {
		const int bothAreNegative = -1,
			minIsNegative = 0,
			noneAreNegative = 1;
		double temp = 0;
		//are the sizes correct?
		if(max < min) {
			//swap values for correct order
			temp = min;
			min = max;
			max = temp;
		} //if
		//is min is negative?
		if(min < 0) {
			max = max + -(min)+1;
		} //if
	} //checkIfSelectedRangeIsNegative

	/************** GET AND SET FUNCTIONS **********************/

	bool GeneticAlgorithm::getUsingIntAsNumeric() {
		return _useInteger;
	}

	int GeneticAlgorithm::getGeneration() {
		return _generation;
	} //getGeneration

	double GeneticAlgorithm::getSumFitness() {
		return _sumFitness;
	} //getSumFitness

	double GeneticAlgorithm::getAvgFit() {
		return _avgFit;
	} //getAvgFit

	List<String ^>^ GeneticAlgorithm::getBinaryValues() {
		return _binaryValues;
	} //getBinaryValues

	List<double>^ GeneticAlgorithm::getFitnessValues() {
		return _fitnessValues;
	} //getFitnessValues

	List<double>^ GeneticAlgorithm::getProbabilityPercentage() {
		return _probPercentage;
	} //getProbabilityPercentage

	List<String ^>^ GeneticAlgorithm::getSelectedParents() {
		return _selectedParents;
	} //getSelectedParents

	int GeneticAlgorithm::getDecimalLength() {
		return _decimalLength;
	} //getDecimalLength

	void GeneticAlgorithm::setProbabilityPercentage(List<double>^ probPercentage) {
		//cler of previous content and add the new values
		_probPercentage = gcnew List<double>();
		_probPercentage->AddRange(probPercentage);
	} //setProbabilityPercentage

	void GeneticAlgorithm::setSelectedParents(List<String ^>^ selectedParents) {
		_selectedParents = gcnew List<String ^>();
		_selectedParents->AddRange(selectedParents);
	} //setSelectedParents

	void GeneticAlgorithm::setUsingIntAsNumeric(bool useInteger) {
		_useInteger = useInteger;
	} //setUsingIntAsNumeric

	void GeneticAlgorithm::setDecimalLength(int decimalLength) {
		_decimalLength = decimalLength;
	} //setDecimalLength
} //namespace