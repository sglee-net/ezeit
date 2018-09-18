/**
 * @ author SG Lee
 * @ since 2002
*/

#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include <list>
#include <assert.h>
#include <limits>
#include <iostream>
#include <string>
#include "QuadTreeNode.h"
#include "QuadTreePoint.h"
#include "QuadTreePointCollection.h"

using namespace std;

template <typename T>
struct GreaterDensity {
	bool operator()(
		const QuadTreeNode<T> *_l, 
		const QuadTreeNode<T> *_r) {
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
		const QuadTreeNode<T> *_l, 
		const QuadTreeNode<T> *_r) {
		if(_l->get_density() > _r->get_density()) {
			return true;
		} else {
			return false;
		}
	}
};

template <typename T>
class QuadTree {
private:
	QuadTree();
	QuadTree(const QuadTree &) {}
	QuadTree &operator=(const QuadTree<T> &) { return *this; }
public:
	~QuadTree();
public:
	static QuadTree<T> *get_instance() {
		static QuadTree<T> instance;
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
	QuadTreeNode<T> *root;
public:
	bool add_points_and_make_partition(
		const QuadTreePointCollection<T> *_collection);
	const QuadTreeNode<T> *get_root()const { return root; }
	// ref_point +- di, dj => rectagle box
	// compare overlap btw two rectagles
	void find_neighbor_node(
		list<const QuadTreeNode<T> *> &_nodelist, 
		const QuadTreePoint<T> *_ref_point,
		const QuadTreeNode<T> *_target_node,
		const dtype_t _dx,
		const dtype_t _dy,
		const bool _empty_node_is_possible = true) const;
	void find_neighbor_node(
		list<const QuadTreeNode<T> *> &_node_list,
		const QuadTreeNode<T> *_target_node,
		const dtype_t _from_x,
		const dtype_t _to_x,
		const dtype_t _from_y,
		const dtype_t _to_y,
		const bool _empty_node_is_possible = true) const;
	void traverse_all_nodes(
		list<const QuadTreeNode<T> *> &_list,
		function<const QuadTreeNode<T> *(
			const QuadTreeNode<T> *)> _func);
	void sort_nodes(
		list<const QuadTreeNode<T> *> &_nodelist,
		function<bool(
			const QuadTreeNode<T> *, 
			const QuadTreeNode<T> *)> 
			_func,
			const bool _empty_node_is_possible = true);
private:
	void traverse_sub_nodes(
		list<const QuadTreeNode<T> *> &_list,
		const QuadTreeNode<T> *_target_node,
		function<const QuadTreeNode<T> *(
			const QuadTreeNode<T> *)> _func);
};

template <typename T>
QuadTree<T>::QuadTree() {
	root = 0;
}

template <typename T>
QuadTree<T>::~QuadTree() {
	finalize();
}

template <typename T>
void 
QuadTree<T>::finalize() {
	if (root) {
		delete root;
		root = 0;
	}
}

template <typename T>
void 
QuadTree<T>::initialize(
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

	root = new QuadTreeNode<T>(
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
QuadTree<T>::is_initialized() const {
	return root ? true : false;
}

template <typename T>
bool 
QuadTree<T>::add_points_and_make_partition(
	const QuadTreePointCollection<T> *_collection) {
	assert(this->is_initialized());
	if (!this->is_initialized()) {
		return false;
	}

	typename list<QuadTreePoint<T> *>::const_iterator citr_point = 
		_collection->begin();
	while (citr_point != _collection->end()) {
		QuadTreePoint<T> *point = (*citr_point);

		QuadTreeNode<T> *node = 
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
QuadTree<T>::find_neighbor_node(
	list<const QuadTreeNode<T> *> &_node_list,
	const QuadTreePoint<T> *_ref_point,
	const QuadTreeNode<T> *_target_node,
	const dtype_t _dx,
	const dtype_t _dy,
	const bool _empty_node_is_possible) const {
	dtype_t cx = _ref_point->get_x();
	dtype_t cy = _ref_point->get_y();
	if(!_target_node->is_overlapped(
		cx-_dx, 
		cx+_dx, 
		cy-_dy, 
		cy+_dy)) {
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
	const QuadTreeNode<T> *child_node = 
		_target_node->get_first_child();
	// iteration of child nodes, node 1, 2, 3, 4
	while(child_node != 0) { 
		find_neighbor_node(
			_node_list,
			_ref_point,
			child_node,
			_dx,
			_dy);
		child_node = child_node->get_next_brother();
	}
}

template <typename T>
void 
QuadTree<T>::find_neighbor_node(
	list<const QuadTreeNode<T> *> &_node_list,
	const QuadTreeNode<T> *_target_node,
	const dtype_t _from_x,
	const dtype_t _to_x,
	const dtype_t _from_y,
	const dtype_t _to_y,
	const bool _empty_node_is_possible) const {
	if(!_target_node->is_overlapped(
			_from_x,
			_to_x,
			_from_y,
			_to_y)) {
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
	const QuadTreeNode<T> *child_node = 
		_target_node->get_first_child();
	// iteration of child nodes, node 1, 2, 3, 4
	while(child_node != 0) { 
		find_neighbor_node(
			_node_list,
			child_node,
			_from_x,
			_to_x,
			_from_y,
			_to_y);
		child_node = child_node->get_next_brother();
	}
}

template <typename T>
void 
QuadTree<T>::traverse_sub_nodes(
	list<const QuadTreeNode<T> *> &_list,
	const QuadTreeNode<T> *_target_node,
	function<const QuadTreeNode<T> *(const QuadTreeNode<T> *)> _func) {
	const QuadTreeNode<T> *node = _func(_target_node);
	if(node != 0 ) {
		_list.push_back(node);
	}

	if (!_target_node->has_subnodes()) {
		return;
	}

	// check children 
	const QuadTreeNode<T> *child_node =
		_target_node->get_first_child();
	// iteration of child nodes, node 1, 2, 3, 4
	while(child_node != 0) { 
		traverse_sub_nodes(_list,child_node,_func);
		child_node = child_node->get_next_brother();
	}
}

template <typename T>
void 
QuadTree<T>::traverse_all_nodes(
	list<const QuadTreeNode<T> *> &_list,
	function<const QuadTreeNode<T> *(const QuadTreeNode<T> *)> _func) {
	const QuadTreeNode<T> *root = this->get_root();
	traverse_sub_nodes(_list,root,_func);
}

template <typename T>
void 
QuadTree<T>::sort_nodes(
	list<const QuadTreeNode<T> *> &_nodelist,
	function<bool(const QuadTreeNode<T> *, const QuadTreeNode<T> *)> 
		_func,
		const bool _is_empty_node_possible) {
	traverse_all_nodes(
		_nodelist,
		[&](const QuadTreeNode<T> *_node) -> const QuadTreeNode<T> *{
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
