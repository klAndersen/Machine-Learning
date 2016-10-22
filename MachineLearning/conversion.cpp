#include "Stdafx.h"
#include <sstream>
#include "conversion.h"
using namespace std;

namespace MachineLearning {

	/*
	Default constructor
	*/
	Conversion::Conversion() {

	} //constructor

	/*
	Destructor
	*/
	Conversion::~Conversion() {

	} //destructor

	/*
	Converts value of type integer to string.
	---------------------------------------------------------
	@param number: integer - number to be converted to string
	@return: String ^ - result of conversion
	*/
	String ^ Conversion::convertIntegerToString(int number) {
		string result;
		ostringstream convertToString;
		//read numeric value to convert
		convertToString << number;
		//convert number to string
		result = convertToString.str();
		//empty the content in the converter
		convertToString.clear();
		convertToString.str("");
		//return the converted string
		return gcnew String(result.c_str());
	} //convertIntegerToString

	/*
	Converts value of type double to string.
	---------------------------------------------------------
	@param number: double - number to be converted to string
	@return: String ^ - result of conversion
	*/
	String ^Conversion::convertDoubleToString(double number) {
		string result;
		ostringstream convertToString;
		//read numeric value to convert
		convertToString << number;
		//convert number to string
		result = convertToString.str();
		//empty the content in the converter
		convertToString.clear();
		convertToString.str("");
		//return the converted string
		return gcnew String(result.c_str());
	} //convertDoubleToString

	/*
	Converts value of type string to integer.
	If value cannot be converted, -999 is returned.
	---------------------------------------------------------
	@param number: String ^ - value (numeric) to convert to integer
	@return: int || -9999 if conversion fails
	*/
	int Conversion::convertStringToInteger(String ^ number) {
		int result = 0;
		string value = "";
		MarshalString(number, value);
		//read the string value to convert
		stringstream convertToInteger(value);
		//could the value be converted?
		if(!(convertToInteger >> result)) {
			//Couldn't convert, set value to -999
			result = INPUT_NOT_A_NUMBER;
		} //if (!(convertToInteger >> result))
		return result;
	} //convertStringToInteger

	/*
	Converts value of type string to double.
	If value cannot be converted, -999 is returned.
	---------------------------------------------------------
	@param number: String ^ - value (numeric) to convert to double
	@return: double || -9999 if conversion fails
	*/
	double Conversion::convertStringToDouble(String ^ number) {
		double result = 0;
		string value = "";
		MarshalString(number, value);
		//read the string value to convert
		stringstream convertToDouble(value);
		//could the value be converted?
		if(!(convertToDouble >> result)) {
			//Couldn't convert, set value to null
			result = static_cast<double>(INPUT_NOT_A_NUMBER);
		} //if (!(convertToDouble >> result))
		return result;
	} //convertStringToDouble

	/*
	Converts the values passed in the list to a binary string.
	The converted values are then added to List<String ^> ^ which is returned.
	The first bit tells if the binary is negative (0) or positive (1).
	---------------------------------------------------------
	@param valueList: List<int> - values to convert to binary
	@return: List<String> - list containing the binary values
	*/
	List<String ^> ^ Conversion::binaryConverter(List<int> ^ valueList) {
		int maxSize = 0,
			currentValue = 0;
		String ^ strBinary = "";
		String ^ sign = "";
		List<String ^> ^ binaryValues = gcnew List<String ^>();
		maxSize = static_cast<int>(valueList->Count);
		//loop through the values in the list
		for(int i = 0; i < maxSize; i++) {
			strBinary = "";
			//get the current value and convert it to binary
			currentValue = valueList[i];
			//is value negative?
			if(currentValue < 0) {
				sign = "0";
				//make it positive to convert properly
				currentValue = -currentValue;
			} else {
				sign = "1";
			} //if
			divideUntilZero(currentValue, strBinary);
			//since the binary string is backwards, reverse it
			strBinary = sign + reverseString(strBinary);
			//add the converted value to the list
			binaryValues->Add(strBinary);
		} //for
		return binaryValues;
	} //binaryConverter

