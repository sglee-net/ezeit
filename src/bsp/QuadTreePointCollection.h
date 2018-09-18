/**
 * @author SG Lee
 * @since 2002
*/

#ifndef __QUADTREEPOINTCOLLECTION_H__
#define __QUADTREEPOINTCOLLECTION_H__

#include <map>
#include <list>
#include "QuadTreePoint.h"

using namespace std;

template <typename T>
class QuadTreePointCollection {
private:
	QuadTreePointCollection();
	QuadTreePointCollection(const QuadTreePointCollection &) {}
	QuadTreePointCollection &operator=(const QuadTreePointCollection &) { 
		return *this; 
	}
public:
	~QuadTreePointCollection();
public:
	static QuadTreePointCollection<T> *get_instance() {
		static QuadTreePointCollection<T> instance;
		return &instance;
	}
private:
	list<QuadTreePoint<T> *> point_list;
	map<std::pair<int, int>, QuadTreePoint<T> *> point_map;
public:
	bool empty() const;
	size_t size() const;
	void clear();
	void insert_point(const int _i, const int _j, QuadTreePoint<T> *_pt);
	QuadTreePoint<T> *get_point(const int _i, const int _j);
	bool erase_point(const int _i, const int _j);
	typename list<QuadTreePoint<T> *>::const_iterator begin()const;
	typename list<QuadTreePoint<T> *>::const_iterator end()const;
};

template <typename T>
QuadTreePointCollection<T>::QuadTreePointCollection() {
}

template <typename T>
QuadTreePointCollection<T>::~QuadTreePointCollection() {
	clear();
}

template <typename T>
size_t 
QuadTreePointCollection<T>::size() const {
	return point_map.size();
}

template <typename T>
bool 
QuadTreePointCollection<T>::empty() const {
	return point_map.empty() ? true : false ;
}

template <typename T>
void 
QuadTreePointCollection<T>::clear() {
	typename list<QuadTreePoint<T> *>::iterator itr_point = 
		point_list.begin();
	while(itr_point!=point_list.end()) {
		QuadTreePoint<T> *pt = (*itr_point);
		// dealloc
		delete pt;
		pt = 0;

		++itr_point;
	}
	point_list.clear();
	point_map.clear();
}

template <typename T>
void 
QuadTreePointCollection<T>::insert_point(
	const int _i, 
	const int _j, 
	QuadTreePoint<T> *_pt) {
	point_map.insert(
		pair< pair<int, int>, 
		QuadTreePoint<T> *>(
			pair<int, int>(_i,_j), _pt));
	point_list.push_back(_pt);
}

template <typename T>
QuadTreePoint<T> *
QuadTreePointCollection<T>::get_point(
	const int _i, 
	const int _j) {
	typename map<pair<int,int>, QuadTreePoint<T> *>::iterator itr = 
		point_map.find(pair<int,int>(_i, _j));
	return  (itr == point_map.end()) ? 0 : itr->second;
}

template <typename T>
bool 
QuadTreePointCollection<T>::erase_point(
	const int _i, 
	const int _j) {
	typename map<pair<int,int>, QuadTreePoint<T> *>::iterator itr = 
		point_map.find(pair<int, int>(_i,_j));
	if (itr == point_map.end()) {
		return false;
	}
	else {
		QuadTreePoint<T> *point = itr->second;
		point_list.remove(point);
		point_map.erase(itr);

		return true;
	}
}

template <typename T>
typename list<QuadTreePoint<T> *>::const_iterator 
QuadTreePointCollection<T>::begin() const {
	return point_list.begin();
}

template <typename T>
typename list<QuadTreePoint<T> *>::const_iterator 
QuadTreePointCollection<T>::end() const {
	return point_list.end();
}

#endif
