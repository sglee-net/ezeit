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

template <typename T>
class QuadTreeNode;

template <typename T>
class QuadTreePoint {
private:
	QuadTreePoint() {
		x = 0;
		y = 0;
		node = 0;
	}
public:
	QuadTreePoint(
		const int _x, 
		const int _y, 
		const T &_v) {
		x = _x;
		y = _y;
		value = _v;
		node = 0;
	}

	~QuadTreePoint() {}
private:
	int x;
	int y;
	T value;
	// node will be set by the function add_point_make_tree of QuadTreeTree
	const QuadTreeNode<T> *node; 
public:
	int get_x() const { return x;  }
	void set_x(const int _v) { x = _v;  }
	int get_y() const { return y; }
	void set_y(const int _v) { y = _v; }
	double get_value() const { return value; }
	void set_value(const double _v) { value = _v; }

	void set_bsp_node(const QuadTreeNode<T> *_node) { node = _node; }
	const QuadTreeNode<T> *get_bsp_node() const { return node; }
	QuadTreeNode<T> *get_bsp_node() { return (QuadTreeNode<T> *)node; }
	
	double get_distance(const QuadTreePoint<T> *_v) const {
		return sqrt(
			pow(_v->get_x() - (double)this->get_x(), 2.0) + 
			pow(_v->get_y() - (double)this->get_y(), 2.0));
	}

	double get_distance(const double _x, const double _y) const {
		return sqrt(
			pow(_x - (double)this->get_x(), 2.0) + 
			pow(_y - (double)this->get_y(), 2.0));
	}
};

template <typename T>
double 
get_distance_btw_points(
	const QuadTreePoint<T> * _v1, 
	const QuadTreePoint<T> * _v2) {
	return sqrt(
		pow(_v2->get_x() - _v1->get_x(), 2.0) + 
		pow(_v2->get_y() - _v1->get_y(), 2.0));
}

template <typename T>
void 
clear_list_with_delete(
	list<QuadTreePoint<T> *> &_list) {
	typename list<QuadTreePoint<T> *>::iterator itr = _list.begin();
	while (itr != _list.end()) {
		delete (*itr);
		(*itr) = 0;
		++itr;
	}
	_list.clear();
}

#endif
