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

template <typename T, typename S>
class QuadTree {
private:
//	QuadTree();
	QuadTree(const QuadTree &) {}
	QuadTree &operator=(const QuadTree<T,S> &) { return *this; }
public:
	QuadTree();
	~QuadTree();
public:
	static QuadTree<T,S> *get_instance() {
		static QuadTree<T,S> instance;
		return &instance;
	}

	void finalize();

	//////////////////////////////////////////////////////
	// initialize is invoked before using other functions
	void initialize(
		const T _i_from, 
		const T _j_from, 
		const T _i_to, 
		const T _j_to, 
		const T _cell_size);
	//////////////////////////////////////////////////////

	bool is_initialized() const;

private:
	QuadTreeNode<T,S> *root;
public:
	bool add_points_and_make_partition(
		const QuadTreePointCollection<T,S> *_collection);
	const QuadTreeNode<T,S> *get_root()const { return root; }
	// ref_point +- di, dj => rectagle box
	// compare overlap btw two rectagles
	void find_neighbor_node(
		list<const QuadTreeNode<T,S> *> &_nodelist, 
		const QuadTreePoint<T,S> *_ref_point,
		const QuadTreeNode<T,S> *_target_node,
		const T _dx,
		const T _dy,
		const bool _empty_node_is_possible = true) const;
	void find_neighbor_node(
		list<const QuadTreeNode<T,S> *> &_node_list,
		const QuadTreeNode<T,S> *_target_node,
		const T _from_x,
		const T _from_y,
		const T _to_x,
		const T _to_y,
		const bool _empty_node_is_possible = true) const;
	void traverse_all_nodes(
		list<const QuadTreeNode<T,S> *> &_list,
		const function<const QuadTreeNode<T,S> *(
			const QuadTreeNode<T,S> *)> &_func) const;
	void get_sorted_nodes(
		list<const QuadTreeNode<T,S> *> &_nodelist,
		const function<bool(
			const QuadTreeNode<T,S> *, 
			const QuadTreeNode<T,S> *)> &_func,
			const bool _empty_node_is_possible = true) const;
private:
	void traverse_sub_nodes(
		list<const QuadTreeNode<T,S> *> &_list,
		const QuadTreeNode<T,S> *_target_node,
		const function<const QuadTreeNode<T,S> *(
			const QuadTreeNode<T,S> *)> &_func) const;
};

template <typename T, typename S>
QuadTree<T,S>::QuadTree() {
	root = 0;
}

template <typename T, typename S>
QuadTree<T,S>::~QuadTree() {
	finalize();
}

template <typename T, typename S>
void 
QuadTree<T,S>::finalize() {
	if (root) {
		delete root;
		root = 0;
	}
}

template <typename T, typename S>
void 
QuadTree<T,S>::initialize(
	const T _x_from, 
	const T _y_from, 
	const T _x_to, 
	const T _y_to, 
	const T _cell_size) {
	assert(_x_from < _x_to);
	assert(_y_from < _y_to);
	assert(_cell_size <= (_x_to-_x_from));
	assert(_cell_size <= (_y_to-_y_from));

	if (root) {
		delete root;
		root = 0;
	}

	root = new QuadTreeNode<T,S>(
		0, 
		0, 
		_x_from, 
		_y_from, 
		_x_to, 
		_y_to, 
		_cell_size);
}

template <typename T, typename S>
bool 
QuadTree<T,S>::is_initialized() const {
	return root ? true : false;
}

template <typename T, typename S>
bool 
QuadTree<T,S>::add_points_and_make_partition(
	const QuadTreePointCollection<T,S> *_collection) {
	assert(this->is_initialized());
	if (!this->is_initialized()) {
		return false;
	}

	typename list<QuadTreePoint<T,S> *>::const_iterator citr_point = 
		_collection->begin();
	while (citr_point != _collection->end()) {
		QuadTreePoint<T,S> *point = (*citr_point);

		QuadTreeNode<T,S> *node = 
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
template <typename T, typename S>
void 
QuadTree<T,S>::find_neighbor_node(
	list<const QuadTreeNode<T,S> *> &_node_list,
	const QuadTreePoint<T,S> *_ref_point,
	const QuadTreeNode<T,S> *_target_node,
	const T _dx,
	const T _dy,
	const bool _empty_node_is_possible) const {
	T cx = _ref_point->get_x();
	T cy = _ref_point->get_y();
	if(!_target_node->is_overlapped(
		cx-_dx, 
		cy-_dy, 
		cx+_dx, 
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
	const QuadTreeNode<T,S> *child_node = 
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

template <typename T, typename S>
void 
QuadTree<T,S>::find_neighbor_node(
	list<const QuadTreeNode<T,S> *> &_node_list,
	const QuadTreeNode<T,S> *_target_node,
	const T _from_x,
	const T _from_y,
	const T _to_x,
	const T _to_y,
	const bool _empty_node_is_possible) const {
	if(!_target_node->is_overlapped(
			_from_x,
			_from_y,
			_to_x,
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
	const QuadTreeNode<T,S> *child_node = 
		_target_node->get_first_child();
	// iteration of child nodes, node 1, 2, 3, 4
	while(child_node != 0) { 
		find_neighbor_node(
			_node_list,
			child_node,
			_from_x,
			_from_y,
			_to_x,
			_to_y);
		child_node = child_node->get_next_brother();
	}
}

template <typename T, typename S>
void 
QuadTree<T,S>::traverse_sub_nodes(
	list<const QuadTreeNode<T,S> *> &_list,
	const QuadTreeNode<T,S> *_target_node,
	const function<const QuadTreeNode<T,S> *(
		const QuadTreeNode<T,S> *)> &_func) const {
	const QuadTreeNode<T,S> *node = _func(_target_node);
	if(node != 0 ) {
		_list.push_back(node);
	}

	if (!_target_node->has_subnodes()) {
		return;
	}

	// check children 
	const QuadTreeNode<T,S> *child_node =
		_target_node->get_first_child();
	// iteration of child nodes, node 1, 2, 3, 4
	while(child_node != 0) { 
		traverse_sub_nodes(_list,child_node,_func);
		child_node = child_node->get_next_brother();
	}
}

template <typename T, typename S>
void 
QuadTree<T,S>::traverse_all_nodes (
	list<const QuadTreeNode<T,S> *> &_list,
	const function<const QuadTreeNode<T,S> *(
	const QuadTreeNode<T,S> *)> &_func) const {
	const QuadTreeNode<T,S> *root = this->get_root();
	traverse_sub_nodes(_list,root,_func);
}

template <typename T, typename S>
void 
QuadTree<T,S>::get_sorted_nodes (
	list<const QuadTreeNode<T,S> *> &_nodelist,
	const function<bool(
		const QuadTreeNode<T,S> *, 
		const QuadTreeNode<T,S> *)> &_func,
		const bool _is_empty_node_possible) const {
	traverse_all_nodes(
		_nodelist,
		[&](const QuadTreeNode<T,S> *_node) 
			-> const QuadTreeNode<T,S> *{
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
