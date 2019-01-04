#ifndef __GEXECUTORTHRIFT_H__
#define __GEXECUTORTHRIFT_H__

#include <map>
#include <string>
#include "gtaskque/gtaskque.h"
#include "thrift/TransferService.h"
#include "thrift/messenger_types.h"
#include <fstream>

#ifdef __linux__
#include <unistd.h>
#endif

using namespace std;
using namespace thrift_gen_messenger;

template <typename T, typename U>
class GExecutorThrift :public GExecutorInterface<T,U> {
private:
private:
	GExecutorThrift() { }
	GExecutorThrift(const GExecutorThrift &) { }
	GExecutorThrift& operator=(const GExecutorThrift &) { 
		return *this; }
public:
	GExecutorThrift(U *_val, const bool _autoRemove) 
		: GExecutorInterface<T,U>(_val, _autoRemove) {
	}
	virtual int execute(T &_arg);
};

// specialization
template <>
class GExecutorThrift<Message, TransferServiceClient>
	:public GExecutorInterface<Message, TransferServiceClient> {
private:
	GExecutorThrift() {}
	GExecutorThrift(const GExecutorThrift &) {}
	GExecutorThrift& operator=(const GExecutorThrift &) { 
		return *this; }
public:
	GExecutorThrift(TransferServiceClient *_val, const bool _autoRemove)
		:GExecutorInterface<Message, TransferServiceClient>
			(_val,_autoRemove) {}
	virtual int execute(Message &_arg) {
		if (!attribute) {
			return -1;
		}

		TransferServiceClient *thriftClient = 
			(TransferServiceClient *)attribute;
		string ret;
		try {
			thriftClient->writeMessage(ret,_arg);
		} catch (exception &e) {
			cout << e << endl;
		}

		return 0;
	}
};


#endif
