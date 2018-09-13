#include <unistd.h>
#include <sstream>
#include <iostream>
#include <string>
#include "gtaskque/gtaskque.h"
#include "gtest/gtest.h"

using namespace std;

template <typename T, typename E>
class TestExecutor: public GExecutorInterface<T,E> {
public:
	TestExecutor(E *_attri,bool _b):GExecutorInterface<T,E>(_attri,_b) {}
	int execute(T &_arg) {
		ostringstream *oss = (ostringstream *)(this->getAttribute());
		(*oss) << string(_arg) << endl;
		return 0;
	}
};

class TestGTaskQue: public ::testing::Test {
private:
	const size_t buffer_size = 10;
protected:
	TestGTaskQue() {
		que = nullptr;
	}
	virtual ~TestGTaskQue() {
		if(que) {
			delete que;
		}
//		executor will be delete automatically
//		if(executor) {
//			delete executor;
//		}
	}
	virtual void SetUp() {
//		cout<<"SetUp()"<<endl;
		executor = new TestExecutor<string,ostringstream>(&oss,false);
		que = new GTaskQue<string,ostringstream>(executor,buffer_size);
	}
	virtual void TearDown() {
//		cout<<"TearDown()"<<endl;
	}
private:
public:
	ostringstream oss;
	GTaskQue<string,ostringstream> *que;
	GExecutorInterface<string,ostringstream> *executor;
};

TEST_F(
	TestGTaskQue,
	StringOut1000) {
	const size_t iteration_count = 1000;

	que->doAutoExecution(true);
	
	ostringstream sstr;
	for(size_t i=0; i<iteration_count; i++) {
		string strNumber = to_string(i);
		que->pushBack(strNumber);
		sstr << strNumber << endl;
	}

	while(que->isRunning()) {
		usleep(1000);
		if(que->areAllTasksExecuted()) {
			break;
		}
	}
}

TEST_F(
	TestGTaskQue,
	StringOut10000) {
	const size_t iteration_count = 10000;

	que->doAutoExecution(true);
	
	ostringstream sstr;
	for(size_t i=0; i<iteration_count; i++) {
		string strNumber = to_string(i);
		que->pushBack(strNumber);
		sstr << strNumber << endl;
	}

	while(que->isRunning()) {
		usleep(1000);
		if(que->areAllTasksExecuted()) {
			break;
		}
	}
}

TEST_F(
	TestGTaskQue,
	StringOut100000) {
	const size_t iteration_count = 100000;

	que->doAutoExecution(true);
	
	ostringstream sstr;
	for(size_t i=0; i<iteration_count; i++) {
		string strNumber = to_string(i);
		que->pushBack(strNumber);
		sstr << strNumber << endl;
	}

	while(que->isRunning()) {
		usleep(1000);
		if(que->areAllTasksExecuted()) {
			break;
		}
	}
}

int main(int argc, char **argv) {

	::testing::InitGoogleTest(&argc,argv);
	int rt = RUN_ALL_TESTS();

	return rt;
}

