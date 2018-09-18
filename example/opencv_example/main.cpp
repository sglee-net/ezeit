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
#include "ioiextraction.hpp"
#include "quadtree_density.hpp"

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

	const int threshold = 100;

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);

	Canny(blurImage9, canny_output, threshold, threshold*2, 3);
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
		cout<<i<<" area: "<<area<<endl;
		if(fabs(area) > 50) {
			roi = boundingRect(contours[i]);
			double aspect_ratio = 
				std::max(roi.height,roi.width)/
				std::min(roi.height,roi.width);
			cout<< aspect_ratio <<endl;
			if(aspect_ratio < 2.0) {
				Point2f center;
				float radius=0.0f;
				minEnclosingCircle(contours[i],
					center,
					radius);

				rectangle(
					refImage, 
					roi, 
					Scalar(0,0,255), 
					2, 
					LINE_8,
					0);
				circle(
					refImage,
					center,
					radius,
					Scalar(0,255,0));
				drawContours(
					refImage,
					contours,
					(int)i,
					Scalar(255,0,0),
					1,
					4,
					hierarchy,
					0,
					Point());
					}

		}
	}

	// step1: detect the keypoints
	
//	int minHessian = 1000;
//	Ptr<Feature2D> detector = SURF::create(minHessian);
	Ptr<Feature2D> detector = AKAZE::create(
		AKAZE::DESCRIPTOR_MLDB,
		0,
		3,
		0.0015f);

	vector<KeyPoint> keypoints;
	
	// step2: calculate descriptorss (fecture vectors)
	Mat descriptors;
	detector->detectAndCompute(
		blurImage9, 
//		contourMat,
		Mat(), 
		keypoints, 
		descriptors);

	cout<<descriptors.rows<<", "<<descriptors.cols<<endl;

//	Mat covarMat, meanMat;
//	cv::calcCovarMatrix(
//		descriptors,
//		covarMat,
//		meanMat,
//		COVAR_NORMAL | COVAR_COLS | COVAR_SCALE);
//	cout<<covarMat.rows<<", "<<covarMat.cols<<endl;
// 	Mat correlMat;
// 	getCorMatrix(
//		correlMat,
//		descriptors);

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

//	addCovarCountHL(
//		filteredIndexes,
//		keypoints,
//		covarMat);
//
//	addCovarCountLL(
//		filteredIndexes,
//		keypoints,
//		covarMat);
//
//	removeCovarCountHH(
//		filteredIndexes,
//		keypoints,
//		covarMat);


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

	extract_IOI(
		filteredIndexes,
		descriptors,
		keypoints);

	Mat filteredMat;
	vector<KeyPoint> filteredKeypoints;
	for(list<size_t>::const_iterator citr = filteredIndexes.begin();
		citr != filteredIndexes.end();
		++citr) {
		filteredMat.push_back(descriptors.row(*citr));
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

//	Mat descriptors2;
//	descriptors.copyTo(descriptors2);

/*
	// step3: match descriptors
	FlannBasedMatcher matcher;
	std::vector<DMatch> matches;
	matcher.match(descriptors, descriptor2, matches);
	for(int i=0; i<descriptors.rows; i++) {
		double dist = matches[i].distance;
		cout<<dist<<endl;
	}
*/

	const int image_width = refImage.rows;
	const int image_height = refImage.cols;
	const int cell_size = 64;

	int x_min = numeric_limits<int>::max();
	int x_max = 0;
	int y_min = numeric_limits<int>::max();
	int y_max = 0;

	QuadTreePointCollection<KeyPoint *> *kpcollection =
		QuadTreePointCollection<KeyPoint *>::get_instance();
	for_each(
		keypoints.begin(),
		keypoints.end(),
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

			x_min = std::min(x_min, (int)_keypoint.pt.x);
			x_max = std::max(x_max, (int)_keypoint.pt.x);
			y_min = std::min(y_min, (int)_keypoint.pt.y);
			y_max = std::max(y_max, (int)_keypoint.pt.y);
		});

	cout<<"x min, max " << x_min << ", "<< x_max <<endl;
	cout<<"y min, max " << y_min << ", " << y_max <<endl;
	cout<<"image size w, h " << refImage.cols << ", " << refImage.rows <<endl;

	cout<<"points are inserted to QuadTreePointCollection"<<endl;

	QuadTree<KeyPoint *> *kptree = 
		QuadTree<KeyPoint *>::get_instance();
	kptree->initialize(0, image_height, 0, image_width, cell_size);
	kptree->add_points_and_make_partition(kpcollection);

	cout<<"QuadTree is generated"<<endl;

	list<const QuadTreeNode<KeyPoint *> *> node_list;
	kptree->sort_nodes(
		node_list,
		[](const QuadTreeNode<KeyPoint *> *_l,
			const QuadTreeNode<KeyPoint *> *_r) -> bool {
			return _l->get_density() < _r->get_density() 
				? true : false;
		},
		false);

	list<double> density_list;
	for_each(
		node_list.begin(),
		node_list.end(),
		[&](const QuadTreeNode<KeyPoint *> *_node) {
			_node->print_index();
			cout<<", density: "
			<<_node->get_density()
			<<endl;

			density_list.push_back(_node->get_density());
		});

	double density_avr = 0.0;
	double density_var = 0.0;
	double density_med = 0.0;
	double density_min = 0.0;
	double density_max = 0.0;
	double density_rng = 0.0;
	statistics::get_summary(
		density_avr,
		density_var,
		density_med,
		density_min,
		density_max,
		density_rng,
		density_list);
	cout<<"density info"
		<<", avr:"
		<<density_avr
		<<", var:"
		<<density_var
		<<", med:"
		<<density_med
		<<", min:"
		<<density_min
		<<", max:"
		<<density_max
		<<", rng:"
		<<density_rng
		<<endl;
	cout<<"standard dev:"<<sqrtf(density_var)<<endl;



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
	imshow("descriptors", descriptors);
	imshow("canny edge", canny_output);
	imshow("contours", contourMat);

	waitKey(0);

	return 0;
}
