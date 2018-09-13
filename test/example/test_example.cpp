#include <iostream>
#include "example/example.h"
#include "examplelib/examplelib.h"
#include "gtest/gtest.h"

using namespace std;

class ExampleTest: public ::testing::Test {
protected:
	ExampleTest() {}
	virtual ~ExampleTest() {}
	virtual void SetUp() {
		cout<<"SetUp()"<<endl;
		example = new Example();
		lib = new ExampleLib();
	}
	virtual void TearDown() {
		cout<<"TearDown()"<<endl;
	}
private:
public:
	Example *example;
	ExampleLib *lib;
};

int sum(int a,int b) {
	return 1;
}

TEST_F(
	ExampleTest,
	Constructor) {
	ASSERT_TRUE(example!=nullptr);
	ASSERT_TRUE(lib!=nullptr);
}

TEST_F(
	ExampleTest,
	Print) {
	EXPECT_STREQ("Hello World",example->print().c_str());
}

int main(int argc, char **argv) {

	::testing::InitGoogleTest(&argc,argv);
	int rt = RUN_ALL_TESTS();
	
//	Example *example = new Example();
//	example->print();
//	
//	ExampleLib *lib = new ExampleLib();
//	cout<<lib->sum(1,2)<<endl;
//
//	delete lib;
//	delete example;

	return 0;
}


