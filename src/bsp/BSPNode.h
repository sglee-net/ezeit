/**
 * @ author SG Lee
 * @ since 2016
 * Binary Spacitial Partiniting
*/

#ifndef __BSPNODE_H__
#define __BSPNODE_H__

#include <list>
#include <math.h>
#include <assert.h>
#include <limits>
#include <iostream>
#include <algorithm>
#include "BSPPoint.h"

using namespace std;

template <typename T>
class BSPNode {
private:
	BSPNode();
public:
	BSPNode(
		BSPNode<T> *_root,
		int _node_index,
		int _i_from, int _i_to, 
		int _j_from, int _j_to, 
		const double _cell_size);
	~BSPNode();
private:
	void destroy_subnodes();
private:
	BSPNode<T> *parent;
	BSPNode<T> *node1;
	BSPNode<T> *node2;
	BSPNode<T> *node3;
	BSPNode<T> *node4;
	int node_index;
	int level;
	int i_from;
	int j_from;
	int i_to;
	int j_to;
	double cell_size;
	list<const BSPPoint<T> *> point_list;
public:
	double center_i() const;
	double center_j() const;
	int get_width() const { return (i_to-i_from); }
	int get_height() const { return (j_to-j_from); }
	const int get_level() const { return level; }
	const int get_index() const { return node_index; }
	void print_history_index() const {
		cout << this->get_index();
		const BSPNode<T> *parent_node =
			this->get_parent();
		while(parent_node != 0) {
			cout
			<< "<"
			<< parent_node->get_index();
			parent_node = 
				parent_node->get_parent();
		}
		cout << endl;
	}
	BSPNode * add_point_and_make_partition(const BSPPoint<T> * _p);
	bool remove_point(const BSPPoint<T> *_p);
private:
	bool can_be_divided() const;
public:
	typename list<const BSPPoint<T> *>::const_iterator 
	begin_point_list()const { return point_list.begin(); }
	typename list<const BSPPoint<T> *>::const_iterator 
	end_point_list()const { return point_list.end(); }
	const list<const BSPPoint<T> *> *
	get_point_list() { return &point_list; }
	size_t get_size_of_points() const;
	bool has_subnodes() const;
	const BSPNode<T> *get_root() const;
	const BSPNode<T> *get_parent() const { return parent; }
	const BSPNode<T> *get_first_child() const;
	const BSPNode<T> *get_next_brother() const;
	bool is_child_end() const;
	double get_density() const;
	void make_recursion(
		std::function<void(
			const list<const BSPPoint<T> *> &)> &) const;
private:
	const BSPNode<T> *get_node1() const { return node1; }
	const BSPNode<T> *get_node2() const { return node2; }
	const BSPNode<T> *get_node3() const { return node3; }
	const BSPNode<T> *get_node4() const { return node4; }
public:
	bool is_overlapped(
		const int _pi, 
		const int _pj, 
		const int _di, 
		const int _dj) const;
};

template <typename T>
BSPNode<T>::BSPNode() {
	parent = 0;
	node1 = 0;
	node2 = 0;
	node3 = 0;
	node4 = 0;
	node_index = 0;
	level = 0;
	i_from = 0;
	i_to = 0;
	j_from = 0;
	j_to = 0;
	cell_size = 1.0;
}

template <typename T>
BSPNode<T>::BSPNode(
	BSPNode<T> *_root, 
	int _node_index, 
	int _i_from, int _i_to, 
	int _j_from, int _j_to, 
	const double _cell_size) {
	assert(int(_i_to - _i_from) >= 1);
	assert(int(_j_to - _j_from) >= 1);

	parent = _root;
	node1 = 0;
	node2 = 0;
	node3 = 0;
	node4 = 0;
	node_index = _node_index;
	if(_root == 0) {
		level = 0;
	} else {
		level = _root->get_level() + 1;
	}
	i_from = _i_from;
	i_to = _i_to;
	j_from = _j_from;
	j_to = _j_to;
	cell_size = _cell_size;
}

template <typename T>
BSPNode<T>::~BSPNode() {
	this->destroy_subnodes();
}

template <typename T>
void 
BSPNode<T>::destroy_subnodes() {
	if (this->has_subnodes()) {
		node1->destroy_subnodes();
		node2->destroy_subnodes();
		node3->destroy_subnodes();
		node4->destroy_subnodes();

		delete node1; node1 = 0;
		delete node2; node2 = 0;
		delete node3; node3 = 0;
		delete node4; node4 = 0;
	}
}

template <typename T>
double 
BSPNode<T>::center_i() const {
	return fabs(double(i_to + i_from)) / 2.0;
}

template <typename T>
double 
BSPNode<T>::center_j() const {
	return  fabs(double(j_to + j_from)) / 2.0;
}

