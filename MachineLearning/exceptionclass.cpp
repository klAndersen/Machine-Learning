#include "Stdafx.h"
#include "exceptionclass.h"
#include "matrix.h"

namespace MachineLearning {

	/*
	Default constructor
	*/
	ExceptionClass::ExceptionClass() : Exception() {
		_errorMsg = "";
		_errorTitle = "";
	} //constructor

	/*
	Constructor setting the error message based on the error
	that occurred.
	---------------------------------------------------------
	@param errorMsg: String^ - message containing description of error
	*/
	ExceptionClass::ExceptionClass(String ^ errorMsg) : Exception(errorMsg) {
		setExceptionMessage(errorMsg);
	} //constructor

	/*
	Constructor setting the error message and error title
	based on the error that occurred.
	---------------------------------------------------------
	@param errorMsg: String^ - message containing description of error
	@param errorTitle: String^ - title description of error that occurred
	*/
	ExceptionClass::ExceptionClass(String ^ errorMsg, String ^ errorTitle) : Exception(errorMsg) {
		setExceptionMessage(errorMsg);
		setExceptionTitle(errorTitle);
	} //constructor

	/*
	Destructor
	*/
	ExceptionClass::~ExceptionClass() {

	} //destructor

	String ^ ExceptionClass::ToString() {
		return _errorMsg;
	} //ToString

	String ^ ExceptionClass::getExceptionTitle() {
		return _errorTitle;
	} //getExceptionTitle

	void ExceptionClass::setExceptionMessage(String ^ errorMsg) {
		_errorMsg = errorMsg;
	} //setExceptionMessage

	void ExceptionClass::setExceptionTitle(String ^ errorTitle) {
		_errorTitle = errorTitle;
	} //setExceptionTitle

	/***************** MATRIX CLASS EXCEPTIONS *************************/

	/*
	Checks if the passed rowNo value is within range.
	If the value isn't within range, an ExceptionClass is thrown with the
	included content of additionalInfo.
	Note! This function is only useful for the original matrix.
	Temporary matrices will not match the conditions this function checks for.
	---------------------------------------------------------
	@param rowNo: const int - the rowNo value to check if is within range
	@param rowCount: const int - the total number of rows
	@param additionalInfo: String ^ - Additional information about error/exception
	@throw: ExceptionClass - Throws error if the rowNo is out of range
	*/
	void ExceptionClass::throwErrorIfMatrixRowOutOfRange(const int rowNo, const int rowCount, String ^ additionalInfo) {
		//check that the number of rows is within range
		if(rowNo >= rowCount || rowNo < 0) {
			throw gcnew ExceptionClass("Rownumber " + Conversion::convertIntegerToString(rowNo)
				+ " is out of range!" + additionalInfo);
		} //if
	} //throwErrorIfMatrixRowOutRange

	/*
	Checks if the passed columnNo value is within range.
	If the value isn't within range, an ExceptionClass is thrown with the
	included content of additionalInfo.
	Note! This function is only useful for the original matrix.
	Temporary matrices will not match the conditions this function checks for.
	---------------------------------------------------------
	@param columnNo: const int - the columnNo value to check if is within range
	@param columnCount: const int - the total number of columns
	@param additionalInfo: String ^ - Additional information about error/exception
	@throw: ExceptionClass - Throws error if the columnNo is out of range
	*/
	void ExceptionClass::throwErrorIfMatrixColumnOutOfRange(const int columnNo, const int columnCount, String ^ additionalInfo) {
		//check that the number of columns is within range
		if(columnNo >= columnCount || columnNo < 0) {
			throw gcnew ExceptionClass("Columnnumber " + Conversion::convertIntegerToString(columnNo)
				+ " is out of range!" + additionalInfo);
		} //if
	} //throwErrorIfMatrixColumnOutOfRange

	/*
	Checks if the passed matrix is empty and throws error
	if the passed rowCount or columnCount equals zero
	---------------------------------------------------------
	@param rowCount: int - The matrix row count
	@param columnCount: int - The matrix column count
	@throw: ExceptionClass - Throws error if the matrix is empty
	*/
	void ExceptionClass::throwErrorIfMatrixIsEmpty(const int rowCount, const int columnCount) {
		//check if the matrix is empty
		if(rowCount == 0 || columnCount == 0) {
			throw gcnew ExceptionClass("The matrix is empty!");
		} //if
	} //throwErrorIfMatrixIsEmpty

	/*
	Checks if the passed list is empty, and throws error if it is.
	---------------------------------------------------------
	@param valueList: List<double> - The matrix row count
	@throw: ExceptionClass - Throws error if the list is empty
	*/
	void ExceptionClass::throwErrorIfValueListIsEmpty(List<double> ^valueList) {
		//check that the list has content
		if(valueList->Count == 0) {
			String ^ errorMsg = "List is empty! \nCan't fill matrix with values.";
			throw gcnew ExceptionClass(errorMsg);
		} //if
	} //throwErrorIfValueListIsEmpty