	/*
	Divides the passed value until it becomes zero. The way this is done
	is by first running modulus, if modulus is 1, then the current value
	can't be divided by 2. If this is the case, value is subtracted by 1
	to make it divideable by 2. If the value is greater then zero, then
	the function recurses until the value is zero.
	---------------------------------------------------------
	@param value: int - The value to convert to binary
	@param reverseBinary: String ^ - string to fill with binary values
	*/
	void Conversion::divideUntilZero(int value, String ^%reverseBinary) {
		int mod2Value = 0;
		//get modulus, and if modulus is 1, subtract one to get proper division
		mod2Value = value % 2;
		if(mod2Value == 1) {
			value--;
		} //if
		//add modulus to string and divide value by 2
		//		reverseBinary += convertIntegerToString(mod2Value);
		value = value / 2;
		//if division is still possible, recurse
		if(value > 0) {
			divideUntilZero(value, reverseBinary);
		} //if
	} //divideUntilZero

	/*
	Reverses the passed text. Example: if text="abc" then the
	returned string would be "cba". An example of use would be
	when dealing with converted binary string values, since these
	would then be backwards.
	---------------------------------------------------------
	@param text: String ^ - The text to be reversed
	@return: String ^ - The reversed version of the text
	*/
	String ^ Conversion::reverseString(String ^text) {
		String ^ reversedString = "";
		int length = 0,
			endLoop = -1;
		length = text->Length - 1;
		for(int i = length; i > endLoop; i--) {
			reversedString += text->Substring(i, 1);
		} //for
		return reversedString;
	} //reverseString

	/*
	Converts the values passed in the list to a binary string.
	The converted values are then added to List<String ^> ^ which is returned.
	The first bit tells if the value is positive (1) or negative (0).
	---------------------------------------------------------
	@param valueList: List<double> - values to convert to binary
	@param decimalLength: int - The number of decimals, including the decimal sign ('.')
	@return: List<String> - list containing the binary values
	*/
	List<String ^> ^Conversion::binaryConverter(List<double> ^valueList, const int decimalLength) {
		int maxSize = 0;
		String ^ strDouble = "";
		String ^ strBinary = "";
		String ^ sign = "";
		double currentValue = 0;
		List<String ^> ^ binaryValues = gcnew List<String ^>();
		maxSize = static_cast<int>(valueList->Count);
		//loop through the values in the list
		for(int i = 0; i < maxSize; i++) {
			strBinary = "";
			currentValue = valueList[i];
			//is value negative?
			if(currentValue < 0) {
				sign = "0";
				//make it positive to convert properly
				currentValue = -currentValue;
			} else {
				sign = "1";
			} //if
			//get the current value and convert it to string
			strDouble = convertDoubleToString(currentValue);
			//convert the retrieved value into a binary string
			createBinaryStringFromDouble(strDouble, strBinary, decimalLength);
			strBinary = sign + strBinary;
			//add the converted value to the list
			binaryValues->Add(strBinary);
		} //for
		return binaryValues;
	} //binaryConverter

