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

using namespace std;
using namespace std::chrono;
using namespace cv;
using namespace cv::xfeatures2d;
using namespace cv::ml;

struct lowCovariance {
	bool operator() (
		const KeyPoint &_keypoint1,
		const KeyPoint &_keypoint2,
		const float _distThreshold,
		const float _covariance,
		const float _covarianceThreshold) {
		float dist = cv::norm(_keypoint1.pt-_keypoint2.pt);
//		if(dist < _distThreshold) {
//			return false;
//		}
		if(fabs(_covariance) < _covarianceThreshold) {
			return true;
		} else {
			return false;
		}
	}
};

struct highCovariance {
	bool operator() (
		const KeyPoint &_keypoint1,
		const KeyPoint &_keypoint2,
		const float _distThreshold,
		const float _covariance,
		const float _covarianceThreshold) {
		float dist = cv::norm(_keypoint1.pt-_keypoint2.pt);
//		if(dist < _distThreshold) {
//			return false;
//		}
		if(fabs(_covariance) > _covarianceThreshold) {
			return true;
		} else {
			return false;
		}
	}
};

//void applyCovarCountLH(
//	list<size_t> &_filteredIndexes,
//	const vector<KeyPoint> &_keypoints,
//	const Mat &_covarMat) {
//	// index, count
//	map<size_t,size_t> index_count_map;
//
//	for(size_t i=0; i<_covarMat.rows; i++) {
//		for(size_t j=i+1; j<_covarMat.cols; j++) {
//			KeyPoint keypoint1 = _keypoints[i];
//			KeyPoint keypoint2 = _keypoints[j];
//			float covar = fabs(_covarMat.at<float>(i,j));
//			float dist = cv::norm(keypoint1.pt-keypoint2.pt);
//			// Covariance High
//			if(fabs(covar) < 1e-1) { // && dist > 60) {
//				map<size_t,size_t>::iterator itr_i = 
//					index_count_map.find(i);
//				if(itr_i == index_count_map.end()) {
//					index_count_map.insert(
//						pair<size_t,size_t>
//						(i,1));
//				} else {
//					itr_i->second++;
//				}
//
//				map<size_t,size_t>::iterator itr_j = 
//					index_count_map.find(j);
//				if(itr_j == index_count_map.end()) {
//					index_count_map.insert(
//						pair<size_t,size_t>
//						(j,1));
//				} else {
//					itr_j->second++;
//				}
//			}
//		}
//	}
//
//	// <index,count>
//	vector<pair<size_t,size_t>> index_count_list;
//	std::copy(index_count_map.begin(),
//		index_count_map.end(),
//		std::back_inserter<vector<pair<size_t,size_t>>>(
//			index_count_list));
//	std::sort(index_count_list.begin(),
//		index_count_list.end(),
//		[]( const pair<size_t,size_t> &l,
//			const pair<size_t,size_t> &r) {
//			if(l.second != r.second) {
//				// < ascending
//				//return l.second < r.second;
//				// > descending
//				return l.second > r.second;
//			}
//			return l.first < r.first;
//		});
//
//
//	vector<pair<size_t,size_t>>::iterator itr_index_count_list = 
//		index_count_list.begin();
//	size_t pop_size = index_count_list.size() * 0.1;
//	for(size_t i = 0; i < pop_size; i++) {
//		index_count_list.pop_back();
//	}
//
//	for(pair<size_t,size_t> element: index_count_list) {
//		_filteredIndexes.remove(element.first);
//	}
//
//	cout<<"size of keypoints: "<<_keypoints.size()<<endl;
//	cout<<"size of index_count_list: "<<index_count_list.size()<<endl;
//	cout<<"size of filteredIndexes: "<<_filteredIndexes.size()<<endl;
//}

