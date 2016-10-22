#include "Stdafx.h"
#include "matrix.h"
#include <cmath>

namespace MachineLearning {

	/*
	Default constructor
	*/
	Matrix::Matrix() {
		//default initialisation
		_numRows = 0;
		_numColumns = 0;
		_matrixArray = 0;
		_cellValue = 0;
		_matrixArray = 0;
	} //constructor

	/*
	Constructor creating an empty matrix where the size is based
	on the numRows and numColumns
	---------------------------------------------------------
	@param numRows - int: The number of rows in the matrix
	@param numColumns - int: The number of columns in the matrix
	*/
	Matrix::Matrix(int numRows, int numColumns) {
		//set the values for the matrix
		setNewRowCount(numRows);
		setNewColumnCount(numColumns);
		//create the matrix array
		_matrixArray = createMatrixArray(numRows, numColumns);
	} //constructor

	/*
	Constructor creating a matrix filled with the values from the
	passed list. The matrix is filled in the following way;
	List->Count = noColumns, List[0]->Count = noRows.
	Throws error if list is empty.
	---------------------------------------------------------
	@param valueList: List<List<double>^>^ - The list containing values to add to matrix
	*/
	Matrix::Matrix(List<List<double> ^> ^valueList) {
		if(valueList->Count == 0 || valueList[0]->Count == 0) {
			throw gcnew ExceptionClass("The passed list is empty. Cannot fill Matrix with values.");
		} //if
		setNewColumnCount(valueList->Count);
		setNewRowCount(valueList[0]->Count);
		//create the matrix array
		_matrixArray = createMatrixArray(getNumRows(), getNumColumns());
		fillMatrixWithValues(valueList);
	} //constructor

	/*
	Constructor creating a matrix filled with the values from the
	passed list. The matrix is filled in the following way;
	The no. of samples = rowCount and the no. of attributes = columnCount.
	Throws error if list is empty.
	---------------------------------------------------------
	@param valueList: List<List<trainingData>^>^ - The list containing values to add to matrix
	*/
	Matrix::Matrix(List<Training ^> ^trainingData) {
		ExceptionClass::throwErrorIfValueListIsEmpty(trainingData);
		setNewRowCount(trainingData->Count);
		setNewColumnCount(trainingData[0]->getNoAttributes());
		//create the matrix array
		_matrixArray = createMatrixArray(getNumRows(), getNumColumns());
		fillMatrixWithValues(trainingData);
	} //constructor

	/*
	Destructor
	*/
	Matrix::~Matrix() {

	} //destructor

	/**************** CALCULATION FUNCTIONS *******************/

