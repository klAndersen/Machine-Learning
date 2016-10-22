#pragma once

#ifndef FUZZY_H

#define FUZZY_H	

#include "Stdafx.h"

namespace MachineLearning {

	/*
	Abstract class for Fuzzy operations, functioning 
	as a basic parent to seperate children into sub-groups.
	-----------------------------------------
	@Author: Knut Lucas Andersen
	*/
	public ref class Fuzzy abstract {
	public:
		//an example of virtual function making this class abstract
		//virtual void MakeClassAbstract() = 0;
		//constructor
		explicit Fuzzy();
		//destructor
		virtual ~Fuzzy();
		//Note: all functions need to be declared 'virtual'
	private:
		
	}; //Fuzzy
} //namespace
#endif