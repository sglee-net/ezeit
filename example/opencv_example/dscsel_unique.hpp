#ifndef __DSCSEL_UNIQUE_HPP__
#define __DSCSEL_UNIQUE_HPP__

#include <opencv2/core/core.hpp>
#include <opencv2/core/cvdef.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/ml/ml.hpp>

struct DSCSELUnique {
	static DSCSELUnique *get_instance() {
		static DSCSELUnique instance;
		return &instance;
	}

	/**
	 * @param ioi
	 * Index of Interest
	 * @param keypoints
	 * keypoints by local feature detector such as SIFT, SURF, etc.
	 * @param descriptor
	 * descritor vectors corresponding to keypoints
	 * @param correlation
	 * correlation matrix of descriptor
	 * @param kind_of_function
	 * '+' for addition and '-' for removal
	 * @param properties
	 * miscellaneous properties for algorithms
	 * @return
	*/
	void operator() (
		list<size_t> &_ioi,
		const vector<KeyPoint *> *_keypoints,
		const Mat *_descriptor,
		const Mat &_correlation,
		const char kind_of_function,
		const map<string,double> &_properties) const {
		list<size_t> ioi;
		find_correlated_index(
			ioi,
			_keypoints,
			_descriptor,
			_correlation,
			"HIGH",
			"LOW",
			0.2);
		switch(kind_of_function) {
			case '+':
			for_each(ioi.begin(),
				ioi.end(),
				[&](const size_t i) {
					_ioi.push_back(i);
				});
			break;
			case '-':
			for_each(ioi.begin(),
				ioi.end(),
				[&](const size_t i) {
					_ioi.remove(i);
				});
			break;
			default:
			throw("unsupported kind of function");
			break;
		}
		
		find_correlated_index(
			ioi,
			_keypoints,
			_descriptor,
			_correlation,
			"LOW",
			"LOW",
			0.2);
		switch(kind_of_function) {
			case '+':
			for_each(ioi.begin(),
				ioi.end(),
				[&](const size_t i) {
					_ioi.push_back(i);
				});
			break;
			case '-':
			for_each(ioi.begin(),
				ioi.end(),
				[&](const size_t i) {
					_ioi.remove(i);
				});
			break;
			default:
			throw("unsupported kind of function");
			break;
		}
		_ioi.sort();
		_ioi.unique();
	}
};

#endif
