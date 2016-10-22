#include "Stdafx.h"
#include "statistics.h"
#include "exceptionclass.h"
#include <cmath>

namespace MachineLearning {

	/*
	Default constructor
	*/
	Statistics::Statistics() {

	} //constructor

	/*
	Destructor
	*/
	Statistics::~Statistics() {

	} //destructor

	/*
	Calculate the mean based on the values in the passed list.
	Requires a list filled with integers.
	---------------------------------------------------------------
	@param valueList: List<double> - list of values for calculating the mean
	@return double: mean || 0 (if mean couldn't be calculated)
	*/
	double Statistics::calculateMean(List<int> ^valueList) {
		double mean = 0;
		int n = 0;
		n = static_cast<int>(valueList->Count);
		mean = sumOfList(valueList);
		//check that mean isn't zero 
		//(to avoid divide-by-zero error)
		if(mean != 0) {
			mean = mean / n;
		} //if
		return mean;
	} //calculateMean

	/*
	Calculate the mean based on the values in the passed list.
	Requires a list filled with double.
	---------------------------------------------------------------
	@param valueList: List<double> - list of values for calculating the mean
	@return double: mean || 0 (if mean couldn't be calculated)
	*/
	double Statistics::calculateMean(List<double> ^valueList) {
		double mean = 0;
		int n = 0;
		n = static_cast<int>(valueList->Count);
		mean = sumOfList(valueList);
		//check that mean isn't zero 
		//(to avoid divide-by-zero error)
		if(mean != 0) {
			mean = mean / n;
		} //if
		return mean;
	} //calculateMean

	/*
	Calculates the variance for the values passed in the list valueList.
	---------------------------------------------------------------
	@param valueList: List<double> - The list of values to be used to calculate the variance
	@param mean: double - the mean based on the values in valueList
	@return double: calculated variance || 0 (if variance couldn't be calculated)
	*/
	double Statistics::calculateVariance(List<double> ^valueList, double mean) {
		double variance = 0,
			sumOfValues = 0;
		int n = 0;
		n = static_cast<int>(valueList->Count);
		//calculate the sum of the values
		for(int i = 0; i < n; i++) {
			sumOfValues = valueList[i] - mean;
			variance += pow(sumOfValues, 2);
		} //for
		//check that variance isn't zero 
		//(to avoid divide-by-zero error)
		if(variance != 0) {
			variance = variance / (n - 1);
		} //if
		return variance;
	} //calculateVariance

	/*
	Calculates the standard deviance. It checks that the passed variance is
	greater then zero and then uses sqrt() to calculate the stdDev
	---------------------------------------------------------------
	@param variance: double - the variance for the the value to get stdDev
	@return double: calculated standard deviance || 0 (if stdDev couldn't be calculated)
	*/
	double Statistics::calculateStdDev(double variance) {
		double stdDev = 0;
		if(variance > 0) {
			stdDev = sqrt(variance);
		} //if
		return stdDev;
	} //calculateStdDev