	/*
	This function takes a string containing a double (strDouble) and first checks if
	it has decimal values. If the string doesn't contain a decimal, it's
	checked if the value can be converted to an integer. If this is the case,
	a "normal" binary conversion is done by use of the function divideByZero(...).
	If the value in the string is a double, it's first checked if the length of the
	value exceeds the set parameter for the number of decimals (decimalLength).
	If the length exceeds decimalLength, the remaining values are cut off
	(not the best solution, but this isn't the main focus for the task at hand ).
	Thereafter the values are converted, first the values before the decimal are converted
	(if none exists, only a zero preceeds the decimal values), then the decimals after the
	decimal is converted, and added to the string reverseBinary.
	---------------------------------------------------------
	@param strDouble: String ^ - String containing the double value
	@param reverseBinary: String ^ - String to fill with binary values
	@param decimalLength: int - The number of decimals, including the decimal sign ('.')
	@throw: ExceptionClass - Throws error if strDouble isn't double or a numeric
	*/
	void Conversion::createBinaryStringFromDouble(String ^%strDouble, String ^%reverseBinary, const int decimalLength) {
		String ^ decimalPoint = ".";
		int counter = 0,
			length = 0,
			position = 0,
			raisedPower = 1,
			convertToInt = 0;
		double tempValue = 0;
		//find the decimal point in the string
		position = strDouble->IndexOf(decimalPoint);
		if(position == -1) {
			//if the decimal point wasn't found, check if it has a ','
			decimalPoint = ",";
			position = strDouble->IndexOf(decimalPoint);
		} //if
		if(position == -1) {
			//no decimal point, assuming value doesn't have any decimals
			tempValue = convertStringToDouble(strDouble);
			//check that number conversion succeded
			if(convertToInt == INPUT_NOT_A_NUMBER) {
				throw gcnew ExceptionClass("Value (" + strDouble + ") isn't a double or numeric!");
			} //if
			divideDoubleUntilZero(tempValue, reverseBinary);
			//since the binary string is backwards, reverse it
			reverseBinary = reverseString(reverseBinary);
			//since this double doesn't have any decimals, add zeros to fill up the bits
			for(int i = 0; i <= decimalLength; i++) {
				reverseBinary += "0";
			} //for
		} else {
			//get the value after the decimal point and check if it's length 
			//is greater then the required/set length
			length = strDouble->Substring(position + 1)->Length;
			if(length > decimalLength) {
				strDouble = strDouble->Substring(0, decimalLength) + strDouble->Substring(position, decimalLength);
			} //if
			//get the value before the decimal point
			tempValue = convertStringToDouble(strDouble->Substring(0, position));
			//if the value is divideable, divide it until it reaches zero
			if((tempValue / 2) > 0) {
				divideDoubleUntilZero(tempValue, reverseBinary);
				//since the binary string is backwards, reverse it
				reverseBinary = reverseString(reverseBinary);
			} else {
				//no value before the decimal point
				reverseBinary = "0";
			} //if
			//convert the value behind the decimal point to double 
			//and divide the value until it reaches zero
			tempValue = convertStringToDouble(strDouble->Substring(position));
			if(tempValue > 0) {
				subtractDecimalUntilZero(tempValue, reverseBinary, raisedPower, decimalLength, counter);
			} //if
		} //if
	} //createBinaryStringFromDouble

	/*
	Divides the passed value until it becomes zero. The way this is done
	is by first running modulus, if modulus is 1, then the current value
	can't be divided by 2. If this is the case, value is subtracted by 1
	to make it divideable by 2. If the value is greater then zero, then
	the function recurses until the value is zero.
	---------------------------------------------------------
	@param value: double - the current value to be converted to binary
	@param reverseBinary: String ^ - String to fill with binary values
	*/
	void Conversion::divideDoubleUntilZero(double value, String ^%reverseBinary) {
		double mod2Value = 0;
		//get modulus, and if modulus is 1, subtract one to get proper division
		mod2Value = fmod(value, 2);
		if(mod2Value == 1) {
			value--;
		} //if
		//add modulus to string and divide value by 2
		reverseBinary += convertDoubleToString(mod2Value);
		value = value / 2;
		//if division is still possible, recurse
		if(value > 0) {
			divideDoubleUntilZero(value, reverseBinary);
		} //if
	} //divideUntilZero

