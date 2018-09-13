#ifndef __GEXECUTORFILEWRITER_H__
#define __GEXECUTORFILEWRITER_H__

#include <map>
#include <string>
#include "gtaskque/gtaskque.h"
#include <fstream>

#ifdef __linux__
#include <unistd.h>
#endif

#ifdef WIN32
#include <Windows.h>
#include <process.h>
#endif

using namespace std;

template <typename T, typename U>
class GExecutorFileWriter :public GExecutorInterface<T,U> {
private:
	bool line_end;
private:
	GExecutorFileWriter() { }
	GExecutorFileWriter(const GExecutorFileWriter &) { }
	GExecutorFileWriter& operator=(const GExecutorFileWriter &) { 
		return *this; }
public:
	GExecutorFileWriter(U *_val, const bool _autoRemove) 
		: GExecutorInterface<T,U>(_val, _autoRemove) {
		line_end = true;
	}
	void setLineEnd(const bool _arg=true)=0;
	virtual int execute(T &_arg)=0;
};

// specialization
template <>
class GExecutorFileWriter<string,ofstream> 
	:public GExecutorInterface<string,ofstream> {
private:
	bool line_end;
private:
	GExecutorFileWriter() {}
	GExecutorFileWriter(const GExecutorFileWriter &) {}
	GExecutorFileWriter& operator=(const GExecutorFileWriter &) { 
		return *this; }
public:
	GExecutorFileWriter(ofstream *_val, const bool _autoRemove) 
		: GExecutorInterface<string,ofstream>(_val, _autoRemove) {
		line_end = true;
	}
	void setLineEnd(const bool _arg=true) {
		line_end = _arg;
	}
	virtual int execute(string &_arg) {
		if (!attribute) {
			return -1;
		}

		ofstream *fileout = (ofstream *)attribute;
		(*fileout) << _arg ;
		if(line_end) {
			(*fileout) << endl;
		}

		return 0;
	}
};

// specialization
template <>
class GExecutorFileWriter<wstring,wofstream> 
	:public GExecutorInterface<wstring,wofstream> {
private:
	bool line_end;
private:
	GExecutorFileWriter() {}
	GExecutorFileWriter(const GExecutorFileWriter &) {}
	GExecutorFileWriter& operator=(const GExecutorFileWriter &) { 
		return *this; }
public:
	GExecutorFileWriter(wofstream *_val, const bool _autoRemove) 
		: GExecutorInterface<wstring,wofstream>(_val, _autoRemove) {
		line_end = true;
	}
	void setLineEnd(const bool _arg=true) {
		line_end = _arg;
	}
	virtual int execute(wstring &_arg) {
		if (!attribute) {
			return -1;
		}

		wofstream *fileout = (wofstream *)attribute;
		(*fileout) << _arg ;
		if(line_end) {
			(*fileout) << endl;
		}

		return 0;
	}
};

#endif
