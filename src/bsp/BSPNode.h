/**
 * @ author SG Lee
 * @ since 2002
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
class BSPNode {
public:
private:
	BSPNode();
public:
	BSPNode(
		BSPNode<T> *_root,
		const int _node_index,
		const dtype_t _i_from, 
		const dtype_t _i_to, 
		const dtype_t _j_from, 
		const dtype_t  _j_to, 
		const dtype_t _cell_size);
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
	dtype_t i_from;
	dtype_t j_from;
	dtype_t i_to;
	dtype_t j_to;
	dtype_t cell_size;
	list<const BSPPoint<T> *> point_list;
public:
	double center_i() const;
	double center_j() const;
	dtype_t get_width() const { return (i_to-i_from); }
	dtype_t get_height() const { return (j_to-j_from); }
	dtype_t get_i_from() const { return i_from; }
	dtype_t get_i_to() const { return i_to; }
	dtype_t get_j_from() const { return j_from; }
	dtype_t get_j_to() const { return j_to; }
	const int get_level() const { return level; }
	const int get_index() const { return node_index; }
	void print_index() const; 
	void get_index_list(list<int> &_list);
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
	double get_density(const bool _including_children=true) const;
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
		const dtype_t _pi, 
		const dtype_t _pj, 
		const dtype_t _di, 
		const dtype_t _dj) const;
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
	cell_size = 1;
}

template <typename T>
BSPNode<T>::BSPNode(
	BSPNode<T> *_root, 
	const int _node_index, 
	const dtype_t _i_from, 
	const dtype_t _i_to, 
	const dtype_t _j_from, 
	const dtype_t _j_to, 
	const dtype_t _cell_size) {
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
void 
BSPNode<T>::print_index() const {
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
//	cout << endl;
}

template <typename T>
void 
BSPNode<T>::get_index_list(list<int> &_list) {
	_list.push_back(this->get_index());
	const BSPNode<T> *parent_node =
		this->get_parent();
	while(parent_node != 0) {
		_list.push_back(parent_node->get_index());
		parent_node = 
			parent_node->get_parent();
	}
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
		const dtype_t i = _p->get_i();
		const dtype_t j = _p->get_j();

		// if this node has sub-nodes, 
		// points are not inserted into this.

		// center i, j
		dtype_t ci = dtype_t((i_to - i_from) / 2.0) + i_from;
		dtype_t cj = dtype_t((j_to - j_from) / 2.0) + j_from;

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
//		if (ci <= i && i <= i_to &&
//			j_from <= j && j < cj) {
		if( LE(ci,i) && LE(i,i_to) &&
			LE(j_from,j) && LT(j,cj)) {
			return node1->add_point_and_make_partition(_p);

		}
		// node 2
//		else if (i_from <= i && i < ci &&
//			j_from <= j && j < cj) {
		else if ( LE(i_from,i) && LT(i,ci) &&
			LE(j_from,j) && LT(j,cj)) {
			return node2->add_point_and_make_partition(_p);
		}
		// node 3
//		else if (i_from <= i && i < ci &&
//			cj <= j && j <= j_to) {
		else if( LE(i_from,i) && LT(i,ci) &&
			LE(cj,j) && LE(j,j_to)) {
			return node3->add_point_and_make_partition(_p);
		}
		// node 4
//		else if (ci <= i && i <= i_to &&
//			cj <= j && j <= j_to) {
		else if( LE(ci,i) && LE(i,i_to) &&
			LE(cj,j) && LE(j,j_to)) {
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
	dtype_t di = dtype_t(i_to - i_from);
	dtype_t dj = dtype_t(j_to - j_from);
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
BSPNode<T>::get_density(const bool _including_children) const {
	double density = double(point_list.size())/
			double(((i_to-i_from)*(j_to-j_from)));
	if(!_including_children) {
		return density;
	} else {
		if(!this->has_subnodes()) {
			return density;
		} else {
			const BSPNode<T> *child_node = 
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
	const dtype_t _pi, 
	const dtype_t _pj, 
	const dtype_t _di, 
	const dtype_t _dj) const {
	dtype_t i_min = _pi - _di;
	dtype_t i_max = _pi + _di;
	dtype_t j_min = _pj - _dj;
	dtype_t j_max = _pj + _dj;
//	const BSPNode<T> *root = this->get_root();
//	if (i_min < root->i_from) { 
//		i_min = root->i_from; 
//	}
//	if (i_max > root->i_to) { 
//		i_max = root->i_to; 
//	}
//	if (j_min < root->j_from) { 
//		j_min = root->j_from; 
//	}
//	if (j_max > root->j_to) { 
//		j_max = root->j_to; 
//	}

	// 4 points from given (pi,pj)
	// (i_max, j_min), 1
	// (i_min, j_min), 2
	// (i_min, j_max), 3
	// (i_max, j_max), 4

	// point 1 is overlapped with this node
//	if (this->i_from <= i_max && 
//		i_max <= this->i_to &&
//		this->j_from <= j_min && 
//		j_min <= this->j_to) {
	if( LE(i_from, i_max) &&
		LE(i_max, i_to) &&
		LE(j_from, j_min) &&
		LE(j_min, j_to) ) {
		return true;
	}

	// point 2 is overlapped with this node
//	if (this->i_from <= i_min && 
//		i_min <= this->i_to &&
//		this->j_from <= j_min && 
//		j_min <= this->j_to) {
	if( LE(i_from, i_min) &&
		LE(i_min, i_to) &&
		LE(j_from, j_min) &&
		LE(j_min, j_to) ) {
		return true;
	}

	// point 3 is overlapped with this node
//	if (this->i_from <= i_min && 
//		i_min <= this->i_to &&
//		this->j_from <= j_max && 
//		j_max <= this->j_to) {
	if( LE(i_from, i_min) &&
		LE(i_min, i_to) &&
		LE(j_from, j_max) &&
		LE(j_max, j_to) ) {
		return true;
	}

	// point 4 is overlapped with this node
//	if (this->i_from <= i_max && 
//		i_max <= this->i_to &&
//		this->j_from <= j_max && 
//		j_max <= this->j_to) {
	if( LE(i_from, i_max) &&
		LE(i_max, i_to) &&
		LE(j_from, j_max) &&
		LE(j_max, j_to) ) {
		return true;
	}

	return false;
}

#endif