	/*
	Function that recurses through the decimals after the decimal point.
	The way this works is that a divisor is calculated
	(divisor = 1 / (2^raisedPower) {raisedPower = 0, 1, 2, ..., n})
	and then subtracted from the passed decimal value. If the remainder
	after the subtraction is more or less positive (> -0.01f), then the
	binary for this value is set to 1 and the subtraction is then done
	on the passed decimalValue. This process continues until either all
	decimals have been converted (counter == decimalLength) or there is
	only zero's left (these are then added as '0' at the end of the string).
	---------------------------------------------------------
	@param decimalValue: double - the current value (after decimal)
	to be converted to binary
	@param reverseBinary: String ^ - String to fill with binary values
	@param raisedPower: int - the power to raise the current value
	(set to 1 on first call) used to find divisor for current round (1 / 2^raisedPower)
	@param decimalLength: int - The number of decimals, including the decimal sign ('.')
	@param counter: int - the number of runs the recursion has runned - set to 0 when calling function
	*/
	void Conversion::subtractDecimalUntilZero(const double decimalValue, String ^%reverseBinary, int raisedPower, const int decimalLength, int counter) {
		//constant used in check to see what binary value to use. 
		//if the remainder is less then this constant, set binary to 0
		const float constNullValue = -0.01f;
		//constant used as denominator when getting the divided value: 1 / (2^raisedPower)
		const double constPowValue = 2.0;
		int binaryValue = 0;
		double remainingValue = 0,
			dividedValue = 0;
		//get the divided value of 1 / (2^raisedPower) {raisedPower = 0, 1, 2, ..., n}
		dividedValue = 1 / pow(constPowValue, raisedPower);
		//get the binary value and add it to the string
		binaryValue = ((decimalValue - dividedValue) < constNullValue) ? 0 : 1;
		reverseBinary += convertDoubleToString(binaryValue);
		//check if this decimal value was subtractable
		//e.g. if dividedValue = 0.25 and the decimal value is 0.16
		//then this would be a negative number. Here we only want 
		//positive numbers, so 0.25 is in this example not present,
		//making binaryValue equal to zero
		if(binaryValue == 1) {
			remainingValue = decimalValue - dividedValue;
		} else {
			remainingValue = decimalValue;
		} //if
		//if division is still possible, recurse
		if(remainingValue > 0 && counter < decimalLength) {
			counter++;
			raisedPower++;
			subtractDecimalUntilZero(remainingValue, reverseBinary, raisedPower, decimalLength, counter);
		} else {
			//if the remaining value is zero, but there are still decimal to convert to binary, 
			//add a zero for each of them
			for(int i = 0; i < (decimalLength - counter); i++) {
				reverseBinary += "0";
			} //for
		} //if
	} //subtractDecimalUntilZero
	/*
	Takes a string containing a binary value and converts it
	to an integer. This is done by a loop and then retrieving
	each binary value and then multiplying it by 2 raised to
	the power of the length (subtracted by the position in the
	string and 1 - since the last value is 1).
	Example: The string "1111" has a length of 4.
	This gives that the first number is 1 * 2^(4-1-0)
	=> 1 * 2^3 = 8. The next round is 1 * 2^(4-1-1) => 1 * 2^2 = 4,
	and so on.
	---------------------------------------------------------
	@param binary: String ^ - the binary string to convert to integer
	@param isDouble - bool: Is this function called from the
	convertBinaryStringToDouble(...)?
	@return int: result of conversion
	*/
	int Conversion::convertBinaryStringToInteger(String ^ binary, bool isDouble) {
		int length = 0,
			currentBinary = 0,
			result = 0;
		double raisedPower = 0;
		bool isPositive = false;
		(Conversion::convertStringToInteger(binary->Substring(0, 1))) ? isPositive = true : isPositive = false;
		if(!isDouble) {
			//remove the first bit
			binary = binary->Substring(1);
		} //if
		length = binary->Length;
		//loop through the binary values in the passed string
		for(int i = 0; i < length; i++) {
			//get the current binary value from the string
			currentBinary = convertStringToInteger(binary->Substring(i, 1));
			//increase the power to raise (2^raisedPower)
			raisedPower = length - i - 1;
			//calculate the binary and add it to the result
			currentBinary = currentBinary * static_cast<int>(pow(2.0, raisedPower));
			result += currentBinary;
		} //for
		if(!isDouble && !isPositive) {
			//make result into a negative integer
			result = -result;
		} //if
		return result;
	} //convertBinaryStringToInteger

