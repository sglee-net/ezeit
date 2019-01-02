#include <iostream>
#include <fstream>
#include <string>
#include "glogger/glogger.h"
#include "gexecutor/gexecutor_filewriter.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/stdcxx.h>

#include "thrift/messenger_constants.h"
#include "thrift/messenger_types.h"
#include "thrift/TransferService.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace thrift_gen_messenger;

int main(int argc, char *argv[]) {
	ofstream *fileout=new ofstream("log.txt",ios::out);
	GLogger<string,ofstream> *logger = 
		GLogger<string,ofstream>::getInstance();
	GExecutorInterface<string,ofstream> *appender = 
		new GExecutorFileWriter<string,ofstream>(fileout,true);
	logger->addAppender("filewriter",appender);
	
	stdcxx::shared_ptr<TTransport> socket(new TSocket("192.168.0.41", 9091));
  	stdcxx::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    	stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	TransferServiceClient client(protocol);

        try {
		transport->open();
	
		Message msg;
		msg._list_string.push_back("Test message");
		
		//client.setMsg(msg);
		for(int32_t i=0; i<10000; i++) {
			cout<<i<<endl;
			string ret;
			client.writeI32(ret,"id",i);
			logger->debug(__FILE__,__LINE__,"writeI32(%d)",i);
			//usleep(1000);
		}

                transport->close();

	} catch (TException& tx) {
		cout << "ERROR: " << tx.what() << endl;
	}

	cout<< "The end"<<endl;

	logger->stop();
	while(logger->isRunning()) {
		usleep(1000);
	}

	fileout->close();
	//delete fileout;
	
	return 0;
}
