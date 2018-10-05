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

template<typename T>
bool GT(const T &_l, const T &_r) {
	return (_l > _r) ? true : false;
}

template<typename T>
bool GE(const T &_l, const T &_r) {
	return ( _l >= _r || 
		fabs(_l-_r) < numeric_limits<T>::epsilon() ) 
		? true : false;
}

template<typename T>
bool LE(const T &_l, const T &_r) {
	return ( _l <= _r || 
		fabs(_l-_r) < numeric_limits<T>::epsilon() ) 
		? true : false;
}

template<typename T>
bool LT(const T &_l, const T &_r) {
	return ( _l < _r) ? true : false;
}

template<typename T>
bool EQ(const T &_l, const T &_r) {
	return (fabs(_l-_r) < numeric_limits<T>::epsilon()) 
		? true : false;
}

template <typename T, typename S>
class QuadTreeNode {
public:
private:
	QuadTreeNode();
public:
	QuadTreeNode(
		QuadTreeNode<T,S> *_root,
		const int _index,
		const T _x_from, 
		const T _y_from, 
		const T _x_to, 
		const T _y_to, 
		const T _cell_size);
	~QuadTreeNode();
private:
	void destroy_subnodes();
private:
	QuadTreeNode<T,S> *parent;
	QuadTreeNode<T,S> *node1;
	QuadTreeNode<T,S> *node2;
	QuadTreeNode<T,S> *node3;
	QuadTreeNode<T,S> *node4;
	int index; // 1, 2, 3, 4
	int level; // root:0 -> child1 -> child2 -> child3 ...
//	size_t count;
	T x_from;
	T y_from;
	T x_to;
	T y_to;
	T cell_size;
	list<const QuadTreePoint<T,S> *> point_list;
public:
	double center_x() const;
	double center_y() const;
	T get_width() const { return (x_to-x_from); }
	T get_height() const { return (y_to-y_from); }
	T get_x_from() const { return x_from; }
	T get_x_to() const { return x_to; }
	T get_y_from() const { return y_from; }
	T get_y_to() const { return y_to; }
	const int get_level() const { return level; }
	const int get_index() const { return index; }
	void print_index() const; 
	void get_index_list(list<int> &_list);
	QuadTreeNode * add_point_and_make_partition(
		const QuadTreePoint<T,S> * _p);
	bool remove_point(const QuadTreePoint<T,S> *_p);
private:
	bool can_be_divided() const;
public:
	typename list<const QuadTreePoint<T,S> *>::const_iterator 
	begin_point_list()const { return point_list.begin(); }

	typename list<const QuadTreePoint<T,S> *>::const_iterator 
	end_point_list()const { return point_list.end(); }
	
	const list<const QuadTreePoint<T,S> *> *
	get_point_list() { return &point_list; }

	size_t get_size_of_points() const;
	bool has_subnodes() const;
	const QuadTreeNode<T,S> *get_root() const;
	const QuadTreeNode<T,S> *get_parent() const { return parent; }
	const QuadTreeNode<T,S> *get_first_child() const;
	const QuadTreeNode<T,S> *get_next_brother() const;
	bool is_child_end() const;
	double get_density(const bool _including_children=true) const;
	void make_recursion(
		std::function<void(
			const list<const QuadTreePoint<T,S> *> &)> &) const;
private:
	const QuadTreeNode<T,S> *get_node1() const { return node1; }
	const QuadTreeNode<T,S> *get_node2() const { return node2; }
	const QuadTreeNode<T,S> *get_node3() const { return node3; }
	const QuadTreeNode<T,S> *get_node4() const { return node4; }
public:
	// x_min, y_min, x_max, y_max
	bool is_overlapped(
		const T _x_min, 
		const T _y_min, 
		const T _x_max, 
		const T _y_max) const;
};

template <typename T, typename S>
QuadTreeNode<T,S>::QuadTreeNode() {
	parent = 0;
	node1 = 0;
	node2 = 0;
	node3 = 0;
	node4 = 0;
	index = 0;
	level = 0;
	x_from = 0;
	y_from = 0;
	x_to = 0;
	y_to = 0;
	cell_size = 1;
}

