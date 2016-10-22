#pragma once

#ifndef SOM_H

#define SOM_H	

#include "Stdafx.h"
#include "neuralnetwork.h"

namespace MachineLearning {
	
	/*

	-----------------------------------------
	@Author: Knut Lucas Andersen
	*/
	public ref class SelfOrganisingMap sealed : public NeuralNetwork {
	public:
		//default constructor
		SelfOrganisingMap();
		//destructor
		~SelfOrganisingMap();
		//override parent function (needed???)
		void trainPerceptron() override;
	private:
		
	}; //SelfOrganisingMap
} //namespace
#endif