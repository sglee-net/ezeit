/**
 * @author SG Lee
 * @since 2016
*/

#ifndef __BSPPOINT_H__
#define __BSPPOINT_H__

#include <list>
#include <math.h>
#include <limits>
#include "BSPNode.h"

using namespace std;

template <typename T>
class BSPNode;

template <typename T>
class BSPPoint {
private:
	BSPPoint() {
		i = 0;
		j = 0;
		node = 0;
	}
public:
	BSPPoint(
		const int _i, 
		const int _j, 
		const T &_v) {
		i = _i;
		j = _j;
		value = _v;
		node = 0;
	}

	~BSPPoint() {}
private:
	int i;
	int j;
	T value;
	// node will be set by the function add_point_make_tree of BSPTree
	const BSPNode<T> *node; 
public:
	int get_i() const { return i;  }
	void set_i(const int _v) { i = _v;  }
	int get_j() const { return j; }
	void set_j(const int _v) { j = _v; }
	double get_value() const { return value; }
	void set_value(const double _v) { value = _v; }

	void set_bsp_node(const BSPNode<T> *_node) { node = _node; }
	const BSPNode<T> *get_bsp_node() const { return node; }
	BSPNode<T> *get_bsp_node() { return (BSPNode<T> *)node; }
	
	double get_distance(const BSPPoint<T> *_v) const {
		return sqrt(
			pow(_v->get_i() - (double)this->get_i(), 2.0) + 
			pow(_v->get_j() - (double)this->get_j(), 2.0));
	}

	double get_distance(const double _x, const double _y) const {
		return sqrt(
			pow(_x - (double)this->get_i(), 2.0) + 
			pow(_y - (double)this->get_j(), 2.0));
	}
};

template <typename T>
double 
get_distance_btw_points(
	const BSPPoint<T> * _v1, 
	const BSPPoint<T> * _v2) {
	return sqrt(
		pow(_v2->get_i() - _v1->get_i(), 2.0) + 
		pow(_v2->get_j() - _v1->get_j(), 2.0));
}

template <typename T>
void 
clear_list_with_delete(
	list<BSPPoint<T> *> &_list) {
	typename list<BSPPoint<T> *>::iterator itr = _list.begin();
	while (itr != _list.end()) {
		delete (*itr);
		(*itr) = 0;
		++itr;
	}
	_list.clear();
}

#endif
