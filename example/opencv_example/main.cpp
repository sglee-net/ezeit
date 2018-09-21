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
#include <statistics/statistics.hpp>
#include "ioiextraction.hpp"
#include "qtreefunc_density.hpp"

using namespace std;
using namespace std::chrono;
using namespace cv;
using namespace cv::xfeatures2d;
using namespace cv::ml;


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

void inverse(Mat &_img) {
	if(_img.channels() == 1) {
		for(size_t j=0; j<_img.rows; j++) {
			uchar *p=_img.ptr<uchar>(j);
			for(size_t i=0; i<_img.cols; i++) {
				p[i] = 255 - p[i];
			}
		}
	} else if(_img.channels() == 3) {
		for(size_t j=0; j<_img.rows; j++) {
			Vec3b *p=_img.ptr<Vec3b>(j);
			for(size_t i=0; i<_img.cols; i++) {
				p[i][0] = 255 - p[i][0];
				p[i][1] = 255 - p[i][1];
				p[i][2] = 255 - p[i][2];
			}
		}
	}
}

int main(void)
{
	std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
	std::chrono::duration<double> sec;

	cout<<"OpenCV example"<<endl;

//	string path = "./refImage.jpeg";
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

//	inverse(refImage);
//	threshold(refImage, refImage, 100, 200, 0);

	const size_t RAW_IMAGE = 1;
	const size_t BLUR3_IMAGE = 3;
	const size_t BLUR5_IMAGE = 5;
	const size_t BLUR7_IMAGE = 7;
	const size_t BLUR9_IMAGE = 9;
	const size_t BLUR_INCREMENT = 2;
	// 1 : raw image
	// 3 : blur 3x3
	// 5 : blur 5x5
	// 7 : blur 7x7
	// 9 : blur 9x9
	const size_t blur_pixel_start = RAW_IMAGE;
	const size_t blur_pixel_end = BLUR9_IMAGE;
	
	map<size_t,Mat *> image_map;
	for(size_t i = RAW_IMAGE; 
		i <= BLUR9_IMAGE; 
		i = i+BLUR_INCREMENT) {
		Mat *blur_image = new Mat();
		if(i == RAW_IMAGE) {
			refImage.copyTo(*blur_image);
		} else {
			GaussianBlur(refImage, 
				*blur_image,
				Size(i,i),
				0,
				0);
		}
		image_map.insert(pair<size_t,Mat *>(
			i,blur_image));
	}

	///////////////////////////////////////////////////////////////////
	// Step. find countours
	const int canny_threshold1 = 100;
	const int canny_threshold2 = 200;
	const int canny_area_detection_threshold = 50;
	const Scalar canny_rect_color = Scalar(0,0,255);
	const Scalar canny_circle_color = Scalar(0,255,0);
	const Scalar canny_contour_color = Scalar(255,0,0);
	Mat cannyImage;
	refImage.copyTo(cannyImage);
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);

	Canny(*(image_map.find(RAW_IMAGE)->second),
		canny_output, 
		canny_threshold1, 
		canny_threshold2, 
		3);
	findContours(
		canny_output, 
		contours, 
		hierarchy, 
		RETR_EXTERNAL, 
//		RETR_CCOMP,
//		RETR_TREE,
		CHAIN_APPROX_SIMPLE, 
		Point(0,0));
	Mat contourMat = Mat::zeros(canny_output.size(), CV_8UC3);
	for(size_t i=0; i<contours.size(); i++) {
		Scalar color = 
			Scalar(rng.uniform(0,255),
			rng.uniform(0,255),
			rng.uniform(0,25));
		drawContours(
			contourMat,
			contours,
			(int)i,
			color,
			1,
			4,
			hierarchy,
			0,
			Point());
	}

	Rect roi;
	for(size_t i=0; i<contours.size(); i++) {
		double area = contourArea(contours[i], false);
//		cout<<i<<" area: "<<area<<endl;
		if(fabs(area) > canny_area_detection_threshold) {
			roi = boundingRect(contours[i]);
			double aspect_ratio = 
				std::max(roi.height,roi.width)/
				std::min(roi.height,roi.width);
//			cout<< aspect_ratio <<endl;
			if(aspect_ratio < 2.0) {
				Point2f center;
				float radius=0.0f;
				minEnclosingCircle(contours[i],
					center,
					radius);

				rectangle(
					cannyImage, 
					roi,
					canny_rect_color,
					2, 
					LINE_8,
					0);
				circle(
					cannyImage,
					center,
					radius,
					canny_circle_color);
				drawContours(
					cannyImage,
					contours,
					(int)i,
					canny_contour_color,
					1,
					4,
					hierarchy,
					0,
					Point());
					}

		}
	}
	///////////////////////////////////////////////////////////////////

	
	///////////////////////////////////////////////////////////////////
	// step1: create the method to find out keypoints 
	
