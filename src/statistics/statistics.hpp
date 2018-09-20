#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#include <map>
#include <list>
#include <limits>

using namespace std;

namespace statistics {
	void get_summary(
		double &_avr,
		double &_var,
		double &_med,
		double &_min,
		double &_max,
		double &_rng,
		const list<double> &_list);
};

void statistics::get_summary(
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

#endif
