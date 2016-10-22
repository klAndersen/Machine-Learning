#pragma once

#ifndef DECISION_TREE_H

#define DECISION_TREE_H

#include "Stdafx.h"
#include "training.h"
#include "matrix.h"

namespace MachineLearning {

	/**
	Class creating a Decision Tree, with the option to also 
	create a Binary struct (BinaryTree).
	@Author: Knut Lucas Andersen
	*/
	public ref class DecisionTree sealed {
	public:
		//default constructor
		DecisionTree();
		//destructor
		~DecisionTree();
		//ref struct used for building the binary tree
		ref struct BinaryTree {
			int classID;
			String ^ attributeName;
			double attributeValue;
			ref struct BinaryTree ^left;
			ref struct BinaryTree ^right;
		};
		//ref struct used for building the binary tree for LDA
		ref struct BinaryTreeLDA {
			int classID;
			String ^ attributeName;
			double attributeValue;
			bool isValueGreaterThenThis(double value) {
				bool isGreaterThen = false;
				(value > attributeValue) ? isGreaterThen = true : isGreaterThen = false;
				return isGreaterThen;
			} //compareValues
			ref struct BinaryTreeLDA ^left;
			ref struct BinaryTreeLDA ^right;
		};
		//function that creates a binary tree
		ref struct BinaryTree ^createBinaryTree(List<Training ^> ^trainingData, String ^%debugString);
		//function that creates a binary tree for use with LDA
		ref struct BinaryTreeLDA ^createBinaryTree(List<Training ^> ^trainingData, Matrix ^%optimalLineMatrix, 
			Matrix ^%meanMatrixC1, Matrix ^%meanMatrixC2, String ^%debugString);
		//debugging - check to see if tree was built correctly
		void printBinaryTree(ref struct BinaryTree ^binaryTree, String ^%debugString);
		//constant for use in the binary tree for lda, telling that this sample couldn't be classified
		static const int CAN_NOT_CLASSIFY_SAMPLE = -9999;
	private:
		//constant for checking if two attributes are equal
		static const int ATTRIBUTE_NOT_FOUND = -9999;
		// a ref struct used for storing attribute value and 
		//number of current attribute, with the goal of 
		//finding a root value
		ref struct AttributeCounter {
			int numCurrentAttribute;
			double attributeValue;
		};
		//find the value to be used as root from a list
		double findRootValue(List<Training ^> ^%trainingData);
		//find root value to be used from a matrix
		double findRootValueLDA(List<Training ^> ^%trainingData, Matrix ^%optimalLineMatrix, Matrix ^%meanMatrixC1, Matrix ^%meanMatrixC2);
		//return a list containing the class based on passed classID (only for 2 attributes class)
		List<Training ^> ^returnClassList(List<Training ^> ^%trainingData, int classID);
		//return a matrix containing the class based on values from passed list (only for 2 attributes class)
		Matrix ^returnClassMatrix(List<Training ^> ^%classList);
		//return the optimal line w based on passed class matrices (only for 2 attributes class)
		Matrix ^returnOptimalLineMatrix(Matrix ^%class1, Matrix ^%class2);
		//return the attribute that exists in each class or ATTRIBUTE_NOT_FOUND
		double returnRepeatingAttribute(double c1a1, double c2a1, double c1a2, double c2a2);
		//compare an attribute with attributes in list (use index for location if exists)
		bool compareAttributesInList(List<DecisionTree::AttributeCounter ^> ^%attributeList, 
			double currentValue, int &index);
		//if multiple repeated distinct attributes were retrieved, sort them from low to high
		void sortAttributesAscending(List<DecisionTree::AttributeCounter ^> ^%attributeList);
		//create a leaf containing the passed values
		ref struct BinaryTreeLDA ^createLeaf(String ^name, double attributeValue,
			int classID, ref struct BinaryTreeLDA ^nodeLeft, ref struct BinaryTreeLDA ^nodeRight);
		//start to build a binary tree
		ref struct BinaryTree ^buildTree(double root, List<Training ^> ^sampleData, String ^%debugString);
		//get the root value from list if more then one equal attribute was found
		double getRootValueFromList(List<DecisionTree::AttributeCounter ^> ^%distinctAttributeValues);
		//return a List filled with distinct repeated attributes found in both classes.
		List<DecisionTree::AttributeCounter^> ^fillListWithDistinctAttributes(List<Training ^> ^class1, 
			List<Training ^> ^class2);
		//creates a new leaf containing the next node in the tree
		ref struct DecisionTree::BinaryTree ^DecisionTree::createLeaf(double parent, ref struct BinaryTree ^node, 
			int &attributeNo, int classID, double attrValue);
	}; //DecisionTree
} //namespace
#endif