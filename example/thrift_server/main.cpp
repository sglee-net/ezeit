#include <iostream>
#include <fstream>
#include <string>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/stdcxx.h>

#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/StdThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include "thrift/messenger_constants.h"
#include "thrift/messenger_types.h"
#include "thrift/ThriftService.h"

#include "thrift_server.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace thrift_gen;

int main(int argc, char *argv[]) {

	int port = 9090;
	TThreadedServer server(
		std::make_shared<ThriftServiceProcessorFactory>(
			std::make_shared<ThriftServiceImpFactory>()),
		std::make_shared<TServerSocket>(port),
		std::make_shared<TBufferedTransportFactory>(),
		std::make_shared<TBinaryProtocolFactory>());

	cout << "starting the thrift server..." << endl;
	server.serve();
	cout << "Done." << endl;
	return 0;
}
