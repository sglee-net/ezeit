#ifndef __CLUSTER_HPP__
#define __CLUSTER_HPP__

#include <typeinfo>
#include <typeindex>
#include <functional>
#include <list>
#include <statistics/statistics.hpp>
#include "rapidjson/document.h"

using namespace std;
using namespace statistics;
using namespace rapidjson;

// default distance function (Euclidean distance)
//
// default similarity function (Cosine similarity)
//
//
template <typename T>
class Cluster {
public:
	static const string key_name;
	static const string key_centroid;
	static const string key_distance_summary;
	static const string key_similarity_summary;
public:
	Cluster() {
		// set default functions
		// Euclidean distance
		// 
		// Cosine similarity
	}
	virtual ~Cluster() {}
private:
	string name;
	list<T> centroid;
	SummaryStatistics distance_summary;
	SummaryStatistics similarity_summary;
	std::function<double(
		const list<T> &_l,
		const list<T> &_r)> dist_func;
	std::function<double(
		const list<T> &_l,
		const list<T> &_r)> simil_func;
public:
	size_t get_size_of_centroid() const { return centroid.size(); }
	
	list<T> *get_centroid() const { return &centroid; }

	void set_distance_function(
		std::function<double(
			const list<T> &_l,
			const list<T> &_r)> &_func) {
		dist_func = _func;
	}

	void set_similarity_function(
		std::function<double(
			const list<T> &_l,
			const list<T> &_r)> &_func) {
		simil_func = _func;
	}

	void compute_centroid(
		list<T> &_centroid,
		const list<list<T> *> &_vset);
	void compute(const list<list<T> *> &_vset); 

	void read_json(const string &_json);

	void read_json(const rapidjson::Value &_obj);
	
	template <typename Writer>
	void serialize(Writer &writer) const;
};

template <typename T>
const string Cluster<T>::key_name = "name";

template <typename T>
const string Cluster<T>::key_centroid = "centroid";

template <typename T>
const string Cluster<T>::key_distance_summary = "distance_summary";

template <typename T>
const string Cluster<T>::key_similarity_summary = "similarity_summary";

template <typename T>
void Cluster<T>::compute_centroid(
	list<T> &_centroid,
	const list<list<T> *> &_vset) {
	assert(_vset.size()!=0);
	_centroid.clear();
	typename list<list<T> *>::const_iterator citr = _vset.begin();
	typename list<list<T> *>::const_iterator citr_next = citr;
	++citr_next;
	std::copy((*citr)->begin(), (*citr)->end(), _centroid.begin());

	while(citr_next != _vset.end()) {
		list<T> *l = (*citr_next);
		typename list<T>::iterator itr_centroid =
			_centroid.begin();
		for_each(l->begin(), 
			l->end(), 
			[&](const T &_v) {
			(*itr_centroid) += _v;
			(*itr_centroid) /= 2.0;
			++itr_centroid;	
		});
		++citr_next;
	}
}

template <typename T>
void Cluster<T>::compute(
	const list<list<T> *> &_vset) {
	compute_centroild(
		centroid,
		_vset);

	list<double> distance_list;
	list<double> similarity_list;
	typename list<list<T> *>::const_iterator citr = _vset.begin();
	while(citr != _vset.end()) {
		list<T> *l = (*citr);
		double d = dist_func(&centroid, l);
		double s = simil_func(&centroid, l);
		distance_list.push_back(d);
		similarity_list.push_back(s);
		++citr;
	}
	SummaryStatistics::get_summary(distance_summary, distance_list);
	SummaryStatistics::get_summary(similarity_summary, similarity_list);
}

template <typename T>
void Cluster<T>::read_json(const string &_json) {
	StringStream s(_json.c_str());
	Document d;
	d.ParseStream(s);

	this->read_json(d);
}

