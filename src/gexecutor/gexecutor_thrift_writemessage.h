#ifndef __GEXECUTORTHRIFT_H__
#define __GEXECUTORTHRIFT_H__

#include <map>
#include <string>
#include "gtaskque/gtaskque.h"
#include "thrift/ThriftRWService.h"
#include "thrift/messenger_types.h"
#include <fstream>

#ifdef __linux__
#include <unistd.h>
#endif

using namespace std;
using namespace thrift_gen;

template <typename T, typename U>
class GExecutorThriftWriteMessage :public GExecutorInterface<T,U> {
private:
private:
	GExecutorThriftWriteMessage() { }
	GExecutorThriftWriteMessage(const GExecutorThriftWriteMessage &) { }
	GExecutorThriftWriteMessage& operator=(const GExecutorThriftWriteMessage &) { 
		return *this; }
public:
	GExecutorThriftWriteMessage(U *_val, const bool _autoRemove) 
		: GExecutorInterface<T,U>(_val, _autoRemove) {
	}
	virtual int execute(T &_arg);
};

// specialization
template <>
class GExecutorThriftWriteMessage<ThriftMessage, ThriftRWServiceClient>
	:public GExecutorInterface<ThriftMessage, ThriftRWServiceClient> {
private:
	GExecutorThriftWriteMessage() {}
	GExecutorThriftWriteMessage(const GExecutorThriftWriteMessage &) {}
	GExecutorThriftWriteMessage& operator=(const GExecutorThriftWriteMessage &) { 
		return *this; }
public:
	GExecutorThriftWriteMessage(ThriftRWServiceClient *_val, const bool _autoRemove)
		:GExecutorInterface<ThriftMessage, ThriftRWServiceClient>
			(_val,_autoRemove) {}
	virtual int execute(ThriftMessage &_arg) {
		if (!attribute) {
			return -1;
		}

		ThriftRWServiceClient *thriftClient = 
			(ThriftRWServiceClient *)attribute;
		string ret;
		try {
			thriftClient->writeThriftMessage(ret,_arg);
		} catch (...) {
		}

		return 0;
	}
};


#endif
