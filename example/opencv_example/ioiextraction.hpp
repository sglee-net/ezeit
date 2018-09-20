#ifndef __ROIEXTRACTION_HPP__
#define __ROIEXTRACTION_HPP__
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <map>
#include <list>
#include <algorithm>
#include <time.h>
#include <chrono>
#include <unistd.h>
#include <math.h>
#include <string>
#include <thread>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/core/cvdef.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/ml/ml.hpp>
#include <bsp/QuadTreeNode.h>
#include <bsp/QuadTreePoint.h>
#include <bsp/QuadTreePointCollection.h>
#include <bsp/QuadTree.h>
#include <statistics/statistics.h>

using namespace std; 
using namespace std::chrono;
using namespace cv;
using namespace cv::xfeatures2d;
using namespace cv::ml;

void addCovCountHL (
	list<size_t> &_filteredIndexes,
	const vector<KeyPoint> &_keypoints,
	const Mat &_cov_mat) {
	// index, count
	map<size_t,size_t> index_count_map;

	for(size_t i=0; i<_cov_mat.rows; i++) {
		for(size_t j=i+1; j<_cov_mat.cols; j++) {
			KeyPoint keypoint1 = _keypoints[i];
			KeyPoint keypoint2 = _keypoints[j];
			float cov = fabs(_cov_mat.at<float>(i,j));
			float dist = cv::norm(keypoint1.pt-keypoint2.pt);
			// Covariance High 
			if(fabs(cov) > 1e1) { //&& dist > 60) {
				map<size_t,size_t>::iterator itr_i = 
					index_count_map.find(i);
				if(itr_i == index_count_map.end()) {
					index_count_map.insert(
						pair<size_t,size_t>
						(i,1));
				} else {
					itr_i->second++;
				}

				map<size_t,size_t>::iterator itr_j = 
					index_count_map.find(j);
				if(itr_j == index_count_map.end()) {
					index_count_map.insert(
						pair<size_t,size_t>
						(j,1));
				} else {
					itr_j->second++;
				}
			}
		}
	}

	// <index,count>
	vector<pair<size_t,size_t>> index_count_list;
	std::copy(index_count_map.begin(),
		index_count_map.end(),
		std::back_inserter<vector<pair<size_t,size_t>>>(
			index_count_list));
	std::sort(index_count_list.begin(),
		index_count_list.end(),
		[]( const pair<size_t,size_t> &l,
			const pair<size_t,size_t> &r) {
			if(l.second != r.second) {
				// < ascending
				return l.second < r.second;
				// > descending
				//return l.second > r.second;
			}
			return l.first < r.first;
		});


	vector<pair<size_t,size_t>>::iterator itr_index_count_list = 
		index_count_list.begin();
	size_t push_size = index_count_list.size() * 0.1;
	for(size_t i = 0; i < push_size; i++) {
		_filteredIndexes.push_back(itr_index_count_list->first);
		++itr_index_count_list;
	}

	cout<<"size of keypoints: "<<_keypoints.size()<<endl;
	cout<<"size of index_count_list: "<<index_count_list.size()<<endl;
	cout<<"size of filteredIndexes: "<<_filteredIndexes.size()<<endl;
}

