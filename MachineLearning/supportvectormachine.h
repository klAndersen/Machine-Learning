#pragma once

#ifndef SVM_H

#define SVM_H	

#include "Stdafx.h"

namespace MachineLearning {
	
	/*
	~Not implemented
	-----------------------------------------
	@Author: Knut Lucas Andersen
	*/
	public ref class SupportVectorMachine {
	public:
		SupportVectorMachine();
		~SupportVectorMachine();
	private:
		int noAttributes;
		int noLearningExamples;
		double sign(double value);
	}; //SupportVectorMachine
} //namespace
#endif