/**
 * @author SG Lee
 * @since 2002
*/

#ifndef __QUADTREEPOINT_H__
#define __QUADTREEPOINT_H__

#include <list>
#include <math.h>
#include <limits>
#include "QuadTreeNode.h"

using namespace std;

template <typename T, typename S>
class QuadTreeNode;

template <typename T, typename S>
class QuadTreePoint {
private:
	QuadTreePoint() {
		x = 0;
		y = 0;
		node = 0;
	}
public:
	QuadTreePoint(
		const T _x, 
		const T _y, 
		const S &_v) {
		x = _x;
		y = _y;
		value = _v;
		node = 0;
	}

	~QuadTreePoint() {}
private:
	T x;
	T y;
	S value;
	// node will be set by the function add_point_make_tree of QuadTreeTree
	const QuadTreeNode<T,S> *node; 
public:
	T get_x() const { return x;  }
	void set_x(const T _v) { x = _v;  }
	T get_y() const { return y; }
	void set_y(const T _v) { y = _v; }
	S get_value() const { return value; }
	void set_value(const S _v) { value = _v; }

	void set_bsp_node(const QuadTreeNode<T,S> *_node) { node = _node; }
	const QuadTreeNode<T,S> *get_bsp_node() const { return node; }
	QuadTreeNode<T,S> *get_bsp_node() { 
		return (QuadTreeNode<T,S> *)node; }
	
	double get_distance(const QuadTreePoint<T,S> *_v) const {
		return sqrt(
		pow((double)_v->get_x()-(double)this->get_x(),2.0)
		+pow((double)_v->get_y()-(double)this->get_y(),2.0));
	}

	double get_distance(const double _x, const double _y) const {
		return sqrt(
		pow((double)_x-(double)this->get_x(),2.0) + 
		pow((double)_y-(double)this->get_y(),2.0));
	}
};

template <typename T, typename S>
double 
get_distance_btw_points(
	const QuadTreePoint<T,S> * _v1, 
	const QuadTreePoint<T,S> * _v2) {
	return sqrt(
	pow((double)_v2->get_x()-(double)_v1->get_x(),2.0) + 
	pow((double)_v2->get_y()-(double)_v1->get_y(),2.0));
}

template <typename T, typename S>
void 
clear_list_with_delete(
	list<QuadTreePoint<T,S> *> &_list) {
	typename list<QuadTreePoint<T,S> *>::iterator itr = _list.begin();
	while (itr != _list.end()) {
		delete (*itr);
		(*itr) = 0;
		++itr;
	}
	_list.clear();
}

#endif