template <typename T>
void Cluster<T>::read_json(const Value &_obj) {
	name = _obj[key_name.c_str()].GetString();
//	cout<< name <<endl;

	const Value &obj_centroid = _obj[key_centroid.c_str()];
	assert(obj_centroid.IsArray());
	for(SizeType i=0; i<obj_centroid.Size(); i++) {
		T v;
		if(typeid(v).name() == typeid(float).name() ||
			typeid(v).name() == typeid(double).name()){
			v = obj_centroid[i].GetDouble();
		} else if(typeid(v).name() == typeid(int).name()) {
			v = obj_centroid[i].GetInt();
		} else if(typeid(v).name() == typeid(bool).name()) {
			v = obj_centroid[i].GetBool();
		} else {
			throw("unsupported data type");
		}
//		cout<<v<<endl;
		centroid.push_back(v);
	}

	distance_summary = 
		SummaryStatistics::read_json(
			_obj[key_distance_summary.c_str()]);
	
	similarity_summary = 
		SummaryStatistics::read_json(
			_obj[key_similarity_summary.c_str()]);
}

template <typename T>
template <typename Writer>
void Cluster<T>::serialize(Writer &writer) const {
	writer.StartObject();

	writer.String(key_name.c_str());
	writer.String(name.c_str());
	
	writer.String(key_centroid.c_str());
	writer.StartArray();
	for_each(
		centroid.begin(),
		centroid.end(),
		[&](T v) {
			if(typeid(v).name() == typeid(float).name() ||
				typeid(v).name() == typeid(double).name()){
				writer.Double(v);
			} else if(typeid(v).name() == typeid(int).name()) {
				writer.Int(v);
			} else if(typeid(v).name() == typeid(bool).name()) {
				writer.Bool(v);
			} else {
				throw("unsupported data type");
			}
		});
	writer.EndArray();

	writer.String(key_distance_summary.c_str());
	distance_summary.serialize(writer);

	writer.String(key_similarity_summary.c_str());
	similarity_summary.serialize(writer);

	writer.EndObject();
}

template <typename T>
class Clusters {
public:
	static const string key_clusters;
public:
	Clusters() {}
	virtual ~Clusters();
private:
	list<Cluster<T> *> cluster_list;
	void remove_all();
public:
	void add_cluster(Cluster<T> *_cluster);

	void read_json(const string &_json);

	void read_json(const Value &_obj);

	template <typename Writer>
	void serialize(Writer &writer) const;
};

template <typename T>
const string Clusters<T>::key_clusters = "clusters";

template <typename T>
Clusters<T>::~Clusters() {
	remove_all();
}

template <typename T>
void 
Clusters<T>::remove_all() {
	for_each(
		cluster_list.begin(),
		cluster_list.end(),
		[](Cluster<T> *_cluster) {
			if(_cluster) {
				delete _cluster;
				_cluster=0;
			}
		});
	cluster_list.clear();
}

template <typename T>
void 
Clusters<T>::add_cluster(Cluster<T> *_cluster) {
	cluster_list.push_back(_cluster);
}

template <typename T>
void Clusters<T>::read_json(const string &_json) {
	StringStream s(_json.c_str());
	Document d;
	d.ParseStream(s);

	this->read_json(d);
}

template <typename T>
void Clusters<T>::read_json(const Value &_obj) {
	remove_all();	
	const Value &obj_cluster = _obj[key_clusters.c_str()];
	assert(obj_cluster.IsArray());
	for(SizeType i=0; i<obj_cluster.Size(); i++) {
		Cluster<T> *cluster = new Cluster<T>();
		cluster->read_json(obj_cluster[i]);
		cluster_list.push_back(cluster);
	}
}

template <typename T>
template <typename Writer>
void Clusters<T>::serialize(Writer &writer) const {
	writer.StartObject();

	writer.String(key_clusters.c_str());
	writer.StartArray();
	for_each(
		cluster_list.begin(),
		cluster_list.end(),
		[&](Cluster<T> *_cluster) {
			if(_cluster) {
				_cluster->serialize(writer);
			}
		});
	writer.EndArray();

	writer.EndObject();
}

#endif
