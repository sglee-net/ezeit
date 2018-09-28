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
#include <bsp/QuadTreeNode.h>
#include <bsp/QuadTree.h>
#include <bsp/QuadTreePointCollection.h>
#include <bsp/QuadTreePoint.h>

using namespace std;
using namespace std::chrono;

//static int gcount=0;
//struct ShowPointList {
//	void operator()(const list<const QuadTreePoint<double> *> &_list) const {
//		for_each(
//			_list.begin(),
//			_list.end(),
//			[](const QuadTreePoint<double> *point) {
//				const QuadTreeNode<double> *node = 
//					point->get_bsp_node();
//				cout 
//				<< "(i,j) "
//				<< point->get_x()
//				<< ", "
//				<< point->get_y()
//				<<", level: "
//				<< node->get_level()
//				<< ", density: "
//				<< node->get_density()
//				<< ", "
//				<< " index: "
//				<< node->get_index();
//				const QuadTreeNode<double> *parent_node = 
//					node->get_parent();
//				while(parent_node!=0) {
//					cout
//					<< "<"
//					<< parent_node->get_index();
//					parent_node =
//						parent_node->get_parent();
//				}
//				cout << endl;
//				gcount++;
//		});
//	}
//};

int main(void) {
	std::chrono::system_clock::time_point start_time = 
		std::chrono::system_clock::now();
	std::chrono::duration<double> sec;

	cout<<"bsp example"<<endl;

	const size_t point_count=10;

	const QuadTreePoint<double,double> *ref_point = 0;

	QuadTreePointCollection<double,double> *collection = 
		QuadTreePointCollection<double,double>::get_instance();
	for(size_t i=0; i<=point_count; i++) {
		for(size_t j=0; j<=point_count; j++) {
			QuadTreePoint<double,double> *point = 
				new QuadTreePoint<double,double>(i,j,1);
			collection->insert_point(
				point->get_x(),
				point->get_y(),
				point);
			if(i==5 && j==5) {
				ref_point = point;
			}
		}
	}

	QuadTree<double,double> *bsptree =
		QuadTree<double,double>::get_instance();
	bsptree->initialize(0,0,10,10,2);	
	bsptree->add_points_and_make_partition(collection);

	cout << "points are added" << endl;
	
	const QuadTreeNode<double,double> *rootNode = bsptree->get_root();
//	std::function<void(
//		const list<const QuadTreePoint<double> *> &)> functionObj =
//		ShowPointList();
//	rootNode->make_recursion(functionObj);
//	cout << "points count " << gcount << endl;

	list<const QuadTreeNode<double,double> *> node_list;
	const double dx=1;
	const double dy=1;
	bsptree->find_neighbor_node(
		node_list,
		ref_point,
		ref_point->get_bsp_node()->get_root(),
		dx,
		dy);

	cout<<"find neighbor "
		<<ref_point->get_bsp_node()
		<<", "<<node_list.size()<<endl;
	
	for_each(node_list.begin(),
		node_list.end(),
		[](const QuadTreeNode<double,double> *_node) {
			cout
			<< _node
			<< ", index: ";
			_node->print_index();
			cout<<endl;
		});

	node_list.clear();
	bsptree->get_sorted_nodes(
		node_list,
		[](const QuadTreeNode<double,double> *_l,
			const QuadTreeNode<double,double> *_r) -> bool {
			if(_l->get_density() < _r->get_density()) {
				return true;
			} else {
				return false;
			}
		},
		true);

	int scount = 0;
	for_each(node_list.begin(),
		node_list.end(),
		[&](const QuadTreeNode<double,double> *_node) {
			cout
		//	<< _node 
			<< "id: ";
			_node->print_index();
			cout<<", density: "<<_node->get_density()
			<<", #num: "<<_node->get_size_of_points()
			<<", (w,h)"
			<<_node->get_width()
			<<","
			<<_node->get_height()
			<<",i "
			<<_node->get_x_from()
			<<", "
			<<_node->get_x_to()
			<<",j "
			<<_node->get_y_from()
			<<", "
			<<_node->get_y_to()
			<<endl;
			scount += _node->get_size_of_points();
		});

	cout<<"the number of searched points "<<scount<<endl;
	return 0;
}

