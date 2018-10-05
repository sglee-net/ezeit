#ifndef __SIFTFUNC_DENSITY_HPP__
#define __SIFTFUNC_DENSITY_HPP__

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
#include "qtreefunc_density.hpp"

using namespace std; 
using namespace std::chrono;
using namespace cv;
using namespace cv::xfeatures2d;
using namespace cv::ml;

template <typename T, typename S>
struct SIFTFuncDensity {
	double weight = 1.0;
	void operator() (
		map<const QuadTreeNode<T,S> *,double> &_probability_map,
		const QuadTree<T,S> *_ref_tree,
		const QuadTree<T,S> *_target_tree,
		const size_t i,
		const map<size_t,vector<KeyPoint *> *> &_keypoints_map,
		const map<size_t,Mat *> &_descriptors_map,
		const map<string,double> _properties
	) const {
		QTreeFuncDensity<T,S> density_func;
		list<const QuadTreeNode<T,S> *> node_list;
		density_func(node_list,_target_tree);

//		cout<<"The size of density nodes "<<node_list.size()<<endl;

		for_each(
			node_list.begin(),
			node_list.end(),
			[&](const QuadTreeNode<T,S> *_node) {
			// find out reference nodes
			// that are overlapped with _node 
			list<const QuadTreeNode<T,S> *> 
				overlapped_nodes; 
			_ref_tree->find_neighbor_node(
				overlapped_nodes,
				_ref_tree->get_root(),
				_node->get_x_from(),
				_node->get_y_from(),
				_node->get_x_to(),
				_node->get_y_to(),
				false);

//			cout
//			<<"given node "
//			<<_node
//			<<", coordinate "
//			<<_node->get_x_from()
//			<<", "
//			<<_node->get_y_from()
//			<<", "
//			<<_node->get_x_to()
//			<<", "
//			<<_node->get_y_to()
//			<<endl;

//			cout<<"overlapped_nodes size() "<<
//			overlapped_nodes.size()<<endl;

			for_each(
				overlapped_nodes.begin(),
				overlapped_nodes.end(),
				[&](const QuadTreeNode<T,S> *_node) {
				typename 
				map<const QuadTreeNode<T,S> *,double>
				::iterator itr = 
					_probability_map.find(_node);
				if(itr != _probability_map.end()) {
//			cout
//			<<"prob node "
//			<<itr->first
//			<<", coordinate "
//			<<itr->first->get_x_from()
//			<<", "
//			<<itr->first->get_y_from()
//			<<", "
//			<<itr->first->get_x_to()
//			<<", "
//			<<itr->first->get_y_to()
//			<<endl;
//					cout
//					<<"p "
//					<<_node
//					<<", "
//					<<itr->first
//					<<", "
//					<<itr->second
//					<<endl;

					itr->second += 1.0 * weight;
				} else {
				}
			});

		});

	}
};

#endif
