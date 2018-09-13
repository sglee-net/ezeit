/** 
 * @author SG Lee
 * @since 2/01/2017
 * @version 0.1
 * @description
 */

#ifndef __GSUBF_H__
#define __GSUBF_H__

#include <string>
#include <map>
#include <fstream>
#include <algorithm>
#include <list>
#include <vector>

#ifdef __linux__
#include <unistd.h>
#endif

#ifdef WIN32
#include <Windows.h>
#include <process.h>
#endif

#ifdef WIN32
typedef HANDLE			MUTEX_TYPE;
typedef HANDLE			THREADHANDLE_TYPE;
#elif __linux__
typedef pthread_mutex_t		MUTEX_TYPE;
typedef pthread_t		THREADHANDLE_TYPE;
#endif

using namespace std;

template <typename T>
class Criterion {
private:
	list<T> nameList;
	T sumOfName;
public:
	Criterion() {}
	Criterion(const Criterion<T> &_val) { *this = _val; }

	Criterion<T> & operator = (const Criterion<T> &_val) {
		nameList.clear();
		nameList.assign(_val.nameList.begin(), _val.nameList.end());
		sumOfName = _val.sumOfName;
		return *this;
	}

	bool operator == (const Criterion<T> &_val) const {
		// compare contents of list => implement later if needed
		return (sumOfName == _val.sumOfName) ? true : false;
	}

	bool operator < (const Criterion<T> &_val) const {
		return (sumOfName < _val.sumOfName) ? true : false;
	}

	bool operator > (const Criterion<T> &_val) const {
		return (sumOfName > _val.sumOfName) ? true : false;
	}
public:
	virtual size_t size() const = 0;
	virtual bool empty() const = 0;
	virtual void addName(const T &_val) = 0;
	virtual T getSumOfName() const = 0; 
	virtual typename list<T>::const_iterator begin()const = 0;
	virtual typename list<T>::iterator begin() = 0; 
	virtual typename list<T>::const_iterator end()const = 0;
	virtual typename list<T>::iterator end() = 0;
};

template <>
class Criterion<string> {
private:
	list<string> nameList;
	string sumOfName;
public:
	Criterion() {}
	Criterion(const Criterion<string> &_val) {
		*this = _val;
	}

	Criterion<string> & operator = (const Criterion<string> &_val) {
		nameList.clear();
		nameList.assign(_val.nameList.begin(), _val.nameList.end());
		sumOfName = _val.sumOfName;
		return *this;
	}

	bool operator == (const Criterion<string> &_val) const {
		// compare contents of list => implement later if needed
		return (sumOfName == _val.sumOfName) ? true : false;
	}

	bool operator < (const Criterion<string> &_val) const {
		return (sumOfName < _val.sumOfName) ? true : false;
	}

	bool operator > (const Criterion<string> &_val) const {
		return (sumOfName > _val.sumOfName) ? true : false;
	}
public:
	size_t size() const {
		return nameList.size();
	}

	bool empty() const {
		return nameList.empty();
	}

	void addName(const string &_val) {
		string name = _val;
		while (name.at(0) == '\"') {
			name.erase(0, 1);
		}

		while (name.at(name.length() - 1) == '\"') {
			name.erase(name.length() - 1, 1);
		}

		nameList.push_back(name);
		if (!sumOfName.empty()) {
			sumOfName += "_";
		}
		sumOfName += name;
	}

	string getSumOfName() const {
		return sumOfName;
	}

	list<string>::const_iterator begin()const {
		return nameList.begin();
	}

	list<string>::iterator begin() {
		return nameList.begin();
	}

	list<string>::const_iterator end()const {
		return nameList.end();
	}

	list<string>::iterator end() {
		return nameList.end();
	}
};

template <>
class Criterion<wstring> {
private:
	list<wstring> nameList;
	wstring sumOfName;
public:
	Criterion() {}
	Criterion(const Criterion<wstring> &_val) {
		*this = _val;
	}

	Criterion<wstring> & operator = (const Criterion<wstring> &_val) {
		nameList.clear();
		nameList.assign(_val.nameList.begin(), _val.nameList.end());
		sumOfName = _val.sumOfName;
		return *this;
	}

	bool operator == (const Criterion<wstring> &_val) const {
		// compare contents of list => implement later if needed
		return (sumOfName == _val.sumOfName) ? true : false;
	}

	bool operator < (const Criterion<wstring> &_val) const {
		return (sumOfName < _val.sumOfName) ? true : false;
	}

	bool operator > (const Criterion<wstring> &_val) const {
		return (sumOfName > _val.sumOfName) ? true : false;
	}
public:
	size_t size() const {
		return nameList.size();
	}