void addCovCountLL (
	list<size_t> &_filteredIndexes,
	const vector<KeyPoint> &_keypoints,
	const Mat &_cov_mat) {
	// index, count
	map<size_t,size_t> index_count_map;

	for(size_t i=0; i<_cov_mat.rows; i++) {
		for(size_t j=i+1; j<_cov_mat.cols; j++) {
			KeyPoint keypoint1 = _keypoints[i];
			KeyPoint keypoint2 = _keypoints[j];
			float cov = fabs(_cov_mat.at<float>(i,j));
			float dist = cv::norm(keypoint1.pt-keypoint2.pt);
			// Covariance Low 
			if(fabs(cov) < 1e-1) {// && dist > 10) {
				map<size_t,size_t>::iterator itr_i = 
					index_count_map.find(i);
				if(itr_i == index_count_map.end()) {
					index_count_map.insert(
						pair<size_t,size_t>
						(i,1));
				} else {
					itr_i->second++;
				}

				map<size_t,size_t>::iterator itr_j = 
					index_count_map.find(j);
				if(itr_j == index_count_map.end()) {
					index_count_map.insert(
						pair<size_t,size_t>
						(j,1));
				} else {
					itr_j->second++;
				}
			}
		}
	}

	// <index,count>
	vector<pair<size_t,size_t>> index_count_list;
	std::copy(index_count_map.begin(),
		index_count_map.end(),
		std::back_inserter<vector<pair<size_t,size_t>>>(
			index_count_list));
	std::sort(index_count_list.begin(),
		index_count_list.end(),
		[]( const pair<size_t,size_t> &l,
			const pair<size_t,size_t> &r) {
			if(l.second != r.second) {
				// < ascending
				return l.second < r.second;
				// > descending
				//return l.second > r.second;
			}
			return l.first < r.first;
		});


	vector<pair<size_t,size_t>>::iterator itr_index_count_list = 
		index_count_list.begin();
	size_t push_size = index_count_list.size() * 0.1;
	for(size_t i = 0; i < push_size; i++) {
		_filteredIndexes.push_back(itr_index_count_list->first);
		++itr_index_count_list;
	}

	cout<<"size of keypoints: "<<_keypoints.size()<<endl;
	cout<<"size of index_count_list: "<<index_count_list.size()<<endl;
	cout<<"size of filteredIndexes: "<<_filteredIndexes.size()<<endl;
}

void removeCovCountHH (
	list<size_t> &_filteredIndexes,
	const vector<KeyPoint> &_keypoints,
	const Mat &_cov_mat) {
	// index, count
	map<size_t,size_t> index_count_map;

	for(size_t i=0; i<_cov_mat.rows; i++) {
		for(size_t j=i+1; j<_cov_mat.cols; j++) {
			KeyPoint keypoint1 = _keypoints[i];
			KeyPoint keypoint2 = _keypoints[j];
			float cov = fabs(_cov_mat.at<float>(i,j));
			float dist = cv::norm(keypoint1.pt-keypoint2.pt);
			// Covariance High
			if(fabs(cov) > 1e3){// && dist > 10) {
				map<size_t,size_t>::iterator itr_i = 
					index_count_map.find(i);
				if(itr_i == index_count_map.end()) {
					index_count_map.insert(
						pair<size_t,size_t>
						(i,1));
				} else {
					itr_i->second++;
				}

				map<size_t,size_t>::iterator itr_j = 
					index_count_map.find(j);
				if(itr_j == index_count_map.end()) {
					index_count_map.insert(
						pair<size_t,size_t>
						(j,1));
				} else {
					itr_j->second++;
				}
			}
		}
	}

	// <index,count>
	vector<pair<size_t,size_t>> index_count_list;
	std::copy(index_count_map.begin(),
		index_count_map.end(),
		std::back_inserter<vector<pair<size_t,size_t>>>(
			index_count_list));
	std::sort(index_count_list.begin(),
		index_count_list.end(),
		[]( const pair<size_t,size_t> &l,
			const pair<size_t,size_t> &r) {
			if(l.second != r.second) {
				// < ascending
				//return l.second < r.second;
				// > descending
				return l.second > r.second;
			}
			return l.first < r.first;
		});


	vector<pair<size_t,size_t>>::iterator itr_index_count_list = 
		index_count_list.begin();
	size_t pop_size = index_count_list.size() * 0.1;
	for(size_t i = 0; i < pop_size; i++) {
		index_count_list.pop_back();
	}

	for(pair<size_t,size_t> element: index_count_list) {
		_filteredIndexes.remove(element.first);
	}

	cout<<"size of keypoints: "<<_keypoints.size()<<endl;
	cout<<"size of index_count_list: "<<index_count_list.size()<<endl;
	cout<<"size of filteredIndexes: "<<_filteredIndexes.size()<<endl;
}

void extract_IOI(
	list<size_t> &_IOI, // Index Of Interesting
	const Mat &_descriptors,
	const vector<KeyPoint> &_keypoints,
	const map<string,double> &_properties) {
	
	Mat cov_mat, mean_mat;

	cv::calcCovarMatrix(
		_descriptors,
		cov_mat,
		mean_mat,
		COVAR_NORMAL | COVAR_COLS | COVAR_SCALE);

	addCovCountHL(
		_IOI,
		_keypoints,
		cov_mat);

	addCovCountLL(
		_IOI,
		_keypoints,
		cov_mat);

	removeCovCountHH(
		_IOI,
		_keypoints,
		cov_mat);
}

#endif
