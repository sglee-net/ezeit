/**
 * @ author SG Lee
 * @ since 2016
*/

#ifndef __BSPTREE_H__
#define __BSPTREE_H__

#include <list>
#include <assert.h>
#include <limits>
#include <iostream>
#include <string>
#include "BSPNode.h"
#include "BSPPoint.h"
#include "BSPPointCollection.h"

using namespace std;

template <typename T>
class BSPTree {
private:
	BSPTree();
	BSPTree(const BSPTree &) {}
	BSPTree &operator=(const BSPTree<T> &) { return *this; }
public:
	~BSPTree();
public:
	static BSPTree<T> *get_instance() {
		static BSPTree<T> instance;
		return &instance;
	}

	void finalize();

	//////////////////////////////////////////////////////
	// initialize is invoked before using other functions
	void initialize(
		const int _i_from, const int _i_to, 
		const int _j_from, const int _j_to, 
		const double _cell_size);
	//////////////////////////////////////////////////////

	bool is_initialized() const;

private:
	BSPNode<T> *root;
public:
	bool add_points_and_make_partition(
		const BSPPointCollection<T> *_collection);
	const BSPNode<T> *get_root()const { return root; }
	// ref_point +- di, dj => rectagle box
	// compare overlap btw two rectagles
	void find_neighbor_node(
		list<const BSPNode<T> *> &_nodelist, 
		const BSPPoint<T> *_ref_point,
		const BSPNode<T> *_target_node,
		const int _di,
		const int _dj,
		const bool _empty_node_is_possible) const;
};

template <typename T>
BSPTree<T>::BSPTree() {
	root = 0;
}

template <typename T>
BSPTree<T>::~BSPTree() {
	finalize();
}

template <typename T>
void 
BSPTree<T>::finalize() {
	if (root) {
		delete root;
		root = 0;
	}
}

template <typename T>
void 
BSPTree<T>::initialize(
	const int _i_from, const int _i_to, 
	const int _j_from, const int _j_to, 
	const double _cell_size) {
	if (root) {
		delete root;
		root = 0;
	}

	root = new BSPNode<T>(
		0, 
		0, 
		_i_from, 
		_i_to, 
		_j_from, 
		_j_to, 
		_cell_size);
}

template <typename T>
bool 
BSPTree<T>::is_initialized() const {
	return root ? true : false;
}

template <typename T>
bool 
BSPTree<T>::add_points_and_make_partition(
	const BSPPointCollection<T> *_collection) {
	assert(this->is_initialized());
	if (!this->is_initialized()) {
		return false;
	}

	typename list<BSPPoint<T> *>::const_iterator citr_point = 
		_collection->begin();
	while (citr_point != _collection->end()) {
		BSPPoint<T> *point = (*citr_point);

		BSPNode<T> *node = 
			root->add_point_and_make_partition(point);
		if (node) {
			point->set_bsp_node(node);
		}
		else {
			return false;
		}

		++citr_point;
	}
	return true;
}

// ref_point +- di, dj => rectagle box
// compare overlap btw two rectagles
template <typename T>
void 
BSPTree<T>::find_neighbor_node(
	list<const BSPNode<T> *> &_node_list,
	const BSPPoint<T> *_ref_point,
	const BSPNode<T> *_target_node,
	const int _di,
	const int _dj,
	const bool _empty_node_is_possible = true) const {
	int pi = _ref_point->get_i();
	int pj = _ref_point->get_j();
	if(!_target_node->is_overlapped(pi, pj, _di, _dj)) {
		return;
	}

//	cout << "target ";
//	_target_node->print_history_index();
//	cout<<endl;
	
	// final node
	if (!_target_node->has_subnodes()) {
//		cout << "final ";
//		_target_node->print_history_index();
//		cout<<endl;
		
		if(_empty_node_is_possible) {
			_node_list.push_back(_target_node);
		} else {
			if(_target_node->get_size_of_points()!=0) {
				_node_list.push_back(_target_node);
			}
		}

		return;
	}


	// check children 
	const BSPNode<T> *child_node = 
		_target_node->get_first_child();
	// iteration of child nodes, node 1, 2, 3, 4
	while(child_node != 0) { 
		find_neighbor_node(
			_node_list,
			_ref_point,
			child_node,
			_di,
			_dj);
		child_node = child_node->get_next_brother();
	}
}

#endif
