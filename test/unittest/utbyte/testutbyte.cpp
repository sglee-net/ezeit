#include <iostream>
#include "utbyte/utbyte.hpp"
#include "gtest/gtest.h"
#include <limits>
#include <list>

using namespace std;

using namespace utbyte;

class UTByteTest: public ::testing::Test {
protected:
	UTByteTest() {
	}
	virtual ~UTByteTest() {}
	virtual void SetUp() {
		vlist.push_back(numeric_limits<double>::min());
		vlist.push_back(numeric_limits<double>::max());
		vlist.push_back(0.0);
		vlist.push_back(-numeric_limits<double>::min());
		vlist.push_back(-numeric_limits<double>::max());
		vlist.push_back(1.123456789);
		vlist.push_back(-1.123456789);
		vlist.push_back(1.987654321);
		vlist.push_back(-1.987654321);
	}

	virtual void TearDown() {
	}
private:
public:
	list<double> vlist;
};

TEST_F(
	UTByteTest,
	utbyte_size_int) {
	for_each(vlist.begin(), vlist.end(), [](double &_v) {
		int v = (int)_v;
		std::array<byte,sizeof(int)> bytes;
		object_to_bytes<int>(v,bytes);
		ASSERT_TRUE(bytes.size() == sizeof(int));	
	});
}

TEST_F(
	UTByteTest,
	utbyte_size_char) {
	for_each(vlist.begin(), vlist.end(), [](double &_v) {
		char v = (char)_v;
		std::array<byte,sizeof(char)> bytes;
		object_to_bytes<char>(v,bytes);
		ASSERT_TRUE(bytes.size() == sizeof(char));	
	});
}

TEST_F(
	UTByteTest,
	utbyte_size_long) {
	for_each(vlist.begin(), vlist.end(), [](double &_v) {
		long v = (long)_v;
		std::array<byte,sizeof(long)> bytes;
		object_to_bytes<long>(v,bytes);
		ASSERT_TRUE(bytes.size() == sizeof(long));	
	});
}

TEST_F(
	UTByteTest,
	utbyte_size_float) {
	for_each(vlist.begin(), vlist.end(), [](double &_v) {
		float v = (float)_v;
		std::array<byte,sizeof(float)> bytes;
		object_to_bytes<float>(v,bytes);
		ASSERT_TRUE(bytes.size() == sizeof(float));	
	});
}

TEST_F(
	UTByteTest,
	utbyte_size_double) {
	for_each(vlist.begin(), vlist.end(), [](double &_v) {
		double v = (double)_v;
		std::array<byte,sizeof(double)> bytes;
		object_to_bytes<double>(v,bytes);
		ASSERT_TRUE(bytes.size() == sizeof(double));	
	});
}

template <typename T>
T get_inverse_object(T &_object) {
	std::array<byte,sizeof(T)> bytes;
	T inverse_object;

	object_to_bytes<T>(_object,bytes);
	bytes_to_object<T>(bytes,inverse_object);

	return inverse_object;
}

TEST_F(
	UTByteTest,
	bytes_array_inverse_int) {
	for_each(vlist.begin(), vlist.end(), [](double &_v) {
		int value = (int)_v;
		int inverse_object = 
			get_inverse_object<int>(value);
		ASSERT_EQ(value, inverse_object);
	});
}

TEST_F(
	UTByteTest,
	bytes_array_inverse_char) {
	for_each(vlist.begin(), vlist.end(), [](double &_v) {
		char value = (char)_v;
		char inverse_object = 
			get_inverse_object<char>(value);
		ASSERT_EQ(value, inverse_object);
	});
}

TEST_F(
	UTByteTest,
	bytes_array_inverse_long) {
	for_each(vlist.begin(), vlist.end(), [](double &_v) {
		long value = (long)_v;
		long inverse_object = 
			get_inverse_object<long>(value);
		ASSERT_EQ(value, inverse_object);
	});
}

TEST_F(
	UTByteTest,
	bytes_array_inverse_float) {
	for_each(vlist.begin(), vlist.end(), [](double &_v) {
		float value = (float)_v;
		float inverse_object = 
			get_inverse_object<float>(value);
		ASSERT_FLOAT_EQ(value, inverse_object);
	});
}

TEST_F(
	UTByteTest,
	bytes_array_inverse_double) {
	for_each(vlist.begin(), vlist.end(), [](double &_v) {
		double value = (double)_v;
		double inverse_object = 
			get_inverse_object<double>(value);
		ASSERT_DOUBLE_EQ(value, inverse_object);
	});
}


template <typename T>
T get_inverse_object_v(T &_object) {
	std::vector<byte> bytes;
	T inverse_object = 0x00;

	object_to_bytes<T>(_object,bytes);
	bytes_to_object<T>(bytes,inverse_object);

	return inverse_object;
}

TEST_F(
	UTByteTest,
	bytes_vector_inverse_int) {
	for_each(vlist.begin(), vlist.end(), [](double &_v) {
		int value = (int)_v;
		int inverse_object = 
			get_inverse_object_v<int>(value);
		ASSERT_EQ(value, inverse_object);
	});
}