	/*
	Takes a string containing a binary value and an integer containing the number
	of decimals present in the string. First the values before the decimal is
	calculated by calling the function convertBinaryStringToInteger(...), and
	afterwards the decimal points are handled. This is done in the same way that
	double was converted to binary, only in reverse. The current binary value is
	found by looping through the remainding binary values, multiplying the binary
	value with (1 / 2^(iteration + 1)).
	Example: The string value is "011010" and decimalLength = 4.
	Then the first value, 0, is the value before the decimal point, giving us zero.
	Now, the current value is '0.0'. Then the decimals are checked, so the next binary
	is a 1, giving 1 * (1 / (2^(1)) with the result 1/2 = 0.5. The current result
	is now 0.5. And so it goes on until all values in the string is converted.
	---------------------------------------------------------
	@param binary: String ^ - the binary string to convert to double
	@param decimalLength: int - the number of decimals in the string (after the '.')
	@return double: result of conversion
	*/
	double Conversion::convertBinaryStringToDouble(String ^binary, int decimalLength) {
		String ^ tempStr = "";
		int length = 0,
			tempInt = 0;
		double dividedValue = 0,
			currentBinary = 0,
			raisedPower = 0,
			result = 0;
		bool isPositive = false;
		(Conversion::convertStringToInteger(binary->Substring(0, 1))) ? isPositive = true : isPositive = false;
		//remove the first bit
		binary = binary->Substring(1);
		decimalLength++;
		length = binary->Length;
		//retrieve the value before the decimal point
		tempInt = length - decimalLength;
		tempStr = binary->Substring(0, tempInt);
		result = static_cast<double>(convertBinaryStringToInteger(tempStr, true));
		//remove the previously added extra (no need for decimal point counter now)
		decimalLength--;
		//loop through the values after the decimal point
		for(int i = 0; i < decimalLength; i++) {
			//get the current binary value from the string
			currentBinary = convertStringToInteger(binary->Substring(tempInt, 1));
			//increase power to raise (2^raisedPower)
			raisedPower = i + 1;
			//get the divided value, calculate the binary and add it to the result
			dividedValue = 1 / pow(2.0, raisedPower);
			currentBinary = currentBinary * dividedValue;
			result += currentBinary;
			tempInt++;
		} //for
		if(!isPositive) {
			//make result into a negative double
			result = -result;
		} //if
		return result;
	} //convertBinaryStringToDouble

	/*
	Converts value of type System::String^ to std::string.
	http://msdn.microsoft.com/en-us/library/1b4az623(v=vs.100).aspx
	---------------------------------------------------------
	@param s: String ^ - Value to convert to std::string
	@param os: std::string - Referenced result in a std::string
	*/
	void Conversion::MarshalString(String ^ s, string& os) {
		using namespace Runtime::InteropServices;
		const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		os = chars;
		Marshal::FreeHGlobal(IntPtr((void*)chars));
	} //MarshalString

	/*
	Converts value of type String^ to std::wstring.
	http://msdn.microsoft.com/en-us/library/1b4az623(v=vs.100).aspx
	---------------------------------------------------------
	@param s: String ^ - Value to convert to std::wstring
	@param os: std::wstring - Referenced result in a std::wstring
	*/
	void Conversion::MarshalString(String ^ s, wstring& os) {
		using namespace Runtime::InteropServices;
		const wchar_t* chars = (const wchar_t*)(Marshal::StringToHGlobalUni(s)).ToPointer();
		os = chars;
		Marshal::FreeHGlobal(IntPtr((void*)chars));
	} //MarshalString

	/*
	Converts value of type String^ to char*.
	---------------------------------------------------------
	@param convString: String ^ - Value to convert to char*
	*/
	const char *Conversion::convertStringToChar(String ^convString) {
		const char *convertedChar = nullptr;
		string stdString = "";
		MarshalString(convString, stdString);
		convertedChar = stdString.c_str();
		return convertedChar;
	} //convertStringToChar
} //namespace