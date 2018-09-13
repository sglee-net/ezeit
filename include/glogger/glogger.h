/** 
 * @author SG Lee
 * @since 3/20/2010
 * @version 0.1
 * @description
 * This file needs gtaskque.h that makes logging tasks be perfomed 
 * automatically.
 */

#ifndef __GLOGGER_H__
#define __GLOGGER_H__

#include <map>
#include <list>
#include <memory>
#include <string>
#include <fstream>
#include <algorithm>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include "gtaskque/gtaskque.h"
#include <string.h>

#define MAX_BUFFER_SIZE 1024 

using namespace std;

static const std::string currentTime() {
	time_t t = time(0);
	struct tm now;

#ifdef WIN32
	localtime_s(&now, &t);
#elif __linux__
	now = *localtime(&t);
#endif

	char buf[80];
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &now);
	return buf;
}

static const std::wstring currentTimew() {
	time_t t = time(0);
	struct tm now;

#ifdef WIN32
	localtime_s(&now, &t);
#elif __linux__
	now = *localtime(&t);
#endif

	wchar_t buf[80];
	wcsftime(buf, sizeof(buf), L"%Y-%m-%d.%X", &now);
	return buf;
}

template <typename T>
class CommonLoggingMsg {

};

template<>
class CommonLoggingMsg<string> {
public:
	string getMsg(const string &_file, const long _line, string _msg) {
		string str_line = std::to_string(_line);
		string msg = currentTime() + string(", ") + str_line + string("line, ") + _file + string(", ") + _msg;
		return msg;
	}
};

template<>
class CommonLoggingMsg<wstring> {
public: 
	wstring getMsg(const string &_file, const long _line, wstring _msg) {
		wstring str_line = std::to_wstring(_line);
		wstring wstr_file;
		wstr_file.assign(_file.begin(), _file.end());
		wstring msg = currentTimew() + wstring(L", ") + str_line + wstring(L"line, ") + wstr_file + wstring(L", ") + _msg;
		return msg;
	}
};

template <typename T>
class LoggingMsgDebug {
};

template<>
class LoggingMsgDebug<string> {
public:
	static string getMsg() {
		return string("Debug, ");
	}
};

template<>
class LoggingMsgDebug<wstring> {
public:
	static wstring getMsg() {
		return wstring(L"Debug, ");
	}
};

template <typename T>
class LoggingMsgInfo {
};

template<>
class LoggingMsgInfo<string> {
public:
	static string getMsg() {
		return string("Info, ");
	}
};

template<>
class LoggingMsgInfo<wstring> {
public:
	static wstring getMsg() {
		return wstring(L"Info, ");
	}
};

template <typename T>
class LoggingMsgError {
};

template<>
class LoggingMsgError<string> {
public:
	static string getMsg() {
		return string("Error, ");
	}
};

template<>
class LoggingMsgError<wstring> {
public:
	static wstring getMsg() {
		return wstring(L"Error, ");
	}
};

template <typename T>
class VariableParameterMsg {

};

template <>
class VariableParameterMsg<string> {
private:
public:
	string vsp(const char *_format, va_list &_args) {
#ifndef _MSC_VER
//		size_t size = std::snprintf(nullptr, 0, _format, _args); 
//		std::unique_ptr<char[]> buffer(new char[++size]);
//		vsprintf((char*)buffer.get(),_format,_args);
//		return std::move(buffer.get());
		char buffer[MAX_BUFFER_SIZE];	
		vsprintf(buffer,_format,_args);
		return buffer;
#else
		int size = _vscprintf(_format, _args);
		std::string buff(++size, 0);
		vsnprintf_s((char*)buff.data(),size,_TRUNCATE,_format,_args);
		string rt = std::move(buff);
		return rt;
#endif
	}
};

