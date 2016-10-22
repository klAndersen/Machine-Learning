#pragma once

#ifndef EXCEPTION_CLASS_H

#define EXCEPTION_CLASS_H	

#include "Stdafx.h"
#include "matrix.h"

namespace MachineLearning {
	
	/*
	Class that handles the exceptions occuring for the different classes.
	Mainly made to create more readable code and keep all exception messages
	created within one class.
	-----------------------------------------
	@Author: Knut Lucas Andersen
	*/
	public ref class ExceptionClass sealed : public Exception {
	public:
		//default constructor
		ExceptionClass();
		//constructor setting error message
		ExceptionClass(String ^ errorMsg);
		//constructor setting error message and title
		ExceptionClass(String ^ errorMsg, String ^ errorTitle);
		//destructor
		~ExceptionClass();
		//override error message retrieval (returns error message)
		String ^ ToString() override;
		//returns the error title
		String ^ getExceptionTitle();

		/***************** MATRIX CLASS EXCEPTIONS *************************/
		//throw exception if the passed rowNo isn't within range of the matrix rowCount
		static void throwErrorIfMatrixRowOutOfRange(const int rowNo, const int rowCount, String ^ additionalInfo);
		//throw exception if the passed columnNo isn't within range of the matrix columnCount
		static void throwErrorIfMatrixColumnOutOfRange(const int columnNo, const int columnCount, String ^ additionalInfo);
		//throw exception if the passed matrix is empty
		static void throwErrorIfMatrixIsEmpty(const int rowCount, const int columnCount);
		//throw exception if the passed list is empty
		static void throwErrorIfValueListIsEmpty(List<double>^ valueList);
		static void throwErrorIfValueListIsEmpty(List<Training^> ^valueList);
		//throw exception if the parent matrix is empty
		static void throwErrorIfParentMatrixIsEmpty(const int rowCount, const int columnCount, String ^ additionalInfo);
		//throw exception if the matrices arent equally ordered (i.e. if m1 is 3x3 and m2 2x2, they aren't equally ordered)
		static void throwErrorIfMatrixNotOfEqualOrder(Matrix ^matrix1, Matrix ^matrix2);
		//throw exception if the matrix passed isn't an 2x2 (size less or greater then 2)
		static void throwErrorIfMatrixNot2x2(const int rowCount, const int columnCount);
		//throw exception if the column count in the first matrix doesn't match the row count in the second (e.g. when multiplying)
		static void throwErrorIfColumnAndRowCountDiffers(int matrix1ColumnCount, int matrix2RowCount, String ^ additionalInfo);
		//throw exception if the new matrix size is less then current (possibility for loss of data)
		static void throwErrorIfNewSizeLessThanCurrent(int currentCount, int newCount, bool isRowCount);

		/***************** STATISTICS CLASS EXCEPTIONS *************************/
		//throw exception if the list's aren't of equal size
		static void throwErrorIflistSizeIsNotEqual(List<double>^ listOfX, List<double>^ listOfY, String ^ additionalInfo);

		/***************** RE-USEABLE EXCEPTIONS (NO ERROR MESSAGE) *************************/
		//throw exception if the passed size is greater then the passed count
		static void throwErrorIfSizeGreaterThenValue(const int sizeComparison, const int checkValue, String ^ additionalInfo);
		//throw exception if the passed size is greater then the passed count
		static void throwErrorIfSizeLessThenCount(const int sizeComparison, const int checkValue, String ^ additionalInfo);
	private:
		String ^ _errorMsg;
		String ^ _errorTitle;
		void setExceptionMessage(String ^ errorMsg);
		void setExceptionTitle(String ^ errorTitle);
	}; //ExceptionClass
} //namespace
#endif