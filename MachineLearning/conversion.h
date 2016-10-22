#pragma once

#ifndef CONVERSION_H

#define CONVERSION_H

#include "Stdafx.h"
using namespace System;
using namespace System::Collections::Generic;

namespace MachineLearning {
	
	/*
	public ref class that handles conversion from String ^ to numeric and vica versa.
	If String ^ cannot be converted, a constant with default value is returned.
	-----------------------------------------
	@Author: Knut Lucas Andersen
	*/
	public ref class Conversion sealed {
	public:
		//default constructor
		Conversion();
		//destructor
		~Conversion();
		//convert integer to String^
		static String ^convertIntegerToString(int number);
		//convert double to String^
		static String ^convertDoubleToString(double number);
		//convert String^ to integer
		static int convertStringToInteger(String ^number);
		//convert String^ to double
		static double convertStringToDouble(String ^number);
		//convert from integer to binary 
		static List<String ^>^ binaryConverter(List<int> ^valueList);
		//convert from double to binary
		static List<String ^>^ binaryConverter(List<double> ^valueList, const int decimalLength);
		//convert a binary String^ to an integer
		static int convertBinaryStringToInteger(String ^binaryValue, bool isDouble);
		//convert a binary String^ to a double
		//NOTE! to find out where value stops and decimal starts,
		//count backwards. If decimal length = 5, then count 
		//5 decimals from the back
		static double convertBinaryStringToDouble(String ^binary, const int decimalLength);
		//constant used when input isn't a number
		static const signed int INPUT_NOT_A_NUMBER = -9999;
		//convert System::String^ to std::string
		static void MarshalString(String ^s, std::string& os);
		//convert System::String^ to std::wstring
		static void MarshalString(String ^s, std::wstring& os);
		//convert convert System::String^ to character
		static const char *convertStringToChar(String ^convString);
	private:
		//divides the passed value until it reaches zero
		static void divideUntilZero(int value, String ^%reverseBinary);
		//divides the passed value until it reaches zero
		static void divideDoubleUntilZero(double value, String ^%reverseBinary);
		//subtract the decimal value from a divisor until zero or no more decimals to convert
		static void subtractDecimalUntilZero(double decimalValue, String ^%reverseBinary, int raisedPower, int decimalLength, int counter);
		//divides the passed value until it reaches zero
		static void createBinaryStringFromDouble(String ^%strDouble, String ^%reverseBinary, const int decimalLength);
		//reverses the passed String ^ (e.g. "abc" becomes "cba")
		static String ^reverseString(String ^text);
	}; //Conversion
} //namespace
#endif