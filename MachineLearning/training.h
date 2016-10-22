#pragma once

#ifndef TRAINING_H

#define TRAINING_H	

#include "Stdafx.h"

namespace MachineLearning {

	/*
	Class creating objects of the training and test data 
	provided for the machine learning tasks.
	-----------------------------------------
	@Author: Knut Lucas Andersen
	*/
	public ref class Training {
	public:
		//default constructor
		Training();
		//constructor for testdata
		Training(double attributeValue1, double attributeValue2);
		//constructor for training data
		Training(double attributeValue1, double attributeValue2, int classID);
		//constructor for training data with noAttributes > 2
		Training(List<double>^ attributeValues, int classID);
		//destructor
		~Training();
		//returns the class id
		int getClassID();
		//returns the number of attributes 
		int getNoAttributes();
		//returns the first attribute value
		double getAttributeValue1();
		//returns the second attribute value
		double getAttributeValue2();
		//get the list containing the attribute values
		List<double>^ getAttributeValues();
	private:
		//class id for the training set
		int _classID;
		//the first attribute value
		double _attributeValue1;
		//the second attribute value
		double _attributeValue2;
		//the number of attributes belonging to this object
		int _noAttributes;
		//list containing the attribute values
		List<double>^ _attributeValues;
		//constant for setting values to NA (not applicable)
		static const int NA_VALUE = -1;
	}; //Training
} //namespace
#endif