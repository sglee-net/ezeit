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
#include <bsp/BSPNode.h>
#include <bsp/BSPTree.h>
#include <bsp/BSPPointCollection.h>
#include <bsp/BSPPoint.h>

using namespace std;
using namespace std::chrono;
using namespace cv;
using namespace cv::xfeatures2d;
using namespace cv::ml;

static int gcount=0;
struct ShowPointList {
	void operator()(const list<const BSPPoint<double> *> &_list) const {
		for_each(
			_list.begin(),
			_list.end(),
			[](const BSPPoint<double> *point) {
				const BSPNode<double> *node = 
					point->get_bsp_node();
				cout 
				<< "(i,j) "
				<< point->get_i()
				<< ", "
				<< point->get_j()
				<<", level: "
				<< node->get_level()
				<< ", density: "
				<< node->get_density()
				<< ", "
				<< " index: "
				<< node->get_index();
				const BSPNode<double> *parent_node = 
					node->get_parent();
				while(parent_node!=0) {
					cout
					<< "<"
					<< parent_node->get_index();
					parent_node =
						parent_node->get_parent();
				}
				cout << endl;
				gcount++;
		});
	}
};

int main(void)
{
	std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
	std::chrono::duration<double> sec;

	cout<<"bsp example"<<endl;

	const size_t point_count=100;

	const BSPPoint<double> *ref_point = 0;

	BSPPointCollection<double> *collection = 
		BSPPointCollection<double>::get_instance();
	for(size_t i=0; i<=point_count; i++) {
		for(size_t j=0; j<=point_count; j++) {
			BSPPoint<double> *point = 
				new BSPPoint<double>(i,j,1);
			collection->insert_point(
				point->get_i(),
				point->get_j(),
				point);
			if(i==50 && j==50) {
				ref_point = point;
			}
		}
	}

	BSPTree<double> *bsptree =
		BSPTree<double>::get_instance();
	bsptree->initialize(0,100,0,100,3);	
	bsptree->add_points_and_make_partition(collection);

	cout << "points are added" << endl;
	
	const BSPNode<double> *rootNode = bsptree->get_root();
	std::function<void(
		const list<const BSPPoint<double> *> &)> functionObj =
		ShowPointList();
	rootNode->make_recursion(functionObj);
	cout << gcount << endl;

	list<const BSPNode<double> *> node_list;
	const int di=1;
	const int dj=1;
	bsptree->find_neighbor_node(
		node_list,
		ref_point,
		ref_point->get_bsp_node()->get_root(),
		di,
		dj);

	cout<<"find neighbor "<<ref_point->get_bsp_node()
	<<", "<<node_list.size()<<endl;
	for_each(node_list.begin(),
		node_list.end(),
		[](const BSPNode<double> *node) {
			cout
			<< node
			<< ", ";
			node->print_history_index(); 
		});

	return 0;
}
