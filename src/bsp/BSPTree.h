/**
 * @ author SG Lee
 * @ since 2002
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
struct GreaterDensity {
	bool operator()(
		const BSPNode<T> *_l, const BSPNode<T> *_r) {
		if(_l->get_density() <= _r->get_density()) {
			return true;
		} else {
			return false;
		}
	}
};

template <typename T>
struct LesserDensity {
	bool operator()(
		const BSPNode<T> *_l, const BSPNode<T> *_r) {
		if(_l->get_density() > _r->get_density()) {
			return true;
		} else {
			return false;
		}
	}
};

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
		const bool _empty_node_is_possible = true) const;
	void traverse_all_nodes(
		list<const BSPNode<T> *> &_list,
		function<const BSPNode<T> *(const BSPNode<T> *)> _func);
	void sort_nodes(
		list<const BSPNode<T> *> &_nodelist,
		function<bool(const BSPNode<T> *, const BSPNode<T> *)> 
			_func,
			const bool _empty_node_is_possible = true);
private:
	void traverse_sub_nodes(
		list<const BSPNode<T> *> &_list,
		const BSPNode<T> *_target_node,
		function<const BSPNode<T> *(const BSPNode<T> *)> _func);
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
	assert(_i_from < _i_to);
	assert(_j_from < _j_to);
	assert(_cell_size <= (_i_to-_i_from));
	assert(_cell_size <= (_j_to-_j_from));

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
	const bool _empty_node_is_possible) const {
	int pi = _ref_point->get_i();
	int pj = _ref_point->get_j();
	if(!_target_node->is_overlapped(pi, pj, _di, _dj)) {
		return;
	}
	// final node
	if (!_target_node->has_subnodes()) {
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

template <typename T>
void 
BSPTree<T>::traverse_sub_nodes(
	list<const BSPNode<T> *> &_list,
	const BSPNode<T> *_target_node,
	function<const BSPNode<T> *(const BSPNode<T> *)> _func) {
	const BSPNode<T> *node = _func(_target_node);
	if(node != 0 ) {
		_list.push_back(node);
	}

	if (!_target_node->has_subnodes()) {
		return;
	}

	// check children 
	const BSPNode<T> *child_node =
		_target_node->get_first_child();
	// iteration of child nodes, node 1, 2, 3, 4
	while(child_node != 0) { 
		traverse_sub_nodes(_list,child_node,_func);
		child_node = child_node->get_next_brother();
	}
}

template <typename T>
void 
BSPTree<T>::traverse_all_nodes(
	list<const BSPNode<T> *> &_list,
	function<const BSPNode<T> *(const BSPNode<T> *)> _func) {
	const BSPNode<T> *root = this->get_root();
	traverse_sub_nodes(_list,root,_func);
}

template <typename T>
void 
BSPTree<T>::sort_nodes(
	list<const BSPNode<T> *> &_nodelist,
	function<bool(const BSPNode<T> *, const BSPNode<T> *)> 
		_func,
		const bool _is_empty_node_possible) {
	traverse_all_nodes(
		_nodelist,
		[&](const BSPNode<T> *_node) -> const BSPNode<T> *{
			if(_is_empty_node_possible) {
				return _node;
			} else {
				if(_node->get_size_of_points() != 0) {
					return _node;
				} else {
					return 0;
				}
			}
		});

	_nodelist.sort(_func);
}

#endif
