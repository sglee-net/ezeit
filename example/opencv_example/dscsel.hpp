#ifndef __DSCSEL_HPP__
#define __DSCSEL_HPP__

#include <opencv2/core/core.hpp>
#include <opencv2/core/cvdef.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std; 
using namespace std::chrono;
using namespace cv;
using namespace cv::xfeatures2d;
using namespace cv::ml;

// frequency ratio
// for LOW frequency : 0.1
// for HIGH frequency: 0.9
void findCovCount (
	list<size_t> &_filteredIndexes,
	const vector<KeyPoint> &_keypoints,
	const Mat &_cov_mat,
	const string _covariance_type,
	const string _frequency_type,
	const double _frequency_ratio) {
	// index, count
	map<size_t,size_t> index_count_map;

	for(size_t i=0; i<_cov_mat.rows; i++) {
		for(size_t j=i+1; j<_cov_mat.cols; j++) {
			KeyPoint keypoint1 = _keypoints[i];
			KeyPoint keypoint2 = _keypoints[j];
			float cov = fabs(_cov_mat.at<float>(i,j));
			float dist = cv::norm(keypoint1.pt-keypoint2.pt);
			bool is_covariance_satisfied = false;
			if(_covariance_type == "HIGH") {
				if(fabs(cov) > 1e1) 
					is_covariance_satisfied = true;
			} else if (_type == "LOW") {
				if(fabs(cov) < 1e01) 
					is_covariance_satisfied = true;
			} else {
				throw ("unsupported type in findCovCount");
			}

			if(is_covariance_satisfied) {
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
				if(_frequency_type=="LOW") {
					// < ascending
					return l.second < r.second;
				} else if (_frequency_type=="HIGH") {
					// > descending
					return l.second > r.second;
				} else {
					throw ("unsupported type");
				}
			}
			return l.first < r.first;
		});


	vector<pair<size_t,size_t>>::iterator itr_index_count_list = 
		index_count_list.begin();
	size_t push_size = index_count_list.size() * _frequency_ratio;
	for(size_t i = 0; i < push_size; i++) {
		_filteredIndexes.push_back(itr_index_count_list->first);
		++itr_index_count_list;
	}
}

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
}

#endif
