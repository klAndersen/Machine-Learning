#include "Stdafx.h"
#include "clustering.h"
#include "fileoperations.h"

namespace MachineLearning {

	/*
	Default constructor
	*/
	Clustering::Clustering() {
		_noCentroids = 0;
		_noSamples = 0;
		_noAttributes = 0;
		_progressFinished = 0;
		_centroidMatrix = gcnew Matrix();
		_sampleMatrix = gcnew Matrix();
		_clusterGroupList = gcnew List<List<int>^>();
	} //constructor

	/*
	Constructor initializing the members of the class,
	retrieving values from file and filling matrices.
	---------------------------------------------------------
	@param centroidFile: String ^ - Name of file containing the centroids
	@param testFile: String ^ - Name of file containing unlabeled samples
	@param noClusterGroups: int - The number of cluster groups to analyze
	@param noAttributes: int - The number of attributes
	*/
	Clustering::Clustering(String ^centroidFile, String ^testFile, int noClusterGroups, int noAttributes) : Clustering() {
		try {
			//read data from file
			FileOperations fileOperation;
			_clusterGroupList = gcnew List<List<int>^>();
			List<List<double>^>^ sampleList = gcnew List<List<double>^>();
			List<List<double>^>^ centroidList = gcnew List<List<double>^>();
			fileOperation.readInfiniteClusterAttributesFromFile(testFile, sampleList, noAttributes);
			fileOperation.readInfiniteClusterAttributesFromFile(centroidFile, centroidList, noAttributes);
			//set values
			_progressFinished = 0;
			setNoSamples(sampleList->Count);
			setNoCentroids(centroidList->Count);
			setNoAttributes(noAttributes);
			//fill matrices with data 
			fillMatrixWithSamples(sampleList, _sampleMatrix);
			//is the number of selected cluster groups less then 
			//the number of centroids
			if(noClusterGroups < getNoCentroids()) {
				//less then total, select random values
				fillCentroidMatrixWithSamples(centroidList, _centroidMatrix, noClusterGroups);
			} else {
				//number of groups is equal to total
				fillMatrixWithSamples(centroidList, _centroidMatrix);
			} //if					 
			//check that the number of attributes in both samples are equal
			if(noAttributes != _centroidMatrix->getNumColumns()) {
				String ^ errorMsg = "";
				errorMsg = "The number of attributes in the test sample (count: ";
				errorMsg += Conversion::convertIntegerToString(noAttributes);
				errorMsg += ") is not equal to the number of attributes in the centroid (count: ";
				errorMsg += Conversion::convertIntegerToString(_centroidMatrix->getNumColumns()) + ").";
				throw gcnew ExceptionClass(errorMsg);
			} //if
			//clean up
			delete sampleList;
			delete centroidList;
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
	} //constructor

	/*
	Destructor
	*/
	Clustering::~Clustering() {
		_centroidMatrix->deleteMatrix();
		_sampleMatrix->deleteMatrix();
		delete _centroidMatrix;
		delete _sampleMatrix;
		delete _clusterGroupList;
	} //destructor

	/*
	Fills a matrix with the values in the list.
	---------------------------------------------------------
	@param sampleList: List<List<double>> - List containing the
	samples to fill matrix with
	@param sampleMatrix: Matrix - The matrix to fill with values
	@return Matrix: Matrix filled with the values from the sampleList
	*/
	void Clustering::fillMatrixWithSamples(List<List<double>^> ^%sampleList, Matrix ^%sampleMatrix) {
		int counter = 0;
		List<double> ^tempList = gcnew List<double>();
		sampleMatrix = gcnew Matrix(sampleList->Count, getNoAttributes());
		//loop through the attributes and samples
		for(int i = 0; i < getNoAttributes(); i++) {
			for each(List<double>^ attribute in sampleList) {
				//fill list with values
				tempList->Add(attribute[i]);
			} //for
			//add the retrieved values to the matrix and clear the temp List
			sampleMatrix->fillMatrixWithValues(i, tempList);
			tempList = gcnew List<double>();
		} //for
		delete tempList;
	} //fillMatrixWithSamples	

	/*
	Fills the centroid matrix with values from the centroid list.
	The indexes of the samples to use are selected randomly and
	distinct, to avoid duplicates.
	---------------------------------------------------------
	@param centroidList: List<List<double>> - List containing the
	all the centroid samples
	@param centroidMatrix: Matrix - The matrix to fill with centroids
	@param noClusterGroups: int - the number of selected cluster groups
	@return Matrix: Matrix filled with (random) centroid samples
	*/
	void Clustering::fillCentroidMatrixWithSamples(List<List<double>^>^ %centroidList, Matrix ^%centroidMatrix, int noClusterGroups) {
		int randomCluster = 0;
		List<int> ^randomIndex = gcnew List<int>();
		List<List<double>^> ^tempList = gcnew List<List<double>^>();
		for(int i = 0; i < noClusterGroups; i++) {
			randomCluster = rand() % noClusterGroups;
			//loop until a new, distinct random index is selected
			while(getClusterCount(randomIndex, randomCluster) > 0) {
				randomCluster = rand() % noClusterGroups;
			} //while	 
			randomIndex->Add(randomCluster);
		} //for
		//sort the indexes
		randomIndex->Sort();
		for each(int index in randomIndex) {
			tempList->Add(centroidList[index]);
		} //for		
		//clean up and update the number of centroids
		delete randomIndex;
		setNoCentroids(noClusterGroups);
		fillMatrixWithSamples(tempList, centroidMatrix);
	} //fillCentroidMatrixWithSamples

	/*
	Calculates the distance between the centroid and the samples
	by use of the k-means method.
	---------------------------------------------------------
	@param maxIterations: float - The total number of iterations to run
	@param stopParameter: int - The number of equal results before stopping 
	the cluster analysis
	@param worker: ComponentModel::BackgroundWorker - The backgroundworker
	handling the work in the background
	@param e: ComponentModel::DoWorkEventArgs - The handler for event data
	*/
	List<List<int>^>^ Clustering::calculateDistanceFromCentroids(float maxIterations, int stopParameter,
		ComponentModel::BackgroundWorker^ worker, ComponentModel::DoWorkEventArgs ^ e) {
		int counter = 0,
			round = 0,
			currentIndex = 0;
		double mean = 0,
			distance = 0,
			currentValue = 0;
		int noSamples = getNoSamples();
		bool exitLoop = false;
		Statistics ^stats;
		List<int> ^clusterIndex = gcnew List<int>();
		List<double> ^clusterDistances = gcnew List<double>();
		while(round < maxIterations && !worker->CancellationPending && !exitLoop) {
			updateProgressBar(round, maxIterations, worker);
			//loop through the test samples and centroids attribute values
			for(int i = 0; i < noSamples; i++) {
				for(int j = 0; j < getNoCentroids(); j++) {
					distance = 0;
					for(int k = 0; k < getNoAttributes(); k++) {
						//get the value for the test sample, centroid mean and calculate distance
						currentValue = getSampleMatrix()->getCellValueAt(i, k);
						mean = getCentroidMatrix()->getCellValueAt(j, k);
						distance += calculateDistance(currentValue, mean);
					} //for
					//get the squareroot of the distance, and add it to the List
					distance = sqrt(distance);
					clusterDistances->Add(distance);
				} //for
				clusterDistances->TrimExcess();
				currentIndex = counter = 1;
				distance = clusterDistances[0];
				//loop through the distances and find the shortest for this sample
				for each (currentValue in clusterDistances) {
					if(currentValue < distance) {
						distance = currentValue;
						currentIndex = counter;
					} //if
					counter++;
				} //for
				//clear distance values and add index to list
				clusterDistances = gcnew List<double>();
				clusterIndex->Add(currentIndex);
			} //for
			clusterIndex->TrimExcess();
			//add the index of cluster grouping from this round
			_clusterGroupList->Add(clusterIndex);
			_clusterGroupList->TrimExcess();
			//re-calculate centroids and update the matrix
			updateCentroids(clusterIndex);
			clusterIndex = gcnew List<int>();
			//check if a cancel request has been issued
			e->Cancel = worker->CancellationPending;
			round++;
			exitLoop = isResultEqualToLastRound(round, stopParameter, _clusterGroupList, stats);
		} //while
		delete clusterDistances;
		delete clusterIndex;
		return _clusterGroupList;
	} //calculateDistanceFromCenteroids

	/*
	Passes the progress to the user interface by use of the
	backgroundworker. The progress is calculated in percent.
	---------------------------------------------------------
	@param status: int - The current status of the progress (e.g. iteration)
	@param total: float - The total number of operations before work is done
	@param worker: System::ComponentModel::BackgroundWorker - The active
	backgroundworker handling the background task related to clustering
	*/
	void  Clustering::updateProgressBar(int status, float total, System::ComponentModel::BackgroundWorker ^worker) {
		int percentComplete = 0;
		if(status > 100) {
			status = 100;
		} //if
		percentComplete = (int)((float)status / total * 100);
		if(percentComplete > _progressFinished) {
			_progressFinished = percentComplete;
			worker->ReportProgress(percentComplete);
		} //if
	} //updateProgressBar

	/*
	Calculates the distance between the unlabeled sample attribute
	and the centroid (substraction) and then raises the answer to
	the power of 2 and returns it.
	---------------------------------------------------------
	@param value: double - The current unlabeled sample attribute value
	@param value: double - The current centroid value
	@return double: Result of (value - mean)^2
	*/
	double Clustering::calculateDistance(double value, double mean) {
		double distance = 0;
		distance = value - mean;
		distance = pow(distance, 2);
		return distance;
	} //calculateDistance

	/*
	Updates the centroid matrix, filling in the new averages where
	changes have occurred.
	Excepting the format where the list position is the sample
	(unlabeled) and the actual value in the list is the centroid index.
	---------------------------------------------------------
	@param List<int> - List of cluster indexes
	@param centroidMatrix - Matrix: Matrix containing centroids
	from previous round
	@param sampleMatrix - Matrix:
	@return Matrix: Updated centroid matrix containing the
	average from this round
	*/
	Matrix ^Clustering::updateCentroids(List<int> ^clusterIndex) {
		int rowNo = 0,
			indexCounter = 0;
		double currentValue = 0;
		_centroidMatrix = getCentroidMatrix();
		_sampleMatrix = getSampleMatrix();
		Matrix ^tempMatrix;
		//loop through the centroids and update the mean values
		for(int centroid = 0; centroid < getNoCentroids(); centroid++) {
			if(clusterIndex->Contains(centroid + 1)) {
				rowNo = 0;
				indexCounter = 0;
				//create a matrix and loop through the indexes to retrieve the sample values
				tempMatrix = gcnew Matrix(getClusterCount(clusterIndex, centroid + 1), getNoAttributes());
				for each(int index in clusterIndex) {
					if(index == (centroid + 1)) {
						for(int columnNo = 0; columnNo < getNoAttributes(); columnNo++) {
							currentValue = _sampleMatrix->getCellValueAt(indexCounter, columnNo);
							tempMatrix->changeCellValueAt(rowNo, columnNo, currentValue);
						} //for
						rowNo++;
					} //if
					indexCounter++;
				} //for
				//create matrix containing mean and update the centroid values
				tempMatrix = tempMatrix->createMeanMatrix();
				for(int columnNo = 0; columnNo < tempMatrix->getNumRows(); columnNo++) {
					currentValue = tempMatrix->getCellValueAt(columnNo, 0);
					_centroidMatrix->changeCellValueAt(centroid, columnNo, currentValue);
				} //for
			} //if
		} //for
		return _centroidMatrix;
	} //updateCentroids

	/*
	A function made to check for equal results, mainly to stop
	the iterations before time if cluster analysis is "good enough".
	---------------------------------------------------------
	@param round: int - the current iteration round
	@param stopParameter: int - number of equals telling when
	enough equal results have been found
	@return bool: True - end iteration, false - continue iteration, not enough
	equal results found
	*/
	bool Clustering::isResultEqualToLastRound(int round, int stopParameter, List<List<int>^>^ %clusterGroupList, Statistics ^%stats) {
		bool equalRounds = false;
		if(round > 2) {
			int counter = 0,
				countEquals = 0;
			double currentMean = 0,
				previousMean = 0;
			List<int> ^previous = gcnew List<int>();
			//set it to the first element to initialize
			previous = clusterGroupList[0];
			//loop through all the index lists and compare them
			for each(List<int> ^sample in clusterGroupList) {
				//using the average of the indexes to see if result is equal
				previousMean = stats->calculateMean(previous);
				currentMean = stats->calculateMean(sample);
				if(previousMean == currentMean) {
					countEquals++;
				} //if
				previous = clusterGroupList[counter];
				counter++;
			} //for
			//check if number of equals exceed set parameter
			if(countEquals > stopParameter) {
				equalRounds = true;
			} //if
		} //if
		return equalRounds;
	} //isResultEqualToLastRound

	/*********************GET & SET METHODS *****************************/

	/*
	Returns the cluster count from the passed index list.
	Excepting the format where the list position is the sample
	(unlabeled) and the actual value in the list is the centroid index.
	---------------------------------------------------------
	@param List<int> - List of cluster indexes
	@param clusterValue: int - The value find and count
	@return int: The count of the given cluster value in the list
	*/
	int Clustering::getClusterCount(List<int> ^clusterIndex, int clusterValue) {
		int counter = 0;
		for each (int index in clusterIndex) {
			if(index == clusterValue) {
				counter++;
			} //if
		} //for
		return counter;
	} //getClusterCount

	/*
	Returns a list containing the rescaled values of the attribute index.
	The rescaling is done in the following way: xNew=(x-Min)/(Max-Min)
	---------------------------------------------------------
	@param noAttributes: int - The total number of attributes for this sample
	@return List<double>: List containing the rescaled values (for the x-axis)
	*/
	List<double> ^Clustering::getRescaledList(int noAttributes) {
		double minValue = 1,
			rescaledValue = 0;
		List<double> ^rescaledList = gcnew List<double>();
		for(int x = 1; x <= noAttributes; x++) {
			rescaledValue = (x - minValue) / (noAttributes - minValue);
			rescaledList->Add(rescaledValue);
		} //for
		return rescaledList;
	} //getRescaledList

	/*
	Returns a list containing the indexes of the cluster groups
	(index1: iteration round, index2: the cluster group the sample was added to)
	*/
	List<List<int>^>^ Clustering::getClusterGroupList() {
		return _clusterGroupList;
	} //getClusterGroupList

	/*
	Returns the current centroid matrix
	(values are from the last iteration round)
	*/
	Matrix ^%Clustering::getCentroidMatrix() {
		return _centroidMatrix;
	} //getNoAttributes

	/*
	Returns the test/sample matrix
	(matrix containing the original unlabeled samples)
	*/
	Matrix ^%Clustering::getSampleMatrix() {
		return _sampleMatrix;
	} //getNoSamples

	/*
	Returns the number of attributes
	*/
	int Clustering::getNoAttributes() {
		return _noAttributes;
	} //getNoAttributes

	/*
	Returns the number of test (unlabeled) samples
	*/
	int Clustering::getNoSamples() {
		return _noSamples;
	} //getNoSamples

	/*
	Returns the number of centroid samples
	*/
	int Clustering::getNoCentroids() {
		return _noCentroids;
	} //getNoCentroids

	/*
	Set the number of attributes belonging to the samples
	*/
	void Clustering::setNoAttributes(int noAttributes) {
		_noAttributes = noAttributes;
	} //setNoAttributes

	/*
	Set the number of unlabeled samples
	*/
	void Clustering::setNoSamples(int noSamples) {
		_noSamples = noSamples;
	} //setNoSamples

	/*
	Set the number of centroid samples
	*/
	void Clustering::setNoCentroids(int noCentroids) {
		_noCentroids = noCentroids;
	} //setNoCentroids
} //namespace