TEST_F(
	UTByteTest,
	bytes_vector_inverse_char) {
	for_each(vlist.begin(), vlist.end(), [](double &_v) {
		char value = (char)_v;
		char inverse_object = 
			get_inverse_object_v<char>(value);
		ASSERT_EQ(value, inverse_object);
	});
}

TEST_F(
	UTByteTest,
	bytes_vector_inverse_long) {
	for_each(vlist.begin(), vlist.end(), [](double &_v) {
		long value = (long)_v;
		long inverse_object = 
			get_inverse_object_v<long>(value);
		ASSERT_EQ(value, inverse_object);
	});
}

TEST_F(
	UTByteTest,
	bytes_vector_inverse_float) {
	for_each(vlist.begin(), vlist.end(), [](double &_v) {
		float value = (float)_v;
		float inverse_object = 
			get_inverse_object_v<float>(value);
		ASSERT_FLOAT_EQ(value, inverse_object);
	});
}

TEST_F(
	UTByteTest,
	bytes_vector_inverse_double) {
	for_each(vlist.begin(), vlist.end(), [](double &_v) {
		double value = (double)_v;
		double inverse_object = 
			get_inverse_object_v<double>(value);
		ASSERT_DOUBLE_EQ(value, inverse_object);
	});
}

TEST_F(
	UTByteTest,
	compare_array_and_vector) {
	for_each(vlist.begin(), vlist.end(), [](double &_v) {
		std::array<byte,sizeof(double)> bytes1;
		double inverse_object1;

		try {
			object_to_bytes<double>(_v,bytes1);
			bytes_to_object<double>(bytes1,inverse_object1);
		} catch(...) {
			FAIL();
		}

		std::vector<byte> bytes2;
		double inverse_object2;
		try {
			object_to_bytes<double>(_v,bytes2);
			bytes_to_object<double>(bytes2,inverse_object2);
		} catch(...) {
			FAIL();
		}

//		cout << "(invValue1, invValue2) " << inverse_object1 << ", "<< inverse_object2 <<endl;
		ASSERT_DOUBLE_EQ(inverse_object1, inverse_object2);
	});
}

TEST_F(
	UTByteTest,
	bytes_v_conversion) {
	vector<double> ref;
//	cout<<"ref"<<endl;
	for_each(vlist.begin(), vlist.end(), [&](double &_v) {
//		cout<<_v<<endl;
		ref.push_back(_v);
	});
	vector<byte> bytes;
	try{
		object_to_bytes<double>(ref,bytes);
	} catch(...) {
		FAIL();
	}

	ASSERT_TRUE(bytes.size()/sizeof(double) == vlist.size());

	vector<double> target;
	try {
		bytes_to_object<double>(bytes, target);
	} catch(...) {
		FAIL();
	}

//	cout<<"target"<<endl;
//	for_each(target.begin(), target.end(), [](double &_v) {
//		cout<<_v<<endl;
//	});
	ASSERT_TRUE(ref == target);
}

TEST_F(
	UTByteTest,
	bytes_v_pointer_conversion) {
	double *ref = new double [vlist.size()];
	size_t pos = 0;
	for_each(vlist.begin(), vlist.end(), [&](double &_v) {
		ref[pos] = _v;
		pos++;
	});
	
//	for(size_t i = 0; i < vlist.size(); i++) {
//		cout << "ref " << ref[i] << endl;
//	}
	
	vector<byte> bytes;
	try{
		object_to_bytes<double>(ref, vlist.size(), bytes);
	} catch(...) {
		FAIL();
	}

	ASSERT_TRUE(bytes.size()/sizeof(double) == vlist.size());
	
	double *target = nullptr; //new double [vlist.size()]; //nullptr;
	size_t size_of_target = 0;
	try {
		bytes_to_object<double>(bytes, &target, size_of_target);
	} catch(...) {
		FAIL();
	}

//	cout<<target<<endl;
//	for(size_t i = 0; i < vlist.size(); i++) {
//		cout << "target " << target[i] << endl;
//	}

	for(size_t i = 0; i < vlist.size(); i++) {
		//cout << "ref, target " << ref[i] << " " << target[i] << endl;
		ASSERT_DOUBLE_EQ(ref[i], target[i]);
	}

	ASSERT_DOUBLE_EQ(numeric_limits<double>::min(), target[0]);
	ASSERT_DOUBLE_EQ(numeric_limits<double>::max(), target[1]);
	ASSERT_DOUBLE_EQ(0.0, target[2]);
	ASSERT_DOUBLE_EQ(-numeric_limits<double>::min(), target[3]);
	ASSERT_DOUBLE_EQ(-numeric_limits<double>::max(), target[4]);
	ASSERT_DOUBLE_EQ(1.123456789, target[5]);
	ASSERT_DOUBLE_EQ(-1.123456789, target[6]);
	ASSERT_DOUBLE_EQ(1.987654321, target[7]);
	ASSERT_DOUBLE_EQ(-1.987654321, target[8]);
	
	delete [] ref;
	delete [] target;
}


int main(int argc, char **argv) {

	::testing::InitGoogleTest(&argc,argv);
	int rt = RUN_ALL_TESTS();

	return rt;
}


