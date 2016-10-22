#pragma once

#ifndef STATISTICS_H

#define STATISTICS_H	

#include "Stdafx.h"

namespace MachineLearning {
	
	/*
	Class basically for statistics calculations. 
	Calculates statistics like: 
	- Calculate mean
	- Calculate variance
	- Calculate standard deviation
	- Calculate co-variance
	- Calculate correlation co-efficient
	- Calculate linear regression
	-----------------------------------------
	@Author: Knut Lucas Andersen
	*/
	public ref class Statistics sealed {
	public:
		//default constructor
		Statistics();
		//destructor
		~Statistics();
		//constant used in error messages when failing at variance calculations
		static String ^ FAILED_AT_VARIANCE_CALCULATION = "\nCan't calculate the co-variance.";
		//calculates the mean/average for the content in the passed list (integer)
		double calculateMean(List<int> ^valueList);
		//calculates the mean/average for the content in the passed list (double)
		double calculateMean(List<double> ^valueList);
		//calculates the variance based on the values in passed list and mean
		double calculateVariance(List<double> ^valueList, double mean);
		//calculates the standard deviance based on the passed variance
		double calculateStdDev(double variance);
		//calculates k1
		double calculateK1 (double variance, double stdDev);
		//calculates the co-variance
		double calculateCoVariance(List<double> ^listOfX, List<double> ^listOfY); 
		//calculates the correlation co-efficient
		double calculateCorrelationCoefficient(List<double> ^listOfX, List<double> ^listOfY);
		//calculates the linear regression (based on the x - and y - values in the passed lists)
		double calculateLinearRegression(List<double> ^listOfX, List<double> ^listOfY, double x, double &a, double &b);
	private:
		//returns the sum of the values in the passed list (integer)
		int sumOfList(List<int> ^valueList);
		//returns the sum of the values in the passed list (double)
		double sumOfList(List<double> ^valueList);
		//returns the sum of the result of the multiplication of the two lists
		double sumOfXtimesY(List<double> ^listOfX, List<double> ^listOfY);
		//returns the sum of the values in the passed list raised to the power of 2
		double sumOfVariableToPowerOf2(List<double> ^valueList);
		//calculates the a-part in the linear regression formula (list)
		double calculateRegressionA(List<double> ^listOfX, List<double> ^listOfY, double b);
		//calculates the b-part in the linear regression formula (list)
		double calculateRegressionB(List<double> ^listOfX, List<double> ^listOfY);
	}; //Statistics
} //namespace
#endif