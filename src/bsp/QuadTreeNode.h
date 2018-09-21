/**
 * @ author SG Lee
 * @ since 2002
 * Binary Spacitial Partiniting
*/

#ifndef __QUADTREENODE_H__
#define __QUADTREENODE_H__

#include <list>
#include <math.h>
#include <assert.h>
#include <limits>
#include <iostream>
#include <algorithm>
#include "QuadTreePoint.h"

using namespace std;

typedef int dtype_t;
//typedef double dtype_t;

bool GT(const dtype_t &_l, const dtype_t &_r) {
	return (_l > _r) ? true : false;
}

bool GE(const dtype_t &_l, const dtype_t &_r) {
	return ( _l >= _r || 
		fabs(_l-_r) < numeric_limits<dtype_t>::epsilon() ) 
		? true : false;
}

bool LE(const dtype_t &_l, const dtype_t &_r) {
	return ( _l <= _r || 
		fabs(_l-_r) < numeric_limits<dtype_t>::epsilon() ) 
		? true : false;
}

bool LT(const dtype_t &_l, const dtype_t &_r) {
	return ( _l < _r) ? true : false;
}

bool EQ(const dtype_t &_l, const dtype_t &_r) {
	return (fabs(_l-_r) < numeric_limits<dtype_t>::epsilon()) 
		? true : false;
}

template <typename T>
class QuadTreeNode {
public:
private:
	QuadTreeNode();
public:
	QuadTreeNode(
		QuadTreeNode<T> *_root,
		const int _node_index,
		const dtype_t _x_from, 
		const dtype_t _x_to, 
		const dtype_t _y_from, 
		const dtype_t _y_to, 
		const dtype_t _cell_size);
	~QuadTreeNode();
private:
	void destroy_subnodes();
private:
	QuadTreeNode<T> *parent;
	QuadTreeNode<T> *node1;
	QuadTreeNode<T> *node2;
	QuadTreeNode<T> *node3;
	QuadTreeNode<T> *node4;
	int node_index;
	int level;
	size_t count;
	dtype_t x_from;
	dtype_t y_from;
	dtype_t x_to;
	dtype_t y_to;
	dtype_t cell_size;
	list<const QuadTreePoint<T> *> point_list;
public:
	double center_x() const;
	double center_y() const;
	dtype_t get_width() const { return (x_to-x_from); }
	dtype_t get_height() const { return (y_to-y_from); }
	dtype_t get_x_from() const { return x_from; }
	dtype_t get_x_to() const { return x_to; }
	dtype_t get_y_from() const { return y_from; }
	dtype_t get_y_to() const { return y_to; }
	const int get_level() const { return level; }
	const int get_index() const { return node_index; }
	void print_index() const; 
	void get_index_list(list<int> &_list);
//	size_t get_count() const { return count; }
//	void set_count(const size_t _v) { count = _v; }
	QuadTreeNode * add_point_and_make_partition(const QuadTreePoint<T> * _p);
	bool remove_point(const QuadTreePoint<T> *_p);
private:
	bool can_be_divided() const;
public:
	typename list<const QuadTreePoint<T> *>::const_iterator 
	begin_point_list()const { return point_list.begin(); }
	typename list<const QuadTreePoint<T> *>::const_iterator 
	end_point_list()const { return point_list.end(); }
	const list<const QuadTreePoint<T> *> *
	get_point_list() { return &point_list; }
	size_t get_size_of_points() const;
	bool has_subnodes() const;
	const QuadTreeNode<T> *get_root() const;
	const QuadTreeNode<T> *get_parent() const { return parent; }
	const QuadTreeNode<T> *get_first_child() const;
	const QuadTreeNode<T> *get_next_brother() const;
	bool is_child_end() const;
	double get_density(const bool _including_children=true) const;
	void make_recursion(
		std::function<void(
			const list<const QuadTreePoint<T> *> &)> &) const;
private:
	const QuadTreeNode<T> *get_node1() const { return node1; }
	const QuadTreeNode<T> *get_node2() const { return node2; }
	const QuadTreeNode<T> *get_node3() const { return node3; }
	const QuadTreeNode<T> *get_node4() const { return node4; }
public:
	// x_min, x_max, y_min, y_max
	bool is_overlapped(
		const dtype_t _x_min, 
		const dtype_t _x_max, 
		const dtype_t _y_min, 
		const dtype_t _y_max) const;
};

template <typename T>
QuadTreeNode<T>::QuadTreeNode() {
	parent = 0;
	node1 = 0;
	node2 = 0;
	node3 = 0;
	node4 = 0;
	node_index = 0;
	level = 0;
	count = 0;
	x_from = 0;
	x_to = 0;
	y_from = 0;
	y_to = 0;
	cell_size = 1;
}