void addCovarCountHL (
	list<size_t> &_filteredIndexes,
	const vector<KeyPoint> &_keypoints,
	const Mat &_covarMat) {
	// index, count
	map<size_t,size_t> index_count_map;

	for(size_t i=0; i<_covarMat.rows; i++) {
		for(size_t j=i+1; j<_covarMat.cols; j++) {
			KeyPoint keypoint1 = _keypoints[i];
			KeyPoint keypoint2 = _keypoints[j];
			float covar = fabs(_covarMat.at<float>(i,j));
			float dist = cv::norm(keypoint1.pt-keypoint2.pt);
			// Covariance High 
			if(fabs(covar) > 1e1) { //&& dist > 60) {
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

void addCovarCountLL (
	list<size_t> &_filteredIndexes,
	const vector<KeyPoint> &_keypoints,
	const Mat &_covarMat) {
	// index, count
	map<size_t,size_t> index_count_map;

	for(size_t i=0; i<_covarMat.rows; i++) {
		for(size_t j=i+1; j<_covarMat.cols; j++) {
			KeyPoint keypoint1 = _keypoints[i];
			KeyPoint keypoint2 = _keypoints[j];
			float covar = fabs(_covarMat.at<float>(i,j));
			float dist = cv::norm(keypoint1.pt-keypoint2.pt);
			// Covariance Low 
			if(fabs(covar) < 1e-1) {// && dist > 10) {
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

void removeCovarCountHH (
	list<size_t> &_filteredIndexes,
	const vector<KeyPoint> &_keypoints,
	const Mat &_covarMat) {
	// index, count
	map<size_t,size_t> index_count_map;

	for(size_t i=0; i<_covarMat.rows; i++) {
		for(size_t j=i+1; j<_covarMat.cols; j++) {
			KeyPoint keypoint1 = _keypoints[i];
			KeyPoint keypoint2 = _keypoints[j];
			float covar = fabs(_covarMat.at<float>(i,j));
			float dist = cv::norm(keypoint1.pt-keypoint2.pt);
			// Covariance High
			if(fabs(covar) > 1e3){// && dist > 10) {
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


//void findDefectKeypoints(
//	list<size_t> &_filteredIndexes,
//	const vector<KeyPoint> &_keypoints,
//	const Mat &_covarMat) {
//	// index, count
//	map<size_t,size_t> index_count_map;
//
//	for(size_t i=0; i<_covarMat.rows; i++) {
//		for(size_t j=i+1; j<_covarMat.cols; j++) {
//			KeyPoint keypoint1 = _keypoints[i];
//			KeyPoint keypoint2 = _keypoints[j];
//			float covar = fabs(_covarMat.at<float>(i,j));
//			float dist = cv::norm(keypoint1.pt-keypoint2.pt);
//			if(fabs(covar) > 1e3) { // && dist > 60) {
////			if(_func(
////				keypoint1, 
////				keypoint2, 
////				_distThreshold,
////				covar,
////				_funcThreshold)) {
//				map<size_t,size_t>::iterator itr_i = 
//					index_count_map.find(i);
//				if(itr_i == index_count_map.end()) {
//					index_count_map.insert(
//						pair<size_t,size_t>
//						(i,1));
//				} else {
//					itr_i->second++;
//				}
//
//				map<size_t,size_t>::iterator itr_j = 
//					index_count_map.find(j);
//				if(itr_j == index_count_map.end()) {
//					index_count_map.insert(
//						pair<size_t,size_t>
//						(j,1));
//				} else {
//					itr_j->second++;
//				}
//			}
//		}
//	}
//
//	// <index,count>
//	vector<pair<size_t,size_t>> index_count_list;
//	std::copy(index_count_map.begin(),
//		index_count_map.end(),
//		std::back_inserter<vector<pair<size_t,size_t>>>(
//			index_count_list));
//	std::sort(index_count_list.begin(),
//		index_count_list.end(),
//		[]( const pair<size_t,size_t> &l,
//			const pair<size_t,size_t> &r) {
//			if(l.second != r.second) {
//				// < ascending
//				//return l.second < r.second;
//				// > descending
//				return l.second > r.second;
//			}
//			return l.first < r.first;
//		});
//
////	int i = 0;
////	cout<< "index_count_map" <<endl;
////	for(pair<size_t,size_t> element: index_count_map) {
////		cout << i << ", " 
////		<< element.first << ", " << element.second << endl;
////		i++;
////	}
////
////	i = 0;
////	cout << "index_count_list" << endl;
////	for(pair<size_t,size_t> element: index_count_list) {
////		cout << i << ", " 
////		<< element.first << ", " << element.second << endl;
////		i++;
////	}
//
////	i=0;
////	vector<pair<size_t,size_t>>::iterator itr_index_count_list = 
////		index_count_list.begin();
////	while(itr_index_count_list != index_count_list.end()) {
////		_filteredIndexes.push_back(itr_index_count_list->first);
////		++itr_index_count_list;
////		if(i > index_count_list.size()*0.05) {
////			break;
////		}
////		i++;
////	}
// // //	vector<pair<size_t,size_t>>::iterator itr_index_count_list = 
//		index_count_list.begin();
//	size_t pop_size = index_count_list.size() * 0.1;
//	for(size_t i = 0; i < pop_size; i++) {
//		index_count_list.pop_back();
//	}
//
//	_filteredIndexes.clear();
//	for(size_t i=0; i<_keypoints.size(); i++) {
//		_filteredIndexes.push_back(i);
//	}
//
//	for(pair<size_t,size_t> element: index_count_list) {
//		_filteredIndexes.remove(element.first);
//	}
//
//	cout<<"size of keypoints: "<<_keypoints.size()<<endl;
//	cout<<"size of index_count_list: "<<index_count_list.size()<<endl;
//	cout<<"size of filteredIndexes: "<<_filteredIndexes.size()<<endl;
//
//
//}



//template <typename T>
//void findEffectiveKeypoints(
//	list<size_t> &filteredIndexes,
//	const vector<KeyPoint> &_keypoints,
//	const float _distThreshold,
//	const Mat &_covarMat, 
//	T _func,
//	const float _funcThreshold) {
//	// index, count
//	map<size_t,size_t> index_count_map;
//
//	for(size_t i=0; i<_covarMat.rows; i++) {
//		for(size_t j=i+1; j<_covarMat.cols; j++) {
//			KeyPoint keypoint1 = _keypoints[i];
//			KeyPoint keypoint2 = _keypoints[j];
//			float covar = fabs(_covarMat.at<float>(i,j));
//			if(_func(
//				keypoint1, 
//				keypoint2, 
//				_distThreshold,
//				covar,
//				_funcThreshold)) {
//				map<size_t,size_t>::iterator itr_i = 
//					index_count_map.find(i);
//				if(itr_i == index_count_map.end()) {
//					index_count_map.insert(
//						pair<size_t,size_t>
//						(i,1));
//				} else {
//					itr_i->second++;
//				}
//
//				map<size_t,size_t>::iterator itr_j = 
//					index_count_map.find(j);
//				if(itr_j == index_count_map.end()) {
//					index_count_map.insert(
//						pair<size_t,size_t>
//						(j,1));
//				} else {
//					itr_j->second++;
//				}
//			}
//		}
//	}
//
//	// <index,count>
//	vector<pair<size_t,size_t>> index_count_list;
//	std::copy(index_count_map.begin(),
//		index_count_map.end(),
//		std::back_inserter<vector<pair<size_t,size_t>>>(
//			index_count_list));
//	std::sort(index_count_list.begin(),
//		index_count_list.end(),
//		[]( const pair<size_t,size_t> &l,
//			const pair<size_t,size_t> &r) {
//			if(l.second != r.second) {
//				return l.second < r.second;
//			}
//			return l.first < r.first;
//		});
//
//	int i = 0;
//	cout<< "index_count_map" <<endl;
//	for(pair<size_t,size_t> element: index_count_map) {
//		cout << i << ", " 
//		<< element.first << ", " << element.second << endl;
//		i++;
//	}
//
//	i = 0;
//	cout << "index_count_list" << endl;
//	for(pair<size_t,size_t> element: index_count_list) {
//		cout << i << ", " 
//		<< element.first << ", " << element.second << endl;
//		i++;
//	}
//
//	i=0;
//	vector<pair<size_t,size_t>>::iterator itr_index_count_list = 
//		index_count_list.begin();
//	while(itr_index_count_list != index_count_list.end()) {
//		filteredIndexes.push_back(itr_index_count_list->first);
//		++itr_index_count_list;
//		if(i > index_count_list.size()*0.80) {
//			break;
//		}
//		i++;
//	}
//
////	size_t count_threshold = size_t(index_count_map.size() * 0.05);
////	cout<<"count threshold : " << count_threshold <<endl;
////
////	vector<pair<size_t,size_t>>::iterator itr_index_count_list = 
////		index_count_list.begin();
////	while(itr_index_count_list != index_count_list.end()) {
////		if(itr_index_count_list->second > count_threshold) {
////			filteredIndexes.push_back(itr_index_count_list->first);
////		}
////		++itr_index_count_list;
//////		if(itr_index_count_list->second < count_threshold) {
//////			itr_index_count_list = 
//////				index_count_list.erase(itr_index_count_list);
//////		} else {
//////			++itr_index_count_list;
//////		}
////	}
//
////	filteredIndexes.clear();
////	for(size_t i=0; i<_keypoints.size(); i++) {
////		filteredIndexes.push_back(i);
////	}
////
////	cout<<index_count_list.size()<<endl;
////
////	for(pair<size_t,size_t> element: index_count_list) {
//////		cout << "index,count " 
//////			<< element.first 
//////			<< ", "
//////			<<element.second<<endl;
////		filteredIndexes.remove(element.first);
////	}
//
//	cout<<filteredIndexes.size()<<endl;
//}




 //void getCorrelMatrix(
 //	Mat &_output,
 //	const Mat &_input) {
 //	Mat covarMat, meanMat, varMat;
 //	meanStdDev(
 //		_input,
 //		meanMat,
 //		varMat);
 //	pow(varMat,2.0,varMat);
 //
 // //	cout<<_input.rows<<", "<<meanMat.rows<<endl;
 // //	assert(_input.rows==meanMat.rows);
 //
 //	cv::calcCovarMatrix(
 //		_input,
 //		covarMat,
 //		meanMat,
 //		COVAR_NORMAL | COVAR_COLS | COVAR_SCALE);
 //	cout<<covarMat.rows<<", "<<covarMat.cols<<endl;
 //
 //	covarMat.copyTo(_output);
 //}

int main(void)
{
	std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
	std::chrono::duration<double> sec;

	cout<<"OpenCV example"<<endl;

//	string path = "./refImage.jpeg";
	//string path = "./solid.jpeg";
//	string path = "/mnt/sda1/images/adc/Detection/overlapping/spot_656.jpg";
//	string path = "/mnt/sda1/images/adc/Detection/overlapping/spot_439.jpg";
	string path = "/mnt/sda1/images/adc/Detection/overlapping/brg_37.jpg";
//	string path = "/mnt/sda1/images/adc/Detection/overlapping/lsac_18.jpg";
	Mat refImage;
	refImage = imread(path,1);
	if(refImage.empty()) {
		cout<<"refImage is empty"<<endl;
		return -1;
	}

	Mat blurImage3;
	Mat blurImage5;
	Mat blurImage7;
	Mat blurImage9;
	Mat blurImage11;

	GaussianBlur(refImage, 
		blurImage3,
		Size(3,3),
		0,
		0);
	GaussianBlur(refImage, 
		blurImage5,
		Size(5,5),
		0,
		0);
	GaussianBlur(refImage, 
		blurImage7,
		Size(7,7),
		0,
		0);
	GaussianBlur(refImage, 
		blurImage9,
		Size(9,9),
		0,
		0);
	GaussianBlur(refImage, 
		blurImage11,
		Size(11,11),
		0,
		0);

	// step1: detect the keypoints
	
//	int minHessian = 1000;
//	Ptr<Feature2D> detector = SURF::create(minHessian);
	Ptr<Feature2D> detector = AKAZE::create(
		AKAZE::DESCRIPTOR_MLDB,
		0,
		3,
		0.0015f);

	vector<KeyPoint> keypoints;
	
	// step2: calculate descriptors (fecture vectors)
	Mat descriptor;
	detector->detectAndCompute(
		blurImage9, 
		Mat(), 
		keypoints, 
		descriptor);

	cout<<descriptor.rows<<", "<<descriptor.cols<<endl;

	Mat covarMat, meanMat;
	cv::calcCovarMatrix(
		descriptor,
		covarMat,
		meanMat,
		COVAR_NORMAL | COVAR_COLS | COVAR_SCALE);
	cout<<covarMat.rows<<", "<<covarMat.cols<<endl;
// 	Mat correlMat;
// 	getCorMatrix(
//		correlMat,
//		descriptor);

//	map<float,pair<size_t,size_t>> sortedIndex;
//	//list<pair<float,pair<size_t,size_t>>> index_pairs; 
//	list<size_t> filteredIndexes;
////	highCovariance func;
//	lowCovariance func;
//	float funcThreshold = 0.1;//1e5;
//	findEffectiveKeypoints(
//		//index_pairs, 
//		filteredIndexes,
//		keypoints,
//		64,
//		covarMat, 
//		func,
//		funcThreshold);

	list<size_t> filteredIndexes;
	for(size_t i = 0; i < keypoints.size(); i++) {
//		filteredIndexes.push_back(i);
	}

	addCovarCountHL(
		filteredIndexes,
		keypoints,
		covarMat);

	addCovarCountLL(
		filteredIndexes,
		keypoints,
		covarMat);

	removeCovarCountHH(
		filteredIndexes,
		keypoints,
		covarMat);


//	int i=0;
//	for_each(sortedIndex.begin(),
//		sortedIndex.end(),
//		[&](pair<float,pair<size_t,size_t> > a_pair) {
// //		cout<<i<<", "<<a_pair.first<<", "<<a_pair.second.first<<", "<<a_pair.second.second<<endl;
//			i++;
//		});



//	list<size_t> indexes;
//	i=0;
//	for_each(index_pairs.begin(),
//		index_pairs.end(),
//		[&](pair<float,pair<size_t,size_t>> a_pair) {
////			cout
////			<< i
////			<<", "
////			<<a_pair.first
////			<<", "
////			<<a_pair.second.first
////			<<", "
////			<<a_pair.second.second
////			<<endl;
//			indexes.push_back(a_pair.second.first);
//			indexes.push_back(a_pair.second.second);
//			i++;
//		});
//	indexes.sort();
//	indexes.unique();
//
//	list<size_t> filteredIndexes;
//	for(size_t i=0; i<keypoints.size(); i++) {
//		filteredIndexes.push_back(i);
//	}
//
//	for_each(indexes.begin(),
//		indexes.end(),
//		[&](size_t index) {
//			filteredIndexes.remove(index);
//		//filteredIndexes.push_back(index);
//			//cout << index <<endl;
//			i++;
//		});

	Mat filteredMat;
	vector<KeyPoint> filteredKeypoints;
	for(list<size_t>::const_iterator citr = filteredIndexes.begin();
		citr != filteredIndexes.end();
		++citr) {
		filteredMat.push_back(descriptor.row(*citr));
		filteredKeypoints.push_back(keypoints[*citr]);
	}

 // 	int index =0;
 //	for(map<float,pair<size_t,size_t>>::const_iterator citr=
 //		sortedIndex.begin();
 //		citr!=sortedIndex.end();
 //		citr++) {
 // 			cout
 //			<<index
 //			<<", "
 //			<<citr->first
 //			<<", "
 //			<<citr->second.first
 //			<<", "
 //			<<citr->second.second<<endl;
 //			if(index > 3000) break;
 //			index++;
 //	}

	Mat descriptor2;
	descriptor.copyTo(descriptor2);

/*
	// step3: match descriptor
	FlannBasedMatcher matcher;
	std::vector<DMatch> matches;
	matcher.match(descriptor, descriptor2, matches);
	for(int i=0; i<descriptor.rows; i++) {
		double dist = matches[i].distance;
		cout<<dist<<endl;
	}
*/
	Mat keypointsImage;
	drawKeypoints(
		refImage, 
		keypoints, 
		keypointsImage, 
		Scalar::all(-1), 
		DrawMatchesFlags::DEFAULT);

	Mat filteredKeypointsImage;
	drawKeypoints(
		refImage, 
		filteredKeypoints, 
		filteredKeypointsImage, 
		Scalar::all(-1), 
		DrawMatchesFlags::DEFAULT);

	namedWindow("refImage", WINDOW_NORMAL);
	imshow("refImage", keypointsImage);
	imshow("keypoints Image", filteredKeypointsImage);
	imshow("descriptor", descriptor);
	waitKey(0);

	return 0;
}