template <typename T>
BSPNode<T> *
BSPNode<T>::add_point_and_make_partition(
	const BSPPoint<T> *_p) {
	if (!can_be_divided()) {
		// not divide, just add point to this node
		point_list.push_back(_p);
		return this;
	}
	else {
		// divide this node to four sub-nodes, 
		// and add a point to one of them
		const int i = _p->get_i();
		const int j = _p->get_j();

		// if this node has sub-nodes, 
		// points are not inserted into this.

		// center i, j
		// int() : ex) float(ci)=3.5 => int(3.5)=3
		int ci = int(float(i_to - i_from) / 2.0) + i_from;
		int cj = int(float(j_to - j_from) / 2.0) + j_from;

		// node
		// 2 1
		// 3 4
		if (!this->has_subnodes()) {
			node1 = new BSPNode<T>(
					this, 
					1, 
					ci, 
					i_to, 
					j_from, 
					cj, 
					cell_size);
			node2 = new BSPNode<T>(
					this, 
					2, 
					i_from, 
					ci, 
					j_from, 
					cj, cell_size);
			node3 = new BSPNode<T>(
					this, 
					3, 
					i_from, 
					ci, 
					cj, 
					j_to, 
					cell_size);
			node4 = new BSPNode<T>(
					this, 
					4, 
					ci, 
					i_to, 
					cj, 
					j_to, 
					cell_size);
		}

		// node
		// 2 1
		// 3 4
		
		// node 1
		if (ci <= i && i <= i_to &&
			j_from <= j && j < cj) {
			return node1->add_point_and_make_partition(_p);

		}
		// node 2
		else if (i_from <= i && i < ci &&
			j_from <= j && j < cj) {
			return node2->add_point_and_make_partition(_p);
		}
		// node 3
		else if (i_from <= i && i < ci &&
			cj <= j && j <= j_to) {
			return node3->add_point_and_make_partition(_p);
		}
		// node 4
		else if (ci <= i && i <= i_to &&
			cj <= j && j <= j_to) {
			return node4->add_point_and_make_partition(_p);
		}
		else {
			throw ("you have to modify the above algorithm");
		}
	}
}

template <typename T>
bool 
BSPNode<T>::remove_point(
	const BSPPoint<T> * _p) {
	typename list<const BSPPoint<T> *>::iterator result = 
		find(point_list.begin(), point_list.end(), _p);
	if (result == point_list.end()) {
		return false;
	} else {
		point_list.remove(_p);
		return true;
	}
}

template <typename T>
size_t 
BSPNode<T>::get_size_of_points() const {
	return point_list.size();
}

template <typename T>
bool 
BSPNode<T>::has_subnodes() const {
	return (node1 == 0 && 
		node2 == 0 && 
		node3 == 0 && 
		node4 == 0) ? false : true;
}

template <typename T>
const BSPNode<T> * 
BSPNode<T>::get_root() const {
	const BSPNode<T> *parent = this->get_parent();
	while (parent->get_parent() != 0) {
		parent = parent->get_parent();
	}
	return parent;
}

template <typename T>
bool 
BSPNode<T>::can_be_divided() const {
	int di = int(i_to - i_from);
	int dj = int(j_to - j_from);
	if (di <= 1 || dj <= 1 ||
		di <= cell_size || dj <= cell_size) {
		return false;
	}
	else {
		return true;
	}
}

template <typename T>
const BSPNode<T> * 
BSPNode<T>::get_first_child() const {
	return get_node1();
}

template <typename T>
const BSPNode<T> * 
BSPNode<T>::get_next_brother() const {
	if (this->node_index == 0) {
		return 0;
	} else if (this->node_index == 1) {
		return this->get_parent()->get_node2();
	} else if(this->node_index == 2) {
		return this->get_parent()->get_node3();
	} else if (this->node_index == 3) {
		return this->get_parent()->get_node4();
	} else if (this->node_index == 4) {
		return 0;
	} else {
		throw("check code");
	}
}

template <typename T>
bool 
BSPNode<T>::is_child_end() const {
	return (this->node_index == 4) ? true : false;
}

template <typename T>
double 
BSPNode<T>::get_density() const {
	return double(point_list.size()/
		((i_to-i_from)*(j_to-j_from)));
}

template <typename T>
void 
BSPNode<T>::make_recursion(
	std::function<void(
		const list<const BSPPoint<T> *> &)> &_func) const {
	if(!this->point_list.empty()) {
		_func(point_list);
	}
	const BSPNode<T> *child_node = this->get_first_child();
	while(child_node !=0 ) {
		child_node->make_recursion(_func);
		child_node = child_node->get_next_brother();
	}; 
}

// check overlap between two rectangles
// one rectagle is this
// the other is compolsed of these corners, ci-di ~ ci+di, cj-dj ~ cj+dj
template <typename T>
bool 
BSPNode<T>::is_overlapped(
	const int _pi, 
	const int _pj, 
	const int _di, 
	const int _dj) const {
	int boundary_i_min = _pi - _di;
	int boundary_i_max = _pi + _di;
	int boundary_j_min = _pj - _dj;
	int boundary_j_max = _pj + _dj;
//	const BSPNode<T> *root = this->get_root();
//	if (boundary_i_min < root->i_from) { 
//		boundary_i_min = root->i_from; 
//	}
//	if (boundary_i_max > root->i_to) { 
//		boundary_i_max = root->i_to; 
//	}
//	if (boundary_j_min < root->j_from) { 
//		boundary_j_min = root->j_from; 
//	}
//	if (boundary_j_max > root->j_to) { 
//		boundary_j_max = root->j_to; 
//	}

	// 4 points from given (pi,pj)
	// (i_max, j_min), 1
	// (i_min, j_min), 2
	// (i_min, j_max), 3
	// (i_max, j_max), 4

	// point 1 is overlapped with this node
	if (this->i_from <= boundary_i_max && 
		boundary_i_max <= this->i_to &&
		this->j_from <= boundary_j_min && 
		boundary_j_min <= this->j_to) {
		return true;
	}

	// point 2 is overlapped with this node
	if (this->i_from <= boundary_i_min && 
		boundary_i_min <= this->i_to &&
		this->j_from <= boundary_j_min && 
		boundary_j_min <= this->j_to) {
		return true;
	}

	// point 3 is overlapped with this node
	if (this->i_from <= boundary_i_min && 
		boundary_i_min <= this->i_to &&
		this->j_from <= boundary_j_max && 
		boundary_j_max <= this->j_to) {
		return true;
	}

	// point 4 is overlapped with this node
	if (this->i_from <= boundary_i_max && 
		boundary_i_max <= this->i_to &&
		this->j_from <= boundary_j_max && 
		boundary_j_max <= this->j_to) {
		return true;
	}

	return false;
}

#endif