	/*
	Calculates the mean of the passed columnNo and returns the value.
	If passed columnNo value isn't within range, a ExceptionClass is thrown.
	---------------------------------------------------------
	@param columnNo: int - the columnNo to calculate mean
	@throw: ExceptionClass - Throws error if the columnNo
	value isn't within range
	@return double: the mean
	*/
	double Matrix::calculateMean(int columnNo) {
		double mean = 0;
		try {
			//check that the matrix isn't empty and then check that the column is within range
			ExceptionClass::throwErrorIfMatrixIsEmpty(getNumRows(), getNumColumns());
			ExceptionClass::throwErrorIfMatrixColumnOutOfRange(columnNo, getNumColumns(), "\nCan't calculate mean.");
			_matrixArray = getMatrix();
			for(int i = 0; i < getNumRows(); i++) {
				mean += _matrixArray[i][columnNo];
			} //for
			//check that mean isn't zero 
			//(to avoid divide-by-zero error)
			if(mean != 0) {
				mean = mean / static_cast<double>(getNumRows());
			} //if
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
		return mean;
	} //calculateMean

	/*
	Calculates the mean of the matrix and returns a matrix containing
	1 row and n-columns, where n = calling matrix->getNumColumns().
	After the matrix is created, it's transposed to ensure correct order.
	Throws error if matrix is empty.
	---------------------------------------------------------
	@param columnNo: int - the columnNo to calculate mean
	@throw: ExceptionClass - Throws error if the columnNo
	value isn't within range
	@return: Matrix - A matrix containing the mean(s)
	*/
	Matrix ^Matrix::createMeanMatrix() {
		//check that the matrix isn't empty and then check that the column is within range
		ExceptionClass::throwErrorIfMatrixIsEmpty(this->getNumRows(), this->getNumColumns());
		int numRows = 1,
			numColumns = this->getNumColumns();
		Matrix ^%meanMatrix = gcnew Matrix(numRows, numColumns);
		for(int i = 0; i < this->getNumColumns(); i++) {
			meanMatrix->changeCellValueAt(0, i, this->calculateMean(i));
		} //for
		meanMatrix->transposeMatrix();
		return meanMatrix;
	} //createMeanMatrix

	/*
	Calculates the corrected mean. If the rowNo/columnNo value isn't
	within range, a ExceptionClass is thrown.
	---------------------------------------------------------
	@param rowNo: int - The rowNo to calculate corrected mean from
	@param columnNo: int - The columnNo to calculate corrected mean from
	@param globalMean: double - The global mean
	(mean calculated from the whole columnNo)
	@throw: ExceptionClass - Throws error if the rowNo/columnNo
	value isn't within range
	@return double: the corrected mean
	*/
	double Matrix::calculateCorrectedMean(int rowNo, int columnNo, double globalMean) {
		double correctedMean = 0;
		try {
			//check that the values are within range
			ExceptionClass::throwErrorIfMatrixRowOutOfRange(rowNo, getNumRows(), "\nCan't calculate corrected mean data.");
			ExceptionClass::throwErrorIfMatrixColumnOutOfRange(columnNo, getNumColumns(), "\nCan't calculate corrected mean data.");
			_matrixArray = getMatrix();
			correctedMean = _matrixArray[rowNo][columnNo] - globalMean;
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
		return correctedMean;
	} //calculateCorrectedMean

	/*
	Calculates the variance for the passed columnNo. If the passed
	columnNo isn't within range, a ExceptionClass is thrown.
	---------------------------------------------------------
	@param columnNo: int - The columnNo to calculate co-variance from
	@throw: ExceptionClass - Throws error if the columnNo
	value isn't within range
	@return double: the variance || 0
	*/
	double Matrix::calculateVariance(int columnNo) {
		try {
			//check that the matrix isn't empty and then check that the column is within range
			ExceptionClass::throwErrorIfMatrixIsEmpty(getNumRows(), getNumColumns());
			ExceptionClass::throwErrorIfMatrixColumnOutOfRange(columnNo, getNumColumns(), "\nCan't calculate variance.");
			double mean = 0,
				variance = 0,
				sumOfValues = 0;
			int n = getNumRows();
			_matrixArray = getMatrix();
			mean = calculateMean(columnNo);
			//calculate the sum of the values
			for(int i = 0; i < n; i++) {
				sumOfValues = _matrixArray[i][columnNo] - mean;
				variance += pow(sumOfValues, 2);
			} //for
			//check that variance isn't zero 
			//(to avoid divide-by-zero error)
			if(variance != 0) {
				variance = variance / (n - 1);
				return variance;
			} //if
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
		return 0;
	} //calculateVariance

	/*
	Calculates the co-variance for the two passed columns.
	It first subtracts mean from the value, and then multiply
	the result. When all the columnNo values have been looped through
	the result is divided by (n-1) and returned.
	---------------------------------------------------------
	@param columnX: int - The 1. columnNo to calculate co-variance from
	@param columnY: int - The 2. columnNo to calculate co-variance from
	@return double: the coVariance || 0
	*/
	double Matrix::calculateCoVariance(int columnX, int columnY) {
		double coVariance = 0;
		try {
			ExceptionClass::throwErrorIfMatrixColumnOutOfRange(columnX, getNumColumns(), "\nCan't calculate Co-Variance.");
			ExceptionClass::throwErrorIfMatrixColumnOutOfRange(columnY, getNumColumns(), "\nCan't calculate Co-Variance.");
			double currentX = 0,
				currentY = 0,
				meanX = 0,
				meanY = 0;
			int n = getNumRows();
			_matrixArray = getMatrix();
			//calculate mean
			meanX = calculateMean(columnX);
			meanY = calculateMean(columnY);
			//loop through the values and calculate the sum 
			for(int i = 0; i < n; i++) {
				currentX = _matrixArray[i][columnX] - meanX;
				currentY = _matrixArray[i][columnY] - meanY;
				coVariance += currentX * currentY;
			} //for
			coVariance = coVariance / (n - 1);
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
		return coVariance;
	} //calculateCoVariance

	/*
	Creates a matrix containing the calculated co-variance values,
	based on the values from the passed matrix parent.
	If parent is empty, ExceptionClass is thrown.
	---------------------------------------------------------
	@param parent: Matrix - Matrix containing the original values
	@throw: ExceptionClass - Throws error if parent is empty
	@return: Matrix - a matrix filled with the co-variance calculated
	from the passed Matrix parent
	*/
	Matrix^ Matrix::createCoVarianceMatrix(Matrix ^parent) {
		int numElements = 0;
		double coVariance = 0;
		//since the matrix is a NxN matrix, use columns as counter
		numElements = parent->getNumColumns();
		//check that the parent matrix isn't empty
		String ^ errorMsg = "\nCan't create co-variance matrix.";
		ExceptionClass::throwErrorIfParentMatrixIsEmpty(parent->getNumRows(), parent->getNumColumns(), errorMsg);
		//create the co-variance matrix
		Matrix^ coVarMatrix = gcnew Matrix(numElements, numElements);
		//fill coVarMatrix with values from parent matrix
		for(int i = 0; i < numElements; i++) {
			for(int j = 0; j < numElements; j++) {
				coVariance = parent->calculateCoVariance(i, j);
				coVarMatrix->changeCellValueAt(i, j, coVariance);
			} //for
		} //for
		return coVarMatrix;
	} //createCoVarianceMatrix

	/*
	Attempts to calculate and create a scatter matrix based on the values in
	the passed coVarMatrix. If the coVarMatrix is empty, an attempt to create
	it is done by calling the function createCoVarianceMatrix().
	The values in the co-variance matrix is then multiplied by (n-1) and
	finally the filled scatter matrix is returned.
	If both the co-variance matrix and the coVarParent matrix is empty,
	ExceptionClass is thrown.
	---------------------------------------------------------
	@param coVarParent: Matrix filled with the original values
	@param coVarMatrix: Matrix filled with the co-variance values
	@throw: ExceptionClass - Throws error if both the coVarParent
	and coVarMatrix is empty
	@see createCoVarianceMatrix(Matrix ^parent)
	@return: Matrix - a scatter matrix (filled with the co-variance multiplied with (n-1))
	*/
	Matrix ^Matrix::createScatterMatrix(Matrix ^coVarParent, Matrix ^coVarMatrix) {
		//since the matrix should sizes should be equal, just use the counter
		int n = 0,
			numElements = 0;
		double coVariance = 0;
		//get the columncount
		numElements = coVarMatrix->getNumColumns();
		//check that the covarmatrix isn't empty
		if(numElements == 0) {
			try {
				//the co-variance matris is empty, try to create it
				coVarMatrix = createCoVarianceMatrix(coVarParent);
			} catch(ExceptionClass ^ex) {
				//failed at creating co-var matrix
				//this happens if the parent-matrix is empty
				//get the error message from the createCoVarianceMatrix()
				//and add the error for this function.
				String ^ errorMsg = ex->ToString();
				errorMsg += "\nCan't create scatter matrix.";
				throw gcnew ExceptionClass(errorMsg);
			} //try/catch
		} //if
		n = static_cast<int>(coVarParent->getNumRows()) - 1;
		//create scatter matrix
		Matrix ^scatterMatrix = gcnew Matrix(numElements, numElements);
		//fill scatterMatrix with values 
		for(int i = 0; i < numElements; i++) {
			for(int j = 0; j < numElements; j++) {
				coVariance = n * coVarMatrix->getMatrix()[i][j];
				scatterMatrix->changeCellValueAt(i, j, coVariance);
			} //for
		} //for
		return scatterMatrix;
	} //createScatterMatrix

	/*
	Create a matrix containing the between scatter.
	Based on a two-class problem.
	---------------------------------------------------------
	@param invSw: Matrix - Matrix containing the mean values
	@throw: ExceptionClass - Throws error if one of the matrices is empty
	@return: Matrix - matrix containing the between scatter
	*/
	Matrix ^Matrix::getBetweenClassScatterMatrix(Matrix ^meanMatrix1, Matrix ^meanMatrix2) {
		ExceptionClass::throwErrorIfMatrixIsEmpty(meanMatrix1->getNumRows(), meanMatrix1->getNumColumns());
		ExceptionClass::throwErrorIfMatrixIsEmpty(meanMatrix2->getNumRows(), meanMatrix2->getNumColumns());
		//check that the size of the mean matrices are equal
		ExceptionClass::throwErrorIfMatrixNotOfEqualOrder(meanMatrix1, meanMatrix2);
		int numElements = meanMatrix1->getNumRows();
		Matrix ^sB = gcnew Matrix(numElements, numElements);
		double subtractedMean1 = 0,
			subtractedMean2 = 0,
			value = 0;
		//calculate the subtracted mean
		subtractedMean1 = meanMatrix1->getCellValueAt(0, 0) - meanMatrix2->getCellValueAt(0, 0);
		subtractedMean2 = meanMatrix1->getCellValueAt(1, 0) - meanMatrix2->getCellValueAt(1, 0);
		//since it's only four values, it's done manually
		value = subtractedMean1 * subtractedMean1;
		sB->changeCellValueAt(0, 0, value);
		value = subtractedMean1 * subtractedMean2;
		sB->changeCellValueAt(0, 1, value);
		value = subtractedMean2 * subtractedMean1;
		sB->changeCellValueAt(1, 0, value);
		value = subtractedMean2 * subtractedMean2;
		sB->changeCellValueAt(1, 1, value);
		//return the matrix
		return sB;
	} //getBetweenClassScatterMatrix

	/*
	Create a matrix containing the optimal line v.
	---------------------------------------------------------
	@param invSw: Matrix - Matrix containing values to be additioned with values in matrix2
	@param meanC1: Matrix - Matrix containing values to add to values in matrix1
	@param meanC2: Matrix - Matrix containing values to add to values in matrix1
	@throw: ExceptionClass
	- Throws error if one of the matrices is empty
	- Throws error if the matrices containing the mean aren't of equal size/order
	@return: Matrix - matrix containing the list v that represents the optimal line
	*/
	Matrix ^Matrix::getOptimalLineMatrix(Matrix ^invSw, Matrix ^meanC1, Matrix ^meanC2) {
		//check that the matrices aren't empty
		ExceptionClass::throwErrorIfMatrixIsEmpty(invSw->getNumRows(), invSw->getNumColumns());
		ExceptionClass::throwErrorIfMatrixIsEmpty(meanC1->getNumRows(), meanC1->getNumColumns());
		ExceptionClass::throwErrorIfMatrixIsEmpty(meanC2->getNumRows(), meanC2->getNumColumns());
		//check that the size of the mean matrices are equal
		ExceptionClass::throwErrorIfMatrixNotOfEqualOrder(meanC1, meanC2);
		//check that the 
		String ^ errorMsg = "Can't calculate the optimal line.";
		ExceptionClass::throwErrorIfColumnAndRowCountDiffers(invSw->getNumColumns(), meanC1->getNumRows(), errorMsg);
		//create the matrix
		Matrix ^optimalLine = gcnew Matrix(invSw->getNumRows(), meanC1->getNumColumns());
		double sumValue = 0,
			subtractedMean = 0,
			invSwValue = 0;
		//loop through the matrices
		for(int i = 0; i < invSw->getNumRows(); i++) {
			for(int j = 0; j < meanC1->getNumColumns(); j++) {
				for(int k = 0; k < invSw->getNumColumns(); k++) {
					//get the values, subtract the means and add the
					//multiplied result of the inverse and subtracted mean
					invSwValue = invSw->getCellValueAt(i, k);
					subtractedMean = meanC1->getCellValueAt(k, j) - meanC2->getCellValueAt(k, j);
					sumValue += invSwValue * subtractedMean;
				} //for
				//set value and reset summation variable
				optimalLine->changeCellValueAt(i, j, sumValue);
				sumValue = 0;
			} //for
		} //for
		return optimalLine;
	} //getOptimalLineMatrix

	/*
	Additions the two matrices that are passed if they are of equal size
	(equal row - and column count).
	Throws error if count isn't equal.
	---------------------------------------------------------
	@param matrix1: Matrix - Matrix containing values to be additioned with values in matrix2
	@param matrix2: Matrix - Matrix containing values to add to values in matrix1
	@throw: ExceptionClass
	- Throws error if matrix is empty
	- Throws error if matrices aren't of equal size/order
	@return: Matrix - matrix containing the result of the addition
	*/
	Matrix ^Matrix::matrixAddition(Matrix ^matrix1, Matrix ^matrix2) {
		//check that the matrices isn't empty and that the size/order is equal (rowcount and columncount)
		ExceptionClass::throwErrorIfMatrixIsEmpty(matrix1->getNumRows(), matrix1->getNumColumns());
		ExceptionClass::throwErrorIfMatrixIsEmpty(matrix2->getNumRows(), matrix2->getNumColumns());
		ExceptionClass::throwErrorIfMatrixNotOfEqualOrder(matrix1, matrix2);
		int rowCount = 0,
			columnCount = 0;
		double value = 0;
		rowCount = matrix1->getNumRows();
		columnCount = matrix1->getNumColumns();
		Matrix ^additionedMatrix = gcnew Matrix(rowCount, columnCount);
		for(int i = 0; i < rowCount; i++) {
			for(int j = 0; j < columnCount; j++) {
				value = matrix1->getCellValueAt(i, j) + matrix2->getCellValueAt(i, j);
				additionedMatrix->changeCellValueAt(i, j, value);
			} //for
		} //for
		return additionedMatrix;
	} //matrixAddition

	/*
	Subtracts the two matrices that are passed if they are of equal size
	(equal row - and column count).
	Throws error if count isn't equal.
	---------------------------------------------------------
	@param matrix1: Matrix - Matrix containing values to be subtracted from matrix2
	@param matrix2: Matrix - Matrix containing values to subtract from matrix1
	@throw: ExceptionClass
	- Throws error if matrix is empty
	- Throws error if matrices aren't of equal size/order
	@return: Matrix - matrix containing the result of the subtraction
	*/
	Matrix ^Matrix::matrixSubstraction(Matrix ^matrix1, Matrix ^matrix2) {
		//check that the matrices isn't empty and that the size/order is equal (rowcount and columncount)
		ExceptionClass::throwErrorIfMatrixIsEmpty(matrix1->getNumRows(), matrix1->getNumColumns());
		ExceptionClass::throwErrorIfMatrixIsEmpty(matrix2->getNumRows(), matrix2->getNumColumns());
		ExceptionClass::throwErrorIfMatrixNotOfEqualOrder(matrix1, matrix2);
		int rowCount = 0,
			columnCount = 0;
		double value = 0;
		rowCount = matrix1->getNumRows();
		columnCount = matrix1->getNumColumns();
		Matrix ^subtractedMatrix = gcnew Matrix(rowCount, columnCount);
		for(int i = 0; i < rowCount; i++) {
			for(int j = 0; j < columnCount; j++) {
				value = matrix1->getCellValueAt(i, j) - matrix2->getCellValueAt(i, j);
				subtractedMatrix->changeCellValueAt(i, j, value);
			} //for
		} //for
		return subtractedMatrix;
	} //matrixSubstraction

	/*
	Multiplies the cell values in this matrix with the passed value.
	Throws error if this matrix is empty.
	---------------------------------------------------------
	@param multiplyValue: double - Value to multiply with matrix cells
	@throw: ExceptionClass - Throws error if matrix is empty
	*/
	void Matrix::multiplyCellsWithValue(double multiplyValue) {
		double value = 0;
		ExceptionClass::throwErrorIfMatrixIsEmpty(this->getNumRows(), this->getNumColumns());
		for(int rowNo = 0; rowNo < this->getNumRows(); rowNo++) {
			for(int columnNo = 0; columnNo < this->getNumColumns(); columnNo++) {
				value = multiplyValue * this->getCellValueAt(rowNo, columnNo);
				this->changeCellValueAt(rowNo, columnNo, value);
			} //for
		} //for
	} //multiplyCellsWithValue

	/*
	Multiplies the two matrices. If the columnCount in matrix1 isn't equal to rowCount in
	matrix2 (requirement for matrix multiplication), then a new check is performed if
	allowSwitch is set to true by checking the rowCount in matrix1 vs columnCount in matrix2.
	If allowSwitch is true, and rowCount == columnCount, then the matrices are switched
	before multiplication is executed (Note! m1*m2 isn't necessarily equal to m2*m1).
	If the size/order isn't equal an error is thrown. An error is also thrown if either
	of the matrices are empty.
	---------------------------------------------------------
	@param matrix1: Matrix - Matrix containing values to multiply with matrix2
	@param matrix2: Matrix - Matrix containing values to multiply from matrix1
	@param allowSwitch: bool - boolean value for if a switch is allowed if number of rows in
	matrix1 isn't equal to number of columns in matrix2. If set to true; a check and attempt to
	switch is performed. If false; the row - and column count is compared and error thrown if
	they aren't equal
	@throw: ExceptionClass
	- Throws error if matrix is empty
	- Throws error if matrices aren't of equal size/order
	@return: Matrix - matrix containing the result of the multiplication
	*/
	Matrix ^Matrix::matrixMultiplication(Matrix ^matrix1, Matrix ^matrix2, bool allowSwitch) {
		//check that the matrices isn't empty
		ExceptionClass::throwErrorIfMatrixIsEmpty(matrix1->getNumRows(), matrix1->getNumColumns());
		ExceptionClass::throwErrorIfMatrixIsEmpty(matrix2->getNumRows(), matrix2->getNumColumns());
		bool isEqual = true,
			switchOrder = false;
		//check if they are equal
		if(matrix1->getNumColumns() != matrix2->getNumRows()) {
			isEqual = false;
			//is switch allowed
			if(allowSwitch) {
				//are these equal?
				if(matrix1->getNumRows() == matrix2->getNumColumns()) {
					isEqual = true;
					switchOrder = true;
				} //if
			} //if
		} //if
		//was an equality found?
		if(!isEqual) {
			throw gcnew ExceptionClass("Can't multiply the matrices, they aren't of equal size/order.\n");
		} //if
		//is the matrices to be switched before multiplying them?
		if(switchOrder) {
			return multiplyMatrices(matrix2, matrix1);
		} //if
		return multiplyMatrices(matrix1, matrix2);
	} //matrixMultiplication

	/**
	Creates a new matrice containing the result of the multiplication of
	matrix1 and matrix2. The returned matrix's size/order contains the
	rowCount of matrix1 and the columnCount of matrix2. The multiplication
	process is to take the row value of matrix1 and multiply it with the
	column value in matrix2 and add all these multiplied values together.
	*/
	Matrix ^Matrix::multiplyMatrices(Matrix ^matrix1, Matrix ^matrix2) {
		//matrix to be filled with the multiplied values
		Matrix ^multipliedMatrix = gcnew Matrix(matrix1->getNumRows(), matrix2->getNumColumns());
		double valueMatrix1 = 0,
			valueMatrix2 = 0,
			sumOfM1xM2 = 0;
		//loop through the matrices
		for(int i = 0; i < matrix1->getNumRows(); i++) {
			for(int j = 0; j < matrix2->getNumColumns(); j++) {
				for(int k = 0; k < matrix1->getNumColumns(); k++) {
					//get the values, multiply and add them together
					valueMatrix1 = matrix1->getCellValueAt(i, k);
					valueMatrix2 = matrix2->getCellValueAt(k, j);
					sumOfM1xM2 += valueMatrix1 * valueMatrix2;
				} //for
				//set the multiplied value and reset summation variable
				multipliedMatrix->changeCellValueAt(i, j, sumOfM1xM2);
				sumOfM1xM2 = 0;
			} //for
		} //for
		return multipliedMatrix;
	} //multiplyMatrices

	/*
	Calculates the Co-Correlation coefficient. First it is checked that the matrix isn't empty, 
	then the co-variance is calculated and added to a Matrix by use of the function createCoVarianceMatrix().
	If this matrix isn't empty, the values are used to calculate the Co-Correlation coefficient.
	---------------------------------------------------------------
	@throw ExceptionClass: Error if the matrix or the calculated co-variance matrix is empty
	@return double: calculated correlation co-efficient || 0 (if Co-Correlation coeff. couldn't be calculated)
	*/
	double Matrix::calculateCorrelationCoefficient() {
		double r = 0;
		try {
			ExceptionClass::throwErrorIfMatrixIsEmpty(this->getNumRows(), this->getNumColumns());
			double S_xx = 0,
				S_xy = 0,
				S_yy = 0;
			//calculate the co-variance and check that the matrix isn't empty
			Matrix ^coVarMatrix = this->createCoVarianceMatrix(this);
			ExceptionClass::throwErrorIfMatrixIsEmpty(coVarMatrix->getNumRows(), coVarMatrix->getNumColumns());
			S_xx = coVarMatrix->getCellValueAt(0, 0);
			S_xy = coVarMatrix->getCellValueAt(0, 1);
			S_yy = coVarMatrix->getCellValueAt(1, 1);
			r = (S_xy / sqrt(S_xx * S_yy));
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
		return r;
	} //calculateCorrelationCoefficient

	/********************* CALCULATE LINEAR REGRESSION - MATRIX	***************************/

	/*
	Calculate the linear regression linReg = a + bx.
	The 'a' is calculated from the function calculateRegressionA(),
	and the 'b' is calculated from the function calculateRegressionB()
	---------------------------------------------------------------
	@throw ExceptionClass:Throws error if the matrix is empty
	@return double: linear regression || 0 (if can't calculate the linear regression)
	*/
	double Matrix::calculateLinearRegression(double &a, double &b) {
		double x = 0,
			linReg = 0;
		//check that matrix has content
		ExceptionClass::throwErrorIfMatrixIsEmpty(this->getNumRows(), this->getNumColumns());
		//get the first x-value
		x = this->getCellValueAt(0, 0);
		//calculate b and a
		b = calculateRegressionB();
		a = calculateRegressionA(b);
		//calculate the linear regression
		linReg = a + (b * x);
		return linReg;
	} //calculateLinearRegression

	/*
	Calculates the value a for the linear regression linReg = a + bx.
	The formula for a = (sum(listOfY) - (b * sum(listOfX)) / n;
	---------------------------------------------------------------
	@param listOfX: List<int> - list of values for X
	@param listOfY: List<int> - list of values for Y
	@throw ExceptionClass: Error if the listOfX->Count != listOfY->Count
	@return double: the value a || 0 (if can't calculate a)
	*/
	double Matrix::calculateRegressionA(double b) {
		int n = 0;
		double a = 0,
			sumOfX = 0,
			sumOfY = 0,
			numerator = 0;
		try {
			n = this->getNumRows();
			//calculate the sum of x and y
			sumOfX = this->sumOfColumn(0);
			sumOfY = this->sumOfColumn(1);
			//check that the numerator isn't zero to avoid divide by zero error
			numerator = (sumOfY - (b * sumOfX));
			if(numerator != 0) {
				a = numerator / n;
			} //if
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
		return a;
	} //calculateRegressionA

	/*
	Calculates the value b for the linear regression linReg = a + bx.
	The formula for b is:
	1) numerator = ((n * sum(x*y) - (sum(listOfX) * sum(listOfY))
	2) denominator = ((n * sum(x*x) - sum(listOfX)^2)
	3) b = numerator / denominator
	---------------------------------------------------------------
	@param listOfX: List<int> - list of values for X
	@param listOfY: List<int> - list of values for Y
	@throw gcnew ExceptionClass: Error if the listOfX->Count != listOfY->Count
	@return double: the value b || 0 (if can't calculate b)
	*/
	double Matrix::calculateRegressionB() {
		int n = 0;
		double b = 0;
		try {
			double sumX_2 = 0,
				sumOfXY = 0,
				sumOfX = 0,
				sumOfY = 0,
				numerator = 0,
				denominator = 0;
			n = this->getNumRows();
			//calculate the sum of x and y
			sumOfX = this->sumOfColumn(0);
			sumOfY = this->sumOfColumn(1);
			sumOfXY = sumOfXtimesY();
			sumX_2 = sumOfVariableToPowerOf2();
			//get the value of the numerator
			numerator = ((n * sumOfXY) - (sumOfX * sumOfY));
			//get the value of the denominator
			denominator = ((n * sumX_2) - pow(sumOfX, 2));
			//check that they have value to avoid divide by zero 
			if(numerator != 0 && denominator != 0) {
				//calculate b
				b = numerator / denominator;
			} //if
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
		return b;
	} //calculateRegressionB

	/**
	Calculates the sum of the values in the given column
	based on the passed column no. Throws error if matrix is
	emtpy or if column number is out of range.
	*/
	double Matrix::sumOfColumn(int columnNo) {
		double sum = 0;
		String ^errorMsg = "";
		ExceptionClass::throwErrorIfMatrixIsEmpty(this->getNumRows(), this->getNumColumns());
		errorMsg = "\nCan't calculate the sum of the column.";
		ExceptionClass::throwErrorIfMatrixColumnOutOfRange(columnNo, this->getNumColumns(), errorMsg);
		for(int rowNo = 0; rowNo < this->getNumRows(); rowNo++) {
			sum += this->getCellValueAt(rowNo, columnNo);
		} //for
		return sum;
	} //sumOfColumn

	/*
	Raises each value to the power of two (x^2), and adds all the values together.
	---------------------------------------------------------------
	@return double: Sum of all values raised to the power of 2; (Sum(x^2) )
	*/
	double Matrix::sumOfVariableToPowerOf2() {
		double sumToPower = 0,
			value = 0;
		int max = 0;
		max = this->getNumRows();
		for(int rowNo = 0; rowNo < max; rowNo++) {
			value = this->getCellValueAt(rowNo, 0);
			sumToPower += pow(value, 2);
		} //for
		return sumToPower;
	} //sumOfVariableToPowerOf2

	/**
	Calculates the sum of the multiplication (x * y) and returns the value.
	The 1. column contains the x-values and the 2. column contains the y-values.
	---------------------------------------------------------------
	@return double: Sum(x * y) based on values in the 1. and 2. column
	*/
	double Matrix::sumOfXtimesY() {
		double sumToPower = 0,
			value = 0;
		int max = 0;
		max = this->getNumRows();
		for(int rowNo = 0; rowNo < max; rowNo++) {
			//since x is in colum1 and y is in column 2, just multiply them
			sumToPower += this->getCellValueAt(rowNo, 0) * this->getCellValueAt(rowNo, 1);
		} //for
		return sumToPower;
	} //sumOfXtimesY

	/***************** ALTERATION FUNCTIONS *************************/

	/**
	Deletes the array that contains the original values
	that the matrix was filled with
	*/
	void Matrix::deleteMatrix() {
		deleteTempArrayObjects(_matrixArray);
	} //deleteMatrix

	/**
	Deletes a temporary array by the use of the delete operator and
	then setting the value to zero
	*/
	void Matrix::deleteTempArrayObjects(double** tempArray) {
		delete tempArray;
		tempArray = 0;
	} //deleteTempArrayObjects

	/*
	Calculates a 2x2 matrix based on the original matrix values.
	First the determinants are decided and swapped, and at the end the
	determinant is calculated. If the determinant isn't zero, the inverse
	matrix is calculated and returned.
	---------------------------------------------------------
	@throw: ExceptionClass
	- Throws error if the determinant is zero
	- Throws error if the calling matrix isn't a 2x2 matrix
	@return: Matrix - An inverse 2x2 matrix
	*/
	Matrix ^Matrix::inverse2DMatrix() {
		ExceptionClass::throwErrorIfMatrixNot2x2(getNumRows(), getNumColumns());
		double **inverseArray = 0;
		inverseArray = createMatrixArray(getNumRows(), getNumColumns());
		int matrixSize = 2,
			rowPosition = 0,
			columnPosition = 0;
		double value = 0,
			determinantAD = 0,
			determinantBC = 0;
		//swap a and d
		rowPosition = 0;
		columnPosition = 0;
		determinantAD = _matrixArray[rowPosition][columnPosition];
		swapCellValueAt(rowPosition, columnPosition, getNumRows() - 1, getNumColumns() - 1, inverseArray);
		//swap d and a
		rowPosition = getNumRows() - 1;
		columnPosition = getNumColumns() - 1;
		determinantAD *= _matrixArray[rowPosition][columnPosition];
		//make b negative
		rowPosition = 0;
		columnPosition = getNumColumns() - 1;
		value = -_matrixArray[rowPosition][columnPosition];
		changeCellValueAt(rowPosition, columnPosition, value, inverseArray);
		determinantBC = _matrixArray[rowPosition][columnPosition];
		//make c negative
		rowPosition = getNumRows() - 1;
		columnPosition = 0;
		value = -_matrixArray[rowPosition][columnPosition];
		changeCellValueAt(rowPosition, columnPosition, value, inverseArray);
		determinantBC *= _matrixArray[rowPosition][columnPosition];
		//check that determinant isn't zero
		value = 1 / (determinantAD - determinantBC);
		if(value == 0) {
			String^ errorMsg = "Can't calculate the inverse, determinant is zero.";
			deleteTempArrayObjects(inverseArray);
			throw gcnew ExceptionClass(errorMsg);
		} //if
		Matrix ^inverseMatrix = gcnew Matrix(matrixSize, matrixSize);
		//calculate the inverse
		for(int i = 0; i < getNumRows(); i++) {
			for(int j = 0; j < getNumColumns(); j++) {
				inverseMatrix->changeCellValueAt(i, j, value * inverseArray[i][j]);
			} //for
		} //for
		deleteTempArrayObjects(inverseArray);
		return inverseMatrix;
	} //inverse2DMatrix

	/*
	Transposes the original matrix. First the values from the original matrix is
	copied, then the original matrix is deleted and reconstructed to fit the new
	rowNo and columnNo count. Finally it's filled with the new rowNo and columnNo values.
	*/
	void Matrix::transposeMatrix() {
		//check that the matrix has at least one rowNo and columnNo
		if(getNumRows() > 0 && getNumColumns() > 0) {
			double value = 0;
			int oldRowCount = 0,
				oldColumnCount = 0,
				newRowCount = 0,
				newColumnCount = 0;
			double ** tempArray = 0;
			//get current count for rows and columns
			oldRowCount = getNumRows();
			oldColumnCount = getNumColumns();
			//set the new count for rows and columns (tranpose)
			newRowCount = oldColumnCount;
			newColumnCount = oldRowCount;
			//copy the content of the current matrix
			tempArray = createMatrixArray(oldRowCount, oldColumnCount);
			for(int i = 0; i < oldRowCount; i++) {
				for(int j = 0; j < oldColumnCount; j++) {
					tempArray[i][j] = _matrixArray[i][j];
				} //for
			} //for
			//delete the old matrix and recreate it
			deleteMatrix();
			_matrixArray = createMatrixArray(newRowCount, newColumnCount);
			//set new range for _matrixArray's rowNo and columnNo count
			setNewRowCount(newRowCount);
			setNewColumnCount(newColumnCount);
			//fill _matrixArray with the tranposed values
			for(int i = 0; i < oldRowCount; i++) {
				for(int j = 0; j < oldColumnCount; j++) {
					//getting tranposed position and value
					newRowCount = j;
					newColumnCount = i;
					value = tempArray[i][j];
					//add the value to _matrixArray
					changeCellValueAt(newRowCount, newColumnCount, value);
				} //for
			} //for
			deleteTempArrayObjects(tempArray);
		} //if
	} //transposeMatrix

	/*
	Transposes the passed matrix transposeArray. First the values from are
	copied, then transposeArray is deleted and reconstructed to fit the new
	rowNo and columnNo count. Finally it's filled with the new rowNo and columnNo values.
	---------------------------------------------------------
	@param numRows - int: The transposeArray's original rowNo count
	@param numColumns - int: The transposeArray's original columnNo count
	@param transposeArray - double**: The array to transpose
	*/
	void Matrix::transposeMatrix(int numRows, int numColumns, double **transposeArray) {
		//check that the matrix has at least one rowNo and columnNo
		if(numRows > 0 && numColumns > 0) {
			double value = 0;
			int oldRowCount = 0,
				oldColumnCount = 0,
				newRowCount = 0,
				newColumnCount = 0;
			double ** tempArray = 0;
			//get current count for rows and columns
			oldRowCount = numRows;
			oldColumnCount = numColumns;
			//set the new count for rows and columns (tranpose)
			newRowCount = oldColumnCount;
			newColumnCount = oldRowCount;
			//copy the content of the current matrix
			tempArray = createMatrixArray(oldRowCount, oldColumnCount);
			for(int i = 0; i < oldRowCount; i++) {
				for(int j = 0; j < oldColumnCount; j++) {
					tempArray[i][j] = transposeArray[i][j];
				} //for
			} //for
			//delete the passed matrix and recreate it
			deleteTempArrayObjects(transposeArray);
			transposeArray = createMatrixArray(newRowCount, newColumnCount);
			//fill transposeArray with the tranposed values
			for(int i = 0; i < oldRowCount; i++) {
				for(int j = 0; j < oldColumnCount; j++) {
					//getting tranposed position and value
					newRowCount = j;
					newColumnCount = i;
					value = tempArray[i][j];
					//add the value to _matrixArray
					changeCellValueAt(newRowCount, newColumnCount, value, transposeArray);
				} //for
			} //for
			deleteTempArrayObjects(tempArray);
		} //if
	} //transposeMatrix

	/*
	Fills the matrix with values. The way the matrix is filled is that it
	takes the passed columnNo and fills up that columnNo number with the values
	from the List<double> values. Example: Say that the columnNo passed is
	column1. Then, for each value in list, the column1's row1, row2, ..., rowN
	would be filled with the given values from the list.
	Throws error if the columnNo isn't within range, the list is empty
	or the size of the list is greater then the matrix rowcount.
	---------------------------------------------------------
	@param columnNo: int - The columnNo to fill with values
	@throw: ExceptionClass - Throws error if:
	- The list is empty
	- The columnNo value is out of range
	- The number of values in the list is greater then the number
	of rows (since value goes into 'columnNo' and row1, 2, ..., n)
	@param values: List<double> - The values to fill up in this column
	*/
	void Matrix::fillMatrixWithValues(int columnNo, List<double> ^%values) {
		try {
			String^ errorMsg = "";
			int max_size = 0;
			ExceptionClass::throwErrorIfValueListIsEmpty(values);
			//check that the number of columns is within range
			errorMsg = "\nCan't fill matrix with values.";
			ExceptionClass::throwErrorIfMatrixColumnOutOfRange(columnNo, getNumColumns(), errorMsg);
			//check that the list count is equal to the number of columns
			max_size = values->Count;
			int rowCount = getNumRows();
			errorMsg = "The number of row values is higher then the row count. \nCan't fill matrix with values.";
			ExceptionClass::throwErrorIfSizeGreaterThenValue(max_size, rowCount, errorMsg);
			//fill the matrix array with rowNo values from the list
			for(int i = 0; i < max_size; i++) {
				_matrixArray[i][columnNo] = values[i];
			} //for
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
	} //fillMatrixWithValues

	/**
	*/
	void Matrix::fillMatrixWithValues(List<List<double> ^> ^%valueList) {
		try {
			double value = 0;
			//fill the matrix with the values
			for(int columnNo = 0; columnNo < getNumColumns(); columnNo++) {
				for(int rowNo = 0; rowNo < getNumRows(); rowNo++) {
					value = valueList[columnNo]->ToArray()[rowNo];
					this->changeCellValueAt(rowNo, columnNo, value);
				} //for
			} //for
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
	} //fillMatrixWithValues

	/**
	Fills the matrix with values found in the training dataset.
	*/
	void Matrix::fillMatrixWithValues(List<Training^> ^%trainingData) {
		try {
			//is the number of attributes two?
			if(getNumColumns() == 2) {
				int columnNo1 = 0,
					columnNo2 = 1;
				//fill matrix with two types of attributes 
				for(int rowNo = 0; rowNo < getNumRows(); rowNo++) {
					this->changeCellValueAt(rowNo, columnNo1, trainingData[rowNo]->getAttributeValue1());
					this->changeCellValueAt(rowNo, columnNo2, trainingData[rowNo]->getAttributeValue2());
				} //for
			} else {
				double value = 0;
				//fill the matrix array with n-dimensional no. of attributes
				for(int rowNo = 0; rowNo < getNumRows(); rowNo++) {
					for(int columnNo = 0; columnNo < getNumColumns(); columnNo++) {
						value = trainingData[rowNo]->getAttributeValues()[columnNo];
						this->changeCellValueAt(rowNo, columnNo, value);
					} //for
				} //for
			} //if
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
	} //fillMatrixWithValues


	/*
	Creates a new matrix array filled with zero's. The matrix arrays
	size is based on the passed values numRows and numColumns.
	---------------------------------------------------------
	@param numRows: int - The number of rows the matrix shall have
	@param numColumns: int - The number of columns the matrix shall have
	*/
	double** Matrix::createMatrixArray(int numRows, int numColumns) {
		//create a new array and fill it with rows and columns
		double** newArray = 0;
		newArray = new double*[numRows];
		for(int i = 0; i < numRows; i++) {
			newArray[i] = new double[numColumns];
		} //for
		//fill matrix with default value (zero)
		for(int i = 0; i < numRows; i++) {
			for(int j = 0; j < numColumns; j++) {
				newArray[i][j] = 0;
			} //for
		} //for
		return newArray;
	} //createMatrixArray

	/*
	Changes the cell value at given position in the original matrix.
	Throws error if the passed rowNo/columnNo value isn't within range.
	---------------------------------------------------------
	@param rowNo: int - The rowNo position to change value at
	@param columnNo: int - The columnNo position to change value at
	@param value: double - The new value
	@throw: ExceptionClass - Throws error if rowNo/columnNo isn't within range
	*/
	void Matrix::changeCellValueAt(int rowNo, int columnNo, double value) {
		try {
			String^ errorMsg = "";
			//check that rowNo value is legal
			errorMsg = "\nCan't change value at position ["
				+ Conversion::convertIntegerToString(rowNo) + "," + Conversion::convertIntegerToString(columnNo) + "]";
			ExceptionClass::throwErrorIfMatrixRowOutOfRange(rowNo, getNumRows(), errorMsg);
			//check that columnNo value is legal
			errorMsg = "\nCan't change value at position ["
				+ Conversion::convertIntegerToString(rowNo) + "," + Conversion::convertIntegerToString(columnNo) + "]";
			ExceptionClass::throwErrorIfMatrixColumnOutOfRange(columnNo, getNumColumns(), errorMsg);
			_matrixArray[rowNo][columnNo] = value;
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
	} //changeCellValueAt

	/*
	Retrieves the value from the original matrix's [fromRow][fromColumn]
	and moves the value to position [toRow][toColumn] in the passed matrixArray.
	Then does the same, but this time moves the value from [toRow][toColumn] in the
	original matrix and puts it in [fromRow][fromColumn] (in matrixArray).
	---------------------------------------------------------
	@param fromRow - int: The original rowNo
	@param fromColumn - int: The original columnNo
	@param toRow - int: The place to put the value from fromRow
	@param toColumn - int: The place to put the value from fromColumn
	@param matrixArray: double** - the array that's to get it's value swapped
	*/
	void Matrix::swapCellValueAt(const int fromRow, const int fromColumn, const int toRow, const int toColumn, double **matrixArray) {
		double currentValue = 0;
		currentValue = _matrixArray[fromRow][fromColumn];
		changeCellValueAt(toRow, toColumn, currentValue, matrixArray);
		currentValue = _matrixArray[toRow][toColumn];
		changeCellValueAt(fromRow, fromColumn, currentValue, matrixArray);
	} //swapCellValueAt

	/*
	Changes the cell value at given position in the passed array.
	---------------------------------------------------------
	@param rowNo: int - The rowNo position to change value at
	@param columnNo: int - The columnNo position to change value at
	@param value: double - The new value
	@param matrixArray: double** - The array to change value in
	*/
	void Matrix::changeCellValueAt(int rowNo, int columnNo, double value, double **matrixArray) {
		matrixArray[rowNo][columnNo] = value;
	} //changeCellValueAt

	void Matrix::setNewRowCount(int numRows) {
		_numRows = numRows;
	} //setNewRowCount

	void Matrix::setNewColumnCount(int numColumns) {
		_numColumns = numColumns;
	} //setNewColumnCount

	/***************** GET FUNCTIONS *************************/

	int Matrix::getNumRows() {
		return _numRows;
	} //getNumRows

	int Matrix::getNumColumns() {
		return _numColumns;
	} //getNumColumns

	double** Matrix::getMatrix() {
		return _matrixArray;
	} //getMatrix

	/*
	Function to create and possibly return an identity matrix.
	Currently not created.
	*/
	void Matrix::getIdentityMatrix() {
		//TODO:
		//by use of possibly passed values, 
		//return (?) a N x M I-matrix
		throw gcnew NotImplementedException("Function not created. Can't return Identity matrix.");
	} //getIdentityMatrix

	/*
	Retrieves the matrix's cell value at the given position
	(based on rowNo and columnNo value). Throws error if the passed rowNo/columnNo
	value isn't within range.
	---------------------------------------------------------
	@param rowNo: int - The rowNo position to retrieve the value from
	@param columnNo: int - The columnNo position to retrieve the value from
	@throw: ExceptionClass - Throws error if rowNo/columnNo isn't within range
	@return double: the value at given position (_matrixArray[rowNo][columnNo])
	*/
	double Matrix::getCellValueAt(const int rowNo, const int columnNo) {
		double cellValue = 0;
		try {
			String^ errorMsg = "";
			//check that rowNo value is legal
			errorMsg = "\nCan't retrieve value at position ["
				+ Conversion::convertIntegerToString(rowNo) + "," + Conversion::convertIntegerToString(columnNo) + "]";
			ExceptionClass::throwErrorIfMatrixRowOutOfRange(rowNo, getNumRows(), errorMsg);
			//check that columnNo value is legal
			errorMsg = "\nCan't retrieve value at position ["
				+ Conversion::convertIntegerToString(rowNo) + "," + Conversion::convertIntegerToString(columnNo) + "]";
			ExceptionClass::throwErrorIfMatrixColumnOutOfRange(columnNo, getNumColumns(), errorMsg);
			cellValue = _matrixArray[rowNo][columnNo];
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
		return cellValue;
	} //getCellValueAt

} //namespace