template <>
class VariableParameterMsg<wstring> {
private:
public:
	std::wstring vsp(const char *_format, va_list &_args) {
#ifndef _MSC_VER
//		size_t size = std::snprintf(nullptr, 0, _format, _args); 
//		std::unique_ptr<char[]> buffer(new char[++size]);
//		vsprintf(buffer.get(),_format,_args);
//		string temp = buffer.get();
//		wstring rt;
//		rt.assign(temp.begin(), temp.end());
//		return rt;
		char buffer[MAX_BUFFER_SIZE];	
		vsprintf(buffer,_format,_args);
		string temp=buffer;
		wstring rt;
		rt.assign(temp.begin(), temp.end());
		return rt;
#else
		int size = _vscprintf(_format, _args);
		std::string buff(++size, 0);
		vsnprintf_s((char*)buff.data(),size,_TRUNCATE,_format,_args);
		string temp=std::move(buff);
		wstring rt;
		rt.assign(temp.begin(), temp.end());
		return rt;
#endif
	}
};

template <typename T,typename U>
class GLogger {
private:
	list<GLogger *> loggers;
	map<const string,GTaskQue<T,U> *> appenders;
	VariableParameterMsg<T> variable_parameter_msg;
	CommonLoggingMsg<T> common_logging_msg;

#ifdef WIN32
	HANDLE			mutex;
#elif __linux__
	mutable pthread_mutex_t mutex;
#endif
	bool 			is_mutex_created;

public:
	GLogger();
private:
	GLogger(const GLogger<T,U> &);
	GLogger<T,U> &operator=(const GLogger<T,U> &);
	void initialize();
public:
	~GLogger();
private:
	void mutex_lock();
	void mutex_unlock();
	void createMutex();
	void destroyMutex();
	void destroy();
	void sendMessageToAll(const T &_msg);
	void sendMessageToAppender(const string &_key, const T &_msg);
	void sendMessageToAppenders(const T &_msg);
private:
public:
	static GLogger *getInstance() {
		static GLogger instance;
		return &instance;
	}
public:
	bool isRunning()const;
	void addAppender(const string &_key,GExecutorInterface<T,U> *_val);
	void addLogger(GLogger *_val);
	int start();
	int stop();
	void write(const string &_key, const T &_msg);
	void write(const T &_msg);
public:
	void write(const char *_format, ...);
	void error(
		const string &_file, 
		const long _line, 
		const char * _format, ...);
	void info(
		const string &_file, 
		const long _line, 
		const char * _format, ...);
	void debug(
		const string &_file, 
		const long _line, 
		const char * _format, ...);
};

template <typename T, typename U>
GLogger<T,U>::GLogger() {
	is_mutex_created = false;
	createMutex();
	this->start();
}

template <typename T, typename U>
GLogger<T,U>::GLogger(const GLogger<T,U> &_val) {
	// do not use
	is_mutex_created = false;
	createMutex();
}

template <typename T, typename U>
GLogger<T,U> &GLogger<T,U>::operator=(const GLogger<T,U> &_val) {
	// do not use
	return *this;
}

template <typename T, typename U>
void GLogger<T,U>::initialize() {
}

template <typename T, typename U>
GLogger<T,U>::~GLogger() {
	this->stop();
	destroy();
	destroyMutex();
}

template <typename T, typename U>
void GLogger<T,U>::mutex_lock() {
	if(is_mutex_created) {
#ifdef WIN32
		WaitForSingleObject(mutex, INFINITE);
#elif __linux__
		pthread_mutex_lock(&mutex);
#endif
	}
}

template <typename T, typename U>
void GLogger<T,U>::mutex_unlock() {
	if(is_mutex_created) {
#ifdef WIN32
		ReleaseMutex(mutex);
#elif __linux__
		pthread_mutex_unlock(&mutex);
#endif
	}
}

template <typename T, typename U>
void GLogger<T,U>::createMutex() {
	if(is_mutex_created) {
		return;
	}
#ifdef WIN32
	mutex = CreateMutex(NULL, FALSE, NULL);
#elif __linux__
	if(pthread_mutex_init(&mutex, NULL) != 0) {
		throw("pthread_mutex_init error");
	}
#endif
	is_mutex_created = true;
}

