/**
 * @author SG Lee
 * @since 2002
*/

#ifndef __BSPPOINTCOLLECTION_H__
#define __BSPPOINTCOLLECTION_H__

#include <map>
#include <list>
#include "BSPPoint.h"

using namespace std;

template <typename T>
class BSPPointCollection {
private:
	BSPPointCollection();
	BSPPointCollection(const BSPPointCollection &) {}
	BSPPointCollection &operator=(const BSPPointCollection &) { 
		return *this; 
	}
public:
	~BSPPointCollection();
public:
	static BSPPointCollection<T> *get_instance() {
		static BSPPointCollection<T> instance;
		return &instance;
	}
private:
	list<BSPPoint<T> *> point_list;
	map<std::pair<int, int>, BSPPoint<T> *> point_map;
public:
	bool empty() const;
	size_t size() const;
	void clear();
	void insert_point(const int _i, const int _j, BSPPoint<T> *_pt);
	BSPPoint<T> *get_point(const int _i, const int _j);
	bool erase_point(const int _i, const int _j);
	typename list<BSPPoint<T> *>::const_iterator begin()const;
	typename list<BSPPoint<T> *>::const_iterator end()const;
};

template <typename T>
BSPPointCollection<T>::BSPPointCollection() {
}

template <typename T>
BSPPointCollection<T>::~BSPPointCollection() {
	clear();
}

template <typename T>
size_t 
BSPPointCollection<T>::size() const {
	return point_map.size();
}

template <typename T>
bool 
BSPPointCollection<T>::empty() const {
	return point_map.empty() ? true : false ;
}

template <typename T>
void 
BSPPointCollection<T>::clear() {
	typename list<BSPPoint<T> *>::iterator itr_point = 
		point_list.begin();
	while(itr_point!=point_list.end()) {
		BSPPoint<T> *pt = (*itr_point);
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
BSPPointCollection<T>::insert_point(
	const int _i, 
	const int _j, 
	BSPPoint<T> *_pt) {
	point_map.insert(
		pair< pair<int, int>, 
		BSPPoint<T> *>(
			pair<int, int>(_i,_j), _pt));
	point_list.push_back(_pt);
}

template <typename T>
BSPPoint<T> *
BSPPointCollection<T>::get_point(
	const int _i, 
	const int _j) {
	typename map<pair<int,int>, BSPPoint<T> *>::iterator itr = 
		point_map.find(pair<int,int>(_i, _j));
	return  (itr == point_map.end()) ? 0 : itr->second;
}

template <typename T>
bool 
BSPPointCollection<T>::erase_point(
	const int _i, 
	const int _j) {
	typename map<pair<int,int>, BSPPoint<T> *>::iterator itr = 
		point_map.find(pair<int, int>(_i,_j));
	if (itr == point_map.end()) {
		return false;
	}
	else {
		BSPPoint<T> *point = itr->second;
		point_list.remove(point);
		point_map.erase(itr);

		return true;
	}
}

template <typename T>
typename list<BSPPoint<T> *>::const_iterator 
BSPPointCollection<T>::begin() const {
	return point_list.begin();
}

template <typename T>
typename list<BSPPoint<T> *>::const_iterator 
BSPPointCollection<T>::end() const {
	return point_list.end();
}

#endif
