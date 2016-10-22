#include "Stdafx.h"
#include "decisiontree.h"
#include "statistics.h"

namespace MachineLearning {

	/*
	Default constructor
	*/
	DecisionTree::DecisionTree() {

	} //constructor

	/*
	destructor
	*/
	DecisionTree::~DecisionTree() {

	} //destructor

	/****************** ATTRIBUTE DECISION TREE ********************************/

	/*
	Creates a binary tree passed on the values passed in the List trainingData.
	This function first retrieves a root value, and then builds a tree based on the 
	attributes in the training set
	--------------------------------------------------
	@param trainingData: List<Training > - List containing the
	values for the training samples; Training(a1, a2, classID)
	@param debugString: String ^ - A reference string that gets filled up with data on 
	the tree build
	@throw: ExceptionClass - throws error if size of distinct attribute values is zero
	(cant select a root value)
	@return: Returns the binary decision tree
	*/
	ref struct DecisionTree::BinaryTree ^DecisionTree::createBinaryTree(List<Training ^> ^trainingData, String ^%debugString) {
		try {
			double root = 0;
			int max_size = 0;
			debugString += "Preparing to create binary tree...\n\n";
			root = findRootValue(trainingData);
			debugString += "Root value has been selected.\n";
			debugString += "The root value is: " + Conversion::convertDoubleToString(root) + ".\n\n";
			debugString += "Starting to build tree...\n\n";
			//start to build the binary decision tree
			return buildTree(root, trainingData, debugString);
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
		return nullptr;
	} //createBinaryTree

	/*
	Builds a new binary tree based on the values passed in the vecor sampleData.
	The tree is built by the use of the ref struct BinaryTree, and then the values
	from the sampleData is retrieved through a loop that creates new leafs for the
	values. The parent for attribute2 is attribute1, since it comes first
	(and root is parent for attribute1).
	--------------------------------------------------
	@param root: double - the value selected for root
	@param sampleData: List<Training > - List containing training samples for building the tree
	@param debugString: String ^ - String ^ used for debug purposes
	@return: ref struct BinaryTree - a ref struct containing the root and current leafs
	*/
	ref struct DecisionTree::BinaryTree ^DecisionTree::buildTree(double root, List<Training ^> ^sampleData, String ^%debugString) {
		double attribute1 = 0,
			attribute2 = 0;
		int counter = 1,
			classID = 0;
		BinaryTree ^binaryTree = gcnew BinaryTree();
		binaryTree->attributeName = "root";
		binaryTree->attributeValue = root;
		binaryTree->classID = 0;
		binaryTree->left = nullptr;
		binaryTree->right = nullptr;
		int max_size = static_cast<int>(sampleData->Count);
		//loop through the training examples
		for(int i = 0; i < max_size; i++) {
			attribute1 = sampleData[i]->getAttributeValue1();
			attribute2 = sampleData[i]->getAttributeValue2();
			classID = sampleData[i]->getClassID();
			binaryTree = createLeaf(root, binaryTree, counter, classID, attribute1);
			binaryTree = createLeaf(root, binaryTree, counter, classID, attribute2);
		} //for
		//print the tree to check that values were created correctly
		debugString += "Attribute name:  \tAttribute Value: \n\n";
		printBinaryTree(binaryTree, debugString);
		return binaryTree;
	} //buildTree

	/*
	Prints the values from the binary tree, from left to right.
	Mainly used for debugging.
	--------------------------------------------------
	@param binaryTree: ref struct BinaryTree - The ref struct containing the build tree
	@param debugString: String ^ - String ^ used for debug purposes
	*/
	void DecisionTree::printBinaryTree(ref struct BinaryTree ^binaryTree, String ^%debugString) {
		if(binaryTree != nullptr) {
			printBinaryTree(binaryTree->left, debugString);
			debugString += binaryTree->attributeName + " \t" + Conversion::convertDoubleToString(binaryTree->attributeValue) + "\n";
			printBinaryTree(binaryTree->right, debugString);
		} //if
	} //printBinaryTree

	/*
	Creates a new leaf from the passed ref struct ^node. The nodes parent will be based on the
	value parent and the attributes value is based on attrValue. If the leaf to be built
	already exists, then the node is returned. If the value doesn't exist from before,
	a new leaf node is built with the attribute value, the name "AttributeX" where X is the
	value of attributeNo.
	--------------------------------------------------
	@param parent: double - the parent of the leaf to be built
	@param node: ref struct BinaryTree - the node containing root and previously built leafs
	@param attributeNo: int - The byfer numeric value for the attribute leaf
	@param attrValue: double - The attribute value which the leaf is based on
	@return: ref struct BinaryTree - a ref struct containing the root and current leafs
	*/
	ref struct DecisionTree::BinaryTree ^DecisionTree::createLeaf(double parent, ref struct BinaryTree ^node, int &attributeNo, int classID, double attrValue) {
		//if the node exists with given value, return the node
		if(node != nullptr && (attrValue == node->attributeValue) && (classID == node->classID)) {
			return node;
		} //if
		if(node == nullptr) {
			//create a new leaf
			node = gcnew BinaryTree();
			node->attributeName = "Attribute" + Conversion::convertIntegerToString(attributeNo);
			node->attributeValue = attrValue;
			node->classID = classID;
			node->left = nullptr;
			node->right = nullptr;
			//increase counter for attribute number
			attributeNo++;
		} else if(attrValue <= parent) {
			//less then root, pass it left
			node->left = createLeaf(parent, node->left, attributeNo, classID, attrValue);
		} else {
			//greater then root, pass it right
			node->right = createLeaf(parent, node->right, attributeNo, classID, attrValue);
		} //if
		return node;
	} //createLeaf

	/*
	Attempts to find a value for the root by seperating the training data into
	two Lists containing the classes. Thereafter a List is filled with the
	distinct repeating attribute values for the two attribute list retrieved from
	the two classes. If no distinct attribute values are found an error is thrown.
	If only one repeating attribute value was found, the root is set to this value.
	If there were several distinct repeated attributes, a sort and check is done
	to retrieve the highest count for repeated attribute->
	(this function is not done, @see getRootValueFromList).
	--------------------------------------------------
	@param trainingData: List<Training> - List containing the
	values for the training samples; Training(a1, a2, classID)
	@throw: ExceptionClass - throws error if size of distinct attribute values is zero
	@return: double - the attribute value to be used as root value
	*/
	double DecisionTree::findRootValue(List<Training ^> ^%trainingData) {
		double root = 0;
		int max_size = 0,
			max_elementsC1 = 0,
			max_elementsC2 = 0,
			currentClassID = 0;
		List<Training ^> ^class1 = gcnew List<Training ^>();
		List<Training ^> ^class2 = gcnew List<Training ^>();
		List<DecisionTree::AttributeCounter ^> ^distinctAttributeValues = gcnew List<DecisionTree::AttributeCounter ^>();
		max_size = static_cast<int>(trainingData->Count);
		//loop through to fill up with values from current selected class
		for(int i = 0; i < max_size; i++) {
			currentClassID = trainingData[i]->getClassID();
			if(currentClassID == 1) {
				class1->Add(gcnew Training(trainingData[i]->getAttributeValue1(), trainingData[i]->getAttributeValue2()));
			} else {
				class2->Add(gcnew Training(trainingData[i]->getAttributeValue1(), trainingData[i]->getAttributeValue2()));
			} //if
		} //for
		distinctAttributeValues = fillListWithDistinctAttributes(class1, class2);
		//is there any distinct attributes?
		if(distinctAttributeValues->Count == 0) {
			String ^ error = "There are no distinct attributes. \nCan't select a root value!";
			throw gcnew ExceptionClass(error);
		} //if
		//get the value to be used for root from the List
		if(distinctAttributeValues->Count == 1) {
			root = distinctAttributeValues[0]->attributeValue;
		} else {
			root = getRootValueFromList(distinctAttributeValues);
		} //if
		return root;
	} //findRootValue

	/*
	Loops through the two class lists and checks for equal attributes in the two lists.
	The attributes that are checked for equality is:
	(c1a1 == c2a1 || c1a1 == c2a2) and (c1a2 == c2a2 || c1a2 == c2a1).
	If equality is found, it's checked if the value is registered from before,
	and if it is, the count is increased, if not the new value is added with a
	count of 1.
	--------------------------------------------------
	@param class1: List<Training > - List of attributes belonging to class1
	@param class2: List<Training > - List of attributes belonging to class2
	@return: List<DecisionTree::AttributeCounter> - A List containing structs
	based on the ref struct AttributeCounter.
	@see AttributeCounter: A ref struct containing two values, a value for the attribute
	and a counter for the number of equalities for given attribute
	*/
	List<DecisionTree::AttributeCounter^> ^DecisionTree::fillListWithDistinctAttributes(List<Training ^> ^class1, List<Training ^> ^class2) {
		int index = 0;
		double c1a1 = 0,
			c2a1 = 0,
			c1a2 = 0,
			c2a2 = 0,
			currentAttribute = 0;
		AttributeCounter ^attribute = gcnew AttributeCounter();
		List<DecisionTree::AttributeCounter ^> ^distinctAttributeValues = gcnew List<DecisionTree::AttributeCounter ^>();
		int max_elementsC1 = static_cast<int>(class1->Count);
		int max_elementsC2 = static_cast<int>(class2->Count);
		//loop through the Lists filled with class attributes to find repeating attributes
		for(int i = 0; i < max_elementsC1; i++) {
			c1a1 = class1[i]->getAttributeValue1();
			c1a2 = class1[i]->getAttributeValue2();
			for(int j = 0; j < max_elementsC2; j++) {
				c2a1 = class2[j]->getAttributeValue1();
				c2a2 = class2[j]->getAttributeValue2();
				//check if the current attributes are equal
				currentAttribute = returnRepeatingAttribute(c1a1, c2a1, c1a2, c2a2);
				if(currentAttribute != ATTRIBUTE_NOT_FOUND) {
					if(compareAttributesInList(distinctAttributeValues, currentAttribute, index)) {
						//already in List, increase count
						distinctAttributeValues[index]->numCurrentAttribute++;
					} else {
						//new attribute value, add to List
						attribute->numCurrentAttribute = 1;
						attribute->attributeValue = currentAttribute;
						distinctAttributeValues->Add(attribute);
					} //if
				} //if
			} //for
		} //for
		return distinctAttributeValues;
	} //fillListWithDistinctAttributes

	/*
	If there are more then one repeating value of the attributes, sort the list
	and check if the two last values are equal or not.
	IMPLEMENTATION NEED:
	here a further check would be needed if several attributes had the same count
	- this is not done due to lack of time
	--------------------------------------------------
	@param attributeList: List<DecisionTree::AttributeCounter> - the list with distinct attributes
	*/
	double DecisionTree::getRootValueFromList(List<DecisionTree::AttributeCounter^> ^%attributeList) {
		int max_size = 0;
		double root = 0;
		max_size = static_cast<int>(attributeList->Count);
		sortAttributesAscending(attributeList);
		int lastCount = 0,
			secLastCount = 0;
		lastCount = attributeList[max_size - 1]->numCurrentAttribute;
		secLastCount = attributeList[max_size - 2]->numCurrentAttribute;
		//check if the last value is greater then the rest
		if(lastCount > secLastCount) {
			root = attributeList[max_size - 1]->attributeValue;
			//NOT IMPLEMENTED: @above
		} //if
		return root;
	} //getRootValueFromList

	/*
	Loops through the attributes in the list and checks if the attribute value is registered.
	Returns true if the attribute exists, and false if its not in the list.
	--------------------------------------------------
	@param attributeList: List<DecisionTree::AttributeCounter> - the list with distinct attributes
	@param currentValue - double: the attribute value to check if exist in list
	@param index - int: byref variable to keep the index of found attribute
	@return: bool - if true, the attribute is in the attributeList, if false, not in the attributeList
	*/
	bool DecisionTree::compareAttributesInList(List<DecisionTree::AttributeCounter ^> ^%attributeList, double currentValue, int &index) {
		bool found = false;
		int counter = 0;
		int max_size = static_cast<int>(attributeList->Count);
		//loop through the List to see if the attribute is registered
		while(!found && counter < max_size) {
			if(currentValue == attributeList[counter]->attributeValue) {
				found = true;
				index = counter;
			} //if
			counter++;
		} //while
		return found;
	} //compareAttributesInList

	/*
	Returns the attribute thats common for both classes or
	the constant ATTRIBUTE_NOT_FOUND. The comparison is between
	(c1a1 == c2a1 || c1a1 == c2a2) and (c1a2 == c2a2 || c1a2 == c2a1).
	---------------------------------------------------------
	@param c1a1: double - The first attribute in class1
	@param c2a1: double - The first attribute in class2
	@param c1a2: double - The second attribute in class1
	@param c2a2: double - The second attribute in class2
	@return: double - the attribute || ATTRIBUTE_NOT_FOUND
	*/
	double DecisionTree::returnRepeatingAttribute(double c1a1, double c2a1, double c1a2, double c2a2) {
		if(c1a1 == c2a1 || c1a1 == c2a2) {
			return c1a1;
		} //if	
		if(c1a2 == c2a2 || c1a2 == c2a1) {
			return c1a2;
		} //if
		return ATTRIBUTE_NOT_FOUND;
	} //returnRepeatingAttribute

	/*
	Sorts the attributes in passed list in ascending order.
	--------------------------------------------------
	@param attributeList: List<DecisionTree::AttributeCounter> - the list with distinct attributes
	*/
	void DecisionTree::sortAttributesAscending(List<DecisionTree::AttributeCounter ^> ^%attributeList) {
		double currentValue = 0,
			nextValue = 0;
		AttributeCounter ^tempAttribute;
		int max_size = static_cast<int>(attributeList->Count);
		for(int i = 0; i < max_size - 1; i++) {
			currentValue = attributeList[i]->numCurrentAttribute;
			for(int j = i + 1; j < max_size; j++) {
				nextValue = attributeList[j]->numCurrentAttribute;
				//is the current count higher then the next?
				if(currentValue > nextValue) {
					//bubblesort the values
					tempAttribute = attributeList[j];
					attributeList[j] = attributeList[i];
					attributeList[i] = tempAttribute;
				} //if
			} //for
		} //for
	} //sortAttributesAscending

	/****************** LDA DECISION TREE *************************************/

	/*
	Creates a binary tree passed on the values passed in the list trainingData.
	By the use of the values it calculates and creates a matrix containing the 
	optimal line vectors, and creates a binary tree containing a root with 
	two children. The left child assigns the sample to either class1 or class2,
	while the other child is a "can't classify" child.
	--------------------------------------------------
	@param trainingData: List<Training > - List containing the
	values for the training samples; Training(a1, a2, classID)
	@param optimalLineMatrix: Matrix - Empty matrix to be filled with the optimal line vector
	@param meanMatrixC1: Matrix - Empty matrix to be filled with the mean values for class1
	@param meanMatrixC2: Matrix - Empty matrix to be filled with the mean values for class1
	@param debugString: String ^ - A reference string that gets filled up with data on 
	the tree build
	@throw: ExceptionClass - throws error if size of distinct attribute values is zero
	(cant select a root value)
	@return: Returns the binary decision tree
	*/
	ref struct DecisionTree::BinaryTreeLDA ^DecisionTree::createBinaryTree(List<Training ^> ^trainingData, 
		Matrix ^%optimalLineMatrix, Matrix ^%meanMatrixC1, Matrix ^%meanMatrixC2, String ^%debugString) {
		try {
			double root = 0,
				selectClassValue = 0;
			int max_size = 0;
			debugString += "Preparing to create binary tree...\n\n";
			root = findRootValueLDA(trainingData, optimalLineMatrix, meanMatrixC1, meanMatrixC2);
			//start to build the binary decision tree
			String ^name = "";
			BinaryTreeLDA ^binaryTree = gcnew BinaryTreeLDA();
			BinaryTreeLDA ^child = gcnew BinaryTreeLDA();
			name = "root";
			binaryTree = createLeaf(name, root, 0, nullptr, nullptr);
			name = "Compare values";
			selectClassValue = (optimalLineMatrix->getCellValueAt(0, 0) * meanMatrixC1->getCellValueAt(0, 0)) + (optimalLineMatrix->getCellValueAt(1, 0) * meanMatrixC1->getCellValueAt(1, 0));
			child = createLeaf(name, selectClassValue, 0, nullptr, nullptr);
			name = "Class1";
			child->left = createLeaf(name, 0, 1, nullptr, nullptr);
			name = "Class2";
			child->right = createLeaf(name, 0, 2, nullptr, nullptr);
			binaryTree->left = child;
			name = "Cannot classify sample";
			binaryTree->right = createLeaf(name, CAN_NOT_CLASSIFY_SAMPLE, CAN_NOT_CLASSIFY_SAMPLE, nullptr, nullptr);
			//add the result to the debugString
			debugString += "Root value has been selected.\n";
			debugString += "The root value is: " + Conversion::convertDoubleToString(root) + ".\n\n";
			debugString += "Starting to build tree...\n";
			debugString += "Root->Left has has child with value " + selectClassValue + ".\n";
			debugString += "Root->Left->Left has has child with value " + binaryTree->left->left->attributeName + ".\n";
			debugString += "Root->Left->Right has has child with value " + binaryTree->left->right->attributeName + ".\n";
			debugString += "Root->Right has has child with value \"" + binaryTree->right->attributeName + "\".\n";
			return binaryTree;
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
		return nullptr;
	} //createBinaryTree

	/*
	Creates a node containing the passed values
	--------------------------------------------------
	@param name: String ^ - The name of this leaf 
	@param attributeValue: double - 
	@param classID: int - The class id for this leaf || 0 if none
	@param nodeLeft: BinaryTreeLDA - the left child of this leaf || nullptr if none
	@param nodeRight: BinaryTreeLDA - the right child of this leaf || nullptr if none
	@return: Returns a leaf/node containing the passed values
	*/
	ref struct DecisionTree::BinaryTreeLDA ^DecisionTree::createLeaf(String ^name, double attributeValue,
		int classID, ref struct BinaryTreeLDA ^nodeLeft, ref struct BinaryTreeLDA ^nodeRight) {
		//create a new leaf
		BinaryTreeLDA ^node = gcnew BinaryTreeLDA();
		node->attributeName = name;
		node->attributeValue = attributeValue;
		node->classID = classID;
		node->left = nodeLeft;
		node->right = nodeRight;
		return node;
	} //createLeaf

	/*
	Finds the root value by use of the optimal line vector.
	--------------------------------------------------
	@param trainingData: List<Training> - List containing the
	values for the training samples; Training(a1, a2, classID)	
	@param optimalLineMatrix: Matrix - Empty matrix to be filled with the optimal line vector
	@param meanMatrixC1: Matrix - Empty matrix to be filled with the mean values for class1
	@param meanMatrixC2: Matrix - Empty matrix to be filled with the mean values for class1
	@return: double - the value to be used as root
	*/
	double DecisionTree::findRootValueLDA(List<Training ^> ^%trainingData, Matrix ^%optimalLineMatrix, Matrix ^%meanMatrixC1, Matrix ^%meanMatrixC2) {
		double root = 0;
		try {
			int noAttributes = 2;
			double meanX = 0,
				meanY = 0;
			//get the attributes for the first class and create its matrix
			//(then do the same for the second class)
			List<Training ^> ^tempList;
			tempList = returnClassList(trainingData, 1);
			Matrix ^class1 = returnClassMatrix(tempList);
			tempList = returnClassList(trainingData, 2);
			Matrix ^class2 = returnClassMatrix(tempList);
			//get the matrix containing the optimal line w
			optimalLineMatrix = returnOptimalLineMatrix(class1, class2);
			//get the mean values for both classes and divide by two (the no. of classes)
			meanMatrixC1 = class1->createMeanMatrix();
			meanMatrixC2 = class2->createMeanMatrix();
			meanX = meanMatrixC1->getCellValueAt(0, 0) + meanMatrixC2->getCellValueAt(0, 0);
			meanY = meanMatrixC1->getCellValueAt(1, 0) + meanMatrixC2->getCellValueAt(1, 0);
			meanX = meanX / static_cast<double>(noAttributes);
			meanY = meanY / static_cast<double>(noAttributes);
			//calculate the root value
			root = (optimalLineMatrix->getCellValueAt(0, 0) * meanX) + (optimalLineMatrix->getCellValueAt(1, 0) * meanY);
			//clean up
			class1->deleteMatrix();
			class2->deleteMatrix();
			delete class1;
			delete class2;
			delete tempList;
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
		return root;
	} //findRootValue

	/*
	Extracts all the values for given class based on passed class id and adds 
	it to a list which then is returned.
	--------------------------------------------------
	@param trainingData: List<Training> - List containing all the training data
	@param classID: int - The class to extract from the list
	@return: List<Training> - List containing all the values for the class based 
	on the passed classID
	*/
	List<Training ^> ^DecisionTree::returnClassList(List<Training ^> ^%trainingData, int classID) {
		List<Training ^> ^classList = gcnew List<Training ^>();
		for(int i = 0; i < trainingData->Count; i++) {
			if(trainingData[i]->getClassID() == classID) {
				classList->Add(trainingData[i]);
			} //if
		} //for
		return classList;
	} //returnClassList

	/*
	Fills a matrix with the values from the passed list
	--------------------------------------------------
	@param classList: List<Training> - List containing all the class values
	@return: Matrix containing all the values based on values passed in list
	*/
	Matrix ^DecisionTree::returnClassMatrix(List<Training ^> ^%classList) {
		int noAttributes = 2;
		Matrix ^classMatrix = gcnew Matrix(classList->Count, noAttributes);
		for(int i = 0; i < classList->Count; i++) {
			//since its only two attributes, no need for inner loop
			classMatrix->changeCellValueAt(i, 0, classList[i]->getAttributeValue1());
			classMatrix->changeCellValueAt(i, 1, classList[i]->getAttributeValue2());
		} //for
		return classMatrix;
	} //returnClassList

	/*
	Returns the optimal line matrix based on the calculation of 
	the covariance matrix, scatter matrix and the inverse of the 
	within class scatter matrix.
	--------------------------------------------------
	@param class1: Matrix - Matrix containing all the values from class1
	@param class2: Matrix - Matrix containing all the values from class2
	@return: 
	*/
	Matrix ^DecisionTree::returnOptimalLineMatrix(Matrix ^%class1, Matrix ^%class2) {
		Matrix ^optimalLineMatrix;
		try {
			//create the scatter matrices
			Matrix ^scatterMatrix1;
			scatterMatrix1 = class1->createScatterMatrix(class1, 
				class1->createCoVarianceMatrix(class1));
			Matrix ^scatterMatrix2;
			scatterMatrix2 = class2->createScatterMatrix(class2, 
				class2->createCoVarianceMatrix(class2));
			//create the within the class scatter matrix 
			//and inverse it to calculate the optimal line 
			Matrix ^Sw = class1->matrixAddition(scatterMatrix1, scatterMatrix2);
			optimalLineMatrix = Sw->getOptimalLineMatrix(Sw->inverse2DMatrix(), 
				class1->createMeanMatrix(), class2->createMeanMatrix());
			//clean up
			scatterMatrix1->deleteMatrix();
			scatterMatrix2->deleteMatrix();
			Sw->deleteMatrix();
			delete scatterMatrix1;
			delete scatterMatrix2;
			delete Sw;
		} catch(ExceptionClass ^) {
			throw;
		} //try/catch
		return optimalLineMatrix;
	} //returnClassList
} //namespace