template <typename T>
QuadTreeNode<T>::QuadTreeNode(
	QuadTreeNode<T> *_root, 
	const int _node_index, 
	const dtype_t _x_from, 
	const dtype_t _x_to, 
	const dtype_t _y_from, 
	const dtype_t _y_to, 
	const dtype_t _cell_size) {
	assert(int(_x_to - _x_from) >= 1);
	assert(int(_y_to - _y_from) >= 1);

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
	count = 0;
	x_from = _x_from;
	x_to = _x_to;
	y_from = _y_from;
	y_to = _y_to;
	cell_size = _cell_size;
}

template <typename T>
QuadTreeNode<T>::~QuadTreeNode() {
	this->destroy_subnodes();
}

template <typename T>
void 
QuadTreeNode<T>::destroy_subnodes() {
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
QuadTreeNode<T>::center_x() const {
	return fabs(double(x_to + x_from)) / 2.0;
}

template <typename T>
double 
QuadTreeNode<T>::center_y() const {
	return  fabs(double(y_to + y_from)) / 2.0;
}

template <typename T>
void 
QuadTreeNode<T>::print_index() const {
	cout << this->get_index();
	const QuadTreeNode<T> *parent_node =
		this->get_parent();
	while(parent_node != 0) {
		cout
		<< "<"
		<< parent_node->get_index();
		parent_node = 
			parent_node->get_parent();
	}
//	cout << endl;
}

template <typename T>
void 
QuadTreeNode<T>::get_index_list(list<int> &_list) {
	_list.push_back(this->get_index());
	const QuadTreeNode<T> *parent_node =
		this->get_parent();
	while(parent_node != 0) {
		_list.push_back(parent_node->get_index());
		parent_node = 
			parent_node->get_parent();
	}
}

template <typename T>
QuadTreeNode<T> *
QuadTreeNode<T>::add_point_and_make_partition(
	const QuadTreePoint<T> *_p) {
	if (!can_be_divided()) {
		// not divide, just add point to this node
		point_list.push_back(_p);
		return this;
	}
	else {
		// divide this node to four sub-nodes, 
		// and add a point to one of them
		const dtype_t i = _p->get_x();
		const dtype_t j = _p->get_y();

		// if this node has sub-nodes, 
		// points are not inserted into this.

		// center i, j
		dtype_t ci = dtype_t((x_to - x_from) / 2.0) + x_from;
		dtype_t cj = dtype_t((y_to - y_from) / 2.0) + y_from;

		// node
		// 2 1
		// 3 4
		if (!this->has_subnodes()) {
			node1 = new QuadTreeNode<T>(
					this, 
					1, 
					ci, 
					x_to, 
					y_from, 
					cj, 
					cell_size);
			node2 = new QuadTreeNode<T>(
					this, 
					2, 
					x_from, 
					ci, 
					y_from, 
					cj, cell_size);
			node3 = new QuadTreeNode<T>(
					this, 
					3, 
					x_from, 
					ci, 
					cj, 
					y_to, 
					cell_size);
			node4 = new QuadTreeNode<T>(
					this, 
					4, 
					ci, 
					x_to, 
					cj, 
					y_to, 
					cell_size);
		}

		// node
		// 2 1
		// 3 4
		
		// node 1
//		if (ci <= i && i <= x_to &&
//			y_from <= j && j < cj) {
		if( LE(ci,i) && LE(i,x_to) &&
			LE(y_from,j) && LT(j,cj)) {
			return node1->add_point_and_make_partition(_p);

		}
		// node 2
//		else if (x_from <= i && i < ci &&
//			y_from <= j && j < cj) {
		else if ( LE(x_from,i) && LT(i,ci) &&
			LE(y_from,j) && LT(j,cj)) {
			return node2->add_point_and_make_partition(_p);
		}
		// node 3
//		else if (x_from <= i && i < ci &&
//			cj <= j && j <= y_to) {
		else if( LE(x_from,i) && LT(i,ci) &&
			LE(cj,j) && LE(j,y_to)) {
			return node3->add_point_and_make_partition(_p);
		}
		// node 4
//		else if (ci <= i && i <= x_to &&
//			cj <= j && j <= y_to) {
		else if( LE(ci,i) && LE(i,x_to) &&
			LE(cj,j) && LE(j,y_to)) {
			return node4->add_point_and_make_partition(_p);
		}
		else {
			throw ("you have to modify the above algorithm");
		}
	}
}

template <typename T>
bool 
QuadTreeNode<T>::remove_point(
	const QuadTreePoint<T> * _p) {
	typename list<const QuadTreePoint<T> *>::iterator result = 
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
QuadTreeNode<T>::get_size_of_points() const {
	return point_list.size();
}

template <typename T>
bool 
QuadTreeNode<T>::has_subnodes() const {
	return (node1 == 0 && 
		node2 == 0 && 
		node3 == 0 && 
		node4 == 0) ? false : true;
}

template <typename T>
const QuadTreeNode<T> * 
QuadTreeNode<T>::get_root() const {
	const QuadTreeNode<T> *parent = this->get_parent();
	while (parent->get_parent() != 0) {
		parent = parent->get_parent();
	}
	return parent;
}

template <typename T>
bool 
QuadTreeNode<T>::can_be_divided() const {
	dtype_t di = dtype_t(x_to - x_from);
	dtype_t dj = dtype_t(y_to - y_from);
//	if (di <= 1 || dj <= 1 ||
//		di <= cell_size || dj <= cell_size) {
	if( LE(di,1) || LE(dj,1) ||
		LE(di,cell_size) || LE(dj,cell_size) ) {
		return false;
	}
	else {
		return true;
	}
}

template <typename T>
const QuadTreeNode<T> * 
QuadTreeNode<T>::get_first_child() const {
	return get_node1();
}

template <typename T>
const QuadTreeNode<T> * 
QuadTreeNode<T>::get_next_brother() const {
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
QuadTreeNode<T>::is_child_end() const {
	return (this->node_index == 4) ? true : false;
}

template <typename T>
double 
QuadTreeNode<T>::get_density(const bool _including_children) const {
	double density = double(point_list.size())/
			double(((x_to-x_from)*(y_to-y_from)));
	if(!_including_children) {
		return density;
	} else {
		if(!this->has_subnodes()) {
			return density;
		} else {
			const QuadTreeNode<T> *child_node = 
				this->get_first_child();
			while(child_node != 0) {
				density += child_node->get_density();
				child_node = child_node->get_next_brother();
			}
			return density/4.0;
		}
	}
}

template <typename T>
void 
QuadTreeNode<T>::make_recursion(
	std::function<void(
		const list<const QuadTreePoint<T> *> &)> &_func) const {
	if(!this->point_list.empty()) {
		_func(point_list);
	}
	const QuadTreeNode<T> *child_node = this->get_first_child();
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
QuadTreeNode<T>::is_overlapped(
	const dtype_t _x_min, 
	const dtype_t _x_max, 
	const dtype_t _y_min, 
	const dtype_t _y_max) const {
//	dtype_t x_min = _px - _dx;
//	dtype_t x_max = _px + _dx;
//	dtype_t y_min = _py - _dy;
//	dtype_t y_max = _py + _dy;
//
//
//	const QuadTreeNode<T> *root = this->get_root();
//	if (i_min < root->x_from) { 
//		i_min = root->x_from; 
//	}
//	if (i_max > root->x_to) { 
//		i_max = root->x_to; 
//	}
//	if (j_min < root->y_from) { 
//		j_min = root->y_from; 
//	}
//	if (j_max > root->y_to) { 
//		j_max = root->y_to; 
//	}

	// 4 points from given (pi,pj)
	// (i_max, j_min), 1
	// (i_min, j_min), 2
	// (i_min, j_max), 3
	// (i_max, j_max), 4

	// point 1 is overlapped with this node
//	if (this->x_from <= i_max && 
//		i_max <= this->x_to &&
//		this->y_from <= j_min && 
//		j_min <= this->y_to) {
	if( LE(this->x_from, _x_max) &&
		LE(_x_max, this->x_to) &&
		LE(this->y_from, _y_min) &&
		LE(_y_min, this->y_to) ) {
		return true;
	}

	// point 2 is overlapped with this node
//	if (this->x_from <= i_min && 
//		i_min <= this->x_to &&
//		this->y_from <= j_min && 
//		j_min <= this->y_to) {
	if( LE(this->x_from, _x_min) &&
		LE(_x_min, this->x_to) &&
		LE(this->y_from, _y_min) &&
		LE(_y_min, this->y_to) ) {
		return true;
	}

	// point 3 is overlapped with this node
//	if (this->x_from <= i_min && 
//		i_min <= this->x_to &&
//		this->y_from <= j_max && 
//		j_max <= this->y_to) {
	if( LE(this->x_from, _x_min) &&
		LE(_x_min, this->x_to) &&
		LE(this->y_from, _y_max) &&
		LE(_y_max, this->y_to) ) {
		return true;
	}

	// point 4 is overlapped with this node
//	if (this->x_from <= i_max && 
//		i_max <= this->x_to &&
//		this->y_from <= j_max && 
//		j_max <= this->y_to) {
	if( LE(this->x_from, _x_max) &&
		LE(_x_max, this->x_to) &&
		LE(this->y_from, _y_max) &&
		LE(_y_max, this->y_to) ) {
		return true;
	}

	return false;
}

#endif