template <typename T, typename U>
void GLogger<T,U>::destroyMutex() {
	if(is_mutex_created) {
#ifdef WIN32
		CloseHandle(mutex);
#elif __linux__
		pthread_mutex_destroy(&mutex);
#endif
	}
	is_mutex_created = false;
}

template <typename T, typename U>
void GLogger<T,U>::destroy() {
//	mutex_lock();

	for_each(
		this->appenders.begin(),
		this->appenders.end(),
		[=](pair<const string, GTaskQue<T,U> *> &a_pair) {
		a_pair.second->quitThread();
		while (a_pair.second->isRunning()) {
#ifdef WIN32
			Sleep(1);
#elif __linux__
			usleep(1*10);
#endif
			cout<< "destroying 1"<<endl;
		}
		delete a_pair.second;
		a_pair.second = 0;
	});
	appenders.clear();

	for_each(
		this->loggers.begin(),
		this->loggers.end(),
		[](GLogger *logger) {
		logger->destroy();
		while (logger->isRunning()) {
#ifdef WIN32
			Sleep(1);
#elif __linux__
			usleep(1*10);
#endif
			cout<<"destroying 2"<<endl;

		}
		delete logger;
		logger = 0;
	});
	loggers.clear();

//	mutex_unlock();
}

// private
// Recurvice => do not use mutex lock
template <typename T, typename U>
void GLogger<T,U>::sendMessageToAll(const T &_msg) { 
//	mutex_lock();

	sendMessageToAppenders(_msg);
	for_each(
		this->loggers.begin(),
		this->loggers.end(),
		[_msg](GLogger *logger) {
		logger->sendMessageToAll(_msg);
	});
	
//	mutex_unlock();
}

// private
template <typename T, typename U>
void GLogger<T,U>::sendMessageToAppender(const string &_key, const T &_msg) { 
//	mutex_lock();

	typename map<const string, GTaskQue<T,U> *>::iterator itr = 
		appenders.find(_key);
	if (itr != this->appenders.end()) {
		itr->second->pushBack(_msg);
	}

//	mutex_unlock();
}

// private
template <typename T, typename U>
void GLogger<T,U>::sendMessageToAppenders(const T &_msg) {
//	mutex_lock();

	for_each(
		this->appenders.begin(),
		this->appenders.end(),
		[_msg](pair<const string, GTaskQue<T,U> *> &a_pair) {
		a_pair.second->pushBack(_msg);
	});
	
//	mutex_unlock();
}

// Recursive => do not use mutex lock
template <typename T, typename U>
bool GLogger<T,U>::isRunning()const {
	if (any_of(
		this->appenders.begin(),
		this->appenders.end(),
		[](const pair<const string, GTaskQue<T,U> *> &a_pair) {
		return a_pair.second->isRunning() ? true : false;
	})) {
		return true;
	}

	if (any_of(
		this->loggers.begin(),
		this->loggers.end(),
		[](GLogger *logger) {
		return logger->isRunning() ? true : false;
	})) {
		return true;
	}

	return false;
}

template <typename T, typename U>
void GLogger<T,U>::addAppender(
	const string &_key, 
	GExecutorInterface<T,U> *_val) {
//	mutex_lock();

	typename map<const string, GTaskQue<T,U> *>::const_iterator citr =
		this->appenders.find(_key);
	if (citr == appenders.end() || appenders.size() == 0) {
		GExecutorInterface<T,U> *executor = 
			(GExecutorInterface<T,U> *)(_val);
		GTaskQue<T,U> *threadque = 
			new GTaskQue<T,U>(executor,DEFAULT_SIZE_BACK_BUFFER);
		threadque->setDelayInBatch(0);
		threadque->setDalayBetweenBatch(1);
		appenders.insert(
			pair<const string,GTaskQue<T,U> *>(_key, threadque));
		if(!threadque->isRunning()) {
			threadque->doAutoExecution(true);
		}
	}

//	mutex_unlock();
}

