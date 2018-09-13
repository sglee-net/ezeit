#include "gsubf.h"
#include <iostream>
#include <sstream>
#include <stdarg.h>
#include "glogger/glogger.h"
#include "gtaskque/gtaskque.h"
#include "glogger/gexecutor_filewriter.h"

#define USLEEP_DELAY 100

using namespace std;

int GSubf::gen_subfiles(
	const string &_path,
	const string &_filename,
	const string &_delimiter,
	const bool _skip_first_line,
	const list<int> &_column_indexes,
	const string &_output_path,
	const string &_postfix) {
	GLogger<string,ofstream> *logger = 
		GLogger<string,ofstream>::getInstance();
	logger->debug(__FILE__,__LINE__,"logger %p",(void *)logger);

	list<Criterion<string> > line_criterion;
	list<Criterion<string> > searching_criterion;
	if (GSubf::extract_sorting_criterion(
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

		string file_path = _output_path + name + _postfix;
		
		logger->debug(__FILE__,__LINE__,"file_path: %s",file_path.c_str());
		//cout<<"file_path: "<<file_path<<endl;

		ofstream *fileout = new ofstream(file_path, ios::out);
		GExecutorInterface<string,ofstream> *appender = 
			new GExecutorFileWriter<string,ofstream>(fileout, true);
		string key;
		key.assign(name.begin(), name.end());
		fileout_root->addAppender(key, appender);
	});

	ifstream filein(_path + _filename, ios::in);
	if (!filein.is_open()) {
		logger->error(__FILE__,__LINE__,"file open failed");

		fileout_root->stop();
		while(fileout_root->isRunning()) {
			usleep(USLEEP_DELAY);
		}
		delete fileout_root;
		fileout_root = 0;
		throw("file open failed");
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

		//logger->debug(__FILE__,__LINE__,"nline %d",nline);

		nline++;
		citr_line_criterion++;
		if (citr_line_criterion == line_criterion.end()) {
			break;
		}
	}
	filein.close();

	fileout_root->stop();
	while (fileout_root->isRunning()) {
		usleep(USLEEP_DELAY);
	}
	delete fileout_root;
	fileout_root = 0;

	return 0;
}