//	int minHessian = 1000;
//	Ptr<Feature2D> detector = SURF::create(minHessian);
	
	Ptr<Feature2D> detector = AKAZE::create(
		AKAZE::DESCRIPTOR_MLDB,
		0,
		3,
		0.0015f);

	// step2: compute keypoints and descriptors (fecture vectors)
	map<size_t,vector<KeyPoint> *> keypoints_map;
	map<size_t,Mat *> descriptors_map;
	for(size_t i = RAW_IMAGE; 
		i <= BLUR9_IMAGE; 
		i = i+BLUR_INCREMENT) {
		vector<KeyPoint> *keypoints = new vector<KeyPoint>();
		keypoints_map.insert(pair<size_t,vector<KeyPoint> *>(
				i,keypoints));
		
		Mat *descriptors = new Mat(*(image_map.find(i)->second));
		descriptors_map.insert(pair<size_t, Mat *>(
				i,descriptors));

		detector->detectAndCompute(
			*image_map.find(i)->second,
			Mat(), 
			*keypoints, 
			*descriptors);
	}
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	// extract IOI (Index of Interest)
	const vector<KeyPoint> *ioi_keypoints = 0;
	ioi_keypoints = keypoints_map.find(1)->second;
	const Mat *ioi_descriptors = 0;
	ioi_descriptors = descriptors_map.find(1)->second;

	map<string, double> ioi_properties;
	list<size_t> filteredIndexes;
	extract_IOI(
		filteredIndexes,
		*ioi_descriptors,
		*ioi_keypoints,
		ioi_properties);

	Mat filteredMat;
	vector<KeyPoint> filteredKeypoints;
	for(list<size_t>::const_iterator citr = filteredIndexes.begin();
		citr != filteredIndexes.end();
		++citr) {
		filteredMat.push_back(ioi_descriptors->row(*citr));
		filteredKeypoints.push_back(ioi_keypoints->at(*citr));
	}
	///////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////
	// make QuadTree and PointCollection according to each image
	const int image_width = refImage.rows;
	const int image_height = refImage.cols;
	const int cell_size = 64;
	int x_min = numeric_limits<int>::max();
	int x_max = 0;
	int y_min = numeric_limits<int>::max();
	int y_max = 0;
	map<size_t, QuadTreePointCollection<KeyPoint *> *> kpcollection_map;
	map<size_t, QuadTree<KeyPoint *> *> kptree_map;

	for(size_t i = RAW_IMAGE; 
		i <= BLUR9_IMAGE; 
		i = i+BLUR_INCREMENT) {
		vector<KeyPoint> *keypoints = 0;
		map<size_t,vector<KeyPoint> *>::const_iterator citr =
			keypoints_map.find(i);
		assert(citr != keypoints_map.end());
		if(citr == keypoints_map.end()) {
			cout 
			<< "you have to insert vector<KeyPoint*> first" 
			<< endl;
		}

		keypoints = citr->second;
		QuadTreePointCollection<KeyPoint *> *kpcollection =
			new QuadTreePointCollection<KeyPoint *>();
//			QuadTreePointCollection<KeyPoint *>::get_instance();
		for_each(
			keypoints->begin(),
			keypoints->end(),
			[&](KeyPoint &_keypoint) {
				QuadTreePoint<KeyPoint *> *pt = 
					new QuadTreePoint<KeyPoint *>(
						_keypoint.pt.x,
						_keypoint.pt.y,
						&_keypoint);
				kpcollection->insert_point(
					pt->get_x(),
					pt->get_y(),
					pt);

				x_min=std::min(x_min,(int)_keypoint.pt.x);
				x_max=std::max(x_max,(int)_keypoint.pt.x);
				y_min=std::min(y_min,(int)_keypoint.pt.y);
				y_max=std::max(y_max,(int)_keypoint.pt.y);
			});

		QuadTree<KeyPoint *> *kptree = 
			new QuadTree<KeyPoint *>();
//			QuadTree<KeyPoint *>::get_instance();
		kptree->initialize(
			0, 
			image_height, 
			0, 
			image_width, 
			cell_size);
		kptree->add_points_and_make_partition(kpcollection);
		kpcollection_map.insert(
			pair<size_t,
			QuadTreePointCollection<KeyPoint *> *>(
				i,kpcollection));
		kptree_map.insert(
			pair<size_t,
			QuadTree<KeyPoint *> *>(
				i,kptree));
		cout<<"QuadTree is generated for image "<<i<<endl;
	}
	///////////////////////////////////////////////////////////////////

	map<const QuadTreeNode<KeyPoint *> *,double> 
		probability_map;
	// make reference tree with a raw image and keypoints
	QuadTree<KeyPoint *> *ref_kptree = 
		kptree_map.find(RAW_IMAGE)->second;
	list<const QuadTreeNode<KeyPoint *> *> temp_list;
	// insert all nodes that contain keypoints
	ref_kptree->traverse_all_nodes(
		temp_list,
		[&](const QuadTreeNode<KeyPoint *> *_node) 
			-> const QuadTreeNode<KeyPoint *> *{
////			if(_node->get_size_of_points() != 0) {
				probability_map.insert(
				pair<const QuadTreeNode<KeyPoint *> *,
					double>(_node,0.0));

//				cout
//				<<_node
//				<<", "<<_node->get_size_of_points()
//				<<", ";
//				_node->print_index();
//				cout<<endl;
////			}
			return _node;
		});

	///////////////////////////////////////////////////////////////////
	for(size_t i = BLUR3_IMAGE; 
		i <= BLUR9_IMAGE; 
		i = i+BLUR_INCREMENT) {

		QuadTree<KeyPoint *> *kptree = 
			kptree_map.find(i)->second;

		QTreeFuncDensity<KeyPoint *> density_func;
		list<const QuadTreeNode<KeyPoint *> *> node_list;
		density_func(node_list,kptree);

		cout<<"density is calculated"<<endl;

		double weight = 1.0;
		for_each(
			node_list.begin(),
			node_list.end(),
			[&](const QuadTreeNode<KeyPoint *> *_node) {
			// find out reference nodes
			// that are overlapped with _node 
			list<const QuadTreeNode<KeyPoint *> *> 
				ref_node_list; 
			ref_kptree->find_neighbor_node(
				ref_node_list,
				ref_kptree->get_root(),
				_node->get_x_from(),
				_node->get_x_to(),
				_node->get_y_from(),
				_node->get_y_to(),
				false);
//			for_each(
//				ref_node_list.begin(),
//				ref_node_list.end(),
//				[](const QuadTreeNode<KeyPoint *> *
//					_ref_node) {
//				cout
//				<<_ref_node<<", ";
//				_ref_node->print_index();
//				cout<<endl;
//			});
			for_each(
				ref_node_list.begin(),
				ref_node_list.end(),
				[&](const QuadTreeNode<KeyPoint *> *
					_ref_node) {
				map<const QuadTreeNode<KeyPoint *> *, 
					double>::iterator itr = 
					probability_map.find(_ref_node);
				if(itr != probability_map.end()) {
					itr->second += 1.0 * weight;
				} else {
					cout<<"check"<<endl;
					throw("check");
				}
			});
		});
	}
	///////////////////////////////////////////////////////////////////

	Mat keypoints_raw_image;
	drawKeypoints(
		refImage, 
		*keypoints_map.find(RAW_IMAGE)->second,
//		*keypoints, 
		keypoints_raw_image, 
		Scalar::all(-1), 
		DrawMatchesFlags::DEFAULT);

	Mat filtered_keypoints_raw_image;
	drawKeypoints(
		refImage, 
		filteredKeypoints, 
		filtered_keypoints_raw_image,
		Scalar::all(-1), 
		DrawMatchesFlags::DEFAULT);

	for_each(
		probability_map.begin(),
		probability_map.end(),
		[&](pair<const QuadTreeNode<KeyPoint *> *,double> 
			a_pair) {
			Rect rect(
				a_pair.first->get_x_from(),
				a_pair.first->get_y_from(),
				a_pair.first->get_x_to(),
				a_pair.first->get_y_to());
			
			rectangle(
				filtered_keypoints_raw_image, 
				rect,
			//	Scalar(255,255,0),
				canny_rect_color,
				2, 
				LINE_8,
				0);
		});

	namedWindow("refImage", WINDOW_NORMAL);
	imshow("keypoints image", keypoints_raw_image);
	imshow("filtered keypoints image", filtered_keypoints_raw_image);
