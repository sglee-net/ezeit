/**
 * @author SG Lee
 * @since 2002
*/

#ifndef __QUADTREEPOINTCOLLECTION_H__
#define __QUADTREEPOINTCOLLECTION_H__

#include <map>
#include <list>
#include <limits>
#include "QuadTreePoint.h"

using namespace std;

struct cmpLessDoublePair {
	template <typename T>
//	: public std::binary_function<pair<T,T>, pair<T,T>, bool> {
	bool operator()(const pair<T,T> &_l,const pair<T,T> &_r) const {
		if(fabs(_l.first-_r.first) < numeric_limits<T>::epsilon()) {
			return (_l.second < _r.second)? true:false;
		} else {
			return (_l.first < _r.first)? true:false; 
		}
	}
};

template <typename T, typename S>
class QuadTreePointCollection {
private:
	QuadTreePointCollection(const QuadTreePointCollection &) {}
	QuadTreePointCollection &operator=(const QuadTreePointCollection &) { 
		return *this; 
	}
public:
	// for multiple instances
	QuadTreePointCollection();
	~QuadTreePointCollection();
public:
	// for singleton instance
	static QuadTreePointCollection<T,S> *get_instance() {
		static QuadTreePointCollection<T,S> instance;
		return &instance;
	}
private:
	list<QuadTreePoint<T,S> *> point_list;
	map<pair<T, T>, QuadTreePoint<T,S> *,cmpLessDoublePair> 
		point_map;
public:
	bool empty() const;
	size_t size() const;
	void clear();
	void insert_point(const T _x, const T _y, QuadTreePoint<T,S> *_pt);
	QuadTreePoint<T,S> *get_point(const T _x, const T _y);
	bool erase_point(const T _x, const T _y);
	typename list<QuadTreePoint<T,S> *>::const_iterator begin()const;
	typename list<QuadTreePoint<T,S> *>::const_iterator end()const;
};

template <typename T, typename S>
QuadTreePointCollection<T,S>::QuadTreePointCollection() {
}

template <typename T, typename S>
QuadTreePointCollection<T,S>::~QuadTreePointCollection() {
	clear();
}

template <typename T, typename S>
size_t 
QuadTreePointCollection<T,S>::size() const {
	return point_map.size();
}

template <typename T, typename S>
bool 
QuadTreePointCollection<T,S>::empty() const {
	return point_map.empty() ? true : false ;
}

template <typename T, typename S>
void 
QuadTreePointCollection<T,S>::clear() {
	typename list<QuadTreePoint<T,S> *>::iterator itr_point = 
		point_list.begin();
	while(itr_point!=point_list.end()) {
		QuadTreePoint<T,S> *pt = (*itr_point);
		// dealloc
		delete pt;
		pt = 0;

		++itr_point;
	}
	point_list.clear();
	point_map.clear();
}

template <typename T, typename S>
void 
QuadTreePointCollection<T,S>::insert_point(
	const T _x, 
	const T _y, 
	QuadTreePoint<T,S> *_pt) {
	point_map.insert(
		pair< pair<T,T>, 
		QuadTreePoint<T,S> *>(
			pair<T,T>(_x,_y), _pt));
	point_list.push_back(_pt);
}

template <typename T, typename S>
QuadTreePoint<T,S> *
QuadTreePointCollection<T,S>::get_point(
	const T _x, 
	const T _y) {
	typename map<pair<T,T>, QuadTreePoint<T,S> *>::iterator itr = 
		point_map.find(pair<T,T>(_x, _y));
	return  (itr == point_map.end()) ? 0 : itr->second;
}

template <typename T, typename S>
bool 
QuadTreePointCollection<T,S>::erase_point(
	const T _x, 
	const T _y) {
	typename map<pair<T,T>, QuadTreePoint<T,S> *>::iterator itr = 
		point_map.find(pair<T,T>(_x,_y));
	if (itr == point_map.end()) {
		return false;
	}
	else {
		QuadTreePoint<T,S> *point = itr->second;
		point_list.remove(point);
		point_map.erase(itr);

		return true;
	}
}

template <typename T, typename S>
typename list<QuadTreePoint<T,S> *>::const_iterator 
QuadTreePointCollection<T,S>::begin() const {
	return point_list.begin();
}

template <typename T, typename S>
typename list<QuadTreePoint<T,S> *>::const_iterator 
QuadTreePointCollection<T,S>::end() const {
	return point_list.end();
}

#endif
