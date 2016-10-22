#pragma once

#ifndef MATRIX_H

#define MATRIX_H

#include "Stdafx.h"
#include "training.h"

namespace MachineLearning {

	/*
	Class functioning as a matrix by use of a 2D array.
	The class has the ability to:
	- Create a mean matrix
	- Calculate corrected mean
	- Calculate the variance
	- Create a co-variance matrix
	- Addition, subtraction and multiplication of matrices
	- Create a scatter matrix and between class scatter matrix
	- Create a matrix containing the optimal line (w)
	- Transpose the matrix (or transpose a temporary matrix)
	- Inverse a 2D matrix
	- Create an identity matrix (not implemented)
	-----------------------------------------
	@Author: Knut Lucas Andersen
	*/
	public ref class Matrix sealed {
	public:
		//default constructor
		Matrix();
		//constructor creating a matrix of size numRows x numColumns
		Matrix(int numRows, int numColumns);
		//constructor creating a matrix filled with values from passed list (double)
		Matrix(List<List<double> ^> ^valueList);
		//constructor creating a matrix filled with values from passed list (Training)
		Matrix(List<Training ^> ^trainingData);
		//destructor
		~Matrix();

		/***************** CALCULATION FUNCTIONS *************************/
		//calculate the mean for given columnNo
		double calculateMean(int columnNo);
		//create a matrix containing the mean value
		Matrix ^createMeanMatrix();
		//calculate the corrected mean for given cell value in matrix
		//(instead of subtracting Class1.mean, one subtracts the mean for the whole data set)
		double calculateCorrectedMean(int rowNo, int columnNo, double globalMean);
		//calculate the variance for the given columnNo
		double calculateVariance(int columnNo);
		//create a co-variance matrix by using the values in parent
		Matrix ^createCoVarianceMatrix(Matrix ^parent);
		//create a scatter matrix
		Matrix ^createScatterMatrix(Matrix ^coVarParent, Matrix ^coVarMatrix);
		//addition of passed matrices 
		Matrix ^matrixAddition(Matrix ^matrix1, Matrix ^matrix2);
		//subtraction of passed matrices
		Matrix ^matrixSubstraction(Matrix ^matrix1, Matrix ^matrix2);
		//multiplication of matrix cells with value
		void multiplyCellsWithValue(double multiplyValue);
		//multiplication of passed matrices; matrix1.Row x matrix2.Columns
		Matrix ^matrixMultiplication(Matrix ^matrix1, Matrix ^matrix2, bool allowSwitch);
		//calculate the optimal line for the two classes
		Matrix ^getOptimalLineMatrix(Matrix ^invSw, Matrix ^meanC1, Matrix ^meanC2);
		//calculate between class scatter matrix
		Matrix ^getBetweenClassScatterMatrix(Matrix ^meanMatrix1, Matrix ^meanMatrix2);
		//
		double calculateCorrelationCoefficient();
		//calculates the linear regression for the content in this matrix
		double calculateLinearRegression(double &a, double &b);
		//calculate the sum of the given column
		double sumOfColumn(int columnNo);

		/***************** ALTERATION FUNCTIONS *************************/
		//delete the original matrix (cleanup function)
		void deleteMatrix();
		//transpose the matrix
		void transposeMatrix();
		//create an inverse 2x2 matrix based on the original matrix
		Matrix ^inverse2DMatrix();
		//fill the matrix columnNo with values from the list (double)
		void fillMatrixWithValues(int columnNo, List<double> ^%valueData);
		//change value in given cell in the original matrix
		void changeCellValueAt(int rowNo, int columnNo, double value);

		/***************** GET FUNCTIONS *************************/
		//get rowNo count
		int getNumRows();
		//get columnNo count
		int getNumColumns();
		//get the array containing the matrix
		double** getMatrix();
		//get identity matrix (not created)
		void getIdentityMatrix();
		//get cell value from given position
		double getCellValueAt(const int rowNo, const int columnNo);

	private:
		//the number of rows in the matrix
		int _numRows;
		//the number of columns in the matrix
		int _numColumns;
		//array containing the matrix values
		double **_matrixArray;
		//the value at given row and column
		double _cellValue;
		//create a new matrix
		double **createMatrixArray(int numRows, int numColumns);
		//set new rowcount
		void setNewRowCount(int numRows);
		//set new columncount
		void setNewColumnCount(int numColumns);
		//calculate the co-variance for the two passed columns
		double calculateCoVariance(int columnX, int columnY);
		//multiply the passed matrices
		Matrix ^multiplyMatrices(Matrix ^matrix1, Matrix ^matrix2);
		//delete arrays that were used temporary (placeholders, etc - cleanup function)
		void deleteTempArrayObjects(double** tempArray);
		//change cellvalue at given position in passed array (i.e. in a transposed array)
		void changeCellValueAt(int rowNo, int columnNo, double value, double **transposeArray);
		//transpose a passed matrix with the size based on passed rowNo - and columnNo count
		void transposeMatrix(int numRows, int numColumns, double **transposeArray);
		//swap the values at fromRow, fromColumn to toRow, toColumn (and vica versa) in the passed matrix
		void swapCellValueAt(const int fromRow, const int fromColumn, const int toRow, const int toColumn, double **matrixArray);

		/*** ALTERATION FUNCTIONS (made private since they are called through the constructor) ***/

		//fill the matrix columnNo with values from the list (double)
		void fillMatrixWithValues(List<List<double> ^> ^%values);
		//fill the matrix columnNo with values from the list (training)
		void fillMatrixWithValues(List<Training^> ^%trainingData);

		/*** REGRESSION FUNCTIONS ***/

		//calculates the a-part in the linear regression formula (matrix)
		double calculateRegressionA(double b);
		//calculates the b-part in the linear regression formula (matrix)
		double calculateRegressionB();
		//calculates the sum of all the x-values raised to the power of 2
		double sumOfVariableToPowerOf2();
		//calculates the sum of (x * y)
		double sumOfXtimesY();
	}; //Matrix
} //namespace
#endif