	/*
	Checks if the passed list is empty, and throws error if it is.
	---------------------------------------------------------
	@param valueList: List<double> - The matrix row count
	@throw: ExceptionClass - Throws error if the list is empty
	*/
	void ExceptionClass::throwErrorIfValueListIsEmpty(List<Training^> ^valueList) {
		//check that the list has content
		if(valueList->Count == 0) {
			String ^ errorMsg = "List is empty! \nCan't fill matrix with values.";
			throw gcnew ExceptionClass(errorMsg);
		} //if
	} //throwErrorIfValueListIsEmpty

	/*
	This function shares some similarities with the function throwErrorIfMatrixIsEmpty(...),
	but this was made as an own function to satisfy the need for an seperate exception handling.
	If one just need to check if the matrix is empty, use throwErrorIfMatrixIsEmpty(...),
	but if the calling function relies on a filled parent matrix, this is useful, as it throws
	an error explaining that the parent matrix is empty.
	---------------------------------------------------------
	@param rowCount: int - The parent matrix row count
	@param columnCount: int - The parent matrix column count
	@param additionalInfo: String ^ - Additional information about error/exception
	@throw: ExceptionClass - Throws error if the parent matrix is empty
	@see throwErrorIfMatrixIsEmpty(const int rowCount, const int columnCount)
	*/
	void ExceptionClass::throwErrorIfParentMatrixIsEmpty(const int rowCount, const int columnCount, String ^ additionalInfo) {
		//check that the parent matrix isn't empty
		if(rowCount == 0 || columnCount == 0) {
			String ^ errorMsg = "The parent matrix is empty! \n[Retrieved rowNo count: "
				+ Conversion::convertIntegerToString(rowCount) + "]"
				+ "\n[Retrieved columnNo count: "
				+ Conversion::convertIntegerToString(columnCount) + "]"
				+ additionalInfo;
			throw gcnew ExceptionClass(errorMsg);
		} //if
	}//throwErrorIfParentMatrixIsEmpty

	/*
	Checks that the row count and column count in both matrices is equal.
	If they aren't, an error is thrown. Example:
	If matrix1 is a 2x2 and matrix2 is a 2x3, they aren't of
	equal size/order.
	---------------------------------------------------------
	@param matrix1: Matrix - The matrix to check size/order against matrix2
	@param matrix2: Matrix - The matrix to check size/order against matrix1
	@throw: ExceptionClass - Throws error if size/order isn't equal
	*/
	void ExceptionClass::throwErrorIfMatrixNotOfEqualOrder(Matrix ^matrix1, Matrix ^matrix2) {
		String ^ errorMsg = "";
		if(matrix1->getNumRows() != matrix2->getNumRows()) {
			errorMsg = "Row count in matrix1 ("
				+ Conversion::convertIntegerToString(matrix1->getNumRows())
				+ " rows) is not equal to row count in matrix 2 ("
				+ Conversion::convertIntegerToString(matrix2->getNumRows()) + " rows).";
			throw gcnew ExceptionClass(errorMsg);
		} //if
		if(matrix1->getNumColumns() != matrix2->getNumColumns()) {
			errorMsg = "Column count in matrix1 ("
				+ Conversion::convertIntegerToString(matrix1->getNumColumns())
				+ " columns) is not equal to column count in matrix 2 ("
				+ Conversion::convertIntegerToString(matrix2->getNumColumns()) + " columns).";
			throw gcnew ExceptionClass(errorMsg);
		} //if
	} //throwErrorIfMatrixNotOfEqualOrder

	/*
	Throws an error if the matrix row - and column count doesn't equal 2.
	An example of usage is where there is an requirement that the matrix/matrices
	to be used is of explicit size/order 2x2. Uses the throwErrorIfSizeGreaterThenValue(...)
	to control the values.
	---------------------------------------------------------
	@param rowCount: int - The parent matrix row count
	@param columnCount: int - The parent matrix column count
	@throw: ExceptionClass - Throws error if the row - and/or column count isn't equal to 2
	@see throwErrorIfSizeGreaterThenValue(int size, int count, String ^ additionalInfo)
	*/
	void ExceptionClass::throwErrorIfMatrixNot2x2(const int rowCount, const int columnCount) {
		int sizeCheck = 2;
		String ^ errorMsg = "Can't calculate the inverse matrix.\n";
		errorMsg += "The requirement is a 2x2 matrix, but the passed matrix is a "
			+ Conversion::convertIntegerToString(rowCount) + "x"
			+ Conversion::convertIntegerToString(columnCount) + " matrix.";
		//check the size
		throwErrorIfSizeGreaterThenValue(rowCount, sizeCheck, errorMsg);
		throwErrorIfSizeGreaterThenValue(columnCount, sizeCheck, errorMsg);
	} //throwErrorIfMatrixNot2x2

