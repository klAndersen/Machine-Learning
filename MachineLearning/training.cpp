#include "Stdafx.h"
#include "training.h"

namespace MachineLearning {

	/*
	Default constructor
	*/
	Training::Training() {
		_noAttributes = 0;
		_attributeValue1 = NA_VALUE;
		_attributeValue2 = NA_VALUE;
		_classID = NA_VALUE;
		_attributeValues = gcnew List<double>();
	} //constructor

	/*
	Constructor for test/unlabeled data containing two attributes
	---------------------------------------------------------
	@param attributeValue1 - double: The first attribute value
	@param attributeValue2 - double: The second attribute value
	*/
	Training::Training(double attributeValue1, double attributeValue2) {
		_noAttributes = 2;
		_attributeValue1 = attributeValue1;
		_attributeValue2 = attributeValue2;
		//set to -1; N/A for test samples being read		
		_classID = NA_VALUE;
		_attributeValues = gcnew List<double>();
		_attributeValues->Add(NA_VALUE);
	} //constructor

	/*
	Constructor for training sample containing a class id
	and two attributes
	---------------------------------------------------------
	@param attributeValue1 - double: The first attribute value
	@param attributeValue2 - double: The second attribute value
	@param classID - int: The class id
	*/
	Training::Training(double attributeValue1, double attributeValue2, int classID) {
		_noAttributes = 2;
		_attributeValue1 = attributeValue1;
		_attributeValue2 = attributeValue2;
		_classID = classID;
		_attributeValues = gcnew List<double>();
		_attributeValues->Add(NA_VALUE);
	} //constructor

	/*
	Constructor for training samples containing multiple attributes
	(no. of attributes > 2)
	---------------------------------------------------------
	@param attributeValue1
	@param classID - int: The class id
	*/
	Training::Training(List<double>^ attributeValues, int classID) {
		_attributeValues = gcnew List<double>();
		_attributeValues = attributeValues;
		_classID = _attributeValues->Count;
		_noAttributes = 2;
		//set to -1;  N/A for data containing attribute values > 2
		_attributeValue1 = NA_VALUE;
		_attributeValue2 = NA_VALUE;
	} //constructor

	/*
	Destructor
	*/
	Training::~Training() {

	} //destructor

	/*
	Returns the training sets class id
	*/
	int Training::getClassID() {
		return _classID;
	} //getClassID

	/*
	Returns the first attribute value
	*/
	double Training::getAttributeValue1() {
		return _attributeValue1;
	} //getAttributeValue1

	/*
	Returns the second attribute value
	*/
	double Training::getAttributeValue2() {
		return _attributeValue2;
	} //getAttributeValue2

	/*
	Returns a list containing the attribute values
	*/
	List<double>^ Training::getAttributeValues() {
		return _attributeValues;
	} //getAttributeValues

	/*
	Returns the no. of attributes belonging to this object
	*/
	int Training::getNoAttributes() {
		return _noAttributes;
	} //getAttributeValues
} //namespace