template <typename T, typename S>
QuadTreeNode<T,S>::QuadTreeNode(
	QuadTreeNode<T,S> *_root, 
	const int _index, 
	const T _x_from, 
	const T _y_from, 
	const T _x_to, 
	const T _y_to, 
	const T _cell_size) {
	assert(T(_x_to-_x_from) >= _cell_size);
	assert(T(_y_to-_y_from) >= _cell_size);
	if(std::is_integral<T>::value) {
		assert(_cell_size >= 1);
	}

	parent = _root;
	node1 = 0;
	node2 = 0;
	node3 = 0;
	node4 = 0;
	index = _index;
	if(_root == 0) {
		level = 0;
	} else {
		level = _root->get_level() + 1;
	}
	x_from = _x_from;
	x_to = _x_to;
	y_from = _y_from;
	y_to = _y_to;
	cell_size = _cell_size;
}

template <typename T, typename S>
QuadTreeNode<T,S>::~QuadTreeNode() {
	this->destroy_subnodes();
}

template <typename T, typename S>
void 
QuadTreeNode<T,S>::destroy_subnodes() {
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

template <typename T, typename S>
double 
QuadTreeNode<T,S>::center_x() const {
	return fabs(double(x_to + x_from)) / 2.0;
}

template <typename T, typename S>
double 
QuadTreeNode<T,S>::center_y() const {
	return  fabs(double(y_to + y_from)) / 2.0;
}

template <typename T, typename S>
void 
QuadTreeNode<T,S>::print_index() const {
	cout << this->get_index();
	const QuadTreeNode<T,S> *parent_node =
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

template <typename T, typename S>
void 
QuadTreeNode<T,S>::get_index_list(list<int> &_list) {
	_list.push_back(this->get_index());
	const QuadTreeNode<T,S> *parent_node =
		this->get_parent();
	while(parent_node != 0) {
		_list.push_back(parent_node->get_index());
		parent_node = 
			parent_node->get_parent();
	}
}

template <typename T, typename S>
QuadTreeNode<T,S> *
QuadTreeNode<T,S>::add_point_and_make_partition(
	const QuadTreePoint<T,S> *_p) {
	if (!can_be_divided()) {
		// not divide, just add point to this node
		point_list.push_back(_p);
		return this;
	}
	else {
		// divide this node to four sub-nodes, 
		// and add a point to one of them
		const T x = _p->get_x();
		const T y = _p->get_y();

		// if this node has sub-nodes, 
		// points are not inserted into this.

		// center i, j
		T cx = T(double(x_to - x_from) * 0.5) + x_from;
		T cy = T(double(y_to - y_from) * 0.5) + y_from;

		// node
		// 2 1
		// 3 4
		if (!this->has_subnodes()) {
			node1 = new QuadTreeNode<T,S>(
					this, 
					1, 
					cx, 
					y_from, 
					x_to, 
					cy, 
					cell_size);
			node2 = new QuadTreeNode<T,S>(
					this, 
					2, 
					x_from, 
					y_from, 
					cx, 
					cy, cell_size);
			node3 = new QuadTreeNode<T,S>(
					this, 
					3, 
					x_from, 
					cy, 
					cx, 
					y_to, 
					cell_size);
			node4 = new QuadTreeNode<T,S>(
					this, 
					4, 
					cx, 
					cy, 
					x_to, 
					y_to, 
					cell_size);
		}

		// node
		// 2 1
		// 3 4
		
		// node 1
		// ---
		// | |
		//
//		if (cx <= x && x <= x_to &&
//			y_from <= y && y < cy) {
		if( LE(cx,x) && LE(x,x_to) &&
			LE(y_from,y) && LT(y,cy)) {
			return node1->add_point_and_make_partition(_p);

		}
		// node 2
		// ---
		// | 
		// ---
//		else if (x_from <= x && x < cx &&
//			y_from <= y && y <= cy) {
		else if ( LE(x_from,x) && LT(x,cx) &&
			LE(y_from,y) && LE(y,cy)) {
			return node2->add_point_and_make_partition(_p);
		}
		// node 3
		// 
		// | |
		// ---
//		else if (x_from <= x && x <= cx &&
//			cy < y && y <= y_to) {
		else if( LE(x_from,x) && LE(x,cx) &&
			LT(cy,y) && LE(y,y_to)) {
			return node3->add_point_and_make_partition(_p);
		}
		// node 4
		// ---
		//   |
		// ---
//		else if (cx < x && x <= x_to &&
//			cy <= y && y <= y_to) {
		else if( LT(cx,x) && LE(x,x_to) &&
			LE(cy,y) && LE(y,y_to)) {
			return node4->add_point_and_make_partition(_p);
		}
		// exactly center
		else if ( EQ(cx,x) && EQ(cy,y) ) {
			return node1->add_point_and_make_partition(_p);
		}
		else {
			cout << "cx, cy" << cx << ", " <<cy<<endl;
			cout<< "x from, to" << x_from << ", "<<x_to <<endl;
			cout<< "y from, to" << y_from << ", "<<y_to <<endl;
			cout<<"x,y"<< x<<", "<<y<<endl;
			throw ("you have to modify the above algorithm");
		}
	}
}

template <typename T, typename S>
bool 
QuadTreeNode<T,S>::remove_point(
	const QuadTreePoint<T,S> * _p) {
	typename list<const QuadTreePoint<T,S> *>::iterator result = 
		find(point_list.begin(), point_list.end(), _p);
	if (result == point_list.end()) {
		return false;
	} else {
		point_list.remove(_p);
		return true;
	}
}

template <typename T, typename S>
size_t 
QuadTreeNode<T,S>::get_size_of_points() const {
	return point_list.size();
}

template <typename T, typename S>
bool 
QuadTreeNode<T,S>::has_subnodes() const {
	return (node1 == 0 && 
		node2 == 0 && 
		node3 == 0 && 
		node4 == 0) ? false : true;
}

template <typename T, typename S>
const QuadTreeNode<T,S> * 
QuadTreeNode<T,S>::get_root() const {
	const QuadTreeNode<T,S> *parent = this->get_parent();
	while (parent->get_parent() != 0) {
		parent = parent->get_parent();
	}
	return parent;
}

template <typename T, typename S>
bool 
QuadTreeNode<T,S>::can_be_divided() const {
	T dx = T(double(x_to - x_from)*0.5);
	T dy = T(double(y_to - y_from)*0.5);
	if( LT(dx,cell_size) || LT(dy,cell_size) ) {
		return false;
	}
	else {
		return true;
	}
}

template <typename T, typename S>
const QuadTreeNode<T,S> * 
QuadTreeNode<T,S>::get_first_child() const {
	return get_node1();
}

template <typename T, typename S>
const QuadTreeNode<T,S> * 
QuadTreeNode<T,S>::get_next_brother() const {
	if (this->index == 0) {
		return 0;
	} else if (this->index == 1) {
		return this->get_parent()->get_node2();
	} else if(this->index == 2) {
		return this->get_parent()->get_node3();
	} else if (this->index == 3) {
		return this->get_parent()->get_node4();
	} else if (this->index == 4) {
		return 0;
	} else {
		throw("check code");
	}
}

template <typename T, typename S>
bool 
QuadTreeNode<T,S>::is_child_end() const {
	return (this->index == 4) ? true : false;
}

template <typename T, typename S>
double 
QuadTreeNode<T,S>::get_density(const bool _including_children) const {
	double density = double(point_list.size())/
			double(((x_to-x_from)*(y_to-y_from)));
	if(!_including_children) {
		return density;
	} else {
		if(!this->has_subnodes()) {
			return density;
		} else {
			const QuadTreeNode<T,S> *child_node = 
				this->get_first_child();
			while(child_node != 0) {
				density += child_node->get_density();
				child_node = child_node->get_next_brother();
			}
			return density/4.0;
		}
	}
}

template <typename T, typename S>
void 
QuadTreeNode<T,S>::make_recursion(
	std::function<void(
		const list<const QuadTreePoint<T,S> *> &)> &_func) const {
	if(!this->point_list.empty()) {
		_func(point_list);
	}
	const QuadTreeNode<T,S> *child_node = this->get_first_child();
	while(child_node !=0 ) {
		child_node->make_recursion(_func);
		child_node = child_node->get_next_brother();
	}; 
}

// check overlap between two rectangles
template <typename T, typename S>
bool 
QuadTreeNode<T,S>::is_overlapped(
	const T _x_from, 
	const T _y_from, 
	const T _x_to, 
	const T _y_to) const {
	if(GE(this->x_from, _x_to) ||
		GE(_x_from, this->x_to)) {
		return false;
	}
	if(GE(this->y_from, _y_to) ||
		GE(_y_from, this->y_to)) {
		return false;
	}

	return true;
}

#endif
