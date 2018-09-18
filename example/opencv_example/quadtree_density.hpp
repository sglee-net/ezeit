#ifndef __OCTREE_DENSITY_HPP
#define __OCTREE_DENSITY_HPP

#include <list>
#include <algorithm>
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

using namespace std;
using namespace std::chrono;
using namespace cv;
using namespace cv::xfeatures2d;
using namespace cv::ml;

template <typename T>
struct QuadTreeDensity {
	void operator() (
		list<const QuadTreeNode<T> > &_node_list,
		const QuadTree<T> *_kpointtree
	) {
		_kpointtree->sort_node^(
			_node_list,
			[&](const QuadTreeNode<T> *_l,
				const QuadTreeNode<T> *_r) -> bool {
				return _l->get_density() < _r->get_density() ? true : false;
			},
			false);
		
		list<pair<
			double,
			const QuadTreeNode<T> *> > 
				density_list;
		for_each(
			_node_list.begin(),
			_node_list.end(),
			[&](pair<
				double,
				const QuadTreeNode<T> *> 
					&_a_pair) {
				density_list
					.push_back(_a_pair.second->get_density());
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
	
		cout<<"density list #"<<density_list.size();

		typename list<pair<
			double,
			const QuadTreeNode<T> *>>::iterator 
				itr_density = 
					density_list.begin();
		while(itr_density != density_list.end()) {
			if(itr_density->first < density_avr) {
				itr_density = density_list.erase(
					itr_density);
			} else {
				break;
			}
		}
		
		cout<<"density list #"<<density_list.size();

		for_each(
			density_list.begin(),
			density_list.end(),
			[](const pair<double,
				const QuadTreeNode<T> *> _a_pair) {
				_a_pair->set_count(
					_a_pair->get_count()+1);
				cout<<"density " <<_a_pair->first<<endl;
			});
	}
};

#endif
