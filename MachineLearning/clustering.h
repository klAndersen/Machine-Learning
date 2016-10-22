#pragma once

#ifndef CLUSTERING_H

#define CLUSTERING_H

#include "Stdafx.h"
#include "statistics.h"

namespace MachineLearning {
	
	/*
	Class that handles operations related to clusters
	and cluster analysis.
	-----------------------------------------
	@Author: Knut Lucas Andersen
	*/
	public ref class Clustering sealed {
	public:
		//default constructor
		Clustering();
		//destructor
		~Clustering();
		//constructor that reads from file and initializes cluster members
		Clustering(String ^centroidFile, String ^testFile, int noClusterGroups, int noAttributes);
		//returns the matrix containing the samples (unlabeled)
		Matrix ^%getSampleMatrix();
		//returns the matrix containing the centroids
		Matrix ^%getCentroidMatrix();
		//returns the  number of attributes
		int getNoAttributes();
		//returns the  number of (unlabeled) samples
		int getNoSamples();
		//returns the  number of centroid samples
		int getNoCentroids();
		//index1: iteration round, index2: cluster group
		List<List<int>^>^ getClusterGroupList();
		//returns a list of rescaled attribute values between 0 - 1
		static List<double> ^getRescaledList(int noAttributes);
		//calculates the distance from the selected centroids
		List<List<int>^>^ calculateDistanceFromCentroids(float maxIterations, int stopParameter,
			ComponentModel::BackgroundWorker^ worker, ComponentModel::DoWorkEventArgs ^ e);
	private:
		//number of selected centroids
		int _noCentroids;
		//number of unlabeled samples
		int _noSamples;
		//number of attributes belonging to the samples
		int _noAttributes;
		//matrix containing the unlabeled samples
		Matrix ^_sampleMatrix;
		//matrix containing the centroid samples
		Matrix ^_centroidMatrix;
		//variable that keeps the progress between updates
		int _progressFinished;
		//list containing the clustering results from each round
		//index1: iteration round, index2: cluster group
		List<List<int>^>^ _clusterGroupList;
		//calculates the distance between the passed value and mean, then raises 
		//the result to the power of 2 (does not sqrt the result!)
		double calculateDistance(double value, double mean);
		//gets the number of clusters in the list based on the cluster value
		int getClusterCount(List<int> ^clusterIndex, int clusterValue);
		//sets the  number of attributes
		void setNoAttributes(int noAttributes);
		//sets the  number of (unlabeled) samples
		void setNoSamples(int noSamples);
		//sets the number of centroid samples
		void setNoCentroids(int noCentroids);
		//returns a matrix filled with the values from the passed list
		void fillMatrixWithSamples(List<List<double>^> ^%sampleList, Matrix ^%sampleMatrix);
		//returns a matrix filled with (random) centroid values
		void fillCentroidMatrixWithSamples(List<List<double>^>^ %centroidList, Matrix ^%centroidMatrix, int noClusterGroups);
		//updates the value for the centroids based on result from current round
		Matrix ^updateCentroids(List<int> ^clusterIndex);
		//update the progressbar
		void  Clustering::updateProgressBar(int status, float total, System::ComponentModel::BackgroundWorker ^worker);
		//compare the current result with the previous to check if analysis is done
		bool isResultEqualToLastRound(int round, int stopParameter, List<List<int>^>^ %clusterGroupList, Statistics ^%stats);
	}; //Clustering
} //namespace
#endif