//	imshow("descriptors", descriptors_raw);
	imshow("canny edge", canny_output);
	imshow("contours", contourMat);

	waitKey(0);

	for_each(image_map.begin(),
		image_map.end(),
		[&](pair<size_t,Mat *> _a_pair) {
			if(_a_pair.second) {
				delete _a_pair.second;
			}
		});

	for_each(keypoints_map.begin(),
		keypoints_map.end(),
		[&](pair<size_t,vector<KeyPoint> *> _a_pair) {
			if(_a_pair.second) {
				delete _a_pair.second;
			}
		});

	for_each(descriptors_map.begin(),
		descriptors_map.end(),
		[&](pair<size_t,Mat *> _a_pair) {
			if(_a_pair.second) {
				delete _a_pair.second;
			}
		});

	for_each(kptree_map.begin(),
		kptree_map.end(),
		[](pair<size_t,QuadTree<KeyPoint *> *> _a_pair) {
			if(_a_pair.second) {
				delete _a_pair.second;
			}
		});

	for_each(kpcollection_map.begin(),
		kpcollection_map.end(),
		[](pair<size_t,QuadTreePointCollection<KeyPoint *> *>
			_a_pair) {
			if(_a_pair.second) {
				delete _a_pair.second;
			}
		});

	return 0;
}