	/*
	Compares the coumn count against the row count and then throws error
	if the number of columns in matrix1 isn't equal to the number of rows in
	matrix2. An example of usage is when attempting to multiply two matrices,
	since the columns and rows must be equal to be able to multiply two matrices.
	---------------------------------------------------------
	@param matrix1ColumnCount: int - The number of columns in matrix1
	@param matrix2RowCount: int -  The number of rows in matrix2
	@param additionalInfo: String ^ - Additional info, e.g. where error was captured
	@throw: ExceptionClass - Throws error if matrix1ColumnCount != matrix2RowCount
	*/
	void ExceptionClass::throwErrorIfColumnAndRowCountDiffers(int matrix1ColumnCount, int matrix2RowCount, String ^ additionalInfo) {
		if(matrix1ColumnCount != matrix2RowCount) {
			String ^ errorMsg = "The number of columns("
				+ Conversion::convertIntegerToString(matrix1ColumnCount) + ") in matrix1  is not equal to the number of rows("
				+ Conversion::convertIntegerToString(matrix2RowCount) + ") in matrix2 .\n" + additionalInfo;
			throw gcnew ExceptionClass(errorMsg);
		} //if
	}//throwErrorIfColumnAndRowCountDiffers

	/*
	Mostly a placeholder function, developed with the thought of
	catching error when attempting to extend the size of a
	previously created matrix.
	---------------------------------------------------------
	@param currentCount: int - The current count of the row/columns
	(based on isRowCount)
	@param newCount: int -  The new row/column count
	@param isRowCount: bool - If true; increase the number of rows,
	else increase the number of columns
	*/
	void ExceptionClass::throwErrorIfNewSizeLessThanCurrent(int currentCount, int newCount, bool isRowCount) {
		String ^ type = "";
		String ^ errorMsg = "";
		if(isRowCount) {
			type = "rows";
		} else {
			type = "columns";
		} //if
		//check if a new count is set and if the new count is lower then current
		if(newCount > 0 && currentCount > newCount) {
			errorMsg = "The current number of " + type + "(" + Conversion::convertIntegerToString(currentCount);
			errorMsg += ") is greater then the new(" + Conversion::convertIntegerToString(newCount) + ")!";
			errorMsg += "\nCannot extend the size of the matrix.";
			throw gcnew ExceptionClass(errorMsg);
		} //if
	} //throwErrorIfNewSizeLessThanCurrent

	/***************** STATISTICS CLASS EXCEPTIONS *************************/

	/*
	Compares the passed lists to check that they are of equal size.
	If they aren't of equal size, an error is thrown. Use String ^ additionalInfo
	to add additional information about the exception thrown.
	---------------------------------------------------------
	@param listOfX: List<double> - A list containing values to be compared against listOfY->Count
	@param listOfY: List<double> - A list containing values to be compared against listOfX->Count
	@param additionalInfo: String ^ - Additional information about error/exception
	@throw: ExceptionClass - Throws error if the parent matrix is empty
	*/
	void ExceptionClass::throwErrorIflistSizeIsNotEqual(List<double> ^listOfX, List<double> ^listOfY, String ^ additionalInfo) {
		if(listOfX->Count != listOfY->Count) {
			String ^ errorMsg = "The size of listX isn't equal to the size of listY." + additionalInfo;
			throw gcnew ExceptionClass(errorMsg);
		} //if
	}//throwErrorIflistSizeIsNotEqual

	/***************** RE-USEABLE EXCEPTIONS (NO ERROR MESSAGE) *************************/

	/*
	Checks if the passed size is greater then checkValue.
	If size is greater then checkValue, then an error is thrown.
	To display an error message, a value must be passed through the
	String ^ errorMsg. This is so that this function can be
	re-used if needed.
	---------------------------------------------------------
	@param sizeComparison: int - The size to use as comparison
	@param checkValue: int - The value to check against the comparison
	@param additionalInfo: String ^ - The error message to be displayed
	(if none is entered, then errorMsg = "")
	@throw: ExceptionClass - Throws error if size > checkValue
	*/
	void ExceptionClass::throwErrorIfSizeGreaterThenValue(const int sizeComparison, const int checkValue, String ^ additionalInfo) {
		if(sizeComparison > checkValue) {
			throw gcnew ExceptionClass(additionalInfo);
		} //if
	}//throwErrorIfSizeGreaterThenValue

	/*
	Checks if the passed size is lesser then checkValue.
	If size is lesser then checkValue, then an error is thrown.
	To display an error, a value must be passed through the
	String ^ errorMsg. This is so that this function can be
	re-used if needed.
	---------------------------------------------------------
	@param sizeComparison: int - The size to use as comparison
	@param checkValue: int - The value to check against the comparison
	@param additionalInfo: String ^ - The error message to be displayed
	(if none is entered, then errorMsg = "")
	@throw: ExceptionClass - Throws error if size < checkValue
	*/
	void ExceptionClass::throwErrorIfSizeLessThenCount(const int sizeComparison, const int checkValue, String ^ additionalInfo) {
		if(sizeComparison < checkValue) {
			throw gcnew ExceptionClass(additionalInfo);
		} //if
	}//throwErrorIfSizeLessThenCount
} //namespace