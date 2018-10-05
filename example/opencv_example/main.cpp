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
#include "cluster.hpp"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include <cstdio>
#include "dscsel_major.hpp"
#include "dscsel_unique.hpp"
#include "siftfunc_density.hpp"
#include "siftfunc_frequency.hpp"

using namespace rapidjson;

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

typedef int type_t;

int main(void)
{
	std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
	std::chrono::duration<double> sec;


//	FILE *infp = fopen("clusters.json", "r");
//	char readBuffer[65536];
//	FileReadStream is(infp, readBuffer, sizeof(readBuffer));
//	Document d;
//	d.ParseStream(is);
//	Clusters<double> clusters;
//	clusters.read_json(d);
//	fclose(infp);
//
//	
//	FILE *outfp = fopen("clusters_out.json", "w");
//	char writeBuffer[65536];
//	FileWriteStream os(outfp, writeBuffer, sizeof(writeBuffer));
//	Writer<FileWriteStream> writer(os);
//	clusters.serialize(writer);
//	//d.Accept(writer);
//
//	fclose(outfp);
//
//	return 0;


	cout<<"OpenCV example"<<endl;

	namedWindow("refImage", WINDOW_NORMAL);

//	string path = "./refImage.jpeg";
//	string path = "/mnt/sda1/images/adc/Detection/overlapping/spot_656.jpg";
//	string path = "/mnt/sda1/images/adc/Detection/overlapping/spot_439.jpg";
//	string path = "/mnt/sda1/images/adc/Detection/overlapping/brg_37.jpg";
//	string path = "/mnt/sda1/images/adc/Detection/overlapping/lsac_18.jpg";

//	string path = "./refImage.jpeg";
//	string path = "./images/spot_656.jpg";
	string path = "./images/spot_439.jpg";
//	string path = "./images/brg_37.jpg";
//	string path = "./images/lsac_18.jpg";

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
		double area = contourArea(contours[i], false);
		cout<<i<<" area: "<<area<<endl;
		if(fabs(area) < 10) {
			continue;
		}
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
	map<size_t,vector<KeyPoint *> *> keypoints_map;
	map<size_t,Mat *> descriptors_map;
	for(size_t i = RAW_IMAGE; 
		i <= BLUR9_IMAGE; 
		i = i+BLUR_INCREMENT) {
		vector<KeyPoint *> *keypoints = new vector<KeyPoint* >();
		keypoints_map.insert(pair<size_t,vector<KeyPoint* > *>(
				i,keypoints));
		
		Mat *descriptors = new Mat(*(image_map.find(i)->second));
		descriptors_map.insert(pair<size_t, Mat *>(
				i,descriptors));

		vector<KeyPoint> temp_keypoints;

		detector->detectAndCompute(
			*image_map.find(i)->second,
			Mat(), 
			temp_keypoints, 
			*descriptors);
		for_each(temp_keypoints.begin(),
			temp_keypoints.end(),
			[&](const KeyPoint pt) {
				KeyPoint *kpt = new KeyPoint(pt);
				keypoints->push_back(kpt);
			});
	}
	///////////////////////////////////////////////////////////////////



//	///////////////////////////////////////////////////////////////////
//	// extract IOI (Index of Interest)
//	const vector<KeyPoint *> *ioi_keypoints = 0;
//	ioi_keypoints = keypoints_map.find(BLUR9_IMAGE)->second;
//	const Mat *ioi_descriptors = 0;
//	ioi_descriptors = descriptors_map.find(BLUR9_IMAGE)->second;
//
//	map<string, double> ioi_properties;
//	list<size_t> filteredIndexes;
//	extract_IOI(
//		filteredIndexes,
//		*ioi_descriptors,
//		*ioi_keypoints,
//		ioi_properties);
//
//	Mat filteredMat;
//	vector<KeyPoint> filteredKeypoints;
//	for(list<size_t>::const_iterator citr = filteredIndexes.begin();
//		citr != filteredIndexes.end();
//		++citr) {
//		filteredMat.push_back(ioi_descriptors->row(*citr));
//		filteredKeypoints.push_back(ioi_keypoints->at(*citr));
//	}
//	///////////////////////////////////////////////////////////////////
//
////	filteredKeyPoints.clear();
////	copy(KeyPoints.begin(),KeyPoints.end(),filteredKeyPoints.begin());


	///////////////////////////////////////////////////////////////////
	// make QuadTree and PointCollection according to each image
	const int image_width = refImage.rows;
	const int image_height = refImage.cols;
	const int cell_size = 32;
	map<size_t, QuadTreePointCollection<type_t,KeyPoint *> *> 
		kpcollection_map;
	map<size_t, QuadTree<type_t,KeyPoint *> *> kptree_map;

	for(size_t i = RAW_IMAGE; 
		i <= BLUR9_IMAGE; 
		i = i+BLUR_INCREMENT) {
		vector<KeyPoint *> *keypoints = 0;
		map<size_t,vector<KeyPoint *> *>::const_iterator citr =
			keypoints_map.find(i);
		assert(citr != keypoints_map.end());
		if(citr == keypoints_map.end()) {
			cout 
			<< "you have to insert vector<KeyPoint*> first" 
			<< endl;
		}

		keypoints = citr->second;
		QuadTreePointCollection<type_t,KeyPoint *> *kpcollection =
			new QuadTreePointCollection<type_t,KeyPoint *>();
		for_each(
			keypoints->begin(),
			keypoints->end(),
			[&](KeyPoint *_keypoint) {
				QuadTreePoint<type_t,KeyPoint *> *pt = 
				new QuadTreePoint<type_t,KeyPoint *>(
						_keypoint->pt.x,
						_keypoint->pt.y,
						_keypoint);
				kpcollection->insert_point(
					pt->get_x(),
					pt->get_y(),
					pt);
			});

		QuadTree<type_t,KeyPoint *> *kptree = 
			new QuadTree<type_t,KeyPoint *>();
		kptree->initialize(
			0, 
			0, 
			image_height, 
			image_width, 
			cell_size);
		kptree->add_points_and_make_partition(kpcollection);
		kpcollection_map.insert(
			pair<size_t,
			QuadTreePointCollection<type_t,KeyPoint *> *>(
				i,kpcollection));
		kptree_map.insert(
			pair<size_t,
			QuadTree<type_t,KeyPoint *> *>(
				i,kptree));
		cout<<"QuadTree is generated for image "<<i<<endl;
	}
	///////////////////////////////////////////////////////////////////

	map<const QuadTreeNode<type_t,KeyPoint *> *,double> 
		probability_map;
	// make reference tree with a raw image and keypoints
	QuadTree<type_t,KeyPoint *> *ref_kptree = 
		kptree_map.find(RAW_IMAGE)->second;
	list<const QuadTreeNode<type_t,KeyPoint *> *> temp_list;
	// insert all nodes that contain keypoints
	ref_kptree->traverse_all_nodes(
		temp_list,
		[&](const QuadTreeNode<type_t,KeyPoint *> *_node) 
			-> const QuadTreeNode<type_t,KeyPoint *> *{
			if(_node->get_size_of_points() != 0) {
				probability_map.insert(
				pair<
				const QuadTreeNode<type_t,KeyPoint *> *,
				double> (_node,0.0));

//			double aspect_ratio = 
//				double(_node->get_y_to()-_node->get_y_from())
//				/double(_node->get_x_to()-_node->get_x_from());
//			_node->print_index();
//			cout<<" ar "<<aspect_ratio
//			<<" "
//			<<_node->get_x_from()
//			<<", "
//			<<_node->get_x_to()
//			<<", "
//			<<_node->get_y_from()
//			<<", "
//			<<_node->get_y_to()
//			<<endl;
//
//			Mat temp;
//			refImage.copyTo(temp);
//			Rect rect(_node->get_x_from(),
//				_node->get_y_from(),
//				_node->get_width(),
//				_node->get_height());
//			rectangle(
//				temp, 
//				rect,
//				Scalar(255,255,0),
//				1, 
//				LINE_8,
//				0);
//			imshow("bbox", 
//				temp);
//			waitKey(0);

//				cout
//				<<_node
//				<<", "<<_node->get_size_of_points()
//				<<", ";
//				_node->print_index();
//				cout<<endl;
			
				return _node;
			} else {
				return 0;
			}
		});

	list<size_t> ioi_index;
	///////////////////////////////////////////////////////////////////
	size_t test_count = 0;
	for(size_t i = BLUR3_IMAGE; 
		i <= BLUR3_IMAGE; 
		i = i+BLUR_INCREMENT) {

		ioi_index.clear();

		QuadTree<type_t,KeyPoint *> *kptree = 
			kptree_map.find(i)->second;

		map<string,double> properties;

//		SIFTFuncDensity<type_t,KeyPoint *> siftfunc_density;
//		siftfunc_density(
//			probability_map,
//			ref_kptree,
//			kptree,
//			i,
//			keypoints_map,
//			descriptors_map,
//			properties
//			);
			
		SIFTFuncFrequency<type_t,KeyPoint *> siftfunc_frequency;
		siftfunc_frequency.weight = 1.0;
		siftfunc_frequency(
			ioi_index,
			probability_map,
			ref_kptree,
			kptree,
			i,
			keypoints_map,
			descriptors_map,
			properties
			);

		test_count++;
	}
	///////////////////////////////////////////////////////////////////

	vector<KeyPoint> vKeyPoint;
	vector<KeyPoint *> *ref_keypoints = 
		keypoints_map.find(RAW_IMAGE)->second;
	for_each(ref_keypoints->begin(),
		ref_keypoints->end(),
		[&](KeyPoint *pt) {
			vKeyPoint.push_back(*pt);
		});
	Mat keypoints_raw_image;
	drawKeypoints(
		refImage, 
//		*keypoints_map.find(RAW_IMAGE)->second,
		vKeyPoint,
		keypoints_raw_image, 
		Scalar::all(-1), 
		DrawMatchesFlags::DEFAULT);

	vector<KeyPoint> temp;
	vKeyPoint.clear();
	ref_keypoints = keypoints_map.find(BLUR9_IMAGE)->second;
	for_each(ref_keypoints->begin(),
		ref_keypoints->end(),
		[&](KeyPoint *pt) {
//			vKeyPoint.push_back(*pt);
			temp.push_back(*pt);
		});
	for_each(
	ioi_index.begin(),
	ioi_index.end(),
	[&](size_t i) {
		vKeyPoint.push_back(temp[i]);
	});

	Mat filtered_keypoints_raw_image;
	drawKeypoints(
		refImage, 
//		*keypoints_map.find(BLUR9_IMAGE)->second,
		vKeyPoint,
		filtered_keypoints_raw_image,
		Scalar::all(-1), 
		DrawMatchesFlags::DEFAULT);

	list<double> probability_list;
	for_each(
		probability_map.begin(),
		probability_map.end(),
		[&](pair<const QuadTreeNode<type_t,KeyPoint *> *,double> 
			a_pair) {
			probability_list.push_back(a_pair.second);
			});

	SummaryStatistics probability_summary;
	SummaryStatistics::get_summary(
		probability_summary,
		probability_list);
	double fano_factor = 
		probability_summary.var / probability_summary.avr;

	cout
	<<"[probability summary] avr:"
	<<probability_summary.avr
	<<", med:"
	<<probability_summary.med
	<<", var:"
	<<probability_summary.var
	<<", fano factor:"
	<<fano_factor
	<<endl;

	const double pthres = 
		probability_summary.avr + 1.0*sqrt(probability_summary.var);

	for_each(
		probability_map.begin(),
		probability_map.end(),
		[&](pair<const QuadTreeNode<type_t,KeyPoint *> *,double> 
			a_pair) {
			Rect rect(
				a_pair.first->get_x_from()+2,
				a_pair.first->get_y_from()+2,
				a_pair.first->get_width()-4,
				a_pair.first->get_height()-4);
			cout
			<<"node,prob "
			<<a_pair.first
			<<", "
			<<a_pair.second<<endl;

			Scalar color(255,255,0);
			if(a_pair.second>= pthres) {
//				std::max(probability_summary.avr,
//				probability_summary.med)) {
				color=Scalar(0,0,255);
			}
			rectangle(
				filtered_keypoints_raw_image, 
				rect,
				color,
				1, 
				LINE_8,
				0);
//			imshow("filtered keypoints image", 
//				filtered_keypoints_raw_image);
//			waitKey(0);
		});


	imshow("keypoints image", keypoints_raw_image);
	imshow("filtered keypoints image", filtered_keypoints_raw_image);
//	imshow("descriptors", descriptors_raw);
	imshow("canny edge", canny_output);
	imshow("contours", contourMat);

	waitKey(0);

	for_each(image_map.begin(),
		image_map.end(),
		[&](const pair<size_t,Mat *> &_a_pair) {
			if(_a_pair.second) {
				delete _a_pair.second;
			}
		});

	for_each(keypoints_map.begin(),
		keypoints_map.end(),
		[&](const pair<size_t,vector<KeyPoint *> *> &_a_pair) {
			if(_a_pair.second) {
				for_each(_a_pair.second->begin(),
					_a_pair.second->end(),
					[](KeyPoint *_pt) {
						if(_pt) delete _pt;
					});
				delete _a_pair.second;
			}
		});

	for_each(descriptors_map.begin(),
		descriptors_map.end(),
		[&](const pair<size_t,Mat *> &_a_pair) {
			if(_a_pair.second) {
				delete _a_pair.second;
			}
		});

	for_each(kptree_map.begin(),
		kptree_map.end(),
		[](const pair<size_t,QuadTree<type_t,KeyPoint *> *> 
			&_a_pair) {
			if(_a_pair.second) {
				delete _a_pair.second;
			}
		});

	for_each(kpcollection_map.begin(),
		kpcollection_map.end(),
		[](const pair<
			size_t,QuadTreePointCollection<type_t,KeyPoint *> *>
			&_a_pair) {
			if(_a_pair.second) {
				delete _a_pair.second;
			}
		});

	return 0;
}