template <typename T, typename U>
void GLogger<T,U>::addLogger(GLogger *_val) {
//	mutex_lock();

	typename list<GLogger *>::const_iterator citr =
		find(loggers.begin(), loggers.end(), _val);
	if (citr == loggers.end() || loggers.size() == 0) {
		loggers.push_back(_val);
		if(!_val->isRunning()) {
			_val->start();
		}
	}

//	mutex_unlock();
}

// return
// 0 : OK
// 1 : logger is already running
// -value : error
template <typename T, typename U>
int GLogger<T,U>::start() {
//	mutex_lock();

	if(this->isRunning()) {
		//cout<<"Glogger is running"<<endl;
		return 1;
	}

	for_each(
		this->appenders.begin(),
		this->appenders.end(),
		[](pair<const string, GTaskQue<T,U> *> &a_pair) {
		if(!a_pair.second->isRunning()) {
			a_pair.second->doAutoExecution(true);
		}
	});

	for_each(
		this->loggers.begin(),
		this->loggers.end(),
		[](GLogger *logger) {
		if(!logger->isRunning()) {
			logger->start();
		}
	});

//	mutex_unlock();

	return 0;
}

template <typename T, typename U>
int GLogger<T,U>::stop() {
//	mutex_lock();

	if(!this->isRunning()) {
		return 1;
	}

	for_each(
		this->appenders.begin(),
		this->appenders.end(),
		[](pair<const string, GTaskQue<T,U> *> &a_pair) {
		a_pair.second->quitThread();
	});

	for_each(
		this->loggers.begin(),
		this->loggers.end(),
		[](GLogger *logger) {
		logger->stop();
	});

//	mutex_unlock();

	return 0;
}

template <typename T, typename U>
void GLogger<T,U>::write(const string &_key, const T &_msg) {
	mutex_lock();

	assert(this->appenders.size() != 0);
	this->sendMessageToAppender(_key, _msg);
	
	mutex_unlock();
}

template <typename T, typename U>
void GLogger<T,U>::write(const T &_msg) {
	mutex_lock();

	assert(this->appenders.size() != 0);
	this->sendMessageToAll(_msg);

	mutex_unlock();
}

template <typename T, typename U>
void GLogger<T,U>::write(const char *_format, ...) {
	mutex_lock();

	assert(this->appenders.size() != 0);

	va_list va;
	va_start(va, _format);
	T va_result = variable_parameter_msg.vsp(_format, va);
	va_end(va);

	this->sendMessageToAll(va_result);
	
	mutex_unlock();
}

template <typename T, typename U>
void GLogger<T,U>::error(const string &_file, const long _line, const char *_format, ...) {
	mutex_lock();

	assert(this->appenders.size() != 0);

	va_list va;
	va_start(va, _format);
	T va_result = variable_parameter_msg.vsp(_format, va);
	va_end(va);

	T msg = LoggingMsgError<T>::getMsg() + common_logging_msg.getMsg(_file, _line, va_result);
	this->sendMessageToAll(msg);
	
	mutex_unlock();
}

template <typename T, typename U>
void GLogger<T,U>::info(const string &_file, const long _line, const char *_format, ...) {
	mutex_lock();

	assert(this->appenders.size() != 0);

	va_list va;
	va_start(va, _format);
	T va_result = variable_parameter_msg.vsp(_format, va);
	va_end(va);

	T msg = LoggingMsgInfo<T>::getMsg() + common_logging_msg.getMsg(_file, _line, va_result);
	this->sendMessageToAll(msg);
	
	mutex_unlock();
}

template <typename T, typename U>
void GLogger<T,U>::debug(const string &_file, const long _line, const char *_format, ...) {
	mutex_lock();

//#ifdef _DEBUG
	assert(this->appenders.size() != 0);
	
	va_list va;
	va_start(va, _format);
	T va_result = variable_parameter_msg.vsp(_format, va);
	va_end(va);
	T msg = LoggingMsgDebug<T>::getMsg() + common_logging_msg.getMsg(_file, _line, va_result);
	
	this->sendMessageToAll(msg);
//#endif

	mutex_unlock();
}

#endif
