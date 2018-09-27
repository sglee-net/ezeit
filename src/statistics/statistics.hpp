#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#include <map>
#include <list>
#include <limits>
#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

namespace statistics {

class SummaryStatistics {
public:
	double avr;
	double var;
	double med;
	double min;
	double max;
	double rng;
public:
	SummaryStatistics() {
		avr = var = med = min = max = rng = 0.0;
	}

	SummaryStatistics(const SummaryStatistics &_v) {
		*this = _v;
	}

	SummaryStatistics &operator = (const SummaryStatistics &_v) {
		avr = _v.avr;
		var = _v.var;
		med = _v.med;
		min = _v.min;
		max = _v.max;
		rng = _v.rng;
		return *this;
	}

	static void get_summary(
		SummaryStatistics &_summary,
		const list<double> &_list);

	static void get_summary(
		double &_avr,
		double &_var,
		double &_med,
		double &_min,
		double &_max,
		double &_rng,
		const list<double> &_list);

	static SummaryStatistics read_json(const Value &_obj);

	template <typename Writer>
	void serialize(Writer &writer) const;
};

void SummaryStatistics::get_summary(
	SummaryStatistics &_summary,
	const list<double> &_list) {
	get_summary(
		_summary.avr,
		_summary.var,
		_summary.med,
		_summary.min,
		_summary.max,
		_summary.rng,
		_list);
}

void SummaryStatistics::get_summary(
		double &_avr,
		double &_var,
		double &_med,
		double &_min,
		double &_max,
		double &_rng,
		const list<double> &_list) {
	_avr = _var = _med = 0.0;
	_min = numeric_limits<double>::max();
	_max = -_min;
	multimap<double, list<double>::const_iterator> mmap;

	for(list<double>::const_iterator citr = _list.begin();
		citr != _list.end();
		++citr) {
		double value = (*citr);
		_avr += value;
		_min = std::min(_min,value);
		_max = std::max(_max,value);
		mmap.insert(
			pair<double, list<double>::const_iterator>
			(value,citr));
	}

	_avr = _avr / double(_list.size());

	size_t count = 0;
	multimap<double, list<double>::const_iterator>::const_iterator 
		citr_mmap = mmap.begin();
	for_each(
		_list.begin(),
		_list.end(),
		[&](double value) {
			_var += pow(value - _avr, 2.0);
			_rng += (value - _min);
			
			if(count == _list.size()/2) {
				_med = value;
			}
			count++;
			citr_mmap++;
		});
	_var = _var / double(_list.size());
}

SummaryStatistics SummaryStatistics::read_json(const Value &_obj) {
	SummaryStatistics summary;
	{
		const Value &obj = _obj["avr"];
		assert(!obj.IsNull());
		assert(obj.IsDouble());
		summary.avr=obj.GetDouble();
	}
	{
		const Value &obj = _obj["var"];
		assert(!obj.IsNull());
		assert(obj.IsDouble());
		summary.var=obj.GetDouble();
	}
	{
		const Value &obj = _obj["med"];
		assert(!obj.IsNull());
		assert(obj.IsDouble());
		summary.med=obj.GetDouble();
	}
	{
		const Value &obj = _obj["min"];
		assert(!obj.IsNull());
		assert(obj.IsDouble());
		summary.min=obj.GetDouble();
	}
	{
		const Value &obj = _obj["max"];
		assert(!obj.IsNull());
		assert(obj.IsDouble());
		summary.max=obj.GetDouble();
	}
	{
		const Value &obj = _obj["rng"];
		assert(!obj.IsNull());
		assert(obj.IsDouble());
		summary.rng=obj.GetDouble();
	}
}

template <typename Writer>
void SummaryStatistics::serialize(Writer &writer) const {
	writer.StartObject();

	writer.String("avr");
	writer.Double(avr);

	writer.String("var");
	writer.Double(var);

	writer.String("med");
	writer.Double(med);
	
	writer.String("med");
	writer.Double(med);

	writer.String("min");
	writer.Double(min);
	
	writer.String("max");
	writer.Double(max);
	
	writer.String("rng");
	writer.Double(rng);

	writer.EndObject();
}

};

#endif
