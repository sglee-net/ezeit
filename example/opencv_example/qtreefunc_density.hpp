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
#include <statistics/statistics.hpp>
#include "ioiextraction.hpp"

using namespace std;
using namespace std::chrono;
using namespace cv;
using namespace cv::xfeatures2d;
using namespace cv::ml;

template <typename T, typename S>
class QTreeFuncDensity {
public:
	double weight = 1.0;
	double fano_factor_threshold = 0.01;

	void operator() (
		list<const QuadTreeNode<T,S> *> &_node_list,
		const QuadTree<T,S> *_kptree
	) const {
		list<const QuadTreeNode<T,S> *> base_nodes;
		_kptree->get_sorted_nodes(
			base_nodes,
			[](const QuadTreeNode<T,S> *_l,
				const QuadTreeNode<T,S> *_r) -> bool {
				// ascending
				return _l->get_density() 
					< _r->get_density() 
					? true : false;
			},
			false);
		// base_nodes are sorted by density
		
		list<pair<
			double,
			const QuadTreeNode<T,S> *> > 
				density_and_node_list;
		list<double> density_list;
		for_each(
			base_nodes.begin(),
			base_nodes.end(),
			[&](const QuadTreeNode<T,S> *_node) {
				pair<double,const QuadTreeNode<T,S> *>
					a_pair(_node->get_density(),
						_node);
				density_and_node_list.push_back(a_pair);
				density_list.push_back(a_pair.first);
			});

		double avr = 0.0;
		double var = 0.0;
		double med = 0.0;
		double min = 0.0;
		double max = 0.0;
		double rng = 0.0;
		statistics::SummaryStatistics::get_summary(
			avr,
			var,
			med,
			min,
			max,
			rng,
			density_list);
		
		cout<<"avr "<<avr<<endl;
		cout<<"var "<<var<<endl;

		double fano_factor = var / avr;

		cout<<endl;
		cout<<"fano factor "<<fano_factor<<endl;
		cout<<"density&node list #"<<density_and_node_list.size()<<endl;

		typename list<pair<
			double,
			const QuadTreeNode<T,S> *>>::iterator 
				itr_density = 
					density_and_node_list.begin();
		while(itr_density != density_and_node_list.end()) {
			cout<<"node";
			itr_density->second->print_index();
			cout<<", density"
			<<itr_density->second->get_density()
			<<", ref density"
			<<itr_density->first<<endl;

			bool condition_satisfied = false;
			if(fano_factor < fano_factor_threshold) {
				if(itr_density->first 
					< avr) {
					condition_satisfied = true;
				}
			} else {
				if(itr_density->first 
					< (avr - sqrt(var))) {
					condition_satisfied = true;
				}
			}

			if(condition_satisfied) {
				itr_density = density_and_node_list.erase(
					itr_density);
			} else {
				break;
			}
		}
		
		cout<<"density&node list #"<<density_and_node_list.size()<<endl;

		density_and_node_list.sort();
		density_and_node_list.unique();
		for_each(
			density_and_node_list.begin(),
			density_and_node_list.end(),
			[&](const pair<double,
				const QuadTreeNode<T,S> *> &_a_pair) {
				_node_list.push_back(
					_a_pair.second);
			});
	}
};

#endif