	/*
	Calculate the co-variance by the use of the two passed lists.
	The first list should contain the x-values and the second list the
	y-values. The function first calculates the mean for the x - and y-values,
	and if mean was successfully calculated, the co-variance is then calculated.
	---------------------------------------------------------------
	@param listOfX: List<int> - list of values for X
	@param listOfY: List<int> - list of values for Y
	@throw gcnew ExceptionClass: Error if the listOfX->Count != listOfY->Count
	@return double: the co-variance || 0 (if co-variance couldn't be calculated)
	*/
	double Statistics::calculateCoVariance(List<double> ^listOfX, List<double> ^listOfY) {
		double coVariance = 0;
		try {
			double currentX = 0,
				currentY = 0,
				meanX = 0,
				meanY = 0;
			int n = 0;
			//check that the lists is of equal size
			ExceptionClass::throwErrorIflistSizeIsNotEqual(listOfX, listOfY, FAILED_AT_VARIANCE_CALCULATION);
			n = static_cast<int>(listOfX->Count);
			//calculate mean
			meanX = calculateMean(listOfX);
			meanY = calculateMean(listOfY);
			//loop through the values and calculate the sum 
			for(int i = 0; i < n; i++) {
				currentX = listOfX[i] - meanX;
				currentY = listOfY[i] - meanY;
				coVariance += currentX * currentY;
			} //for
			//if the sum was calculated, calculate the covariance
			if(coVariance != 0) {
				coVariance = coVariance / (n - 1);
			} //if
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
		return coVariance;
	} //calculateCoVariance

	/*
	Calculates the k1. It checks that the passed variance isn't zero and 
	that the stdDev is greater then zero, and then calculates and returns k1.
	---------------------------------------------------------------
	@param totVariance: double - the total variance for the x - and y-values
	@param totStdDev: double - the total stdDev for the x - and y-values
	@return double: calculated k1 || 0 (if k1 couldn't be calculated)
	*/
	double Statistics::calculateK1(double totVariance, double totStdDev) {
		double k1 = 0;
		if(totVariance != 0 && totStdDev > 0) {
			k1 = totStdDev / totVariance;
		} //if
		return k1;
	} //calculateK1

	/*
	Calculates the correlation co-efficient.
	The function first checks that the lists passed is of equal size, and then
	calculates (in following order):
	- the mean for x and y
	- the variance for x and y
	- the value for stdDevX * stdDevY (multiStdDev)
	- the co-variance for x and y
	- checks that co-variance and multiStdDev was calculated,
	then calculates and returns the correlation co-efficient
	---------------------------------------------------------------
	@param listOfX: List<int> - list of values for X
	@param listOfY: List<int> - list of values for Y
	@throw gcnew ExceptionClass: Error if the listOfX->Count != listOfY->Count
	@return double: calculated correlation co-efficient || 0 (if corr. co-effic. couldn't be calculated)
	*/
	double Statistics::calculateCorrelationCoefficient(List<double> ^listOfX, List<double> ^listOfY) {
		double r = 0;
		try {
			double coVariance = 0,
				meanX = 0,
				meanY = 0,
				varianceX = 0,
				varianceY = 0,
				multiStdDev = 0;
			//check that the lists is of equal size
			ExceptionClass::throwErrorIflistSizeIsNotEqual(listOfX, listOfY, FAILED_AT_VARIANCE_CALCULATION);
			//calculate the mean of the x - and y-values
			meanX = calculateMean(listOfX);
			meanY = calculateMean(listOfY);
			//calculate the variance for the x - and y-values
			varianceX = calculateVariance(listOfX, meanX);
			varianceY = calculateVariance(listOfY, meanY);
			//calculate the value for Sx * Sy
			multiStdDev = calculateStdDev(varianceX) * calculateStdDev(varianceY);
			//calculate the co-variance
			coVariance = calculateCoVariance(listOfX, listOfY);
			//check that the co-variance and the multiplication of multiStdDev was calculated
			if(coVariance != 0 && multiStdDev != 0) {
				//calculate the correlation co-efficient
				r = coVariance / multiStdDev;
			} //if
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
		return r;
	} //calculateCorrelationCoefficient


	/********************* CALCULATE LINEAR REGRESSION - LISTS ***************************/

	/*

	Calculate the linear regression linReg = a + bx.
	The 'a' is calculated from the function calculateRegressionA(),
	and the 'b' is calculated from the function calculateRegressionB()
	---------------------------------------------------------------
	@param listOfX: List<int> - list of values for X
	@param listOfY: List<int> - list of values for Y
	@param x: double - the value for x to be calculated
	@throw gcnew ExceptionClass: Error if the listOfX->Count != listOfY->Count
	@return double: linear regression || 0 (if can't calculate the linear regression)
	*/
	double Statistics::calculateLinearRegression(List<double> ^listOfX, List<double> ^listOfY, double x, double &a, double &b) {
		double linReg = 0;
		//calculate b and a
		b = calculateRegressionB(listOfX, listOfY);
		a = calculateRegressionA(listOfX, listOfY, b);
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
	double Statistics::calculateRegressionA(List<double> ^listOfX, List<double> ^listOfY, double b) {
		int n = 0;
		double a = 0,
			sumListX = 0,
			sumListY = 0,
			numerator = 0;
		try {
			//check that the lists is of equal size
			ExceptionClass::throwErrorIflistSizeIsNotEqual(listOfX, listOfY, FAILED_AT_VARIANCE_CALCULATION);
			n = static_cast<int>(listOfX->Count);
			//calculate the sum of x and y
			sumListX = sumOfList(listOfX);
			sumListY = sumOfList(listOfY);
			//check that the numerator isn't zero to avoid divide by zero error
			numerator = (sumListY - (b * sumListX));
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
	double Statistics::calculateRegressionB(List<double> ^listOfX, List<double> ^listOfY) {
		int n = 0;
		double b = 0;
		try {
			double numerator = 0,
				denominator = 0,
				sumListX = 0,
				sumListY = 0,
				sumXY = 0,
				sumX_2;
			//check that the lists is of equal size
			ExceptionClass::throwErrorIflistSizeIsNotEqual(listOfX, listOfY, FAILED_AT_VARIANCE_CALCULATION);
			n = static_cast<int>(listOfX->Count);
			//calculate the sum, sum(x*y), sum(x^2)
			sumListX = sumOfList(listOfX);
			sumListY = sumOfList(listOfY);
			sumXY = sumOfXtimesY(listOfX, listOfY);
			sumX_2 = sumOfVariableToPowerOf2(listOfX);
			//get the value of the numerator
			numerator = ((n * sumXY) - (sumListX * sumListY));
			//get the value of the denominator
			denominator = ((n * sumX_2) - pow(sumListX, 2));
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



	/************************ PRIVATE FUNCTIONS *********************************/

	/*
	Calculates the sum for all the values in passed valueList.
	Requires a list filled with integers.
	---------------------------------------------------------------
	@param valueList: List<int> - list of values to calculate the sum of
	@return int: the sum of the passed values in the list valueList
	*/
	int Statistics::sumOfList(List<int> ^valueList) {
		int max = 0,
			sum = 0;
		max = static_cast<int>(valueList->Count);
		for(int i = 0; i < max; i++) {
			sum += valueList[i];
		} //for
		return sum;
	} //sumOfList

	/*
	Calculates the sum for all the values in passed valueList.
	Requires a list filled with double.
	---------------------------------------------------------------
	@param valueList: List<int> - list of values to calculate the sum of
	@return double: the sum of the passed values in the list valueList
	*/
	double Statistics::sumOfList(List<double> ^valueList) {
		int max = 0;
		double sum = 0;
		max = static_cast<int>(valueList->Count);
		for(int i = 0; i < max; i++) {
			sum += valueList[i];
		} //for
		return sum;
	} //sumOfList

	/*
	Calculates the sum the values in the two lists (x * y).
	---------------------------------------------------------------
	@param listOfX: List<int> - list of values for X
	@param listOfY: List<int> - list of values for Y
	@throw gcnew ExceptionClass: Error if the listOfX->Count != listOfY->Count
	@return double: Sum(x * y) for the values in listOfX and listOfY
	*/
	double Statistics::sumOfXtimesY(List<double> ^listOfX, List<double> ^listOfY) {
		int max = 0;
		double x = 0,
			y = 0,
			sumOfXY = 0;
		try {
			//check that the lists is of equal size
			ExceptionClass::throwErrorIflistSizeIsNotEqual(listOfX, listOfY, FAILED_AT_VARIANCE_CALCULATION);
			max = static_cast<int>(listOfX->Count);
			for(int i = 0; i < max; i++) {
				x = listOfX[i];
				y = listOfY[i];
				sumOfXY += x * y;
			} //for
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
		return sumOfXY;
	} //sumOfXtimesY

	/*
	Raises each value to the power of two (x^2), and adds all the values together.
	---------------------------------------------------------------
	@param valueList: List<int> - the values to raise to power of two
	@return double: Sum of all values raised to the power of 2 (Sum(x^2) )
	*/
	double Statistics::sumOfVariableToPowerOf2(List<double> ^valueList) {
		double sumToPower = 0,
			value = 0;
		int max = 0;
		max = static_cast<int>(valueList->Count);
		for(int i = 0; i < max; i++) {
			value = valueList[i];
			sumToPower += pow(value, 2);
		} //for
		return sumToPower;
	} //sumOfVariableToPower
} //namespace