	bool empty() const {
		return nameList.empty();
	}

	void addName(const wstring &_val) {
		wstring name = _val;
		while (name.at(0) == L'\"') {
			name.erase(0, 1);
		}

		while (name.at(name.length() - 1) == L'\"') {
			name.erase(name.length() - 1, 1);
		}

		nameList.push_back(name);
		if (!sumOfName.empty()) {
			sumOfName += L"_";
		}
		sumOfName += name;
	}

	wstring getSumOfName() const {
		return sumOfName;
	}

	list<wstring>::const_iterator begin()const {
		return nameList.begin();
	}

	list<wstring>::iterator begin() {
		return nameList.begin();
	}

	list<wstring>::const_iterator end()const {
		return nameList.end();
	}

	list<wstring>::iterator end() {
		return nameList.end();
	}
};

template <typename T>
class FileInfo {
	T path;
	T filename;
	T delimiter;
	T outputPath;
	T postfix;
	list<int> columnIndexes;
	int maxColumnIndex;
};

template <>
class FileInfo<string> {
	string path;
	string filename;
	string delimiter;
	string outputPath;
	string postfix;
	list<int> columnIndexes;
	int maxColumnIndex;
};

#ifdef WIN32
template <>
class FileInfo<wstring> {
	wstring path;
	wstring filename;
	wstring delimiter;
	wstring outputPath;
	wstring postfix;
	list<int> columnIndexes;
	int maxColumnIndex;
};
#endif

class GSubf {
public:
	static int gen_subfiles(
		const string &_path,
		const string &_filename,
		const string &_delimiter,
		const bool _skip_first_line,
		const list<int> &_column_indexes,
		const string &_output_path,
		const string &_postfix);

	// vector<Criterion> : searching criterion
	// list<Criterion> : list of unique criterion
	// _path : file path
	// _filename : file name
	// _delimeter : delimter to divide colmuns
	// _skip_first_line due to a heading sentence
	// _column_indexes : column indexes to decide the criterion for file extraction
	static int extract_sorting_criterion(
		list<Criterion<string> > &_criterion_of_line,
		list<Criterion<string> > &_criterions,
		const string &_path,
		const string &_filename,
		const string &_delimiter,
		const bool _skip_first_line,
		const list<int> &_column_indexes);

#ifdef WIN32
	static int gen_subfiles(
		const wstring &_path,
		const wstring &_filename,
		const wstring &_delimiter,
		const bool _skip_first_line,
		const list<int> &_column_indexes,
		const wstring &_output_path,
		const wstring &_postfix);

	// vector<Criterion> : searching criterion, for example, [recipe, step]
	// list<Criterion> : list of unique criterion
	// _path : file path
	// _filename : file name
	// _delimeter : delimter to divide colmuns
	// _skip_first_line due to a heading sentence
	// _column_indexes : column indexes to decide the criterion for file extraction
	static int extract_sorting_criterion(
		list<Criterion<wstring> > &_criterion_of_line,
		list<Criterion<wstring> > &_criterions,
		const wstring &_path,
		const wstring &_filename,
		const wstring &_delimiter,
		const bool _skip_first_line,
		const list<int> &_column_indexes);
#endif

};

