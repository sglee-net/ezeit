/** 
 * @author SG Lee
 * @since 7/20/2015
 * @version 0.1
 * @description
 * byte handling, for example number to byte and number array to byte
 */

#ifndef utbyte_hpp
#define utbyte_hpp

#include <array>
#include <vector>
#include <iterator>
#include <algorithm>
#include <assert.h>
#include <string>

using namespace std;

// to get the size of array
template <typename T,unsigned S>
inline unsigned arraysize(const T (&v)[S]) { return S; }
//int main()
//{
//    // different kinds of arrays, all size 5
//    int normalarray[] = {1,2,3,4,5};
//    std::vector<int> vec(5);
//    int* dynamic = new int[5];
//
//    // ARRAY_SIZE will only work for normalarray:
//    cout << ARRAY_SIZE(normalarray);  // prints 5 as expected
//    cout << ARRAY_SIZE(vec);          // prints unreliable garbage
//    cout << ARRAY_SIZE(dynamic);      // prints unreliable garbage
//
//    // on the other hand, arraysize will give you an error if you use it incorrectly:
//    cout << arraysize(normalarray);   // prints 5 as expected
//    cout << arraysize(vec);           // gives you a compiler error
//    cout << arraysize(dynamic);       // gives you a compiler error
//}

namespace utbyte {
    
	using byte=unsigned char;
	//typedef unsigned char byte;

	// arguments
	// _object : reference
	// _bytes  : target
	// ex)
	// double reference = 1.0
	// array<byte,sizeof(double)> target
	// target <- to_bytes(reference, target)
	template<typename T>
	static void object_to_bytes(
		const T &_object, 
		std::array<byte,sizeof(T)> &_bytes) {
		const byte *begin = 
			reinterpret_cast<const byte*>(std::addressof(_object)) ;
		const byte *end = begin + sizeof(T) ;
		std::copy(begin,end,std::begin(_bytes));
	}

	// arguments
	// _bytes  : reference
	// _object : target
	template<typename T>
	static void bytes_to_object(
		const std::array<byte,sizeof(T)> &_bytes, 
		T &_object) {
		assert(_bytes.size() != 0 && _bytes.size() % sizeof(T) == 0);
		byte *begin_object =
			reinterpret_cast<byte *>(std::addressof(_object));
		std::copy(std::begin(_bytes),std::end(_bytes),begin_object);
	}

	// arguments
	// _object : reference
	// _bytes  : target
	template<typename T>
	static void object_to_bytes(
		const T &_object, 
		std::vector<byte> &_bytes) {
		const byte *begin = 
			reinterpret_cast<const byte*>(std::addressof(_object)) ;
		const byte *end = begin + sizeof(T) ;
		_bytes.reserve(sizeof(T)); // do not use copy
		_bytes.assign(begin,end);
	}

	// arguments
	// _bytes  : reference
	// _object : target
	template<typename T>
	static void bytes_to_object(
		const std::vector<byte> &_bytes, 
		T &_object) {
		assert(_bytes.size() != 0 && _bytes.size() % sizeof(T) == 0);
		byte *begin_object =
			reinterpret_cast<byte *>(std::addressof(_object));
		std::copy(std::begin(_bytes),std::end(_bytes),begin_object);
	}

	// arguments
	// _object : reference
	// _bytes  : target
	template<typename T>
	static void object_to_bytes(
		const std::vector<T> &_object, 
		std::vector<byte> &_bytes) {
		for(size_t i = 0; i < _object.size(); i++) {
			const T object = _object[i];
			std::vector<byte> bytes;
			object_to_bytes(object,bytes);
			for_each(bytes.begin(), bytes.end(), [&](byte b) {
				_bytes.push_back(b);
			});
		}
	}

	// arguments
	// _bytes  : reference
	// _object : target
	template<typename T>
	static void bytes_to_object(
		const std::vector<byte> &_bytes, 
		std::vector<T> &_object) {
		assert(_bytes.size() != 0 && _bytes.size() % sizeof(T) == 0);
		for(size_t i = 0; i < _bytes.size(); i += sizeof(T)) {
			T object = 0x00;
			byte *begin_object = 
				reinterpret_cast<byte *>(std::addressof(object));
			std::copy(&(_bytes[i]), &(_bytes[i+sizeof(T)]), begin_object);
			_object.push_back(object);
		}
	}

	// arguments
	// _object : reference
	// _bytes  : target
	template<typename T>
	static void object_to_bytes(
		const T *_object, 
		const size_t _object_size, std::vector<byte> &_bytes) {
		for(size_t i = 0; i < _object_size; i++) {
			const T object = _object[i];
			std::vector<byte> bytes;
			object_to_bytes(object,bytes);
			for_each(bytes.begin(), bytes.end(), [&](byte b) {
				_bytes.push_back(b);
			});
		}
	}

	// arguments
	// _bytes  : reference
	// _object : target
	template<typename T>
	static void bytes_to_object(
		const std::vector<byte> &_bytes, 
		T **_object, size_t &_object_size) {
		assert(*_object == nullptr);
		if(*_object != nullptr) {
			throw("point should be null");
		}
		assert(_bytes.size() != 0 && _bytes.size() % sizeof(T) == 0);
		(*_object) = new T [_object_size];
		size_t index = 0;
		for(size_t i = 0; i < _bytes.size(); i += sizeof(T)) {
			T object = 0x00;
			byte *begin_object = 
				reinterpret_cast<byte *>(std::addressof(object));
			std::copy(&(_bytes[i]), &(_bytes[i+sizeof(T)]), begin_object);
			(*_object)[index] = object;
			index++;
		}
	}
}

#endif /* utbyte_hpp */
