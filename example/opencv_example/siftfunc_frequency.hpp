#ifndef __SIFTFUNC_FREQUENCY_HPP__
#define __SIFTFUNC_FREQUENCY_HPP__

#include <opencv2/core/core.hpp>
#include <opencv2/core/cvdef.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/ml/ml.hpp>
#include <bsp/QuadTreeNode.h>
#include <bsp/QuadTree.h>

using namespace std; 
using namespace std::chrono;
using namespace cv;
using namespace cv::xfeatures2d;
using namespace cv::ml;

template <typename T, typename S>
struct SIFTFuncFrequency {
	double weight = 1.0;
	size_t pixel_margin = 2;
	void operator() (
		list<size_t> &_ioi,
		map<const QuadTreeNode<T,S> *,double> &_probability_map,
		const QuadTree<T,S> *_ref_tree,
		const QuadTree<T,S> *_target_tree,
		const size_t i,
		const map<size_t,vector<KeyPoint *> *> &_keypoints_map,
		const map<size_t,Mat *> &_descriptors_map,
		const map<string,double> _properties
	) const {
		list<size_t> ioi;
		Mat correlation;
		Mat mean_mat;
		cv::calcCovarMatrix(
			*(_descriptors_map.find(i)->second),
			correlation,
			mean_mat,
			COVAR_NORMAL | COVAR_COLS | COVAR_SCALE);
		
		DSCSELUnique *dscsel_unique = DSCSELUnique::get_instance();
		(*dscsel_unique)(
			ioi,
			_keypoints_map.find(i)->second,
			_descriptors_map.find(i)->second,
			correlation,
			'+',
			_properties);

		cout<<"IOI size + "<<ioi.size()<<endl;

		ioi.sort();
		ioi.unique();
		
		DSCSELMajor *dscsel_major = DSCSELMajor::get_instance();
		(*dscsel_major)(
			ioi,
			_keypoints_map.find(i)->second,
			_descriptors_map.find(i)->second,
			correlation,
			'-',
			_properties);
		
		cout<<"IOI size - "<<ioi.size()<<endl;
	
		ioi.sort();
		ioi.unique();

		list<const QuadTreeNode<T,S> *> overlapped_nodes; 
		for(list<size_t>::const_iterator citr = ioi.begin();
			citr != ioi.end();
			++citr) {
			_ioi.push_back(*citr);

			KeyPoint *kpoint = 
				_keypoints_map.find(i)->second->at(*citr);
			_ref_tree->find_neighbor_node(
				overlapped_nodes,
				_ref_tree->get_root(),
				kpoint->pt.x-pixel_margin,
				kpoint->pt.y-pixel_margin,
				kpoint->pt.x+pixel_margin,
				kpoint->pt.y+pixel_margin,
				false);
		}

		overlapped_nodes.sort();
		overlapped_nodes.unique();

		// ioi가 많은 노드가 선택될 수 있도록 코드 변경할 것

		for_each(
		overlapped_nodes.begin(),
		overlapped_nodes.end(),
		[&](const QuadTreeNode<T,S> *_node) {
			typename map<
				const QuadTreeNode<T,S> *,
				double>::iterator itr = 
				_probability_map.find(_node);
			if(itr != _probability_map.end()) {
//					cout
//					<<"pp "
//					<<itr->first
//					<<", "
//					<<itr->second<<endl;
				itr->second += 1.0 * weight;
			} else {
			}
		});
	}
};

#endif