/*
int GenSubfile::gen_subfiles(
	const string &_path,
	const string &_filename,
	const string &_delimiter,
	const bool _skip_first_line,
	const list<int> &_column_indexes,
	const string &_output_path,
	const string &_postfix) {
	list<Criterion<string> > line_criterion;
	list<Criterion<string> > searching_criterion;
	if (GenSubfile::extract_sorting_criterion(
		line_criterion,
		searching_criterion,
		_path, _filename, _delimiter,
		_skip_first_line,
		_column_indexes) != 0) {
		cout << "extract_criterion error" << endl;
	}
	else {
		cout << "extract_criterion success" << endl;
	}

	GLogger<string,ofstream> *fileout_root = 
		new GLogger<string,ofstream>();

	for_each(
		searching_criterion.begin(),
		searching_criterion.end(),
		[=](Criterion<string> &criterion) {
		string name = "";
		list<string>::const_iterator citr_str = criterion.begin();
		while (citr_str != criterion.end()) {
			if (citr_str == criterion.begin()) {
				name = (*citr_str);
			}
			else {
				name = name + "_" + (*citr_str);
			}
			++citr_str;
		}

		cout << name << endl;
		string file_path = _output_path + name + _postfix;
		ofstream *fileout = new ofstream(file_path, ios::out);
		GExecutorInterface<string,ofstream> *appender = 
			new GTaskFileWriter<string,ofstream>(fileout, true);
		string key;
		key.assign(name.begin(), name.end());
		fileout_root->addAppender(key, appender);
	});
	fileout_root->start();

	ifstream filein(_path + _filename, ios::in);
	if (!filein.is_open()) {
		//cerr("error");
		cout << "filein error" << endl;
	}
	filein.seekg(0, filein.beg);
	size_t nline = 0;
	string str_line;
	list<Criterion<string> >::const_iterator citr_line_criterion =
		line_criterion.begin();
	while (getline(filein, str_line)) {
		if (_skip_first_line && nline == 0) {
			nline++;
			continue;
		}

		string name;
		list<string>::const_iterator citr_str = citr_line_criterion->begin();
		while (citr_str != citr_line_criterion->end()) {
			if (citr_str == citr_line_criterion->begin()) {
				name = (*citr_str);
			}
			else {
				name = name + "_" + (*citr_str);
			}
			++citr_str;
		}
		string key;
		key.assign(name.begin(), name.end());
		fileout_root->write(key, str_line);

		//		cout << nline << endl;

		nline++;
		citr_line_criterion++;
		if (citr_line_criterion == line_criterion.end()) {
			break;
		}
	}
	filein.close();

	//fileout_root->start();
	fileout_root->stop();

	int count = 0;
	while (fileout_root->isRunning()) {
		cout << "waiting" << count << endl;
		count++;
		//Sleep(1000);
	}
	delete fileout_root;
	fileout_root = 0;

	return 0;
}

#ifdef WIN32
int GenSubfile::gen_subfiles(
	const wstring &_path,
	const wstring &_filename,
	const wstring &_delimiter,
	const bool _skip_first_line,
	const list<int> &_column_indexes,
	const wstring &_output_path,
	const wstring &_postfix) {
	list<Criterion<wstring> > line_criterion;
	list<Criterion<wstring> > searching_criterion;
	if (GenSubfile::extract_sorting_criterion(
		line_criterion,
		searching_criterion,
		_path, _filename, _delimiter,
		_skip_first_line,
		_column_indexes) != 0) {
		cout << "extract_criterion error" << endl;
	}
	else {
		cout << "extract_criterion success" << endl;
	}

	GLogger<wstring,wofstream> *fileout_root = 
		new GLogger<wstring,wofstream>();

	for_each(
		searching_criterion.begin(),
		searching_criterion.end(),
		[=](Criterion<wstring> &criterion) {
		wstring name = L"";
		list<wstring>::const_iterator citr_str = criterion.begin();
		while (citr_str != criterion.end()) {
			if (citr_str == criterion.begin()) {
				name = (*citr_str);
			}
			else {
				name = name + L"_" + (*citr_str);
			}
			++citr_str;
		}

		wcout << name << endl;
		wstring file_path = _output_path + name + _postfix;
		wofstream *fileout = new wofstream(filepath, ios::out);
		GExecutorInterface<wstring,wofstream> *appender = 
			new GTaskFileWriter<wstring,wofstream>(fileout, true);
		string key;
		key.assign(name.begin(), name.end());
		fileout_root->addAppender(key, appender);
	});
	fileout_root->start();

	wifstream filein(_path + _filename, ios::in);
	if (!filein.is_open()) {
		//cerr("error");
		cout << "filein error" << endl;
	}
	filein.seekg(0, filein.beg);
	size_t nline = 0;
	wstring str_line;
	list<Criterion<wstring> >::const_iterator citr_line_criterion =
		line_criterion.begin();
	while (getline(filein, str_line)) {
		if (_skip_first_line && nline == 0) {
			nline++;
			continue;
		}

		wstring name;
		list<wstring>::const_iterator citr_str = citr_line_criterion->begin();
		while (citr_str != citr_line_criterion->end()) {
			if (citr_str == citr_line_criterion->begin()) {
				name = (*citr_str);
			}
			else {
				name = name + L"_" + (*citr_str);
			}
			++citr_str;
		}
		string key;
		key.assign(name.begin(), name.end());
		fileout_root->write(key, str_line);

		//		cout << nline << endl;

		nline++;
		citr_line_criterion++;
		if (citr_line_criterion == line_criterion.end()) {
			break;
		}
	}
	filein.close();

	//fileout_root->start();
	fileout_root->stop();

	int count = 0;
	while (fileout_root->isRunning()) {
		cout << "waiting" << count << endl;
		count++;
		//Sleep(1000);
	}
	delete fileout_root;
	fileout_root = 0;

	return 0;
}
#endif

// list<Criterion> : searching criterion, for example, [recipe, step]
// list<Criterion> : list of unique criterion
// _path : file path
// _filename : file name
// _delimeter : delimter to divide colmuns
// _skip_first_line due to a heading sentence
// _column_indexes : column indexes to decide the criterion for file extraction
int GenSubfile::extract_sorting_criterion(
	list<Criterion<string> > &_criterion_of_line,
	list<Criterion<string> > &_criterions,
	const string &_path,
	const string &_filename,
	const string &_delimiter,
	const bool _skip_first_line,
	const list<int> &_column_indexes) {
	GLogger<string,ofstream> *logger = GLogger<string,ofstream>::getInstance();

	int max_column_index = 
		*std::max_element(_column_indexes.begin(),
		_column_indexes.end());

	ifstream filein(_path + _filename, ios::in);
	if (!filein.is_open()) {
		return -1;
	}

	filein.seekg(0, filein.beg);
	streampos line_position = filein.tellg();
	size_t nline = 0;
	string line;
	while (getline(filein, line)) {
		if (_skip_first_line && nline == 0) {
			nline++;
			line_position = filein.tellg();
			continue;
		}

		// find out key sets to search files
		size_t pos = 0;
		int count = 0;
		Criterion<string> searching_criterion;
		while ((pos = line.find(_delimiter)) != std::string::npos) {
			if (pos == 0) {
				_criterions.unique();
				filein.close();
				return nline == 0 ? -1 : 0;
			}

			string substr = line.substr(0, pos);
			line.erase(0, pos + _delimiter.length());

			list<int>::const_iterator citr =
				find(_column_indexes.begin(), _column_indexes.end(), count);
			if (citr != _column_indexes.end()) {
				searching_criterion.addName(substr);
			}

			++count;
			if (count > max_column_index) {
				break;
			}
		}

		assert(!searching_criterion.empty());
		if (searching_criterion.empty()) {
			_criterions.unique();
			filein.close();
			cout << "can not find out searching criteria" << endl;
			return -1;
		}

		_criterions.push_back(searching_criterion);
		_criterion_of_line.push_back(searching_criterion);

		//logger->debug(__FILE__, __LINE__, "nline: %d, cursor_position: %d", nline, (int)line_position);
		
		nline++;
		line_position = filein.tellg();

	}

	_criterions.unique();
	filein.close();

	return 0;
}

#ifdef WIN32
// list<Criterion> : searching criterion, for example, [recipe, step]
// list<Criterion> : list of unique criterion
// _path : file path
// _filename : file name
// _delimeter : delimter to divide colmuns
// _skip_first_line due to a heading sentence
// _column_indexes : column indexes to decide the criterion for file extraction
int GenSubfile::extract_sorting_criterion(
	list<Criterion<wstring> > &_criterion_of_line,
	list<Criterion<wstring> > &_criterions,
	const wstring &_path,
	const wstring &_filename,
	const wstring &_delimiter,
	const bool _skip_first_line,
	const list<int> &_column_indexes) {
	GLogger<string,ofstream> *logger = GLogger<string,ofstream>::getInstance();

	int max_column_index = 
		*std::max_element(_column_indexes.begin(),
		_column_indexes.end());

	wifstream filein(_path + _filename, ios::in);
	if (!filein.is_open()) {
		return -1;
	}

	filein.seekg(0, filein.beg);
	streampos line_position = filein.tellg();
	size_t nline = 0;
	wstring line;
	while (getline(filein, line)) {
		if (_skip_first_line && nline == 0) {
			nline++;
			line_position = filein.tellg();
			continue;
		}

		// find out key sets to search files
		size_t pos = 0;
		int count = 0;
		Criterion<wstring> searching_criterion;
		while ((pos = line.find(_delimiter)) != std::wstring::npos) {
			if (pos == 0) {
				_criterions.unique();
				filein.close();
				return nline == 0 ? -1 : 0;
			}

			wstring substr = line.substr(0, pos);
			line.erase(0, pos + _delimiter.length());

			list<int>::const_iterator citr =
				find(_column_indexes.begin(), _column_indexes.end(), count);
			if (citr != _column_indexes.end()) {
				searching_criterion.addName(substr);
			}

			++count;
			if (count > max_column_index) {
				break;
			}
		}

		assert(!searching_criterion.empty());
		if (searching_criterion.empty()) {
			_criterions.unique();
			filein.close();
			cout << "can not find out searching criteria" << endl;
			return -1;
		}

		_criterions.push_back(searching_criterion);
		_criterion_of_line.push_back(searching_criterion);

		//logger->debug(__FILE__, __LINE__, "nline: %d, cursor_position: %d", nline, (int)line_position);
		
		nline++;
		line_position = filein.tellg();

	}

	_criterions.unique();
	filein.close();

	return 0;
}
#endif
*/


#endif
