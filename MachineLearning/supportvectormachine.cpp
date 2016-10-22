#include "Stdafx.h"
#include "supportvectormachine.h"

namespace MachineLearning {

	/* 
	Notes (Kononenko p. 267-273):
	--------------------------------------
	hyperplane equation: 
		(w * t) - b = 0

	optimal hyperplane equation: 
		y_j (w * t_j - b) >= 1

	complexity measure: 
		1/2 (w * w)

	transforming constrained optimization to functional maximization:
		W(alpha) = (j=1)_SUM^(n) ( alpha_j - 1/2*(i,j)_SUM^(n) (alpha_i * alpha_j * y_i * y_j * (t_i * t_j) ) )
	with constraints
		alpha_j >= 0
	and
		(j=1)_SUM^(n) (alpha_j * y_j) = 0

	Classification rule for optimal hyperplane:
		Y(t) = sign (w_o * t - b_0) 
		Y(t) = sign( (support vectors t_j)_SUM (y_j * j_alpha^0 * (t_j * t) - b_0) ) 

	Threshold value b_0:
		b_0 = 1/2 (w_0 * t_*(1) + w_0 t_*(-1))
	where t_*(1) and t_*(-1)denote an arbitrary support vector from the first or the second class

	Coefficients w_0 of the optimal hyperplane is determined by its support vectors:
		w_0 = support vectors t_j)_SUM (y_j * j_alpha^0 * t_j 

	error of magnitude eps_j (eps=epsilon) in classification of j-th example:
		y_j (w * t_j - b) >= 1 - eps_j
	optimization problem now minimizes 
		1/2 (w * w) + C (j=1)_SUM^(n) (eps_j) //C is a constant parameter set in advance

	now changing (alpha_j >= 0) to (C >= alpha_h >= 0)


	*/

	/*
	Default constructor
	*/
	SupportVectorMachine::SupportVectorMachine() {
	} //constructor

	/*
	Destructor
	*/
	SupportVectorMachine::~SupportVectorMachine() {
	} //destructor

	/*
	Returns the sign value (1, 0 or -1) based on passed value.
	Function based on answer found on stackowerflow by Mark Byers:
	http://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
	*/
	double SupportVectorMachine::sign(double value) {
		return (value > 0) ? 1 : ((value < 0) ? -1 : 0);
	} //
} //namespace