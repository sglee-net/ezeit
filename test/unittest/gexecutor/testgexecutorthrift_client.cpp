#include "gtaskque/gtaskque.h"
#include "gexecutor/gexecutor_thrift.h"
#include "gtest/gtest.h"

#include <chrono>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/stdcxx.h>

#include "thrift_service/messenger_constants.h"
#include "thrift_service/messenger_types.h"
#include "thrift_service/TransferService.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace thrift_gen_messenger;

class TestGExecutorThrift: public ::testing::Test {
public:
	TestGExecutorThrift() {
		serviceClient = 0;
		que = 0;
		}
	virtual ~TestGExecutorThrift() {
		if(serviceClient) {
			delete serviceClient;
		}
		if(que) {
			delete que;
		}
	}
	virtual void SetUp() {
		const size_t buffer_size = 10;

		socket = stdcxx::shared_ptr<TTransport>(
			new TSocket("192.168.0.41", 9091));
	        transport = stdcxx::shared_ptr<TTransport>(
			new TBufferedTransport(socket));
	        protocol = stdcxx::shared_ptr<TProtocol>(
			new TBinaryProtocol(transport));
	
		serviceClient = new TransferServiceClient(protocol); 

//		transport->open();
				
		GExecutorInterface
		<Message,TransferServiceClient> *executor = 
			new GExecutorThrift
			<Message,TransferServiceClient>(
				serviceClient, false);
		que = new GTaskQue<Message,TransferServiceClient>
			(executor,buffer_size);
	}
	virtual void TearDown() {
	}
public:
	stdcxx::shared_ptr<TTransport> socket;
	stdcxx::shared_ptr<TTransport> transport;
	stdcxx::shared_ptr<TProtocol> protocol;
	TransferServiceClient *serviceClient;
	GTaskQue<Message,TransferServiceClient> *que;
};

TEST_F(
	TestGExecutorThrift,
	Connect) {

	ASSERT_TRUE(transport!=nullptr);
	transport->open();
}

TEST_F(
	TestGExecutorThrift,
	SendMessage) {

	transport->open();

	que->doAutoExecution(true);

	int tcount = 10;
	int icount = 100;
	for(int t=0; t<tcount; t++) {
		long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		string str_ptime = to_string(timestamp);

		ASSERT_TRUE(serviceClient!=nullptr);

		Message msg;
		msg._sender_id = "sender";
		if(serviceClient) {
			for(int i=0; i<icount; i++) {
				msg._timestamp=str_ptime;
				msg._list_i16.push_back(i);
				//serviceClient->writeI16(i);
				//serviceClient->writeMessage(msg);
			}
	//		serviceClient->writeMessage(msg);
		}
		ASSERT_EQ(icount,msg._list_i16.size());

		que->pushBack(msg);

		while(que->isRunning()) {
			usleep(1000);
			if(que->areAllTasksExecuted()) {
				break;
			}
		}

		usleep(1000);
	}
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