#ifdef WIN32
int GSubf::gen_subfiles(
	const wstring &_path,
	const wstring &_filename,
	const wstring &_delimiter,
	const bool _skip_first_line,
	const list<int> &_column_indexes,
	const wstring &_output_path,
	const wstring &_postfix) {
	list<Criterion<wstring> > line_criterion;
	list<Criterion<wstring> > searching_criterion;
	if (GSubf::extract_sorting_criterion(
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
			new GExecutorFileWriter<wstring,wofstream>(fileout, true);
		string key;
		key.assign(name.begin(), name.end());
		fileout_root->addAppender(key, appender);
	});
	fileout_root->start();

	wifstream filein(_path + _filename, ios::in);
	if (!filein.is_open()) {
		fileout_root->stop();
		while(fileout_root->isRunning()) {
			usleep(USEEP_DELAY);
		}
		
		fileout_root->stop();
		delete fileout_root;
		fileout_root = 0;
		throw("file open failed");
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

	fileout_root->stop();

//	int count = 0;
	while (fileout_root->isRunning()) {
//		cout << "waiting... to stop thread" << count << endl;
//		count++;
		usleep(USLEEP_DELAY);
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
int GSubf::extract_sorting_criterion(
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
		logger->error(__FILE__,__LINE__,"file open failed");
		throw("file open failed");
		//return -1;
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
int GSubf::extract_sorting_criterion(
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
		logger->error(__FILE__,__LINE__,"file open failed");
		throw("file open failed");
		//return -1;
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

/*
int GSubf::gen_subfiles(
	const string &_path,
	const string &_filename,
	const string &_delimiter,
	const bool _skip_first_line,
	const list<int> &_column_indexes,
	const string &_output_path,
	const string &_postfix) {
	GLogger<string,ofstream> *logger = 
		GLogger<string,ofstream>::getInstance();
	
	list<Criterion<string> > line_criterion;
	list<Criterion<string> > searching_criterion;
	if (GSubf::extract_sorting_criterion(
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

		//cout << name << endl;
		logger->debug(__FILE__,__LINE__,name.c_str());

		string file_path = _output_path + name + _postfix;

		logger->debug(__FILE__,__LINE__,file_path.c_str());

		ofstream *fileout = new ofstream(file_path, ios::out);
		GExecutorInterface<string,ofstream> *appender = 
			new GExecutorFileWriter<string,ofstream>(fileout, true);
		string key;
		key.assign(name.begin(), name.end());
		fileout_root->addAppender(key, appender);
	});
	fileout_root->start();

	ifstream filein(_path + _filename, ios::in);
	if (!filein.is_open()) {
		//cout << "file open failed" << endl;
		
		logger->stop();

		fileout_root->stop();
		delete fileout_root;
		fileout_root = 0;
		throw("file open failed");
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
int GSubf::gen_subfiles(
	const wstring &_path,
	const wstring &_filename,
	const wstring &_delimiter,
	const bool _skip_first_line,
	const list<int> &_column_indexes,
	const wstring &_output_path,
	const wstring &_postfix) {
	list<Criterion<wstring> > line_criterion;
	list<Criterion<wstring> > searching_criterion;
	if (GSubf::extract_sorting_criterion(
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
			new GExecutorFileWriter<wstring,wofstream>(fileout, true);
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
int GSubf::extract_sorting_criterion(
	list<Criterion<string> > &_criterion_of_line,
	list<Criterion<string> > &_criterions,
	const string &_path,
	const string &_filename,
	const string &_delimiter,
	const bool _skip_first_line,
	const list<int> &_column_indexes) {
	GLogger<string,ofstream> *logger = 
		GLogger<string,ofstream>::getInstance();

	int max_column_index = 
		*std::max_element(_column_indexes.begin(),
		_column_indexes.end());
	
	logger->debug(__FILE__,__LINE__,"max col index: %d",max_column_index);
	ifstream filein(_path + _filename, ios::in);
	if (!filein.is_open()) {
		logger->error(__FILE__,__LINE__,"file does not exist");
		throw("file does not exsit");
		//return -1;
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

		logger->debug(__FILE__,__LINE__,"getline()");

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

			logger->debug(__FILE__,__LINE__,substr.c_str());

			list<int>::const_iterator citr =
				find(_column_indexes.begin(), _column_indexes.end(), count);
			if (citr != _column_indexes.end()) {
				searching_criterion.addName(substr);
			}
			logger->debug(__FILE__,__LINE__,"count: %d",count);

			++count;
			if (count > max_column_index) {
				break;
				logger->debug(__FILE__,__LINE__,"break");
			}
		}

		logger->debug(__FILE__,__LINE__,"searching criterion %d",(int)searching_criterion.size());
		assert(!searching_criterion.empty());
		if (searching_criterion.empty()) {
			_criterions.unique();
			filein.close();
			//cout << "can not find out searching criteria" << endl;
			logger->error(__FILE__,__LINE__,"can not find out searching criteria");
			return -1;
		}

		_criterions.push_back(searching_criterion);
		_criterion_of_line.push_back(searching_criterion);

		logger->debug(__FILE__, __LINE__, "nline: %d, cursor_position: %d", nline, (int)line_position);
		
		nline++;
		line_position = filein.tellg();

	}

	logger->debug(__FILE__,__LINE__,"before criterions.unique()");
	_criterions.unique();
	filein.close();

	logger->debug(__FILE__,__LINE__,"criterions size: %d",_criterions.size());

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
int GSubf::extract_sorting_criterion(
	list<Criterion<wstring> > &_criterion_of_line,
	list<Criterion<wstring> > &_criterions,
	const wstring &_path,
	const wstring &_filename,
	const wstring &_delimiter,
	const bool _skip_first_line,
	const list<int> &_column_indexes) {
	GLogger<wstring,wofstream> *logger = 
		GLogger<wstring,wofstream>::getInstance();

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
