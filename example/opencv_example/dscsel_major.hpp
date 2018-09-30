#ifndef __DSCSEL_MAJOR_HPP__
#define __DSCSEL_MAJOR_HPP__

#include <opencv2/core/core.hpp>
#include <opencv2/core/cvdef.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/ml/ml.hpp>

struct DSCSELMajor {
	/**
	 * @param ioi
	 * Index of Interest
	 * @param descriptor
	 * descritor vectors corresponding to keypoints
	 * @param keypoints
	 * keypoints by local feature detector such as SIFT, SURF, etc.
	 * @param kind_of_function
	 * '+' for addition and '-' for removal
	 * @param properties
	 * miscellaneous properties for algorithms
	 * @return
	*/
	void operator() (
		list<size_t> &_ioi,
		const Mat &_descriptor,
		const vector<KeyPoint> &_keypoints,
		const char kind_of_function,
		const map<string,double> &_properties) const {
	}
};

#endif
